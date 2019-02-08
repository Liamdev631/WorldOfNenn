/*
	OpenGL-Utils
	Copyright (C) 2015 George Broughton
	GNU GPL Version 3
  
	Shader.cpp
  
	C++ file for container class for openGL shader programs
	Includes tessellation shaders, so opengl v. 4.0+
  
	Simply create class, call loadShader providing file names as parameters, then bind() to use
	Don't forget to call deleteShader() at the end
*/

#include <GL/glew.h>
#include "SFML/OpenGL.hpp"
#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//loadShader
//parameters are filenames to source files for vertex, tessellation control, tessellation evaluator, geometry and fragment shaders
//returns true if shader program was successfully created
//if the function returns false, the cause of the error may be ascertained by calling the getError() function, which returns a string
//can pass NULL as a shader filename to not have that particular shader included in the shader program
bool Shader::loadShader(const char* vFile, const char* tcFile, const char* teFile, const char* gFile, const char* fFile)
{
	error = "";

	hProgram = glCreateProgram();
	if(hProgram == 0)
	{
		error += "Error creating shader program\n";
		return false;
	}

	if(vFile)
	{
		vShader = glCreateShader(GL_VERTEX_SHADER);
		if(vShader == 0)
		{
			error += "Error creating vertex shader\n";
			return false;
		}
		string vTextString = readFile(vFile);
		if(vTextString.empty())
		{
			error += "Error reading vertex shader file\n";
			return false;
		}
		const char* vText = vTextString.c_str();
		glShaderSource(vShader, 1, &vText, NULL);
		glCompileShader(vShader);
		GLint vStatus;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vStatus);
		if(vStatus == GL_FALSE)
		{
			GLint logLength;
			glGetShaderiv(vShader, GL_INFO_LOG_LENGTH , &logLength);
			GLchar* compilerLog = new GLchar[logLength];
			glGetShaderInfoLog(vShader, logLength, 0, compilerLog);
			error += "Error compiling vertex shader: ";		
			error += compilerLog;
			error += "\n";
			delete compilerLog;
			return false;
		}
		glAttachShader(hProgram, vShader);
	}	
	if(tcFile)
	{
		tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		if(tcShader == 0)
		{
			error += "Error creating tessellation control shader\n";
			return false;
		}
		string tcTextString = readFile(tcFile);
		if(tcTextString.empty())
		{
			error += "Error reading tessellation control shader file\n";
			return false;
		}
		const char* tcText = tcTextString.c_str();
		glShaderSource(tcShader, 1, &tcText, NULL);
		glCompileShader(tcShader);
		GLint tcStatus;
		glGetShaderiv(tcShader, GL_COMPILE_STATUS, &tcStatus);
		if(tcStatus == GL_FALSE)
		{
			GLint logLength;
			glGetShaderiv(tcShader, GL_INFO_LOG_LENGTH , &logLength);
			GLchar* compilerLog = new GLchar[logLength];
			glGetShaderInfoLog(tcShader, logLength, 0, compilerLog);
			error += "Error compiling tessellation control shader: ";		
			error += compilerLog;
			error += "\n";
			delete compilerLog;
			return false;
		}
		glAttachShader(hProgram, tcShader);
	}	
	if(teFile)
	{
		teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		if(teShader == 0)
		{
			error += "Error creating tessellation evaluator shader\n";
			return false;
		}
		string teTextString = readFile(teFile);
		if(teTextString.empty())
		{
			error += "Error reading tessellation evaluator shader file\n";
			return false;
		}
		const char* teText = teTextString.c_str();
		glShaderSource(teShader, 1, &teText, NULL);
		glCompileShader(teShader);
		GLint teStatus;
		glGetShaderiv(teShader, GL_COMPILE_STATUS, &teStatus);
		if(teStatus == GL_FALSE)
		{
			GLint logLength;
			glGetShaderiv(teShader, GL_INFO_LOG_LENGTH , &logLength);
			GLchar* compilerLog = new GLchar[logLength];
			glGetShaderInfoLog(teShader, logLength, 0, compilerLog);
			error += "Error compiling tessellation evaluator shader: ";		
			error += compilerLog;
			error += "\n";
			delete compilerLog;
			return false;
		}
		glAttachShader(hProgram, teShader);
	}	
	if(gFile)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		if(gShader == 0)
		{
			error += "Error creating geometry shader\n";
			return false;
		}
		string gTextString = readFile(gFile);
		if(gTextString.empty())
		{
			error += "Error reading geometry shader file\n";
			return false;
		}
		const char* gText = gTextString.c_str();
		glShaderSource(gShader, 1, &gText, NULL);
		glCompileShader(gShader);
		GLint gStatus;
		glGetShaderiv(gShader, GL_COMPILE_STATUS, &gStatus);
		if(gStatus == GL_FALSE)
		{
			GLint logLength;
			glGetShaderiv(gShader, GL_INFO_LOG_LENGTH , &logLength);
			GLchar* compilerLog = new GLchar[logLength];
			glGetShaderInfoLog(gShader, logLength, 0, compilerLog);
			error += "Error compiling geometry shader: ";		
			error += compilerLog;
			error += "\n";
			delete compilerLog;
			return false;
		}
		glAttachShader(hProgram, gShader);
	}	
	if(fFile)
	{
		fShader = glCreateShader(GL_FRAGMENT_SHADER);
		if(fShader == 0)
		{
			error += "Error creating fragment shader\n";
			return false;
		}
		string fTextString = readFile(fFile); 
		if(fTextString.empty())
		{
			error += "Error reading fragment shader file\n";
			return false;
		}
		const char* fText = fTextString.c_str();
		glShaderSource(fShader, 1, &fText, NULL);
		glCompileShader(fShader);
		GLint fStatus;
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fStatus);
		if(fStatus == GL_FALSE)
		{
			GLint logLength;
			glGetShaderiv(fShader, GL_INFO_LOG_LENGTH , &logLength);
			GLchar* compilerLog = new GLchar[logLength];
			glGetShaderInfoLog(fShader, logLength, 0, compilerLog);
			error += "Error compiling fragment shader: ";		
			error += compilerLog;
			error += "\n";
			delete compilerLog;
			return false;
		}
		glAttachShader(hProgram, fShader);
	}

	glLinkProgram(hProgram);

	GLint linkStatus;
	glGetProgramiv(hProgram, GL_LINK_STATUS, &linkStatus);

	if(linkStatus == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(hProgram, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetProgramInfoLog(hProgram, logLength, 0, compilerLog);
		error += "Error linking shader program: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}

	glValidateProgram(hProgram);

	GLint programValidation;
	glGetProgramiv(hProgram, GL_VALIDATE_STATUS, &programValidation);

	if(programValidation == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(hProgram, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetProgramInfoLog(hProgram, logLength, 0, compilerLog);
		error += "Error validating shader program: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}
	
	if(vFile != 0)
	{
		glDetachShader(hProgram, vShader);
		glDeleteShader(vShader);
	}
	if(tcFile != 0)
	{
		glDetachShader(hProgram, tcShader);
		glDeleteShader(tcShader);
	}
	if(teFile != 0)
	{
		glDetachShader(hProgram, teShader);
		glDeleteShader(teShader);
	}
	if(gFile != 0)
	{
		glDetachShader(hProgram, gShader);
		glDeleteShader(gShader);
	}
	if(fFile != 0)
	{
		glDetachShader(hProgram, fShader);
		glDeleteShader(fShader);
	}

	isShader = true;

	return true;
}

//deleteShader
//Used to delete a shader program once it is no longer of use
void Shader::deleteShader()
{
	glDeleteProgram(hProgram);

	isShader = false;
}

//bind
//binds the shader program to the current opengl context
//if no shader program exists, this function does nothing
void Shader::bind()
{
	if(isShader == true)
	{
		glUseProgram(hProgram);
	}
}

//unbind
//unbinds the current shader program from the opengl context
void Shader::unbind()
{
	glUseProgram(0);
}

//getHandle
//Returns a handle to the current shader program
//returns 0 if no shader program exists
GLuint Shader::getHandle()
{
	if(isShader == true)
	{
		return hProgram;
	}

	return 0;
}

//getIsShader
//Returns whether or not class is currently a shader program
bool Shader::getIsShader()
{
	return isShader;
}

//getError
//returns error string generated during loadProgram or readFile(private, called by loadProgram) functions
//error string refers to why the shader program could not be created
string Shader::getError()
{
	return error;
}

//readFile
//returns contents of text file contained at fileName as string
//only called from loadProgram
//any errors opening file can be read using getError()
string Shader::readFile(const char* fileName)
{
	string content;
	ifstream fileStream(fileName, ios::in);

	if(!fileStream.is_open())
	{
		error += "Error opening file: ";
		error += fileName;
		error += "\n";
		return content;
	}

	string line = "";

	while(!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	
	fileStream.close();

	if(content.empty())
	{
		error += "File is empty: ";
		error += fileName;
		error += "\n";
	}

	return content;
}
