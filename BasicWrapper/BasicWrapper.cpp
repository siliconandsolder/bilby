#include "stdafx.h"

#include "BasicWrapper.h"

static const char* convertToCString(String^ string)
{
	const char* str = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(string)).ToPointer();
	return str;
}

void BasicWrapper::Wrapper::Execute(String^ program)
{
	Entry::interpret(convertToCString(program));
}

void BasicWrapper::Wrapper::SetConsoleHandle(IntPtr handle)
{
	Entry::setStdHandle(handle.ToPointer());
}
