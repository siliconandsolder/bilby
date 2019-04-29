#pragma once
#include "../BasicInterpreter/entry.hpp"

using namespace System;

namespace BasicWrapper 
{
	public ref class Wrapper
	{
	public:
		static void Execute(String^ program);
		static void SetConsoleHandle(IntPtr handle);
	};
}
