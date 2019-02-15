/**	@file environment.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Environment class
*/

#include "environment.hpp"
using namespace std;

/**
@name:		define
@purpose:	adds a variable to the environment's variable map
@param:		std::string, Token::pointer_type
@return:	void
*/
void Environment::define(std::string name, Token::pointer_type var)
{
	variables_.insert(std::pair<string, Token::pointer_type>(name, var));
}

/**
@name:		reassign
@purpose:	resets the value of a variable
@param:		std::string, Token::pointer_type
@return:	void
*/
void Environment::reassign(std::string name, Token::pointer_type var)
{
	if (variables_.count(name) == 1)
		variables_[name] = var;
	else if (parent_ != nullptr)
		parent_->reassign(name, var);
	else
	{
		std::string ex = "Variable \"" + name + "\" does not exist in memory.";
		throw exception(ex.c_str());
	}
}

/**
@name:		reassignAt
@purpose:	finds a variable in a higher scope, and resets it's value
@param:		unsigned, std::string, Token::pointer_type
@return:	void
*/
void Environment::reassignAt(unsigned dist, std::string name, Token::pointer_type val)
{
	ancestor(dist)->variables_.insert(pair<string, Token::pointer_type>(name, val));
}

/**
@name:		get
@purpose:	resets a variable's value
@param:		std::string
@return:	Token::pointer_type
*/
Token::pointer_type Environment::get(std::string name)
{
	Token::pointer_type retVar;
	if (variables_.count(name) == 1)
		retVar = variables_.at(name);
	else if (parent_ != nullptr)
		retVar = parent_->get(name);
	else
	{
		std::string ex = "Variable \"" + name + "\" does not exist in memory.";
		throw exception(ex.c_str());
	}

	return retVar;
}

/**
@name:		getAt
@purpose:	finds and returns a variable in a higher scope
@param:		unsigned, std::string
@return:	Token::pointer_type
*/
Token::pointer_type Environment::getAt(unsigned dist, std::string name)
{
	return ancestor(dist)->variables_.find(name)->second;
}

/**
@name:		ancestor
@purpose:	returns the environment of a certain higher scope
@param:		unsigned
@return:	std::shared_ptr<Environment>
*/
std::shared_ptr<Environment> Environment::ancestor(unsigned dist)
{
	auto env = shared_from_this();

	for (unsigned i = 0; i < dist; ++i)
		env = env->parent_;

	return env;
}
