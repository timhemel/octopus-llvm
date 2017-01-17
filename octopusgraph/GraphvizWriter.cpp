#include "llvm/Support/raw_ostream.h"
#include "OctopusGraph.h"
#include "GraphvizWriter.h"

using namespace Octopus;

namespace {
	void writeNode(Node *n)
	{
		errs() << "v" << (size_t) n;
		errs() << "[label=\"" << n->getCode() << "\"];";
		errs() << "\n";
	}
}

void GraphvizWriter::writeOctopusGraph(OctopusGraph &octopus_graph)
{
	// write header
	// write all the file nodes
	// write all the location nodes
	// write all the cfg entry/ cfg exit nodes
	// write all the instruction nodes
	for(OctopusGraph::node_iterator n = octopus_graph.node_begin(), ne = octopus_graph.node_end(); n != ne; ++n) {
		writeNode(*n);
	}
	// get unique edges, write all the edges
	// write footer
}

