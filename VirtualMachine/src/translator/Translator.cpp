#include "../CucuLang/Instructions.hpp"
#include "Translator.hpp"


std::string Translator::Convert()
{
	std::string code,
		        space = " ";
	
	// Poor man's interpreter
	code += Instruction::BOO_PLANT_BOMB;
	code += space;
	code += Instruction::CUC_BYTE;
	return code;
}