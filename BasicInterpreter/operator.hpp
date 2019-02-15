/**	@file oper_exceptions.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines a few custom exceptions of operation exceptions
*/

#pragma once

#include <sstream>
#include <exception>
#include <cstdlib>
#include "operation.hpp"
#include "operand.hpp"
#include "oper_exceptions.hpp"

class Operator : public Operation
{
public:
	DEF_POINTER_TYPE(Operator)
};

	class UnaryOperator : public Operator
	{
	public:
		DEF_POINTER_TYPE(UnaryOperator)
		virtual unsigned numberOfArgs() const override { return 1; }
	};

		class NonAssocOperator : public UnaryOperator {};

			class Factorial : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Factorial)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "!"; }
			};

			class Identity : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Identity)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "+"; }
			};

			class Negation : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Negation)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "-"; }
			};

			class Not : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Not)
				virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "!"; }
			};

	class BinaryOperator : public Operator
	{
	public:
		DEF_POINTER_TYPE(BinaryOperator)
		virtual unsigned numberOfArgs() const override { return 2; }
	};

		class RAssocOperator : public BinaryOperator {};
			
			class Power : public RAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Power)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "^"; }
			};

		class LAssocOperator : public BinaryOperator {};

			class Addition : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Addition)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "+"; }
			};

			class Subtraction : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Subtraction)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "-"; }
			};

			class Multiplication : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Multiplication)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "*"; }
			};

			class Division : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Division)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "/"; }
			};

			class Modulus : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Modulus)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "%"; }
			};

			class And : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(And)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "and"; }
			};

			class Nand : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Nand)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "nand"; }
			};

			class Or : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Or)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "or"; }
			};

			class Nor : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Nor)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "nor"; }
			};

			class Xor : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Xor)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "xor"; }
			};

			class Xnor : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Xnor)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "xnor"; }
			};

			class Relational : public LAssocOperator {};

				class Equality : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Equality)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "=="; }
				};

				class Greater : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Greater)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return ">"; }
				};

				class GreaterEqual : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(GreaterEqual)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return ">="; }
				};

				class Inequality : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Inequality)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "!="; }
				};

				class Less : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Less)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "<"; }
				};

				class LessEqual : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(LessEqual)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "=<"; }
				};
			