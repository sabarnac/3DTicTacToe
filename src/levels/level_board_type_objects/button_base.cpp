# ifndef TTT3D_LEVEL_BOARD_TYPE_BUTTON_BASE

	# define TTT3D_LEVEL_BOARD_TYPE_BUTTON_BASE 1

	# include <glm/glm.hpp>
	# include <glm/gtc/matrix_transform.hpp>

	class levelBoardTypeButtonBase {

		protected:
			
			inputHandler                              * inputObject                   ;
			modelObjectsHandler                       * modelObject                   ;
			GLuint                                      matrixID                      ;
			GLuint                                      textureID                     ;
			GLuint                                      pickingColorID                ;
			mat4                                        baseModelMatrix               ;
			mat4                                        modelMatrix                   ;
			mat4                                        mvpMatrix                     ;
			string                                      fileNo                        ;

			loggingHandler                            * logger                        ;

		public:

			levelBoardTypeButtonBase ( const char * fileNo , loggingHandler * logger = new loggingHandler ) {
				this -> logger = logger ;

				this -> logger -> logInfo ( "levelBoardTypeButtonBase : Initializing button base" ) ;
				this -> fileNo = fileNo ;
				this -> logger -> logInfo ( "levelBoardTypeButtonBase : Setting button object" ) ;
				this -> modelObject = modelObjectsHandler :: checkObjects ( fileNo ) ;
				if ( this -> modelObject == NULL )
					this -> modelObject = new modelObjectsHandler ( fileNo , logger ) ;
				else
					modelObjectsListCount [ fileNo ] ++ ;
				this -> logger -> logInfo ( "levelBoardTypeButtonBase : Initialized button base\n" ) ;
			}

			~levelBoardTypeButtonBase ( ) {
				modelObjectsList . erase ( this -> fileNo ) ;
				modelObjectsListCount . erase ( this -> fileNo ) ;
			}

			virtual void resetModelMatrix ( ) {
				this -> modelMatrix = this -> baseModelMatrix ;
			}

			virtual void setModelMatrix ( mat4 newModelMatrix ) {
				this -> modelMatrix = newModelMatrix ;
			}

			virtual void scaleModelMatrix ( vec3 scaleFactor ) {
				this -> modelMatrix = scale ( ( this -> modelMatrix ) , scaleFactor ) ;
			}

			virtual void rotateModelMatrix ( float rotationFactor , vec3 rotationAxis ) {
				this -> modelMatrix = rotate ( ( this -> modelMatrix ) , radians ( rotationFactor ) , rotationAxis ) ;
			}

			virtual void translateModelMatrix ( vec3 translationFactor ) {
				this -> modelMatrix = translate ( ( this -> modelMatrix ) , translationFactor ) ;
			}

			virtual void updateAndDraw ( ) {
				glUniform4f ( ( this -> pickingColorID ) , 0.0f , 0.0f , 0.0f , 0.0f ) ;

				this -> modelObject -> modelShaders -> useShader ( ) ;

				this -> modelObject -> modelTexture -> bindTexture ( ( this -> textureID ) , 0 ) ;

				this -> mvpMatrix = ( this -> inputObject -> getProjectionMatrix ( ) ) * ( this -> inputObject -> getViewMatrix ( ) ) * ( this -> modelMatrix ) ;
				glUniformMatrix4fv ( ( this -> matrixID ) , 1 , GL_FALSE , & ( ( this -> mvpMatrix ) [ 0 ] [ 0 ] ) ) ;

				this -> modelObject -> bindBuffers ( ) ;

				this -> modelObject -> drawObject ( ) ;
			}

			virtual void setPickingColor ( int r , int g , int b , int a = 255 ) {
				glUniform4f ( ( this -> pickingColorID ) , 0.0f , 0.0f , 0.0f , 1.0f ) ;

				this -> modelObject -> modelShaders -> useShader ( ) ;

				this -> modelObject -> modelTexture -> bindTexture ( ( this -> textureID ) , 0 ) ;

				this -> mvpMatrix = ( this -> inputObject -> getProjectionMatrix ( ) ) * ( this -> inputObject -> getViewMatrix ( ) ) * ( this -> modelMatrix ) ;
				glUniformMatrix4fv ( ( this -> matrixID ) , 1 , GL_FALSE , & ( ( this -> mvpMatrix ) [ 0 ] [ 0 ] ) ) ;

				this -> modelObject -> bindBuffers ( ) ;

				this -> modelObject -> drawObject ( ) ;
			}

	} ;

#endif