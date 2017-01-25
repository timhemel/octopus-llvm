#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "OctopusNode.h"

namespace Octopus {

	using namespace llvm;

	Node::Node()
	{
		id = std::to_string((size_t) this);
	}

	void Node::setProperty(std::string key, std::string value)
	{
		std::list<std::string> values;
		values.push_back(value);
		setProperty(key,values);
	}

	void Node::setProperty(std::string key, std::list<std::string> value)
	{
		properties[key] = value;
	}

	void Node::addProperty(std::string key, std::string value)
	{
		properties[key].push_back(value);
	}

	std::string Node::getCode()
	{
		return *(properties["code"].begin());
	}

	CFGEntryNode::CFGEntryNode(Function *F) : Node(), function(F)
	{
		setProperty("type", "llvm_ir_function_entry");
		setProperty("code", "ENTRY");
	}

	CFGExitNode::CFGExitNode(Function *F) : Node(), function(F)
	{
		setProperty("type", "llvm_ir_function_exit");
		setProperty("code", "EXIT");
	}

	FileNode::FileNode(std::string fn) : Node(), filename(fn)
	{
		setProperty("type", "File");
		setProperty("code", fn);
	}

	LocationNode::LocationNode(DILocation *location)
	{
		line = location->getLine();
		column = location->getColumn();
		setProperty("type", "Location");
		setProperty("code", _getCode());
	}

	std::string LocationNode::_getCode()
	{
		std::string s;
		raw_string_ostream ost(s);
		ost << line << ':' << column;
		return ost.str();
	}

}
