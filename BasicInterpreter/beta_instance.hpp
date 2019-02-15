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
	BetaInstance(BetaClass * clas) : clas_(clas) {}

	Token::pointer_type get(std::string name);
	void set(std::string name, Token::pointer_type val);
	virtual string_type toString() const { return clas_->name_ + " instance"; }
};