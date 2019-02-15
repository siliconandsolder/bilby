/**	@file environment.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of a runtime environment containing scopes and variables
*/

#pragma once
#include "operand.hpp"
#include <map>

class Environment: public std::enable_shared_from_this<Environment>
{
public:
	std::shared_ptr<Environment> parent_;
private:
	std::map<std::string, Token::pointer_type> variables_;
public:
	Environment(std::shared_ptr<Environment> parent = nullptr) : parent_(parent) {}
	void define(std::string, Token::pointer_type);
	void reassign(std::string, Token::pointer_type);
	void reassignAt(unsigned dist, std::string name, Token::pointer_type val);
	Token::pointer_type get(std::string name);
	Token::pointer_type getAt(unsigned dist, std::string name);
	std::shared_ptr<Environment> ancestor(unsigned dist);
};