#include "custom_func.hpp"
#include "return.hpp"
#include "beta_instance.hpp"
#include <memory>
using namespace std;

/**
@name:		bind
@purpose:	binds the 'me' keyword to a class instance
@param:		std::shared_ptr<BetaInstance>
@return:	CustomFunction::pointer_type
*/
CustomFunction::pointer_type CustomFunction::bind(std::shared_ptr<BetaInstance> instance)
{
	std::shared_ptr<Environment> env(new Environment(closure_));
	env->define("me", Token::pointer_type(instance));
	return CustomFunction::pointer_type(new CustomFunction(declaration_, env, isInit_));
}

/**
@name:		call
@purpose:	executes a function call
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type CustomFunction::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto prevEnv = interpreter->env_;

	shared_ptr<Environment> env(new Environment(closure_));

	auto itParams = declaration_->params_.begin();
	auto itArgs = args.begin();

	// add parameters to the environment
	while (itArgs != args.end())
	{
		env->define((*itParams)->toString(),(*itArgs));
		++itParams;
		++itArgs;
	}

	try
	{
		interpreter->executeBlock(declaration_->body_, env);
	}
	catch (ReturnEx &re)
	{
		interpreter->env_ = prevEnv;	// reset the environment in case of a return keyword

		// return a pointer to this instance if it is the init() function
		if (isInit_) 
			return closure_->getAt(0, "me");

		return re.value_;
	}

	// return a pointer anyway, even in the absense of a return statement
	if (isInit_) 
		return closure_->getAt(0, "me");
	
	return nullptr;
}
