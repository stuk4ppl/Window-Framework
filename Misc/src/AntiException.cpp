#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "../include/AntiException.h"

#include <sstream>
#include <locale>
#include <codecvt>

AntiException::AntiException(int line, const char* file) :
	m_Line(line), m_File(file)
{}

const char* AntiException::what() const
{
	std::stringstream ss;
	ss << GetDescription();

	m_WhatBuffer = ss.str();
	return m_WhatBuffer.c_str();
}

const char* AntiException::GetType() const
{
	return "Anti Exception: ";
}

int AntiException::GetLine() const
{
	return m_Line;
}

std::string AntiException::GetFile() const
{
	return m_File;
}

std::string AntiException::GetDescription() const
{
	std::stringstream ss;
	ss << "[FILE] " << m_File << std::endl
		<< "[LINE] " << m_Line;

	return ss.str();
}

void AntiException::SetBuffer(std::string buffer)
{
	m_WhatBuffer = buffer;
}

std::string AntiException::GetBuffer()
{
	return m_WhatBuffer;
}
