#pragma once

using namespace System;

namespace BasicWrapper 
{
	public ref class Wrapper
	{
	private:
		String ^program_;
	public:
		Wrapper(String ^program)
		{
			this->program_ = program;
		}
		
		void Execute()
		{

		}
	};
}
