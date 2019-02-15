#pragma once

#include <sstream>
#include <exception>
#include <cstdlib>
#include "operation.hpp"
#include "operand.hpp"
#include "oper_exceptions.hpp"

typedef enum
{
	MIN = 0,
	ASSIGNMENT,
	LOGOR,
	LOGAND,
	LOGXOR,
	RELATIONAL,
	ADDITIVE,
	MULTIPLICATIVE,
	POWER,
	UNARY,
	POSTFIX,
	MAX
} precedence_type;

#define DEF_PRECENDENCE(category) public: precedence_type getPrecedence() const override { return category; }

class Operator : public Operation
{
public:
	DEF_POINTER_TYPE(Operator)
	virtual precedence_type getPrecedence() const = 0;
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
				DEF_PRECENDENCE(POSTFIX)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "!"; }
			};

			class Identity : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Identity)
					DEF_PRECENDENCE(UNARY)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "+"; }
			};

			class Negation : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Negation)
					DEF_PRECENDENCE(UNARY)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "-"; }
			};

			class Not : public NonAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Not)
					DEF_PRECENDENCE(UNARY)
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
				DEF_PRECENDENCE(POWER)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "^"; }
			};

		class LAssocOperator : public BinaryOperator {};

			class Addition : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Addition)
				DEF_PRECENDENCE(ADDITIVE)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "+"; }
			};

			class Subtraction : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Subtraction)
					DEF_PRECENDENCE(ADDITIVE)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "-"; }
			};

			class Multiplication : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Multiplication)
					DEF_PRECENDENCE(MULTIPLICATIVE)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "*"; }
			};

			class Division : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Division)
					DEF_PRECENDENCE(MULTIPLICATIVE)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "/"; }
			};

			class Modulus : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Modulus)
					DEF_PRECENDENCE(MULTIPLICATIVE)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "%"; }
			};

			class And : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(And)
					DEF_PRECENDENCE(LOGAND)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "and"; }
			};

			class Nand : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Nand)
					DEF_PRECENDENCE(LOGAND)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "nand"; }
			};

			class Or : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Or)
					DEF_PRECENDENCE(LOGOR)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "or"; }
			};

			class Nor : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Nor)
					DEF_PRECENDENCE(LOGOR)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "nor"; }
			};

			class Xor : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Xor)
					DEF_PRECENDENCE(LOGXOR)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "xor"; }
			};

			class Xnor : public LAssocOperator
			{
				DEF_IS_CONVERTIBLE_FROM(Xnor)
					DEF_PRECENDENCE(LOGXOR)
					virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
				string_type toString() const { return "xnor"; }
			};

			class Relational : public LAssocOperator {};

				class Equality : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Equality)
					DEF_PRECENDENCE(RELATIONAL)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "=="; }
				};

				class Greater : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Greater)
						DEF_PRECENDENCE(RELATIONAL)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return ">"; }
				};

				class GreaterEqual : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(GreaterEqual)
						DEF_PRECENDENCE(RELATIONAL)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return ">="; }
				};

				class Inequality : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Inequality)
						DEF_PRECENDENCE(RELATIONAL)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "!="; }
				};

				class Less : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(Less)
						DEF_PRECENDENCE(RELATIONAL)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "<"; }
				};

				class LessEqual : public Relational
				{
					DEF_IS_CONVERTIBLE_FROM(LessEqual)
						DEF_PRECENDENCE(RELATIONAL)
						virtual Operand::pointer_type perform(std::stack<Operand::pointer_type>&);
					string_type toString() const { return "=<"; }
				};
			