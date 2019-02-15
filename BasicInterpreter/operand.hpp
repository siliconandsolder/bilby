/**	@file operand.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of an operand
*/

#pragma once

#include "token.hpp"
#include <cassert>

class Operand : public Token
{
public:
	DEF_POINTER_TYPE(Operand)
		DEF_IS_CONVERTIBLE_FROM(Operand)
};

template<typename new_type, typename old_type>
Operand::pointer_type make_operand(old_type const& param)
{
	return Operand::pointer_type(new new_type(param));
}

template<typename operand_type>
typename operand_type::value_type getValue(Token::pointer_type const& operand)
{
	return dynamic_cast<operand_type*>(operand.get())->getValue();
}