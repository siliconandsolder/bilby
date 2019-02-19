/**	@file beta_instance.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of an instance of a user-written class
*/

#pragma once
#include <map>
#include <string>
#include "beta_class.hpp"

class BetaInstance : public Token, public std::enable_shared_from_this<BetaInstance>
{
public:
	DEF_POINTER_TYPE(BetaInstance)
	DEF_IS_CONVERTIBLE_FROM(BetaInstance)

private:
	BetaClass * clas_;
	std::map<std::string, Token::pointer_type> fields_;

public:	
	BetaInstance(BetaClass * clas, std::list<Variable::pointer_type> data);

	Token::pointer_type get(std::string name);
	void set(std::string name, Token::pointer_type val);
	virtual string_type toString() const { return clas_->name_ + " instance"; }
};