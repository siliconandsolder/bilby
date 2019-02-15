#pragma once
#include "token.hpp"

class ReturnEx
{
public:
	Token::pointer_type value_;
	ReturnEx(Token::pointer_type value) : value_(value) {}
};