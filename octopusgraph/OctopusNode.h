#ifndef OCTOPUS_NODE_H
#define OCTOPUS_NODE_H

#include "llvm/IR/Function.h"

namespace Octopus {

	using namespace llvm;

	class Node {
	public:
		virtual std::string getCode() = 0;
	};

	class CFGEntryNode : public Node {
	public:
		CFGEntryNode(Function *F) : function(F) { }
		virtual std::string getCode() { return "ENTRY"; }
	private:
		Function *function;
	};

	class CFGExitNode : public Node {
	public:
		CFGExitNode(Function *F) : function(F) { }
		virtual std::string getCode() { return "EXIT"; }
	private:
		Function *function;
	};
}

#endif
