#pragma once
#include <list>
#include <map>
#include <string>
#include "token.hpp"
#include "beta_callable.hpp"
#include "custom_func.hpp"

class BetaInstance;

class BetaClass : public BetaCallable
{
public:
	DEF_POINTER_TYPE(BetaClass)
	DEF_IS_CONVERTIBLE_FROM(BetaClass)

	std::string name_;
	BetaClass::pointer_type super_;
private:
	std::map<std::string, CustomFunction::pointer_type> methods_;
public:
	BetaClass(std::string name, BetaClass::pointer_type super, std::map<std::string, CustomFunction::pointer_type> methods) : name_(name), super_(super), methods_(methods) {}

	virtual string_type toString() const { return name_; }
	CustomFunction::pointer_type findMethod(std::shared_ptr<BetaInstance> instance, std::string name);

	virtual unsigned short arity() override;
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
};