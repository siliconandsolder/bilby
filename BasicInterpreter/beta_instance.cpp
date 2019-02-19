/**	@file beta_instance.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of BetaInstance
*/

#include "beta_instance.hpp"
using namespace std;

BetaInstance::BetaInstance(BetaClass * clas, std::list<Variable::pointer_type> data)
{
	clas_ = clas;

	// add class variables to the instance
	for (auto datum : data)
		fields_.insert(pair<string, Token::pointer_type>(datum->getName(), nullptr));
}

/**
@name:		get
@purpose:	returns a pointer to a field, if it exists
@param:		std::string
@return:	Token::pointer_type
*/
Token::pointer_type BetaInstance::get(std::string name)
{
	if (fields_.count(name) == 1)
		return fields_.find(name)->second;

	CustomFunction::pointer_type method = clas_->findMethod(shared_from_this(), name);
	if (method != nullptr) return method;

	stringstream ss;
	ss << "BetaInstanceException: Undefined property: '" << name << "'.";
	throw exception(ss.str().c_str());
}

/**
@name:		set
@purpose:	sets a fields value, or adds it if it doesn't exist
@param:		std::string
@return:	Token::pointer_type
*/
void BetaInstance::set(std::string name, Token::pointer_type val)
{
	if (fields_.count(name) == 1)
		fields_.at(name) = val;
	else
		throw exception(string("Field \"" + name + "\" does not exist in class \"" + clas_->name_ + "\"").c_str());
}
