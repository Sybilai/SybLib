#include "../CucuLang/Instructions.hpp"
#include "Translator.hpp"


std::string Translator::Convert()
{
	std::string code;
	code += Instruction::PLANT_BOMB;
	return code;
}