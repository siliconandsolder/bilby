#pragma once

#include "operand.hpp"

class Variable : public Operand
{
public:
	DEF_POINTER_TYPE(Variable)
	using value_type = string_type;
private:
	value_type name_;
public:
	Variable(value_type value = "") : name_(value) {}
	
	void setName(string_type name) { name_ = name; }
	string_type getName() const { return name_; }

	value_type getValue() const { return name_; }
	
	string_type toString() const;
	DEF_IS_CONVERTIBLE_FROM(Variable)
};