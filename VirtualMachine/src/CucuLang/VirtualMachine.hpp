#ifndef CUC_VM_HPP
#define CUC_VM_HPP
#include "WorldInterface.hpp"
#include "Instructions.hpp"
#include "StackMachine.hpp"
#include <string>


// Execute .cuc (Cucu Bytecode).
// An interpreter would model expressions as a tree of nested objects, but
// this one here wants the speed of a flat list of instructions. I.e. emulate
// what the CPU does: with a stack.
class CVM
{
public:
	CVM();
	void Execute(std::string& bytecode);

private:
	StackMachine m_Stack;
};

#endif // CUC_VM_HPP