#ifndef INSTRUCTION_NODE_H
#define INSTRUCTION_NODE_H

#include <ostream>
#include "llvm/IR/Function.h"
#include "OctopusNode.h"

namespace Octopus {

	class InstructionNode : public Node {
	public:
		InstructionNode(Instruction *instruction);
		Instruction *getLLVMInstruction() { return llvm_instruction; }
		virtual bool isInstruction() { return true; }

	private:
		virtual std::string _getCode();

		Instruction *llvm_instruction;
	};

}

#endif
