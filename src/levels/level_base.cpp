# ifndef TTT3D_LEVEL_BASE

	# define TTT3D_LEVEL_BASE 1

	class levelBase {

		protected:

			windowHandler                             * windowObject                  ;
			inputHandler                              * inputObject                   ;

			loggingHandler                            * logger                        ;

		public:

			levelBase ( windowHandler * windowObject , inputHandler * inputObject , loggingHandler * logger = new loggingHandler ) {
				this -> windowObject = windowObject ;
				this -> inputObject = inputObject ;
				this -> logger = logger ;
				this -> logger -> logInfo ( "levelBase : Initialized level base" ) ;
			}

			virtual int main ( ) { return 0 ; }
	} ;

#endif
