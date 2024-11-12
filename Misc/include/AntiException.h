#pragma once
#include <exception>
#include <string>

class AntiException : public std::exception
{
public:
	AntiException(int line, const char* file);
	const char* what() const override;
public:
	virtual const char* GetType() const;
	int GetLine() const;
	std::string GetFile() const;
	std::string GetDescription() const;
	void SetBuffer(std::string buffer);
	std::string GetBuffer();
private:
	int m_Line;
	std::string m_File;
protected:
	mutable std::string m_WhatBuffer;
};

