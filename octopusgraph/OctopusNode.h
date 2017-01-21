#ifndef OCTOPUS_NODE_H
#define OCTOPUS_NODE_H

#include <list>
#include "llvm/IR/Function.h"

namespace Octopus {

	using namespace llvm;

	class Node {
	public:
		Node();
		virtual bool isInstruction() { return false; }
		void setProperty(std::string key, std::string value);
		void setProperty(std::string key, std::list<std::string> value);
		void addProperty(std::string key, std::string value);
		std::string getCode();

		typedef std::map<std::string,std::list<std::string> >::iterator property_iterator;
		property_iterator properties_begin() { return properties.begin(); }
		property_iterator properties_end() { return properties.end(); }
		
	protected:
		std::map<std::string, std::list<std::string> > properties;
	};

	class CFGEntryNode : public Node {
	public:
		CFGEntryNode(Function *F);
	private:
		Function *function;
	};

	class CFGExitNode : public Node {
	public:
		CFGExitNode(Function *F);
	private:
		Function *function;
	};

	class FileNode : public Node {
	public:
		FileNode(std::string fn);
	private:
		std::string filename;
	};

	class LocationNode : public Node {
	public:
		LocationNode(DILocation *location);
	private:
		std::string _getCode();
		int line;
		int column;
	};
}

#endif
