# Bilby
### A very basic, interpreted language written in C++.

## Disclaimer
Some of the code is this project was adapted from a project I did while attending Fanshawe College's [Computer Programmer Analyst](https://www.fanshawec.ca/programs/cpa2-computer-programmer-analyst/next) (CPA) programme. That project is the property of Garth Santor (gsantor@fanshaweonline.ca), a professor of the CPA programme who has given me permission to use it here. If you have any questions regarding the ownership/legality of the files in this project, feel free to contact either of us.

## What is this, exactly?
This project is partially a C++ implementation of Jlox, the language described in Bob Nystrom's book [*Crafting Intepreters*](http://craftinginterpreters.com/). Bilby shares many similarities with Jlox, but has a few key differences:
* Bilby is strongly-typed. Variables must be declared with a type, and functions must specify a return type. 
* If a program written in Bilby exits with an error, Bilby will inform the user where exactly the program failed.
* Bilby's class variables must be declared in the class itself, whereas JLox allows new variables to be added after instantiation.
* Bilby classes have a rigid structure:
```
class Foo
{
data:
    int a;
    float b;

methods:
    object init()
    {
        shout a + b;
        return me;
    }
}
```
So far, Bilby has exactly six data types:
* Integer (int)
* Floating Point (float)
* Boolean (bool)
* Word (word)
* Object (object)
* Void (void) - only used as a function/method return type

Floats and Ints can be implicitely converted to each another, but mixing any other data types will result in an error.

## How To Use
The project currently compiles as an executable and as a static library. The executable is really only useful for debugging the language. The library provides an entry class that acts as a Bilby interface for other programs. Right now I'm using it to build a small Bilby IDE.
