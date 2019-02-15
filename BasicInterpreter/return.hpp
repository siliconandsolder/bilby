/**	@file return.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif a custom exception used to "throw" return expressions out of functions
*/

#pragma once
#include "token.hpp"

class ReturnEx
{
public:
	Token::pointer_type value_;
	ReturnEx(Token::pointer_type value) : value_(value) {}
};