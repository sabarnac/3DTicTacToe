#version 330 core

layout ( location = 0 ) in vec2 vertexPositionScreenspace ;
layout ( location = 1 ) in vec2 vertexUV ;

out vec2 UV ;

void main ( ) {

	vec2 vertexPositionHomoneneousspace = vertexPositionScreenspace - vec2 ( 683 , 384 ) ;
	vertexPositionHomoneneousspace /= vec2 ( 683 , 384 ) ;
	gl_Position =  vec4 ( vertexPositionHomoneneousspace , 0 , 1 ) ;
	
	UV = vertexUV ;

}

