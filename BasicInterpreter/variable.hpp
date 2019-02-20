/**	@file variable.hpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif a class used to hold any type of operand
*/

#pragma once

#include "operand.hpp"

class Variable : public Operand
{
public:
	DEF_POINTER_TYPE(Variable)
	using value_type = string_type;
	enum class VarType { BOOL, INT, FLOAT, WORD, OBJECT };
private:
	value_type name_;
	VarType varType_;
public:
	Variable(value_type value = "") : name_(value) {}
	
	void setName(string_type name) { name_ = name; }
	string_type getName() const { return name_; }
	value_type getValue() const { return name_; }
	void setType(VarType type) { varType_ = type; }
	VarType getType() { return varType_; }
	
	string_type toString() const;
	DEF_IS_CONVERTIBLE_FROM(Variable)
};