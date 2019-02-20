/**	@file keywords.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines all keywords
*/

#pragma once
#include "token.hpp"

class Keyword : public Token
{
public:
	DEF_POINTER_TYPE(Keyword)
	DEF_IS_CONVERTIBLE_FROM(Keyword)
};

class Var : public Keyword
{
public:
	DEF_POINTER_TYPE(Var)
	DEF_IS_CONVERTIBLE_FROM(Var)
};

	class VarBool : public Var
	{
	public:
		DEF_POINTER_TYPE(VarBool)
		DEF_IS_CONVERTIBLE_FROM(VarBool)
	};

	class VarInt : public Var
	{
	public:
		DEF_POINTER_TYPE(VarInt)
		DEF_IS_CONVERTIBLE_FROM(VarInt)
	};

	class VarFloat : public Var
	{
	public:
		DEF_POINTER_TYPE(VarFloat)
		DEF_IS_CONVERTIBLE_FROM(VarFloat)
	};

	class VarWord : public Var
	{
	public:
		DEF_POINTER_TYPE(VarWord)
		DEF_IS_CONVERTIBLE_FROM(VarWord)
	};

	class VarObject : public Var
	{
	public:
		DEF_POINTER_TYPE(VarObject)
		DEF_IS_CONVERTIBLE_FROM(VarObject)
	};

class Print : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(Print)
};

class If : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(If)
};

class Else : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(Else)
};

class While : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(While)
};

class For : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(For)
};

class Do : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(Do)
};

class Func : public Keyword
{
	DEF_IS_CONVERTIBLE_FROM(Func)
};

class Return : public Keyword
{
public:
	DEF_POINTER_TYPE(Return)
	DEF_IS_CONVERTIBLE_FROM(Return)
};

class Class : public Keyword
{
public:
	DEF_POINTER_TYPE(Class)
	DEF_IS_CONVERTIBLE_FROM(Class)
};

class Me : public Keyword
{
public:
	DEF_POINTER_TYPE(Me)
	DEF_IS_CONVERTIBLE_FROM(Me)
};

class Inherits : public Keyword
{
public:
	DEF_POINTER_TYPE(Inherits)
	DEF_IS_CONVERTIBLE_FROM(Inherits)
};

class Super : public Keyword
{
public:
	DEF_POINTER_TYPE(Super)
	DEF_IS_CONVERTIBLE_FROM(Super)
};

class Data : public Keyword
{
public:
	DEF_POINTER_TYPE(Data)
	DEF_IS_CONVERTIBLE_FROM(Data)
};

class Method : public Keyword
{
public:
	DEF_POINTER_TYPE(Method)
	DEF_IS_CONVERTIBLE_FROM(Method)
};