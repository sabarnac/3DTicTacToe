# ifndef TTT3D_LEVEL_CROSS_CUBE

	# define TTT3D_LEVEL_CROSS_CUBE 1

	# include <unistd.h>

	# include "../include/cpp/logging_handler.cpp"
	# include "../include/cpp/fps_counter.cpp"

	# include "level_base.cpp"

	# include "level_board_cube_objects/board_cube_empty.cpp"
	# include "level_board_cube_objects/board_cube_x.cpp"
	# include "level_board_cube_objects/board_cube_o.cpp"
	# include "level_board_cube_objects/board_cube_dash.cpp"

	struct cornerLinesCrossCubes {
		short int line1 ;
		short int line2 ;
		short int line3 ;
		short int line4 ;
	} ;

	struct singleLinesCrossCubes {
		short int line1 ;
	} ;

	class levelCrossCubeLocal : public levelBase {

		protected:

			fpsCounter                                * fpsObject                     ;
			levelBoardCubeBase               * board [ 3 ] [ 3 ] [ 3 ]       ;
			levelBoardCubeBase               * xCube [ 3 ] [ 3 ] [ 3 ]       ;
			levelBoardCubeBase               * oCube [ 3 ] [ 3 ] [ 3 ]       ;
			levelBoardCubeBase               * dashCube [ 3 ] [ 3 ] [ 3 ]    ;
			struct cornerLinesCrossCubes                cornerCubes [ 4 ]             ;
			struct singleLinesCrossCubes                singleCubes [ 6 ]             ;
			short unsigned int                          playerNo                      ;
			short int                                   totalPlayers                  ;
			double                                      gameEndTimerStart             ;

			void initializeMap ( ) {
				this -> logger -> logInfo ( "levelCrossCubeLocal : Initializing line points" ) ;
				for ( int i = 0 ; i < 4 ; i ++ ) {
					( this -> cornerCubes [ i ] ) . line1 = 0 ;
					( this -> cornerCubes [ i ] ) . line2 = 0 ;
					( this -> cornerCubes [ i ] ) . line3 = 0 ;
					( this -> cornerCubes [ i ] ) . line4 = 0 ;
				}

				for ( int i = 0 ; i < 6 ; i ++ )
					( this -> singleCubes [ i ] ) . line1 = 0 ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Initializing board" ) ;
				for ( int i = - 1 ; i < 2 ; i ++ ) {
					for ( int j = - 1 ; j < 2 ; j ++ ) {
						for ( int k = - 1 ; k < 2 ; k ++ ) {
							if ( ( j != 0 ) && ( ( i != 0 ) || ( k != 0 ) ) ) {
								this -> board [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeEmpty ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> board [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;

								this -> xCube [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeX ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> xCube [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;

								this -> oCube [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeO ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> oCube [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;

								this -> dashCube [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeDash ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> dashCube [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;
							}
							else if ( ( j == 0 ) && ( ( i == k ) || ( ( i + k ) == 0 ) ) ) {
								this -> board [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeEmpty ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> board [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;

								this -> xCube [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeX ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> xCube [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;

								this -> oCube [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeO ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> oCube [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;

								this -> dashCube [ 1 + i ] [ 1 + j ] [ 1 + k ] = new levelBoardCubeDash ( ( this -> inputObject ) , mat4 ( 1.0f ) , ( this -> logger ) ) ;
								this -> dashCube [ 1 + i ] [ 1 + j ] [ 1 + k ] -> translateModelMatrix ( vec3 ( ( 2 * i ) , ( 2 * j ) , ( 2 * k ) ) ) ;
							}
							else
								this -> board [ 1 + i ] [ 1 + j ] [ 1 + k ] = NULL ;
						}
					}
				}

				this -> logger -> logInfo ( "levelCrossCubeLocal : Setting total players to 2 and current player as player 1\n" ) ;
				this -> playerNo = 0 ;
			}

			void destroyMap ( ) {
				for ( int i = 0 ; i < 3 ; i ++ ) {
					for ( int j = 0 ; j < 3 ; j ++ ) {
						for ( int k = 0 ; k < 3 ; k ++ ) {
							if ( this -> board [ i ] [ j ] [ k ] != NULL ) {
								delete this -> board [ i ] [ j ] [ k ] ;
								delete this -> xCube [ i ] [ j ] [ k ] ;
								delete this -> oCube [ i ] [ j ] [ k ] ;
								delete this -> dashCube [ i ] [ j ] [ k ] ;
							}
						}
					}
				}
			}

			void drawMap ( ) {
				for ( int i = 0 ; i < 3 ; i ++ )
					for ( int j = 0 ; j < 3 ; j ++ )
						for ( int k = 0 ; k < 3 ; k ++ )
							if ( this -> board [ i ] [ j ] [ k ] != NULL )
								this -> board [ i ] [ j ] [ k ] -> updateAndDraw ( ) ;
			}

			void actionOnMap ( int * x , int * y , int * z ) {
				this -> logger -> logInfo ( "levelCrossCubeLocal : Initializing colorized board" ) ;
				for ( int i = 0 ; i < 3 ; i ++ )
					for ( int j = 0 ; j < 3 ; j ++ )
						for ( int k = 0 ; k < 3 ; k ++ )
							if ( this -> board [ i ] [ j ] [ k ] != NULL )
								this -> board [ i ] [ j ] [ k ] -> setPickingColor ( ( i + 15 ) , ( j + 15 ) , ( k + 15 ) ) ;
				glFlush ( ) ;
				glFinish ( ) ;
				glPixelStorei ( GL_UNPACK_ALIGNMENT , 1 ) ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Checking selected pixel" ) ;
				int pickedColor = this -> inputObject -> getPixelColorUnderMouse ( ) ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Mapping selected pixel color to its object" ) ;
				for ( int i = 0 ; i < 3 ; i ++ ) {
					for ( int j = 0 ; j < 3 ; j ++ ) {
						for ( int k = 0 ; k < 3 ; k ++ ) {
							int color = ( ( i + 15 ) + ( ( j + 15 ) * 256 ) + ( ( k + 15 ) * 256 * 256 ) ) ;
							if ( color == pickedColor ) {
								if ( this -> playerNo == 0 ) {
									levelBoardCubeBase * temp = this -> board [ i ] [ j ] [ k ] ;
									this -> board [ i ] [ j ] [ k ] = this -> xCube [ i ] [ j ] [ k ] ;
									this -> xCube [ i ] [ j ] [ k ] = temp ;
								}
								else if ( this -> playerNo == 1 ) {
									levelBoardCubeBase * temp = this -> board [ i ] [ j ] [ k ] ;
									this -> board [ i ] [ j ] [ k ] = this -> oCube [ i ] [ j ] [ k ] ;
									this -> oCube [ i ] [ j ] [ k ] = temp ;
								}
								else if ( this -> playerNo == 2 ) {
									levelBoardCubeBase * temp = this -> board [ i ] [ j ] [ k ] ;
									this -> board [ i ] [ j ] [ k ] = this -> dashCube [ i ] [ j ] [ k ] ;
									this -> dashCube [ i ] [ j ] [ k ] = temp ;
								}

								* x = i ;
								* y = j ;
								* z = k ;

								break ;
							}
						}
						if ( ( * x ) != ( - 5 ) )
							break ;
					}
					if ( ( * x ) != ( - 5 ) )
						break ;
				}

				this -> logger -> logInfo ( "levelCrossCubeLocal : Clear screen from colorized board\n" ) ;
				this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
			}

			int filledMap ( ) {
				for ( int i = 0 ; i < 3 ; i ++ )
					for ( int j = 0 ; j < 3 ; j ++ )
						for ( int k = 0 ; k < 3 ; k ++ )
							if ( this -> board [ i ] [ j ] [ k ] != NULL )
								if ( this -> board [ i ] [ j ] [ k ] -> getType ( ) == 0 )
									return 0 ;
				return 1 ;
			}

			int updateMap ( ) {
				this -> logger -> logInfo ( "levelCrossCubeLocal : Checking selected object" ) ;
				int x = ( - 5 ) , y = ( - 5 ) , z = ( - 5 ) ;
				this -> actionOnMap ( & x , & y , & z ) ;
				int incrementValue ;
				if ( this -> playerNo == 0 ) incrementValue = 1 ;
				else if ( this -> playerNo == 1 ) incrementValue = 4 ;
				else if ( this -> playerNo == 2 ) incrementValue = 13 ;
				if ( x != ( - 5 ) ) {
					this -> logger -> logInfo ( "levelCrossCubeLocal : Selected object found! Changed to non-selectable block" ) ;
					this -> logger -> logInfo ( "levelCrossCubeLocal : Modifying line points" ) ;
					if ( ( x == 0 ) && ( z == 0 ) )
						( this -> cornerCubes [ 0 ] ) . line1 += incrementValue ;
					if ( ( x == 0 ) && ( y == 0 ) )
						( this -> cornerCubes [ 0 ] ) . line2 += incrementValue ;
					if ( ( y == 0 ) && ( z == 0 ) )
						( this -> cornerCubes [ 0 ] ) . line3 += incrementValue ;
					if ( ( x == y ) && ( y == z ) )
						( this -> cornerCubes [ 0 ] ) . line4 += incrementValue ;

					if ( ( x == 2 ) && ( z == 2 ) )
						( this -> cornerCubes [ 1 ] ) . line1 += incrementValue ;
					if ( ( x == 2 ) && ( y ==  0 ) )
						( this -> cornerCubes [ 1 ] ) . line2 += incrementValue ;
					if ( ( y == 0 ) && ( z == 2 ) )
						( this -> cornerCubes [ 1 ] ) . line3 += incrementValue ;
					if ( ( x == ( 2 - y ) ) && ( ( 2 - y ) == z ) )
						( this -> cornerCubes [ 1 ] ) . line4 += incrementValue ;

					if ( ( x == 0 ) && ( z == 2 ) )
						( this -> cornerCubes [ 2 ] ) . line1 += incrementValue ;
					if ( ( x == 0 ) && ( y ==  2 ) )
						( this -> cornerCubes [ 2 ] ) . line2 += incrementValue ;
					if ( ( y == 2 ) && ( z == 2 ) )
						( this -> cornerCubes [ 2 ] ) . line3 += incrementValue ;
					if ( ( ( 2 - x ) == y ) && ( y == z ) )
						( this -> cornerCubes [ 2 ] ) . line4 += incrementValue ;

					if ( ( x == 2 ) && ( z == 0 ) )
						( this -> cornerCubes [ 3 ] ) . line1 += incrementValue ;
					if ( ( x == 2 ) && ( y ==  2 ) )
						( this -> cornerCubes [ 3 ] ) . line2 += incrementValue ;
					if ( ( y == 2 ) && ( z == 0 ) )
						( this -> cornerCubes [ 3 ] ) . line3 += incrementValue ;
					if ( ( x == y ) && ( y == ( 2 - z ) ) )
						( this -> cornerCubes [ 3 ] ) . line4 += incrementValue ;

					if ( ( x == 1 ) && ( y == z ) )
						( this -> singleCubes [ 0 ] ) . line1 += incrementValue ;

					if ( ( x == 1 ) && ( ( 2 - y ) == z ) )
						( this -> singleCubes [ 1 ] ) . line1 += incrementValue ;

					if ( ( y == 1 ) && ( x == z ) )
						( this -> singleCubes [ 2 ] ) . line1 += incrementValue ;

					if ( ( y == 1 ) && ( ( 2 - x ) == z ) )
						( this -> singleCubes [ 3 ] ) . line1 += incrementValue ;

					if ( ( z == 1 ) && ( x == y ) )
						( this -> singleCubes [ 4 ] ) . line1 += incrementValue ;

					if ( ( z == 1 ) && ( ( 2 - x ) == y ) )
						( this -> singleCubes [ 5 ] ) . line1 += incrementValue ;

					this -> logger -> logInfo ( "levelCrossCubeLocal : Checking whether any line points are filled" ) ;
					for ( int i = 0 ; i < 4 ; i ++ ) {
						if ( ( ( this -> cornerCubes [ i ] ) . line1 == ( incrementValue * 3 ) ) ||
						     ( ( this -> cornerCubes [ i ] ) . line2 == ( incrementValue * 3 ) ) ||
						     ( ( this -> cornerCubes [ i ] ) . line3 == ( incrementValue * 3 ) ) ||
							 ( ( this -> cornerCubes [ i ] ) . line4 == ( incrementValue * 3 ) ) ) {
							this -> logger -> logInfo ( "levelCrossCubeLocal : A player has won! Notify calling block\n" ) ;
							return 1 ;
						 }
					}

					for ( int i = 0 ; i < 6 ; i ++ ) {
						if ( ( this -> singleCubes [ i ] ) . line1 == ( incrementValue * 3 ) ) {
							this -> logger -> logInfo ( "levelCrossCubeLocal : A player has won! Notify calling block\n" ) ;
							return 1 ;
						}
					}

					this -> logger -> logInfo ( "levelCrossCubeLocal : No line pints filled. Switch to next players turn" ) ;
					this -> playerNo = ( ( this -> playerNo + 1 ) % ( this -> totalPlayers ) ) ;
				}

				this -> logger -> logInfo ( "levelCrossCubeLocal : No one has won yet. Going back" ) ;
				return 0 ;
			}

		public:

			levelCrossCubeLocal ( windowHandler * windowObject , inputHandler * inputObject , short int noOfPlayers = 2 , loggingHandler * logger = new loggingHandler )
				: levelBase ( windowObject , inputObject , logger ) {
				this -> logger -> logInfo ( "levelCrossCubeLocal : Initializing cross cube level\n" ) ;
				this -> fpsObject = new fpsCounter ( this -> logger ) ;
				this -> totalPlayers = noOfPlayers ;
				this -> gameEndTimerStart = ( - 1 ) ;
				this -> logger -> logInfo ( "levelCrossCubeLocal : Initialized cross cube level\n" ) ;
			}

			~levelCrossCubeLocal ( ) {
				delete this -> fpsObject ;
			}

			int main ( ) {
				this -> logger -> logInfo ( "levelCrossCubeLocal : Started cross cube board level" ) ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Setting up cube board" ) ;
				this -> inputObject -> resetValues ( ) ;
				this -> initializeMap ( ) ;
				int checkPress = 0 , checkEscape = 0 , checkSuccess = 0 ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Running frames" ) ;
				this -> fpsObject -> resetTimer ( ) ;
				double fps = 0 , spf = 0 ;
				char textMsg [ 100 ] ;
				do {
					this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

					this -> inputObject -> refreshInputsCameraToCenter ( ) ;

					if ( ( this -> inputObject -> checkMouseButtonPressed ( GLFW_MOUSE_BUTTON_LEFT ) ) && ( checkPress == 0 ) )
						checkPress = 1 ;
					
					if ( ( this -> inputObject -> checkMouseButtonReleased ( GLFW_MOUSE_BUTTON_LEFT ) ) && ( checkPress == 1 ) ) {
						this -> logger -> logInfo ( "levelCrossCubeLocal : Left click triggered! Check what was selected" ) ;
						checkPress = 0 ;

						checkSuccess = this -> updateMap ( ) ;
					}

					do {
						if ( checkSuccess == 1 ) {
							this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
							this -> inputObject -> refreshInputsCameraToCenter ( ) ;
							checkPress = 1 ;
							char textMsg [ 100 ] ;
							snprintf ( textMsg , 100 , "PLAYER %d WINS!!!" , ( ( this -> playerNo ) + 1 ) ) ;
							textObject -> printText2D ( textMsg , 459 , 10 , 32 ) ;

							char msg [ 200 ] ;
							snprintf ( msg , 200 , "levelCrossCubeLocal : Player %d wins!" , ( ( this -> playerNo ) + 1 ) ) ;
							this -> logger -> logInfo ( msg ) ;

							if ( this -> gameEndTimerStart == ( - 1 ) ) this -> gameEndTimerStart = glfwGetTime ( ) ;
							else if ( ( glfwGetTime ( ) - ( this -> gameEndTimerStart ) ) > 5.0f )
								break ;
						}
						else if ( this -> filledMap ( ) ) {
							this -> windowObject -> clearScreen ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
							this -> inputObject -> refreshInputsCameraToCenter ( ) ;
							textObject -> printText2D ( "NO PLAYER WINS!!!" , 459 , 50 , 32 ) ;

							this -> logger -> logInfo ( "levelCrossCubeLocal : No player wins" ) ;

							if ( this -> gameEndTimerStart == ( - 1 ) ) this -> gameEndTimerStart = glfwGetTime ( ) ;
							else if ( ( glfwGetTime ( ) - ( this -> gameEndTimerStart ) ) > 5.0f )
								break ;
						}
						else {
							char textMsg [ 100 ] ;
							snprintf ( textMsg , 100 , "PLAYER %d's TURN" , ( ( this -> playerNo ) + 1 ) ) ;
							textObject -> printText2D ( textMsg , 459 , 50 , 32 ) ;
						}

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

						this -> drawMap ( ) ;

						this -> inputObject -> waitForEventsTillTimeout ( ) ;
					}
					while ( this -> gameEndTimerStart != ( - 1 ) ) ;
					if ( this -> gameEndTimerStart != ( - 1 ) )
						break ;

					if ( this -> inputObject -> checkKeyPressed ( GLFW_KEY_ESCAPE ) && ( checkEscape == 0 ) )
						checkEscape = 1 ;
				}
				while ( ( ( ! this -> inputObject -> checkKeyReleased ( GLFW_KEY_ESCAPE ) ) || ( checkEscape == 0 ) ) && ( this -> inputObject -> checkWindowBeingClosed ( ) == 0 ) ) ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Destroying cube board" ) ;
				this -> destroyMap ( ) ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Resetting camera and inputs" ) ;
				this -> inputObject -> resetValues ( ) ;

				this -> logger -> logInfo ( "levelCrossCubeLocal : Ended the cross cube level. Exiting\n" ) ;
				return 0 ;
			}

	} ;

#endif