# ifndef GL_MODEL_TEXTURE_HANDLER

	# define GL_MODEL_TEXTURE_HANDLER 1

	#define FOURCC_DXT1 0x31545844
	#define FOURCC_DXT3 0x33545844
	#define FOURCC_DXT5 0x35545844

	# include <string>
	# include <string.h>
	# include <map>
	# include <GL/glew.h>

	# include "logging_handler.cpp"

	using namespace std ;

	struct activeTextureDetails {
		GLuint                                          activeTextureID               ;
		GLuint                                          activeTextureNo               ;
		GLuint                                          activeTextureHandler          ;
	} ;

	class modelTextureHandler ;

	map < string , modelTextureHandler * > modelTexturesList ;
	map < string , int > modelTexturesListCount ;

	class modelTextureHandler {

		protected:
			
			static struct activeTextureDetails          activeTexture                 ;
			GLuint                                      modelTextureID                ;
			string                                      fileNo                        ;
			
			loggingHandler                            * logger                        ;

			int loadBMP ( const char * filePath ) {
				unsigned char header [ 54 ] , * data ;
				unsigned int dataPos , imageSize , width , height ;

				FILE *fp ;
				fp = fopen ( filePath , "rb" ) ;
				if ( fp == NULL )
					return ( - 1 ) ;

				if ( fread ( header , 1 , 54 , fp ) != 54 )
					return ( - 1 ) ;
				if ( ( header [ 0 ] != 'B' ) || ( header [ 1 ] != 'M' ) )
					return ( - 1 ) ;
				if ( * ( int * ) & ( header [ 0x1E ] ) != 0 )
					return ( - 1 ) ;
				if ( * ( int * ) & ( header [ 0x1C ] ) != 24 )
					return ( - 1 ) ;

				dataPos = * ( int * ) & ( header [ 0x0A ] ) ;
				imageSize  = * ( int * ) & ( header [ 0x22 ] ) ;
				width = * ( int * ) & ( header [ 0x12 ] ) ;
				height = * ( int * ) & ( header [ 0x16 ] ) ;

				if ( imageSize == 0 ) imageSize = width * height * 3 ;
				if ( dataPos == 0 ) dataPos = 54 ;

				data = new unsigned char [ imageSize ] ;

				fread ( data , 1 , imageSize , fp ) ;

				fclose ( fp ) ;

				glGenTextures ( 1 , & ( this -> modelTextureID ) ) ;
				glBindTexture ( GL_TEXTURE_2D , ( this -> modelTextureID ) ) ;
				glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

				delete data ;

				glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
				glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ;
				glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;
				glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR ) ;
				glGenerateMipmap ( GL_TEXTURE_2D ) ;

				return 0 ;
			}

			int loadDDS ( const char * filePath ) {
				unsigned char header [ 124 ] ;

				FILE *fp ;
				fp = fopen ( filePath , "rb" ) ;
				if ( fp == NULL ) {
					return ( - 1 ) ;
				}
			
				char filecode [ 4 ] ;
				fread ( filecode , 1 , 4 , fp ) ;
				if ( strncmp ( filecode , "DDS " , 4 ) != 0 ) {
					fclose ( fp ) ;
					return ( - 1 ) ;
				}
				
				fread ( & header , 124 , 1 , fp ) ;

				unsigned int height = * ( unsigned int * ) & ( header [ 8 ] ) ;
				unsigned int width = * ( unsigned int * ) & ( header [ 12 ] ) ;
				unsigned int linearSize = * ( unsigned int * ) & ( header [ 16 ] ) ;
				unsigned int mipMapCount = * ( unsigned int * ) & ( header [ 24 ] ) ;
				unsigned int fourCC = * ( unsigned int * ) & ( header [ 80 ] ) ;

			
				unsigned char * buffer ;
				unsigned int bufsize = linearSize ;
				if ( mipMapCount > 1 )
					bufsize = bufsize * 2 ;
				buffer = new unsigned char [ bufsize ] ;
				fread ( buffer , 1 , bufsize , fp ) ;
				fclose ( fp ) ;

				unsigned int components = 4 ;
				if ( fourCC == FOURCC_DXT1 )
					components = 3 ;

				unsigned int format ;
				switch ( fourCC ) {
					case FOURCC_DXT1 :
						format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ;
						break ;
					case FOURCC_DXT3 :
						format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ;
						break ;
					case FOURCC_DXT5 :
						format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT ;
						break ;
					default :
						free ( buffer ) ;
						return ( - 1 ) ;
				}

				glGenTextures ( 1 , & ( this -> modelTextureID ) ) ;
				glBindTexture ( GL_TEXTURE_2D , ( this -> modelTextureID ) ) ;
				glPixelStorei ( GL_UNPACK_ALIGNMENT , 1 ) ;
				
				unsigned int blockSize = 16 ;
				if ( format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
					blockSize = 8 ;
				unsigned int offset = 0 ;

				for ( unsigned int level = 0 ; ( level < mipMapCount ) && ( width || height ) ; ++ level ) {
					unsigned int size = ( ( width + 3 ) / 4 ) * ( ( height + 3 ) / 4 ) * blockSize ;
					glCompressedTexImage2D ( GL_TEXTURE_2D , level , format , width , height , 0 , size , buffer + offset ) ;
				
					offset += size ;
					width /= 2 ;
					height /= 2 ;

					if ( width < 1 )
						width = 1 ;
					if ( height < 1 )
						height = 1 ;
				}

				delete buffer ;

				return 0 ;
			}

		public:

			const static short int                      IMG_DDS                = 0x00 ;
			const static short int                      IMG_BMP                = 0x01 ;

			modelTextureHandler ( const char * fileNo , int type = IMG_DDS , loggingHandler * logger = new loggingHandler ) {
				this -> logger = logger ;

				char logMsg [ 200 ] = "modelTextureHandler : Initializing the model texture handler \'" ;
				strcat ( logMsg , fileNo ) ;
				strcat ( logMsg , "\'" ) ;
				this -> logger -> logInfo ( logMsg ) ;

				this -> logger -> logInfo ( "modelTextureHandler : Loading texture from file" ) ;
				char textureFilePath [ 500 ] = "models/textures/texture-" ;
				strcat ( textureFilePath , fileNo ) ;
				strcat ( textureFilePath , ".tx" ) ;
				if ( type == IMG_DDS ) {
					this -> logger -> logInfo ( "modelTextureHandler : Loading texture using DDS loader" ) ;
					if ( this -> loadDDS ( textureFilePath ) ) {
						this -> logger -> logInfo ( "modelTextureHandler : Could not load texture. Have to exit\n" ) ;
						exit ( - 1 ) ;
					}
				}
				else if ( type == IMG_BMP ) {
					this -> logger -> logInfo ( "modelTextureHandler : Loading texture using BMP loader" ) ;
					if ( this -> loadBMP ( textureFilePath ) ) {
						this -> logger -> logInfo ( "modelTextureHandler : Could not load texture. Have to exit\n" ) ;
						exit ( - 1 ) ;
					}
				}
				else {
					this -> logger -> logInfo ( "modelTextureHandler : Texture file type not supported. Have to exit\n" ) ;
					exit ( - 1 ) ;
				}

				this -> logger -> logInfo ( "modelTextureHandler : Adding current texture to the list of available textures to use directly" ) ;
				modelTexturesList [ fileNo ] = this ;
				modelTexturesListCount [ fileNo ] = 1 ;
				this -> fileNo = fileNo ;

				this -> logger -> logInfo ( "modelTextureHandler : Initialized the model texture handler\n" ) ;
			}

			~modelTextureHandler ( ) {
				glDeleteTextures ( 1 , & ( this -> modelTextureID ) ) ;
				modelTexturesList . erase ( this -> fileNo ) ;
				modelTexturesListCount . erase ( this -> fileNo ) ;
			}

			void bindTexture ( GLuint textureHandler , unsigned short int textureNo = 0 ) {
				if ( ( ( this -> modelTextureID ) != ( ( modelTextureHandler :: activeTexture ) . activeTextureID ) ) ||
				     ( ( textureNo ) != ( ( modelTextureHandler :: activeTexture ) . activeTextureNo ) ) ||
				     ( ( textureHandler ) != ( ( modelTextureHandler :: activeTexture ) . activeTextureHandler ) )
				) {
					unsigned short int textureBlock = GL_TEXTURE0 + textureNo ;
					glActiveTexture ( textureBlock ) ;
					glBindTexture ( GL_TEXTURE_2D , ( this -> modelTextureID ) ) ;
					glUniform1i ( textureHandler , textureNo ) ;
					modelTextureHandler :: activeTexture = {
						( this -> modelTextureID ) ,
						textureNo ,
						textureHandler
					} ;
				}
			}

			static modelTextureHandler * checkTexture ( const char * fileNo ) {
				if ( modelTexturesList . count ( fileNo ) == 0 )
					return NULL ;
				else
					return modelTexturesList [ fileNo ] ;
			} 

	} ;

	struct activeTextureDetails modelTextureHandler :: activeTexture = { 0 , 0 , 0 } ;

#endif