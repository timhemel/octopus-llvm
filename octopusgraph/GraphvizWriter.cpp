#include "llvm/Support/raw_ostream.h"
#include "OctopusGraph.h"
#include "GraphvizWriter.h"

using namespace Octopus;

namespace {

	void writeHeader()
	{
		errs() << "digraph G {\n";
	}

	void writeFooter()
	{
		errs() << "}\n";
	}

	void writeNode(Node *n)
	{
		errs() << "\tv" << (size_t) n;
		errs() << "[label=\"" << n->getCode() << "\"];";
		errs() << "\n";
	}

	void writeNodes(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::node_iterator n = octopus_graph.node_begin(), ne = octopus_graph.node_end(); n != ne; ++n) {
			writeNode(*n);
		}
	}

	void writeEdge(const Edge &e)
	{
		size_t source_id = (size_t) &e.source_node;
		errs() << "\tv" << source_id;
		errs() << " -> ";
		size_t destination_id = (size_t) &e.destination_node;
		errs() << destination_id;
		errs() << " [label=" << e.label << "]\n";
	}

	void writeEdges(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::edge_iterator e = octopus_graph.edge_begin(), ee = octopus_graph.edge_end(); e != ee; ++e) {
			writeEdge(*e);
		}
	}

}

void GraphvizWriter::writeOctopusGraph(OctopusGraph &octopus_graph)
{
	// write header
	writeHeader();
	// write all the file nodes
	// write all the location nodes
	// write all the cfg entry/ cfg exit nodes
	// write all the instruction nodes
	writeNodes(octopus_graph);
	// get unique edges, write all the edges
	writeEdges(octopus_graph);
	// write footer
	writeFooter();
}

