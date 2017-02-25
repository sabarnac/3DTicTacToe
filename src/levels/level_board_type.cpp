# ifndef TTT3D_LEVEL_BOARD_TYPE

	# define TTT3D_LEVEL_BOARD_TYPE 1

	# include <unistd.h>

	# include "../include/cpp/logging_handler.cpp"
	# include "../include/cpp/fps_counter.cpp"

	# include "level_base.cpp"

	# include "level_full_cube_local.cpp"
	# include "level_cross_cube_local.cpp"
	# include "level_outer_cube_local.cpp"

	# include "level_board_type_objects/button_game_title.cpp"
	# include "level_board_type_objects/button_game_full_cube.cpp"
	# include "level_board_type_objects/button_game_cross_cube.cpp"
	# include "level_board_type_objects/button_game_outer_cube.cpp"
	# include "level_board_type_objects/button_back.cpp"

	class levelBoardType : public levelBase {

		protected:

			fpsCounter                                * fpsObject                     ;
			levelBoardTypeButtonBase                  * gameMenu [ 5 ]                ;
			short int                                   totalPlayers                  ;

			void initializeMenu ( ) {
				this -> logger -> logInfo ( "levelBoardType : Initializing menu" ) ;
				this -> gameMenu [ 0 ] = new levelBoardTypeGameTitle ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 0 ] -> scaleModelMatrix ( vec3 ( 2.5 , 2.5 , 2.5 ) ) ;
				this -> gameMenu [ 0 ] -> translateModelMatrix ( vec3 ( 0 , 2.5 , 0 ) ) ;

				this -> gameMenu [ 1 ] = new levelBoardTypeButtonCrossCube ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 1 ] -> translateModelMatrix ( vec3 ( 0 , 1 , 0 ) ) ;

				this -> gameMenu [ 2 ] = new levelBoardTypeButtonOuterCube ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 2 ] -> translateModelMatrix ( vec3 ( 0 , ( - 2 ) , 0 ) ) ;

				this -> gameMenu [ 3 ] = new levelBoardTypeButtonFullCube ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 3 ] -> translateModelMatrix ( vec3 ( 0 , ( - 5 ) , 0 ) ) ;

				this -> gameMenu [ 4 ] = new levelBoardTypeButtonBack ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 4 ] -> translateModelMatrix ( vec3 ( 0 , ( - 8 ) , 0 ) ) ;
			}

			void destroyMenu ( ) {
				for ( int i = 0 ; i < 5 ; i ++ )
					delete this -> gameMenu [ i ] ;
			}

			void drawMenu ( ) {
				for ( int i = 0 ; i < 5 ; i ++ )
					this -> gameMenu [ i ] -> updateAndDraw ( ) ;
			}

			int actionOnMenu ( ) {
				int tmp = 0 ;

				this -> logger -> logInfo ( "levelBoardType : Initializing colorized menu" ) ;
				this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
				for ( int i = 1 ; i < 5 ; i ++ )
					this -> gameMenu [ i ] -> setPickingColor ( ( i * 5 ) , ( i * 5 ) , ( i * 5 ) ) ;
				glFlush ( ) ;
				glFinish ( ) ;
				glPixelStorei ( GL_UNPACK_ALIGNMENT , 1 ) ;

				this -> logger -> logInfo ( "levelBoardType : Checking selected pixel" ) ;
				int pickedColor = this -> inputObject -> getPixelColorUnderMouse ( ) ;

				this -> logger -> logInfo ( "levelBoardType : Mapping selected pixel color to its menu button" ) ;
				short int x = ( pickedColor / ( 256 * 256 ) ) / 5 ;
				pickedColor = pickedColor % ( 256 * 256 ) ;
				short int y = ( pickedColor / ( 256 ) ) / 5 ;
				pickedColor = pickedColor % ( 256 ) ;
				short int z = pickedColor / 5 ;
				if ( ( x != 0 ) && ( x == y ) && ( x == z ) ) tmp = x ;

				this -> logger -> logInfo ( "levelBoardType : Clear screen from colorized menu\n" ) ;
				this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

				return tmp ;
			}

			int checkInputs ( ) {
				static int checkPress = 0 , checkEscape = 0 , checkSelection ;
				checkSelection = 0 ;
				this -> inputObject -> refreshInputs ( ) ;

				if ( this -> inputObject -> checkKeyPressed ( GLFW_KEY_ESCAPE ) && ( checkEscape == 0 ) )
					checkEscape = 1 ;
				else if ( ( ( this -> inputObject -> checkKeyReleased ( GLFW_KEY_ESCAPE ) ) && ( checkEscape == 1 ) ) || ( this -> inputObject -> checkWindowBeingClosed ( ) == 1 ) ) {
					checkEscape = 0 ;
					checkSelection = 4 ;
				}

				if ( ( this -> inputObject -> checkMouseButtonPressed ( GLFW_MOUSE_BUTTON_LEFT ) ) && ( checkPress == 0 ) )
					checkPress = 1 ;
				else if ( ( this -> inputObject -> checkMouseButtonReleased ( GLFW_MOUSE_BUTTON_LEFT ) ) && ( checkPress == 1 ) ) {
					this -> logger -> logInfo ( "levelBoardType : Left click triggered! Check what was selected" ) ;
					checkPress = 0 ;
					checkSelection = this -> actionOnMenu ( ) ;
				}
				return checkSelection ;
			}

		public:

			levelBoardType ( windowHandler * windowObject , inputHandler * inputObject , short int noOfPlayers = 2 , loggingHandler * logger = new loggingHandler )
				: levelBase ( windowObject , inputObject , logger ) {
				this -> totalPlayers = noOfPlayers ;
				this -> fpsObject = new fpsCounter ( this -> logger ) ;
				this -> logger -> logInfo ( "levelBoardType : Initialized board type menu level\n" ) ;
			}

			~levelBoardType ( ) {
				delete this -> fpsObject ;
			}

			int main ( ) {
				this -> logger -> logInfo ( "levelBoardType : Started board type menu level" ) ;

				this -> logger -> logInfo ( "levelBoardType : Setting up board type menu" ) ;
				this -> inputObject -> resetValues ( ) ;
				this -> initializeMenu ( ) ;

				this -> logger -> logInfo ( "levelBoardType : Running frames" ) ;
				double fps = 0 , spf = 0 ;
				char textMsg [ 100 ] ;
				int checkSelection ;
				do {
					this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
					checkSelection = this -> checkInputs ( ) ;

					if ( this -> fpsObject -> checkSecondPassed ( ) ) {
						this -> fpsObject -> setFPSAndSPF ( ) ;
						fps = this -> fpsObject -> getFPS ( ) ;
						spf = this -> fpsObject -> getSPF ( ) ;
						fprintf ( stdout , "FPS : %f\n" , fps ) ;
						fprintf ( stdout , "SPF : %f\n\n" , spf ) ;
					}
					else this -> fpsObject -> incrementFrames ( ) ;
					snprintf ( textMsg , 100 , "FPS : %f" , fps ) ;
					textObject -> printText2D ( textMsg , 10 , 740 , 20 ) ;
					snprintf ( textMsg , 100 , "SPF : %f" , spf ) ;
					textObject -> printText2D ( textMsg , 10 , 715 , 20 ) ;

					if ( checkSelection == 1 ) {
						glFlush ( ) ;
						glFinish ( ) ;
						this -> inputObject -> pollEvents ( ) ;
						
						this -> logger -> logInfo ( "levelBoardType : User selected board type! Starting game" ) ;
						levelBase * levelBoard = new levelCrossCubeLocal ( ( this -> windowObject ) , ( this -> inputObject ) , ( this -> totalPlayers ) , ( this -> logger ) ) ;
						levelBoard -> main ( ) ;
						delete levelBoard ;
						checkSelection = 0 ;
						break ;
					}
					else if ( checkSelection == 2 ) {
						glFlush ( ) ;
						glFinish ( ) ;
						this -> inputObject -> pollEvents ( ) ;
						
						this -> logger -> logInfo ( "levelBoardType : User selected board type! Starting game" ) ;
						levelBase * levelBoard = new levelOuterCubeLocal ( ( this -> windowObject ) , ( this -> inputObject ) , ( this -> totalPlayers ) , ( this -> logger ) ) ;
						levelBoard -> main ( ) ;
						delete levelBoard ;
						checkSelection = 0 ;
						break ;
					}
					else if ( checkSelection == 3 ) {
						glFlush ( ) ;
						glFinish ( ) ;
						this -> inputObject -> pollEvents ( ) ;
						
						this -> logger -> logInfo ( "levelBoardType : User selected board type! Starting game" ) ;
						levelBase * levelBoard = new levelFullCubeLocal ( ( this -> windowObject ) , ( this -> inputObject ) , ( this -> totalPlayers ) , ( this -> logger ) ) ;
						levelBoard -> main ( ) ;
						delete levelBoard ;
						checkSelection = 0 ;
						break ;
					}
					else if ( checkSelection == 4 ) {
						glFlush ( ) ;
						glFinish ( ) ;
						this -> inputObject -> pollEvents ( ) ;
						this -> logger -> logInfo ( "levelBoardType : User wants to exit. Ending board type menu level" ) ;
						break ;
					}

					this -> drawMenu ( ) ;

					this -> inputObject -> waitForEventsTillTimeout ( ) ;
				}
				while ( 1 ) ;

				this -> logger -> logInfo ( "levelBoardType : Destroying board type menu" ) ;
				this -> destroyMenu ( ) ;

				this -> logger -> logInfo ( "levelBoardType : Resetting camera and inputs" ) ;
				this -> inputObject -> resetValues ( ) ;

				this -> logger -> logInfo ( "levelBoardType : Ended the board type menu level. Exiting\n" ) ;
				return 0 ;
			}

	} ;

#endif