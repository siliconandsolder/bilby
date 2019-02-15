#include "beta_class.hpp"
#include "beta_instance.hpp"

/**
@name:		findMethod
@purpose:	finds a class's method, either within itself or in its parent
@param:		std::shared_ptr<BetaInstance>, std::string
@return:	CustomFunction::pointer_type
*/
CustomFunction::pointer_type BetaClass::findMethod(std::shared_ptr<BetaInstance> instance, std::string name)
{
	// when a method is found, bind the instance to it so that variables are available
	if (methods_.count(name) == 1)
		return methods_.find(name)->second->bind(instance);

	if (super_ != nullptr)
		return super_->findMethod(instance, name);

	return nullptr;
}

/**
@name:		arity
@purpose:	Returns the number of parameters in init method
@param:		null
@return:	unsigned short
*/
unsigned short BetaClass::arity()
{
	CustomFunction::pointer_type init = nullptr;
	
	// since this is a class, we check for an initializer
	if (methods_.count("init") == 1)
		init = methods_.find("init")->second;
	
	if (init == nullptr) return 0;
	return init->arity();
}

/**
@name:		call
@purpose:	Calls the init method and then returns a new instance of the class
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type BetaClass::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto instance = BetaInstance::pointer_type(new BetaInstance(this));
	
	CustomFunction::pointer_type init = nullptr;

	if (methods_.count("init") == 1)
		init = methods_.find("init")->second;

	if (init != nullptr)
		init->bind(instance)->call(interpreter, args);

	return instance;
}
