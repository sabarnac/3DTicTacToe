# include <stdio.h>
# include <string.h>

struct shaderDetailsInfo {
	char                                         shaderVertexNo [ 100 ]             ;
	char                                         shaderFragmentNo [ 100 ]           ;
} ;

int main ( ) {
	char shaderName [ 500 ] = "models/shaders/shaders-" ;
	struct shaderDetailsInfo shaderDetails ;
	char temp [ 100 ] ;

	fprintf ( stdout , "Enter shader name : " ) ;
	fscanf ( stdin , "%s" , temp ) ;
	strcat ( shaderName , temp ) ;
	strcat ( shaderName , ".shd" ) ;

	fprintf ( stdout , "Enter vertex shader name : " ) ;
	fscanf ( stdin , "%s" , ( shaderDetails . shaderVertexNo ) ) ;
	fprintf ( stdout , "Enter fragment shader name : " ) ;
	fscanf ( stdin , "%s" , ( shaderDetails . shaderFragmentNo ) ) ;

	FILE * shaderFile = fopen ( shaderName , "wb" ) ;

	fwrite ( & shaderDetails , sizeof ( shaderDetails ) , 1 , shaderFile ) ;

	fclose ( shaderFile ) ;

	return 0 ;
}