/****************************************************************************
 * Copyright (C) 2019 Kirsch-Audio GmbH <info@kirsch-audio.com>				*
 * All rights reserved.														*
 *																			*
 * This file is part of SoundIMP											*
 *																			*
 *	SoundIMP is a software for creating sound immission predictions.		*
 *																			*
 *	Unauthorized copying of this file, via any medium is strictly			*
 *	prohibited																*
 *																			*
 *	Proprietary and confidential.											*
 ****************************************************************************/

/**
 * @file framebuffer.cpp
 * @author Kirsch Audio GmbH
 * @brief This file contains the OpenGL framebuffer class.
 */

#include <iostream>

#include "framebuffer.h"
#include "time_helper.h"
#include "message_manager.h"


Framebuffer::Framebuffer()
	: width_(0), height_(0), internal_format_(GL_RGB), image_format_(GL_RGB), wrap_s_(GL_REPEAT), wrap_t_(GL_REPEAT), filter_min_(GL_NEAREST), filter_max_(GL_NEAREST), tex_id_(0)
{
	glGenFramebuffers(1, &id_);
}

void Framebuffer::Generate(GLuint width, GLuint height, GLenum type)
{
	width_ = width;
	height_ = height;
	this->tex_type_ = type;

	// Bind Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, id_);

	// Generate the texture we're going to render to
	if (tex_id_ != 0)
		glDeleteTextures(1, &tex_id_);

	glGenTextures(1, &tex_id_);
	glBindTexture(this->tex_type_, tex_id_);
	glTexImage2D(this->tex_type_, 0, internal_format_, width_, height_, 0, image_format_, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(this->tex_type_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(this->tex_type_, GL_TEXTURE_MIN_FILTER, filter_min_);

	glBindTexture(this->tex_type_, 0);
	// Set Texture wrap and filter modes
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

	// attach it to currently bound framebuffer object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id_, 0);
#ifdef __EMSCRIPTEN__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id_, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_id_, 0);
#endif

	// The depth buffer
	glGenRenderbuffers(1, &rbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	// Always check that our framebuffer is ok
	bool status = false;
#ifdef __EMSCRIPTEN__
	std::stringstream msg;
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		msg << "Framebuffer successful initialized. ID: " << id_ << "\tWidth: " << width_ << "\tHeight: " << height_;
		MessageManager::AddMessage(msg, message_t::INFO);
		status = true;
	}
	else
	{
		msg << "Framebuffer could not be initialized: " << glGetError();
		MessageManager::AddMessage(msg, message_t::ERROR_T);
		status = false;
	}
#endif
	
#ifdef _WIN32
	FILE* stream;
    freopen_s(&stream, "log.txt", "a", stdout);
	//	freopen_s(&stream, "log.txt", "a", stdout);
	if (stream)
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stream, "%s\tFramebuffer successful initialized. ID: %u\tWidth: %u\tHeight: %u\n", TimeHelper::GetTimeinfo().c_str(), id_, width_, height_);
			status = true;
		}
		else
		{
			fprintf(stream, "%s\t[Error]: Framebuffer could not be initialized.\n", TimeHelper::GetTimeinfo().c_str());
			status = false;
		}
            	fclose(stream);
	}
#endif // _WIN32
#ifdef __linux__
	FILE* stream;
    stream = fopen("./log.txt", "a");
	//	freopen_s(&stream, "log.txt", "a", stdout);
	if (stream)
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stream, "%s\tFramebuffer successful initialized. ID: %u\tWidth: %u\tHeight: %u\n", time_helper::GetTimeinfo().c_str(), id_, width_, height_);
			status = true;
		}
		else
		{
			fprintf(stream, "%s\t[Error]: Framebuffer could not be initialized.\n", time_helper::GetTimeinfo().c_str());
			status = false;
		}
            	fclose(stream);
	}
#endif // __linux__

	// Unbind texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(GLuint width, GLuint height)
{
	width_ = width;
	height_ = height;
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	glBindTexture(this->tex_type_, tex_id_);
	glTexImage2D(this->tex_type_, 0, internal_format_, width, height, 0, image_format_, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(this->tex_type_, 0);
#ifdef __EMSCRIPTEN__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id_, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_id_, 0);
#endif

	// The depth buffer
	//unsigned int rbo;
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Delete()
{
	if (tex_id_ != 0)
		glDeleteTextures(1, &tex_id_);
	if (id_ != 0)
		glDeleteFramebuffers(1, &id_);
	if (rbo_ != 0)
		glDeleteRenderbuffers(1, &rbo_);
}
