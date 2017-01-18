#ifndef INSTRUCTION_NODE_H
#define INSTRUCTION_NODE_H

#include <ostream>
#include "llvm/IR/Function.h"
#include "OctopusNode.h"
// #include "OctopusGraph.h"

namespace Octopus {

	class OctopusGraph;

	// base class for instruction nodes
	class InstructionNode : public Node {
	public:
		InstructionNode(OctopusGraph &ograph, Instruction *instruction);
		virtual std::string getCode();
		Instruction *getLLVMInstruction() { return llvm_instruction; }
		virtual bool needsSlot();

	protected:

		virtual void renderLHS(std::ostream &ost);
		virtual void renderOpcode(std::ostream &ost);
		virtual void renderOperands(std::ostream &ost);
	private:
		OctopusGraph &octopus_graph;
		Instruction *llvm_instruction;
		std::string code;
	};


}

#endif
