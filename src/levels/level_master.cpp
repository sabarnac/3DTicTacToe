# ifndef TTT3D_LEVEL_MASTER

	# define TTT3D_LEVEL_MASTER 1

	# include <unistd.h>

	# include "../include/cpp/logging_handler.cpp"
	# include "../include/cpp/fps_counter.cpp"

	# include "level_base.cpp"

	# include "level_player_type.cpp"

	# include "level_master_objects/button_game_title.cpp"
	# include "level_master_objects/button_play.cpp"
	# include "level_master_objects/button_exit.cpp"

	class levelMaster : public levelBase {

		protected:

			fpsCounter                                * fpsObject                     ;
			levelMasterButtonBase                     * gameMenu [ 3 ]                ;

			void initializeMenu ( ) {
				this -> logger -> logInfo ( "levelMaster : Initializing menu" ) ;
				this -> gameMenu [ 0 ] = new levelMasterGameTitle ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 0 ] -> scaleModelMatrix ( vec3 ( 2.5 , 2.5 , 2.5 ) ) ;
				this -> gameMenu [ 0 ] -> translateModelMatrix ( vec3 ( 0 , 2.5 , 0 ) ) ;

				this -> gameMenu [ 1 ] = new levelMasterButtonPlay ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 1 ] -> translateModelMatrix ( vec3 ( 0 , 0 , 0 ) ) ;

				this -> gameMenu [ 2 ] = new levelMasterButtonExit ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
				this -> gameMenu [ 2 ] -> translateModelMatrix ( vec3 ( 0 , ( - 5 ) , 0 ) ) ;
			}

			void destroyMenu ( ) {
				for ( int i = 0 ; i < 3 ; i ++ )
					delete this -> gameMenu [ i ] ;
			}

			void drawMenu ( ) {
				for ( int i = 0 ; i < 3 ; i ++ )
					this -> gameMenu [ i ] -> updateAndDraw ( ) ;
			}

			int actionOnMenu ( ) {
				int tmp = 0 ;

				this -> logger -> logInfo ( "levelMaster : Initializing colorized menu" ) ;
				this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
				for ( int i = 1 ; i < 3 ; i ++ )
					this -> gameMenu [ i ] -> setPickingColor ( ( i * 5 ) , ( i * 5 ) , ( i * 5 ) ) ;
				glFlush ( ) ;
				glFinish ( ) ;
				glPixelStorei ( GL_UNPACK_ALIGNMENT , 1 ) ;

				this -> logger -> logInfo ( "levelMaster : Checking selected pixel" ) ;
				int pickedColor = this -> inputObject -> getPixelColorUnderMouse ( ) ;

				this -> logger -> logInfo ( "levelMaster : Mapping selected pixel color to its menu button" ) ;
				short int x = ( pickedColor / ( 256 * 256 ) ) / 5 ;
				pickedColor = pickedColor % ( 256 * 256 ) ;
				short int y = ( pickedColor / ( 256 ) ) / 5 ;
				pickedColor = pickedColor % ( 256 ) ;
				short int z = pickedColor / 5 ;
				if ( ( x != 0 ) && ( x == y ) && ( x == z ) ) tmp = x ;

				this -> logger -> logInfo ( "levelMaster : Clear screen from colorized menu\n" ) ;
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
					checkSelection = 2 ;
				}

				if ( ( this -> inputObject -> checkMouseButtonPressed ( GLFW_MOUSE_BUTTON_LEFT ) ) && ( checkPress == 0 ) )
					checkPress = 1 ;
				else if ( ( this -> inputObject -> checkMouseButtonReleased ( GLFW_MOUSE_BUTTON_LEFT ) ) && ( checkPress == 1 ) ) {
					this -> logger -> logInfo ( "levelMaster : Left click triggered! Check what was selected" ) ;
					checkPress = 0 ;
					checkSelection = this -> actionOnMenu ( ) ;
				}
				return checkSelection ;
			}

		public:

			levelMaster ( windowHandler * windowObject , inputHandler * inputObject , loggingHandler * logger = new loggingHandler )
				: levelBase ( windowObject , inputObject , logger ) {
				this -> fpsObject = new fpsCounter ( this -> logger ) ;
				this -> logger -> logInfo ( "levelMaster : Initialized main menu master level\n" ) ;
			}

			~levelMaster ( ) {
				delete this -> fpsObject ;
			}

			int main ( ) {
				this -> logger -> logInfo ( "levelMaster : Started main menu master level" ) ;

				this -> logger -> logInfo ( "levelMaster : Setting up main menu" ) ;
				this -> inputObject -> resetValues ( ) ;
				this -> initializeMenu ( ) ;

				this -> logger -> logInfo ( "levelMaster : Running frames" ) ;
				this -> fpsObject -> resetTimer ( ) ;
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
						
						this -> logger -> logInfo ( "levelMaster : User wants to play! Ask user no of players playing" ) ;
						levelBase * levelPType = new levelPlayerType ( ( this -> windowObject ) , ( this -> inputObject ) , ( this -> logger ) ) ;
						levelPType -> main ( ) ;
						delete levelPType ;
						this -> fpsObject -> resetTimer ( ) ;
						continue ;
					}
					else if ( checkSelection == 2 ) {
						glFlush ( ) ;
						glFinish ( ) ;
						this -> inputObject -> pollEvents ( ) ;
						this -> logger -> logInfo ( "levelMaster : User wants to exit. Ending main menu master level" ) ;
						break ;
					}

					this -> drawMenu ( ) ;

					this -> inputObject -> waitForEventsTillTimeout ( ) ;
				}
				while ( 1 ) ;

				this -> logger -> logInfo ( "levelMaster : Destroying main menu" ) ;
				this -> destroyMenu ( ) ;

				this -> logger -> logInfo ( "levelMaster : Resetting camera and inputs" ) ;
				this -> inputObject -> resetValues ( ) ;

				this -> logger -> logInfo ( "levelMaster : Ended the main menu master level. Exiting\n" ) ;
				return 0 ;
			}

	} ;

#endif