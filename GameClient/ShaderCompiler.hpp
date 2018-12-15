#pragma once
#include <fstream>
#include <string>

static char* readShaderSource(const char* shaderFile)
{
	FILE* fp;
	fopen_s(&fp, shaderFile, "r");
	char* buf;
	long size;

	if (fp == NULL) return NULL;
	fseek(fp, 0L, SEEK_END);//go to end
	size = ftell(fp);       //get size
	fseek(fp, 0L, SEEK_SET);//go to begining

	buf = (char*)malloc(size * sizeof(char));
	fread(buf, sizeof(char), size, fp);
	buf[size] = NULL;
	fclose(fp);
	return buf;
}