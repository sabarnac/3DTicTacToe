# ifndef TTT3D_LEVEL_BOARD_TYPE_BUTTON_BACK

	# define TTT3D_LEVEL_BOARD_TYPE_BUTTON_BACK 1

	# include "../../include/cpp/logging_handler.cpp"
	# include "../../include/cpp/models_handler.cpp"

	# include "button_base.cpp"

	class levelBoardTypeButtonBack : public levelBoardTypeButtonBase {

		public:

			levelBoardTypeButtonBack ( inputHandler * inputObject , mat4 modelMatrix = mat4 ( 1.0f ) , loggingHandler * logger = new loggingHandler ) : levelBoardTypeButtonBase ( "button-back-game" , logger ) {
				this -> inputObject = inputObject ;
				
				this -> logger -> logInfo ( "levelBoardTypeButtonBack : Initializing back button" ) ;
				this -> matrixID = this -> modelObject -> modelShaders -> getShaderVarHandle ( "MVP" ) ;
				this -> textureID  = this -> modelObject -> modelShaders -> getShaderVarHandle ( "textureSampler" ) ;
				this -> pickingColorID  = this -> modelObject -> modelShaders -> getShaderVarHandle ( "pickingColor" ) ;

				this -> baseModelMatrix = this -> modelMatrix = modelMatrix ;

				this -> mvpMatrix = ( this -> inputObject -> getProjectionMatrix ( ) ) * ( this -> inputObject -> getViewMatrix ( ) ) * ( this -> modelMatrix ) ;
				this -> logger -> logInfo ( "levelBoardTypeButtonBack : Initialized back button\n" ) ;
			}

			void resetModelMatrix ( ) {
				this -> modelMatrix = this -> baseModelMatrix ;
			}

			void setModelMatrix ( mat4 newModelMatrix ) {
				this -> modelMatrix = newModelMatrix ;
			}

			void scaleModelMatrix ( vec3 scaleFactor ) {
				this -> modelMatrix = scale ( ( this -> modelMatrix ) , scaleFactor ) ;
			}

			void rotateModelMatrix ( float rotationFactor , vec3 rotationAxis ) {
				this -> modelMatrix = rotate ( ( this -> modelMatrix ) , radians ( rotationFactor ) , rotationAxis ) ;
			}

			void translateModelMatrix ( vec3 translationFactor ) {
				this -> modelMatrix = translate ( ( this -> modelMatrix ) , translationFactor ) ;
			}

			mat4 getMVPMatrix ( vec3 translationFactor ) {
				return this -> mvpMatrix ;
			}

			void setPickingColor ( int r , int g , int b , int a = 255 ) {
				glUniform4f ( ( this -> pickingColorID ) , ( r / 255.0f ) , ( g / 255.0f ) , ( b / 255.0f ) , ( a / 255.0f ) ) ;

				this -> modelObject -> modelShaders -> useShader ( ) ;

				this -> modelObject -> modelTexture -> bindTexture ( ( this -> textureID ) , 0 ) ;

				this -> mvpMatrix = ( this -> inputObject -> getProjectionMatrix ( ) ) * ( this -> inputObject -> getViewMatrix ( ) ) * ( this -> modelMatrix ) ;
				glUniformMatrix4fv ( ( this -> matrixID ) , 1 , GL_FALSE , & ( ( this -> mvpMatrix ) [ 0 ] [ 0 ] ) ) ;

				this -> modelObject -> bindBuffers ( ) ;

				this -> modelObject -> drawObject ( ) ;
			}

			int getType ( ) {
				return 0 ;
			}

	} ;

#endif