# include <stdio.h>
# include <string.h>

struct modelDetailsInfo {
	short int                                    modelInvertUVs                   ;
	char                                         modelStructureNo [ 100 ]          ;
	char                                         modelTextureNo [ 100 ]            ;
	char                                         modelShadersNo [ 100 ]            ;
} ;

int main ( ) {
	char modelName [ 500 ] = "models/models-" ;
	struct modelDetailsInfo modelDetails ;
	char temp [ 100 ] ;

	fprintf ( stdout , "Enter model name : " ) ;
	fscanf ( stdin , "%s" , temp ) ;
	strcat ( modelName , temp ) ;
	strcat ( modelName , ".md" ) ;

	fprintf ( stdout , "Enter model structure name : " ) ;
	fscanf ( stdin , "%s" , ( modelDetails . modelStructureNo ) ) ;
	fprintf ( stdout , "Enter model texture name : " ) ;
	fscanf ( stdin , "%s" , ( modelDetails . modelTextureNo ) ) ;
	fprintf ( stdout , "Enter model shaders name : " ) ;
	fscanf ( stdin , "%s" , ( modelDetails . modelShadersNo ) ) ;
	fprintf ( stdout , "Invert structure UVs ? ( 1 - Yes | 0 - No ) : " ) ;
	fscanf ( stdin , "%hi" , & ( modelDetails . modelInvertUVs ) ) ;

	FILE * modelFile = fopen ( modelName , "wb" ) ;

	fwrite ( & modelDetails , sizeof ( modelDetails ) , 1 , modelFile ) ;

	fclose ( modelFile ) ;

	return 0 ;
}