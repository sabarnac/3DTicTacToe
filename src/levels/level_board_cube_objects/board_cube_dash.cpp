# ifndef TTT3D_LEVEL_BOARD_CUBE_DASH

	# define TTT3D_LEVEL_BOARD_CUBE_DASH 1

	# include "../../include/cpp/logging_handler.cpp"
	# include "../../include/cpp/models_handler.cpp"

	# include "board_cube_base.cpp"

	class levelBoardCubeDash : public levelBoardCubeBase {

		public:

			levelBoardCubeDash ( inputHandler * inputObject , mat4 modelMatrix = mat4 ( 1.0f ) , loggingHandler * logger = new loggingHandler ) : levelBoardCubeBase ( "board-cube-dash" , logger ) {
				this -> inputObject = inputObject ;
				
				this -> logger -> logInfo ( "levelBoardCubeDash : Initializing - board cube" ) ;
				this -> matrixID = this -> modelObject -> modelShaders -> getShaderVarHandle ( "MVP" ) ;
				this -> textureID  = this -> modelObject -> modelShaders -> getShaderVarHandle ( "textureSampler" ) ;
				this -> pickingColorID  = this -> modelObject -> modelShaders -> getShaderVarHandle ( "pickingColor" ) ;

				this -> baseModelMatrix = this -> modelMatrix = modelMatrix ;

				this -> mvpMatrix = ( this -> inputObject -> getProjectionMatrix ( ) ) * ( this -> inputObject -> getViewMatrix ( ) ) * ( this -> modelMatrix ) ;
				this -> logger -> logInfo ( "levelBoardCubeDash : Initialized - board cube\n" ) ;
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

			int getType ( ) {
				return 3 ;
			}

	} ;

#endif