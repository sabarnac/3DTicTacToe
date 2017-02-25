# ifndef GL_TEXT_HANDLER

	# define GL_TEXT_HANDLER 1

	# include <vector>
	# include <glm/glm.hpp>
	# include <glm/gtc/matrix_transform.hpp>

	# include "shaders_handler.cpp"
	# include "texture_handler.cpp"
	# include "logging_handler.cpp"

	using namespace glm ;

	class textHandler {

		private:
			
			modelTextureHandler                       * textTextureNormal             ;
			modelShadersHandler                       * textShaders                   ;
			string                                      fileNo                        ;
			GLuint                                      textTextureID                 ;
			unsigned int                                text2DVertexBufferID          ;
			unsigned int                                text2DUVBufferID              ;
			
			loggingHandler                            * logger                        ;

		public:

			textHandler ( loggingHandler * logger = new loggingHandler ) {
				this -> logger = logger ;

				this -> logger -> logInfo ( "textHandler : Initializing the text handler" ) ;

				this -> logger -> logInfo ( "textHandler : Initializing text texture" ) ;
				this -> textTextureNormal = new modelTextureHandler ( "font-normal" , ( modelTextureHandler :: IMG_BMP ) , ( this -> logger ) ) ;

				this -> logger -> logInfo ( "textHandler : Initializing text shaders" ) ;
				this -> textShaders = new modelShadersHandler ( "text" , ( this -> logger ) ) ;

				this -> textTextureID  = this -> textShaders -> getShaderVarHandle ( "textureSampler" ) ;
				glGenBuffers ( 1 , & ( this -> text2DVertexBufferID ) ) ;
				glGenBuffers ( 1 , & ( this -> text2DUVBufferID ) ) ;

				this -> logger -> logInfo ( "textHandler : Initialized the text handler\n" ) ;
			}

			~textHandler ( ) {
				delete modelTexturesList [ "font-normal" ] ;
				delete modelShadersList [ "text" ] ;
			}

			void printText2D ( const char * text , int x , int y , int size ) {

				unsigned int length = strlen ( text ) ;

				vector < vec2 > vertices ;
				vector < vec2 > uvs ;

				for ( unsigned int i = 0 ; i < length ; i ++ ) {
					vec2 vertexUpLeft = vec2 ( ( x + i * size ) , ( y + size ) ) ;
					vec2 vertexUpRight = vec2 ( ( x + i * size + size ) , ( y + size ) ) ;
					vec2 vertexDownRight = vec2 ( ( x + i * size + size ) , y ) ;
					vec2 vertexDownLeft = vec2 ( ( x + i * size ) , y ) ;

					vertices . push_back ( vertexUpLeft ) ;
					vertices . push_back ( vertexDownLeft ) ;
					vertices . push_back ( vertexUpRight ) ;

					vertices . push_back ( vertexDownRight ) ;
					vertices . push_back ( vertexUpRight ) ;
					vertices . push_back ( vertexDownLeft ) ;

					char character = text [ i ] ;
					float uvX , uvY ;
					uvX = ( ( ( character - 32 ) % 16 ) / 16.0f ) ;
					uvY = ( 1 - ( ( ( character - 32 ) / 16 ) / 14.0f ) ) ;

					vec2 uvUpLeft = vec2 ( uvX , uvY ) ;
					vec2 uvUpRight = vec2 ( ( uvX + 1.0f / 16.0f ) , uvY ) ;
					vec2 uvDownRight = vec2 ( ( uvX + 1.0f / 16.0f ) , ( uvY - 1.0f / 14.0f ) ) ;
					vec2 uvDownLeft = vec2 ( uvX , ( uvY - 1.0f / 14.0f ) ) ;

					uvs . push_back ( uvUpLeft ) ;
					uvs . push_back ( uvDownLeft ) ;
					uvs . push_back ( uvUpRight ) ;

					uvs . push_back ( uvDownRight ) ;
					uvs . push_back ( uvUpRight ) ;
					uvs . push_back ( uvDownLeft ) ;
				}

				glBindBuffer ( GL_ARRAY_BUFFER , ( this -> text2DVertexBufferID ) ) ;
				glBufferData ( GL_ARRAY_BUFFER , vertices . size ( ) * sizeof ( vec2 ) , & ( vertices [ 0 ] ) , GL_STATIC_DRAW ) ;
				glBindBuffer ( GL_ARRAY_BUFFER , ( this -> text2DUVBufferID ) ) ;
				glBufferData ( GL_ARRAY_BUFFER , uvs . size ( ) * sizeof ( vec2 ) , & ( uvs [ 0 ] ) , GL_STATIC_DRAW ) ;

				this -> textShaders -> useShader ( ) ;

				this -> textTextureNormal -> bindTexture ( ( this -> textTextureID ) , 0 ) ;

				glEnableVertexAttribArray ( 0 ) ;
				glBindBuffer ( GL_ARRAY_BUFFER , ( this -> text2DVertexBufferID ) ) ;
				glVertexAttribPointer ( 0 , 2 , GL_FLOAT , GL_FALSE , 0 , ( void * ) 0 ) ;

				glEnableVertexAttribArray ( 1 ) ;
				glBindBuffer ( GL_ARRAY_BUFFER , ( this -> text2DUVBufferID ) ) ;
				glVertexAttribPointer ( 1 , 2 , GL_FLOAT , GL_FALSE , 0 , ( void * ) 0 ) ;

				//glEnable ( GL_BLEND ) ;
				//glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA ) ;

				glDrawArrays ( GL_TRIANGLES , 0 , vertices . size ( ) ) ;

				//glDisable ( GL_BLEND ) ;

				glDisableVertexAttribArray ( 0 ) ;
				glDisableVertexAttribArray ( 1 ) ;
			}

	} ;

	textHandler * textObject = NULL ;

#endif