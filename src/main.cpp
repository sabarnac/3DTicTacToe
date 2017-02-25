char gameVersion [ ] = "0.0.1" ;

# include "include/cpp/logging_handler.cpp"
# include "include/cpp/window_handler.cpp"
# include "include/cpp/input_handler.cpp"
# include "include/cpp/text_handler.cpp"

# include "levels/level_master.cpp"

int main ( ) {
	fileLoggingHandler * logger = new fileLoggingHandler ;
	logger -> logInfo ( "MAIN : Creating window handler" ) ;
	windowHandler * windowObject = new windowHandler ( "Tic Tac To3D" , logger ) ;

	logger -> logInfo ( "MAIN : Creating input handler" ) ;
	inputHandler * inputObject = new inputHandler ( { vec3 ( 0 , 0 , 20 ) , 0.0 , 0.0 , 45.0 , 0.0 , 3 } , logger ) ;

	logger -> logInfo ( "MAIN : Creating 2D text handler" ) ;
	textObject = new textHandler ( logger ) ;

	logger -> logInfo ( "MAIN : Setting clear screen color" ) ;
	windowObject -> setClearScreenColor ( vec4 ( 0.0f , 0.0f , 0.0f , 0.0f ) ) ;

	logger -> logInfo ( "MAIN : Enabling depth testing and face culling" ) ;
	windowObject -> enableDepthTest ( ) ;
	windowObject -> enableFaceCulling ( ) ;
	windowObject -> enableMSAA ( ) ;

	logger -> logInfo ( "MAIN : Setting input modes" ) ;
	inputObject -> setInputMode ( GLFW_STICKY_KEYS , GL_TRUE ) ;
	inputObject -> setInputMode ( GLFW_STICKY_MOUSE_BUTTONS , GL_TRUE ) ;
    
	logger -> logInfo ( "MAIN : Doing a quick refresh on inputs" ) ;
    inputObject -> pollEvents ( ) ;
	inputObject -> resetValues ( ) ;
    inputObject -> refreshInputs ( ) ;

	logger -> logInfo ( "MAIN : Initializing master level" ) ;
	levelMaster * masterLevel = new levelMaster ( windowObject , inputObject , logger ) ;
	logger -> logInfo ( "MAIN : Executing master level" ) ;
	masterLevel -> main ( ) ;

	logger -> logInfo ( "MAIN : Freeing memory" ) ;
	delete masterLevel ;
	delete textObject ;
	delete inputObject ;
	delete windowObject ;

	logger -> logInfo ( "MAIN : Exiting\n" ) ;
	delete logger ;

	return 0 ;
}