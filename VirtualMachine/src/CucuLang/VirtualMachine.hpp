#ifndef CUC_VM_HPP
#define CUC_VM_HPP
#include "WorldInterface.hpp"
#include "Instructions.hpp"
#include "StackMachine.hpp"
#include <ctype.h>
#include <string>
#include <map>


// Execute .cuc (Cucu Bytecode).
// An interpreter would model expressions as a tree of nested objects, but
// this one here wants the speed of a flat list of instructions. I.e. emulate
// what the CPU does: with a stack.
class CVM
{
	using Address_t = uint8_t; // unsigned char
	//using Primitive_t = uint8_t; // unsigned char
	using Byte_t = uint8_t;
public:
	CVM();
	void Execute(std::string& bytecode);

private:
	StackMachine m_Stack;
	std::map<Address_t, Byte_t> m_AddressTable;

	// To be created for each separate scope
	std::map<std::string, Address_t> m_SymbolTable;
};

#endif // CUC_VM_HPP