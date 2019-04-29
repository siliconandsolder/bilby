/**	@file entry.hpp
	@author Benjamin Godin
	@date 2019-03-13
	@version 1.0.0
	@note Developed for C++17
	@breif entry-point for the Wrapper class
*/

#pragma once
#include <string>
#include <Windows.h>

class Entry
{
public:
	static void interpret(std::string text);
	static void setStdHandle(HANDLE handle);
};