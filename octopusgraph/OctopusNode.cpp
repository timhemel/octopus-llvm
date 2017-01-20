#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "OctopusNode.h"

namespace Octopus {

	using namespace llvm;

	LocationNode::LocationNode(DILocation *location)
	{
		line = location->getLine();
		column = location->getColumn();
	}

	std::string LocationNode::getCode()
	{
		std::string s;
		raw_string_ostream ost(s);
		ost << line << ':' << column;
		return ost.str();
	}


}
