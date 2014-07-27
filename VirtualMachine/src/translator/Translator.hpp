#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP
#include "Validator.hpp"
#include <string>


// Convert bot files to .cuc - Cucu Bytecode
class Translator
{
public:
	static std::string Convert();
};

#endif // TRANSLATOR_HPP