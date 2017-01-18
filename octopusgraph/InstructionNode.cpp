#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "InstructionNode.h"
#include "OctopusGraph.h"

namespace Octopus {

	InstructionNode::InstructionNode(OctopusGraph &ograph, Instruction *instruction) : octopus_graph(ograph)
	{
		llvm_instruction = instruction;
	}

	std::string InstructionNode::getCode()
	{
		std::string s;
		raw_string_ostream ost(s);
		llvm_instruction->print(ost);
		return ost.str();
	}

	bool InstructionNode::needsSlot()
	{
		return (!llvm_instruction->hasName() && !llvm_instruction->getType()->isVoidTy());
	}


}

