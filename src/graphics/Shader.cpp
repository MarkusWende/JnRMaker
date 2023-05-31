/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Shader.h"
#include "MessageManager.h"

#include <iostream>

Shader& Shader::Use()
{
	glUseProgram(id_);
	return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
	GLuint sVertex, sFragment, gShader;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// If geometry shader source code is given, also compile geometry shader
	// Shader Program
	id_ = glCreateProgram();
	glAttachShader(id_, sVertex);
	glAttachShader(id_, sFragment);
#ifndef __EMSCRIPTEN__
	gShader = glCreateShader(GL_GEOMETRY_SHADER);
	if (geometrySource != nullptr)
	{
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
		glAttachShader(id_, gShader);
	}
#else
	(void)geometrySource;
	(void)gShader;
#endif // !__EMSCRIPTEN__
	glLinkProgram(id_);
	checkCompileErrors(id_, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
#ifndef __EMSCRIPTEN__
	glDeleteShader(gShader);
#endif // !__EMSCRIPTEN__
}

void Shader::SetFloat(const GLchar* name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(id_, name), value);
}
void Shader::SetInteger(const GLchar* name, GLint value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(id_, name), value);
}
void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(id_, name), x, y);
}
void Shader::SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}
void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}
void Shader::SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}
void Shader::SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetBool(const GLchar* name, GLboolean value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(id_, name), value);
}


void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);

#ifdef __EMSCRIPTEN__
			std::stringstream msg;
			msg << "Compile-time error. Type: " << type << "\tinfo: " << infoLog;
			MessageManager::AddMessage(msg, message_t::ERROR_T);
#endif
#ifdef _WIN32
			FILE* stream;
			freopen_s(&stream, "log.txt", "w", stdout);
			fprintf(stream, "\tERROR::SHADER: Compile-time error: Type: %s\n\t\t%s\n", type.c_str(), infoLog);
			fclose(stream);
#endif // _WIN32
#ifdef __linux__
			FILE* stream;
			stream = fopen("./log.txt", "w");
			fprintf(stream, "\tERROR::SHADER: Compile-time error: Type: %s\n\t\t%s\n", type.c_str(), infoLog);
			fclose(stream);
#endif // __linux__
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
#ifdef __EMSCRIPTEN__
			std::stringstream msg;
			msg << "Link-time error. Type: " << type << "\tinfo: " << infoLog;
			MessageManager::AddMessage(msg, message_t::ERROR_T);
#endif
#ifdef _WIN32
			FILE* stream;
			freopen_s(&stream, "log.txt", "w", stdout);
			fprintf(stream, "\tERROR::Shader: Link-time error: Type: %s\n\t\t%s\n", type.c_str(), infoLog);
			fclose(stream);
#endif // _WIN32
#ifdef __linux__
			FILE* stream;
			stream = fopen("./log.txt", "w");
			fprintf(stream, "\tERROR::Shader: Link-time error: Type: %s\n\t\t%s\n", type.c_str(), infoLog);
			fclose(stream);
#endif // __linux__
		}
	}
}