#ifndef CUC_INSTRUCTIONS_HPP
#define CUC_INSTRUCTIONS_HPP


// The instruction set defines the low-level operations that can be performed.
// These are encoded as a sequence of bytes.
// The Cucu VM executes these instructions one at a time, using a stack for intermediate values.
// By combining instructions, complex high-level behavior can be defined.
// The resulting combination is a list of bytes(therefore, bytecode).
// The JVM and CLR also use single byte instructions.
//
// CUC_ = language specific
// BOO_ = interface specific
enum Instruction
{
	// -------------------------
	// Flow control 0x0
	// -------------------------
	CUC_GOTO = 0x00,
	
	// -------------------------
	// Stack 0x1
	// CUC_STACK_LOAD = push a local variable onto the stack
	// CUC_STACK_STORE = pop off the top of the stack a value and store it on a local variable
	// -------------------------
	CUC_STACK_LOAD = 0x10, 
	CUC_STACK_STORE = 0x11,
	
	// -------------------------
	// Primitives 0x2
	// -------------------------
	CUC_LITERAL_NUMBER = 0x20, // < 2^7
	CUC_BYTE = 0x21, // one-byte signed integer
	//CUC_INT = 0x21,
	

	// -------------------------
	// Arithmetic 0x3
	// -------------------------
	CUC_MULTIPLY = 0x31,
	CUC_ADD = 0x32,
	CUC_SUBSTRACT = 0x33,

	// -------------------------
	// Interface 0x4
	// -------------------------
	BOO_PLANT_BOMB = 0x40,
	BOO_GO_TO = 0x41
};

#endif // CUC_INSTRUCTIONS_HPP