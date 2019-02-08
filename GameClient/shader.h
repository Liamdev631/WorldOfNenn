/*
	OpenGL-Utils
	Copyright (C) 2015 George Broughton
	GNU GPL Version 3
  
	Shader.h
  
	Header file for container class for openGL shader programs
	Includes tessellation shaders, so opengl v. 4.0+
  
	Simply create class, call loadShader providing file names as parameters, then bind() to use
	Don't forget to call deleteShader() at the end
*/

#pragma once
#include <string>
#include "SFML/OpenGL.hpp"

using namespace std;

class Shader
{

public:

	//loadShader
	//parameters are filenames to source files for vertex, tessellation control, tessellation evaluator, geometry and fragment shaders
	//returns true if shader program was successfully created
	//if the function returns false, the cause of the error may be ascertained by calling the getError() function, which returns a string
	//can pass NULL as a shader filename to not have that particular shader included in the shader program
	bool loadShader(const char* vFile, const char* tcFile, const char* teFile, const char* gFile, const char* fFile);
	
	//deleteShader
	//Used to delete a shader program once it is no longer of use
	void deleteShader();

	//bind
	//binds the shader program to the current opengl context
	//if no shader program exists, this function does nothing
	void bind();

	//unbind
	//unbinds the current shader program from the opengl context
	void unbind();
	
	//getHandle
	//Returns a handle to the current shader program
	//returns 0 if no shader program exists
	GLuint getHandle();
	
	//getError
	//returns error string generated during loadProgram or readFile(private, called by loadProgram) functions
	//error string refers to why the shader program could not be created
	string getError();

	//getIsShader
	//Returns whether or not class is currently a shader program
	bool getIsShader();
	
private:
  
	//Handles to shaders
	//Vertex, tessellation control, tessellation evaluator, geometry and fragment
	GLuint vShader;
	GLuint tcShader;
	GLuint teShader;
	GLuint gShader;
	GLuint fShader;
	
	//Handle to shader program
	GLuint hProgram;

	//Contains error messages from compilation
	string error;

	//If shader program currently exists
	bool isShader;

	//Function returns contents of shader file as string
	string readFile(const char* fileName);
};
