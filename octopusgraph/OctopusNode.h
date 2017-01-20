#ifndef OCTOPUS_NODE_H
#define OCTOPUS_NODE_H

#include "llvm/IR/Function.h"

namespace Octopus {

	using namespace llvm;

	class Node {
	public:
		virtual std::string getCode() = 0;
		virtual bool isInstruction() { return false; }
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

	class FileNode : public Node {
	public:
		FileNode(std::string fn) : filename(fn) { }
		virtual std::string getCode() { return filename; }
	private:
		std::string filename;
	};

	class LocationNode : public Node {
	public:
		LocationNode(DILocation *location);
		virtual std::string getCode();
	private:
		int line;
		int column;
	};
}

#endif
