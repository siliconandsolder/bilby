/**	@file token.hpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif a base class for almost all classes in this project
*/

#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <tuple>

#define DEF_POINTER_TYPE(type) using pointer_type = std::shared_ptr<type>;

#define DEF_IS_CONVERTIBLE_FROM(type)\
	public: virtual bool is_convertible_from(Token::pointer_type const& ptr) const\
	{\
		return dynamic_cast<type const*>(ptr.get()) != nullptr;\
	}

class Token {
protected:
	unsigned short lineNum_;
	unsigned short indentNum_;
public:
	DEF_POINTER_TYPE(Token)
	using string_type = std::string;

	virtual string_type toString() const;

	unsigned short getLineNumber() { return lineNum_; }
	unsigned short getIndentNumber() { return indentNum_; }
	void setLineNumber(unsigned short line) { lineNum_ = line; }
	void setIndentNumber(unsigned short indent) { indentNum_ = indent; }
	DEF_IS_CONVERTIBLE_FROM(Token)
};

using TokenList = std::vector<Token::pointer_type>;

template <typename T> Token::pointer_type make(unsigned short line = 1, unsigned short indent = 1) 
{ 
	Token::pointer_type newTok(Token::pointer_type(new T));
	newTok->setLineNumber(line);
	newTok->setIndentNumber(indent);
	return newTok; 
}

template <typename T, typename U> Token::pointer_type make(U const& param, unsigned short line = 1, unsigned short indent = 1)
{ 
	Token::pointer_type newTok(Token::pointer_type(new T(param)));
	newTok->setLineNumber(line);
	newTok->setIndentNumber(indent);
	return newTok;
}

inline bool operator==(Token::pointer_type const& lhs, Token::pointer_type const& rhs) 
{
	return lhs->toString() == rhs->toString();
}

template<typename check_type, typename org_type>
bool is(org_type const& tkn)
{
	return dynamic_cast<check_type const*>(tkn.get()) != nullptr;
}

template<typename convert_type>
typename convert_type::pointer_type convert(Token::pointer_type const& tkn)
{
	return std::dynamic_pointer_cast<convert_type>(tkn);
}