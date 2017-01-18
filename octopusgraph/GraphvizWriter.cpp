#include "llvm/Support/raw_ostream.h"
#include "OctopusGraph.h"
#include "GraphvizWriter.h"

using namespace Octopus;

namespace GraphWriter{

	void GraphvizWriter::writeHeader()
	{
		ost << "digraph G {\n";
	}

	void GraphvizWriter::writeFooter()
	{
		ost << "}\n";
	}

	void GraphvizWriter::writeNode(Node *n)
	{
		ost << "\tv" << (size_t) n;
		ost << "[label=\"" << n->getCode() << "\"];";
		ost << "\n";
	}

	void GraphvizWriter::writeNodes(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::node_iterator n = octopus_graph.node_begin(), ne = octopus_graph.node_end(); n != ne; ++n) {
			writeNode(*n);
		}
	}

	void GraphvizWriter::writeEdge(const Edge &e)
	{
		size_t source_id = (size_t) e.source_node;
		ost << "\tv" << source_id;
		ost << " -> v";
		size_t destination_id = (size_t) e.destination_node;
		ost << destination_id;
		ost << " [label=" << e.label << "]\n";
	}

	void GraphvizWriter::writeEdges(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::edge_iterator e = octopus_graph.edge_begin(), ee = octopus_graph.edge_end(); e != ee; ++e) {
			writeEdge(*e);
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

}
