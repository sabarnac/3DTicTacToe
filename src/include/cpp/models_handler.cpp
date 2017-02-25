# ifndef GL_MODEL_OBJECT_HANDLER

	# define GL_MODEL_OBJECT_HANDLER 1

	# include <string.h>
	# include <map>
	# include <GL/glew.h>
	# include <glm/glm.hpp>

	# include "logging_handler.cpp"
	# include "structure_handler.cpp"
	# include "texture_handler.cpp"
	# include "shaders_handler.cpp"

	using namespace glm ;
	using namespace std ;

	struct modelDetailsInfo {
		short int                                    modelInvertUVs                   ;
		char                                         modelStructureNo [ 100 ]          ;
		char                                         modelTextureNo [ 100 ]            ;
		char                                         modelShadersNo [ 100 ]            ;
	} ;

	class modelObjectsHandler ;

	map < string , modelObjectsHandler * > modelObjectsList ;
	map < string , int > modelObjectsListCount ;

	class modelObjectsHandler {

		protected:

			string                                      fileNo                        ;

			loggingHandler                            * logger                        ;

		public:

			modelStructureHandler                     * modelStructure                ;
			modelTextureHandler                       * modelTexture                  ;
			modelShadersHandler                       * modelShaders                  ;

			modelObjectsHandler ( const char * fileNo , loggingHandler * logger = new loggingHandler ) {
				this -> logger = logger ;

				char logMsg [ 200 ] = "modelObjectsHandler : Initializing the model objects handler \'" ;
				strcat ( logMsg , fileNo ) ;
				strcat ( logMsg , "\'" ) ;
				this -> logger -> logInfo ( logMsg ) ;

				this -> logger -> logInfo ( "modelObjectsHandler : Retrieving object details from file" ) ;
				char objectsFilePath [ 500 ] = "models/models-" ;
				strcat ( objectsFilePath , fileNo ) ;
				strcat ( objectsFilePath , ".md" ) ;
				
				struct modelDetailsInfo objectDetails ;
				FILE * objectFile = fopen ( objectsFilePath , "rb" ) ;
				if ( objectFile == NULL ) {
					this -> logger -> logInfo ( "modelObjectsHandler : Could not open object file. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}
				fread ( & objectDetails , sizeof ( objectDetails ) , 1 , objectFile ) ;
				fclose ( objectFile ) ;

				this -> logger -> logInfo ( "modelObjectsHandler : Initializing model object structure" ) ;
				this -> modelStructure = modelStructureHandler :: checkStructure ( objectDetails . modelStructureNo ) ;
				if ( this -> modelStructure == NULL )
					this -> modelStructure = new modelStructureHandler ( ( objectDetails . modelStructureNo ) , ( modelStructureHandler :: STRUCT_OBJ ) , ( objectDetails . modelInvertUVs ) , ( this -> logger ) ) ;
				else
					modelStructuresListCount [ fileNo ] ++ ;

				this -> logger -> logInfo ( "modelObjectsHandler : Initializing model object texture" ) ;
				this -> modelTexture = modelTextureHandler :: checkTexture ( objectDetails . modelTextureNo ) ;
				if ( this -> modelTexture == NULL ) {
					if ( objectDetails . modelInvertUVs == 1 )
						this -> modelTexture = new modelTextureHandler ( ( objectDetails . modelTextureNo ) , ( modelTextureHandler :: IMG_DDS ) , ( this -> logger ) ) ;
					else
						this -> modelTexture = new modelTextureHandler ( ( objectDetails . modelTextureNo ) , ( modelTextureHandler :: IMG_BMP ) , ( this -> logger ) ) ;
				}
				else
					modelTexturesListCount [ fileNo ] ++ ;

				this -> logger -> logInfo ( "modelObjectsHandler : Initializing model object shaders" ) ;
				this -> modelShaders = modelShadersHandler :: checkShaders ( objectDetails . modelShadersNo ) ;
				if ( this -> modelShaders == NULL )
					this -> modelShaders = new modelShadersHandler ( ( objectDetails . modelShadersNo ) , ( this -> logger ) ) ;
				else
					modelShadersListCount [ fileNo ] ++ ;

				this -> logger -> logInfo ( "modelObjectsHandler : Adding current object to the list of available objects to use directly" ) ;
				modelObjectsList [ fileNo ] = this ;
				modelObjectsListCount [ fileNo ] = 1 ;
				this -> fileNo = fileNo ;

				this -> logger -> logInfo ( "modelObjectsHandler : Initialized the model objects handler\n" ) ;
			}

			~modelObjectsHandler ( ) {
				modelObjectsList . erase ( this -> fileNo ) ;
				modelObjectsListCount . erase ( this -> fileNo ) ;

				char objectsFilePath [ 500 ] = "models/models-" ;
				strcat ( objectsFilePath , ( ( this -> fileNo ) . c_str ( ) ) ) ;
				strcat ( objectsFilePath , ".md" ) ;
				
				struct modelDetailsInfo objectDetails ;
				FILE * objectFile = fopen ( objectsFilePath , "rb" ) ;
				if ( objectFile == NULL ) {
					this -> logger -> logInfo ( "modelObjectsHandler : Could not open object file. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}
				fread ( & objectDetails , sizeof ( objectDetails ) , 1 , objectFile ) ;
				fclose ( objectFile ) ;

				if ( ( -- modelStructuresListCount [ objectDetails . modelStructureNo ] ) == 0 )
					delete modelStructuresList [ objectDetails . modelStructureNo ] ;

				if ( ( -- modelTexturesListCount [ objectDetails . modelTextureNo ] ) == 0 )
					delete modelTexturesList [ objectDetails . modelTextureNo ] ;

				if ( ( -- modelShadersListCount [ objectDetails . modelShadersNo ] ) == 0 )
					delete modelShadersList [ objectDetails . modelShadersNo ] ;
			}

			void bindBuffers ( ) {
				this -> modelStructure -> bindVertexBuffer ( ) ;
				this -> modelStructure -> bindUVBuffer ( ) ;
				this -> modelStructure -> bindNormalBuffer ( ) ;
				this -> modelStructure -> bindIndicesBuffer ( ) ;
			}

			void drawObject ( ) {
				this -> modelStructure -> drawStructure ( ) ;
			}

			static modelObjectsHandler * checkObjects ( const char * fileNo ) {
				if ( modelObjectsList . count ( fileNo ) == 0 )
					return NULL ;
				else
					return modelObjectsList [ fileNo ] ;
			}

	} ;

#endif