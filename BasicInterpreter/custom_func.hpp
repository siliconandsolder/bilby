/**	@file custom_func.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of user-written functions
*/

#pragma once
#include "beta_callable.hpp"

class BetaInstance;
class CustomFunction : public BetaCallable
{
private:
	std::shared_ptr<StmtFunc> declaration_;
	std::shared_ptr<Environment> closure_;
	bool isInit_;
public:
	DEF_POINTER_TYPE(CustomFunction)
	DEF_IS_CONVERTIBLE_FROM(CustomFunction)

	CustomFunction(std::shared_ptr<StmtFunc> declaration, std::shared_ptr<Environment> closure, bool isInit) : declaration_(declaration), closure_(closure), isInit_(isInit) {}
	
	CustomFunction::pointer_type bind(std::shared_ptr<BetaInstance> instance);
	virtual unsigned short arity() { return static_cast<unsigned short>(declaration_->params_.size()); }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args);
};