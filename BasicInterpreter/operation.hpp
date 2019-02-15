#pragma once

#include "operand.hpp"
#include <stack>
#include <exception>

class Operation : public Token
{
public:
	DEF_POINTER_TYPE(Operation)
	virtual unsigned numberOfArgs() const = 0;
	virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&) = 0;
};

class MiscOperators : public Token {};

	class Brackets : public MiscOperators {};

		class LeftBracket : public Brackets
		{
		public:
			DEF_POINTER_TYPE(LeftBracket)
			DEF_IS_CONVERTIBLE_FROM(LeftBracket)
				string_type toString() const { return "("; }
		};

		class RightBracket : public Brackets
		{
		public:
			DEF_POINTER_TYPE(RightBracket)
			DEF_IS_CONVERTIBLE_FROM(RightBracket)
				string_type toString() const { return ")"; }
		};
	
	class Braces : public MiscOperators {};
		
		class LeftBrace : public Braces
		{
		public:
			DEF_POINTER_TYPE(LeftBrace)
			DEF_IS_CONVERTIBLE_FROM(LeftBrace)
			string_type toString() const { return "{"; }
		};

		class RightBrace : public Braces
		{
		public:
			DEF_POINTER_TYPE(RightBrace)
			DEF_IS_CONVERTIBLE_FROM(RightBrace)
			string_type toString() const { return "}"; }
		};


	class Assignment : public Token
	{
	public:
		DEF_POINTER_TYPE(Assignment)
		DEF_IS_CONVERTIBLE_FROM(Assignment)
		string_type toString() const { return "="; }
	};
	
	class ArgSeparator : public MiscOperators
	{
	public:
		DEF_POINTER_TYPE(ArgSeparator)
		DEF_IS_CONVERTIBLE_FROM(ArgSeparator)
			string_type toString() const { return ""; }
	};

	class SemiColon : public MiscOperators
	{
	public:
		DEF_POINTER_TYPE(SemiColon)
		DEF_IS_CONVERTIBLE_FROM(SemiColon)
			string_type toString() const { return ";"; }
	};

	class Comma : public MiscOperators
	{
	public:
		DEF_POINTER_TYPE(Comma)
		DEF_IS_CONVERTIBLE_FROM(Comma)
			string_type toString() const { return ","; }
	};

	class Dot : public MiscOperators
	{
	public:
		DEF_POINTER_TYPE(Dot)
		DEF_IS_CONVERTIBLE_FROM(Dot)
		string_type toString() const { return "."; }
	};