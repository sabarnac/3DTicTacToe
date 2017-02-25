# ifndef GL_INPUT_HANDLER

	# define GL_INPUT_HANDLER 1

	# include <glm/glm.hpp>
	# include <glm/gtc/matrix_transform.hpp>
	# include <glm/gtx/rotate_vector.hpp>

	# include "window_handler.cpp"
	# include "logging_handler.cpp"

	using namespace glm ;

	struct inputVars {
		vec3                                            position                      ;
		float                                           horizontalAngle               ;
		float                                           verticalAngle                 ;
		float                                           fieldOfView                   ;
		float                                           speed                         ;
		float                                           mouseSpeed                    ;
	} ;

	class inputHandler {

		private:
			
			struct inputVars                            initialValues                 ;
			mat4                                        projectionMatrix              ;
			mat4                                        viewMatrix                    ;
			vec3                                        position                      ;
			float                                       horizontalAngle               ;
			float                                       verticalAngle                 ;
			float                                       fieldOfView                   ;
			float                                       speed                         ;
			float                                       mouseSpeed                    ;
			double                                      previousCallTime              ;
			double                                      oldXPos                       ;
			double                                      oldYPos                       ;
			GLFWcursor                                * customCursor                  ;
			
			loggingHandler                            * logger                        ;

		public:

			inputHandler ( struct inputVars requiredValues , loggingHandler * logger = new loggingHandler ) {
				this -> logger = logger ;

				this -> logger -> logInfo ( "inputHandler : Initializing the input handler" ) ;

				this -> logger -> logInfo ( "inputHandler : Setting initial values" ) ;
				this -> initialValues = requiredValues ;
				this -> resetValues ( ) ;

				this -> logger -> logInfo ( "inputHandler : Initialized the input handler\n" ) ;
			}

			~inputHandler ( ) {
				glfwDestroyCursor ( this -> customCursor ) ;
			}

			void resetValues ( ) {
				this -> position = ( this -> initialValues ) . position ;
				this -> horizontalAngle = ( this -> initialValues ) . horizontalAngle ;
				this -> verticalAngle = ( this -> initialValues ) . verticalAngle ;
				this -> fieldOfView = ( this -> initialValues ) . fieldOfView ;
				this -> speed = ( this -> initialValues ) . speed ;
				this -> mouseSpeed = ( this -> initialValues ) . mouseSpeed ;
				this -> previousCallTime = glfwGetTime ( ) ;
				this -> customCursor = glfwCreateStandardCursor ( GLFW_ARROW_CURSOR ) ;
				glfwSetCursor ( mainWindow , ( this -> customCursor ) ) ;
				glfwSetCursorPos ( mainWindow , ( ( windowWidth ) / 2 ) , ( ( windowHeight ) / 2 ) ) ;
				this -> oldXPos = ( ( windowWidth ) / 2 ) ;
				this -> oldYPos = ( ( windowHeight ) / 2 ) ;
				this -> setInputMode ( GLFW_STICKY_KEYS , GL_FALSE ) ;
				this -> setInputMode ( GLFW_STICKY_MOUSE_BUTTONS , GL_FALSE ) ;
				this -> setInputMode ( GLFW_STICKY_KEYS , GL_FALSE ) ;
				this -> setInputMode ( GLFW_STICKY_MOUSE_BUTTONS , GL_FALSE ) ;
			}

			void pollEvents ( ) {
				glfwSwapBuffers ( mainWindow ) ;
				glfwPollEvents ( ) ;
			}

			void waitForEventsTillTimeout ( float timeout = 0.5 ) {
				glfwSwapBuffers ( mainWindow ) ;
				glfwWaitEventsTimeout ( timeout ) ;
			}

			void waitForEvents ( ) {
				glfwSwapBuffers ( mainWindow ) ;
				glfwWaitEvents ( ) ;
			}

			void refreshInputsCameraToCenter ( ) {
				double currentCallTime = glfwGetTime ( ) ;
				float deltaCallTime = ( float ) ( currentCallTime - ( this -> previousCallTime ) ) ;

				double xPos , yPos ;

				glfwGetCursorPos ( mainWindow , & xPos , & yPos ) ;

				vec3 oldPosition = this -> position ;
				if ( this -> checkMouseButtonPressed ( GLFW_MOUSE_BUTTON_RIGHT ) ) {
					this -> horizontalAngle = 0.002 * mouseSpeed * ( ( float ) ( ( this -> oldXPos ) - xPos ) ) ;
					this -> verticalAngle = 0.004 * mouseSpeed * ( ( float ) ( ( this -> oldYPos ) - yPos ) ) ;

					this -> position = rotateY ( ( this -> position ) , ( this -> horizontalAngle ) ) ;
					float tmp = ( 20.0f - abs ( ( this -> position ) . x ) ) / 20.0f ;
					if ( ( ( this -> position ) . z ) >= 0 )
						this -> position = rotateX ( ( this -> position ) , ( tmp * ( this -> verticalAngle ) ) ) ;
					else
						this -> position = rotateX ( ( this -> position ) , ( ( - 1 ) * tmp * ( this -> verticalAngle ) ) ) ;
					tmp = abs ( ( this -> position ) . x ) / 20.0f ;
					if ( ( ( this -> position ) . x ) >= 0 )
						this -> position = rotateZ ( ( this -> position ) , ( ( - 1 ) * tmp * ( this -> verticalAngle ) ) ) ;
					else
						this -> position = rotateZ ( ( this -> position ) , ( tmp * ( this -> verticalAngle ) ) ) ;

					if ( ( ( ( this -> position ) . y ) >= 19.5f ) || ( ( ( this -> position ) . y ) <= -19.5f ) ) {
						this -> position = oldPosition ;
						this -> position = rotateY ( ( this -> position ) , ( this -> horizontalAngle ) ) ;
					}
				}

				if ( xPos < 10 ) xPos = 10 ;
				if ( xPos >= ( windowWidth - 10 ) ) xPos = windowWidth - 11 ;

				if ( yPos < 1 ) yPos = 10 ;
				if ( yPos >= ( windowHeight - 10 ) ) yPos = windowHeight - 11 ;

				glfwSetCursorPos ( mainWindow , xPos , yPos ) ;

				this -> oldXPos = xPos ;
				this -> oldYPos = yPos ;

				this -> previousCallTime = currentCallTime ;

				this -> projectionMatrix = perspective ( ( this -> fieldOfView ) , 4.0f / 3.0f , 0.1f , 100.0f ) ;
				this -> viewMatrix = lookAt ( ( this -> position ) , vec3 ( 0 , 0 , 0 ) , vec3 ( 0 , 1 , 0 ) ) ;
			}

			void refreshInputs ( ) {
				double currentCallTime = glfwGetTime ( ) ;
				float deltaCallTime = ( float ) ( currentCallTime - ( this -> previousCallTime ) ) ;

				double xPos , yPos ;

				glfwGetCursorPos ( mainWindow , & xPos , & yPos ) ;

				if ( xPos < 10 ) xPos = 10 ;
				if ( xPos >= ( windowWidth - 10 ) ) xPos = windowWidth - 11 ;

				if ( yPos < 1 ) yPos = 10 ;
				if ( yPos >= ( windowHeight - 10 ) ) yPos = windowHeight - 11 ;

				glfwSetCursorPos ( mainWindow , xPos , yPos ) ;

				this -> oldXPos = xPos ;
				this -> oldYPos = yPos ;

				this -> previousCallTime = currentCallTime ;

				this -> projectionMatrix = perspective ( ( this -> fieldOfView ) , 4.0f / 3.0f , 0.1f , 100.0f ) ;
				this -> viewMatrix = lookAt ( ( this -> position ) , vec3 ( 0 , 0 , 0 ) , vec3 ( 0 , 1 , 0 ) ) ;
			}

			int getPixelColorUnderMouse ( ) {
				unsigned char data [ 4 ] ;
				glReadPixels ( this -> oldXPos , ( windowHeight - ( this -> oldYPos ) ) , 1 , 1 , GL_RGBA , GL_UNSIGNED_BYTE , data ) ;
				int colorPicked = ( ( data [ 0 ] ) + ( data [ 1 ] * 256 ) + ( data [ 2 ] * 256 * 256 ) ) ;
				return colorPicked ;
			}

			int setInputMode ( int mode = GLFW_CURSOR , int value = GLFW_CURSOR_DISABLED ) {
				glfwSetInputMode ( mainWindow , mode , value ) ;
			}

			int getInputMode ( int mode = GLFW_CURSOR ) {
				return glfwGetInputMode ( mainWindow , mode ) ;
			}

			int checkKeyPressed ( int key ) {
				if ( glfwGetKey ( mainWindow , key ) == GLFW_PRESS ) return 1 ;
				else return 0 ;
			}

			int checkKeyReleased ( int key ) {
				if ( glfwGetKey ( mainWindow , key ) == GLFW_RELEASE ) return 1 ;
				else return 0 ;
			}

			int checkMouseButtonPressed ( int key ) {
				if ( glfwGetMouseButton ( mainWindow , key ) == GLFW_PRESS ) return 1 ;
				else return 0 ;
			}

			int checkMouseButtonReleased ( int key ) {
				if ( glfwGetMouseButton ( mainWindow , key ) == GLFW_RELEASE ) return 1 ;
				else return 0 ;
			}

			int checkWindowBeingClosed ( ) {
				return glfwWindowShouldClose ( mainWindow ) ;
			}

			mat4 getViewMatrix ( ) {
				return this -> viewMatrix ;
			}

			mat4 getProjectionMatrix ( ) {
				return this -> projectionMatrix ;
			}

	} ;

#endif