# ifndef GL_WINDOW_HANDLER

	# define GL_WINDOW_HANDLER 1

	# include <string.h>

	# include <GL/glew.h>
	# include <GLFW/glfw3.h>
	# include <glm/glm.hpp>

	using namespace glm ;

	# include "logging_handler.cpp"

	GLFWwindow * mainWindow ;
	short int windowWidth ;
	short int windowHeight ;

	class windowHandler {

		private:
			
			GLFWmonitor                               * primaryMonitor                ;
			GLuint                                      vertexArrayIDs                ;

			short int                                   errorCode                     ;

			loggingHandler                            * logger                        ;

		public:

			windowHandler ( const char * windowTitle , loggingHandler * logger = new loggingHandler ) {
				this -> logger = logger ;

				this -> logger -> logInfo ( "windowHandler : Initializing the main window and its handler" ) ;
				if ( ! glfwInit ( ) ) {
					this -> logger -> logInfo ( "windowHandler : Failed to initialize GLFW. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}

				this -> logger -> logInfo ( "windowHandler : Reading game window related settings from file" ) ;
				char settingType [ 20 ] , settingValue [ 100 ] ;
				int isFullscreen , windowSamples ;
				FILE * settingsFile = fopen ( "config/settings.cf" , "r" ) ;

				while ( ! feof ( settingsFile ) ) {
					fscanf ( settingsFile , "%s : %s" , settingType , settingValue ) ;
					if ( ! strcmp ( "WINDOW_TYPE" , settingType ) ) {
						if ( ! strcmp ( "FULLSCREEN" , settingValue ) )
							isFullscreen = 1 ;
						else
							isFullscreen = 0 ;
					}
					else if ( ! strcmp ( "WINDOW_WIDTH" , settingType ) ) {
						windowWidth = atoi ( settingValue ) ;
					}
					else if ( ! strcmp ( "WINDOW_HEIGHT" , settingType ) ) {
						windowHeight = atoi ( settingValue ) ;
					}
					else if ( ! strcmp ( "WINDOW_SAMPLES" , settingType ) ) {
						windowSamples = atoi ( settingValue ) ;
					}
				}

				fclose ( settingsFile ) ;

				this -> logger -> logInfo ( "windowHandler : Setting number of samples to be used for multi-sampling and anti-aliasing" ) ;
				glfwWindowHint ( GLFW_SAMPLES , windowSamples ) ;
				this -> logger -> logInfo ( "windowHandler : Setting OpenGL version to be used to 3.3" ) ;
				glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR , 3 ) ;
				glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR , 3 ) ;
				this -> logger -> logInfo ( "windowHandler : Enabling forward compatibility ( Required for Macs apparently )" ) ;
				glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT , GL_TRUE ) ;
				this -> logger -> logInfo ( "windowHandler : Setting OpenGL to use the core/main profile" ) ;
				glfwWindowHint ( GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE ) ;

				this -> logger -> logInfo ( "windowHandler : Retrieving the primary monitor to be used to show the window in" ) ;
				this -> primaryMonitor = glfwGetPrimaryMonitor ( ) ;
				if ( ( this -> primaryMonitor ) == NULL ) {
					this -> logger -> logInfo ( "windowHandler : Could not retrieve which is the primary monitor. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}

				this -> logger -> logInfo ( "windowHandler : Retrieve the video mode the primary monitor is using" ) ;
				const GLFWvidmode * videoMode = glfwGetVideoMode ( this -> primaryMonitor ) ;
				if ( videoMode == NULL ) {
					this -> logger -> logInfo ( "windowHandler : Could not retrieve the video mode of the primary monitor. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}

				this -> logger -> logInfo ( "windowHandler : Setting the gamma value for the primary monitor" ) ;
				glfwSetGamma ( ( this -> primaryMonitor ) , 1.0 ) ;

				this -> logger -> logInfo ( "windowHandler : Creating a window to use the primary monitor in it's currently set video mode resolution" ) ;
				if ( isFullscreen ) {
					mainWindow = glfwCreateWindow ( ( videoMode -> width ) , ( videoMode -> height ) , windowTitle , ( this -> primaryMonitor ) , NULL ) ;
					windowWidth = ( videoMode -> width ) ;
					windowHeight = ( videoMode -> height ) ;
				}
				else
					mainWindow = glfwCreateWindow ( windowWidth , windowHeight , windowTitle , NULL , NULL ) ;
				if ( mainWindow == NULL ) {
					this -> logger -> logInfo ( "windowHandler : Could not create a window. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}

				this -> logger -> logInfo ( "windowHandler : Set the newly created window as the one to be used in context" ) ;
				glfwMakeContextCurrent ( mainWindow ) ;
				glfwSwapInterval ( 0 ) ;

				this -> logger -> logInfo ( "windowHandler : Setting GLEW to use the experimental profile ( required )" ) ;
				glewExperimental = true ;

				this -> logger -> logInfo ( "windowHandler : Initializing GLEW" ) ;
				if ( glewInit ( ) != GLEW_OK ) {
					this -> logger -> logInfo ( "windowHandler : Could not initialize GLEW. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}

				this -> logger -> logInfo ( "windowHandler : Saving the primary monitor's current video mode's resolution" ) ;

				glGenVertexArrays ( 1 , & ( this -> vertexArrayIDs ) ) ;
				glBindVertexArray ( this -> vertexArrayIDs ) ;

				this -> logger -> logInfo ( "windowHandler : Initialized the main window and its handler\n" ) ;
			}

			~windowHandler ( ) {
				glDeleteVertexArrays ( 1 , & ( this -> vertexArrayIDs ) ) ;
				glfwDestroyWindow ( mainWindow ) ;
				glfwTerminate ( ) ;
			}

			void setClearScreenColor ( vec4 color ) {
				glClearColor ( color . x , color . y , color . z , color . w ) ;
			}

			void clearScreen ( GLbitfield mask ) {
				glClear ( mask ) ;
			}

			void enableMSAA ( ) {
				glEnable ( GL_MULTISAMPLE ) ; 
			}

			void enableDepthTest ( ) {
				glEnable ( GL_DEPTH_TEST ) ;
				glDepthFunc ( GL_LESS ) ;
			}

			void disableDepthTest ( ) {
				glDisable ( GL_DEPTH_TEST ) ;
			}

			void enableFaceCulling ( ) {
				glEnable ( GL_CULL_FACE ) ;
			}

			void disableFaceCulling ( ) {
				glDisable ( GL_CULL_FACE ) ;
			}

			int getWindowWidth ( ) {
				return windowWidth ;
			}

			int getWindowHeight ( ) {
				return windowHeight ;
			}

	} ;

#endif