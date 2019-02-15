#include "beta_instance.hpp"
using namespace std;

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
	if (fields_.count(name) == 0)
		fields_.insert(std::pair<string, Token::pointer_type>(name, val));
	else
		fields_.at(name) = val;
}
