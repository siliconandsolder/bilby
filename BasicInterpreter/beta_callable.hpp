#pragma once
#include <list>
#include "interpreter.hpp"
#include "integer.hpp"
#include "float.hpp"
#include "boolean.hpp"
#include "word.hpp"

class BetaCallable : public Token {
public:
	using tok_p = Token::pointer_type;
	using int_p = Integer::pointer_type;
	using float_p = Float::pointer_type;
	using bool_p = Boolean::pointer_type;
	using word_p = Word::pointer_type;

	DEF_POINTER_TYPE(BetaCallable)
	DEF_IS_CONVERTIBLE_FROM(BetaCallable)

	virtual unsigned short arity() = 0;
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) = 0;
};