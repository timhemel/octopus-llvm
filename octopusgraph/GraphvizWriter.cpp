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
		ost << "[label=\"";
		int i = 0;
		for(Node::property_iterator p = n->properties_begin(), E = n->properties_end(); p != E; ++p, ++i) {
			if (i) {
				ost << "\\n";
			}
			ost << p->first << '=';
			int j = 0;
			for(std::list<std::string>::iterator v = p->second.begin(), ve = p->second.end(); v != ve; ++v, ++j) {
				if (j) {
					ost << ',';
				}
				ost << *v;
			}
		}
		ost << "\"]\n";
	}

	void GraphvizWriter::writeNodes(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::node_iterator n = octopus_graph.node_begin(), ne = octopus_graph.node_end(); n != ne; ++n) {
			writeNode(*n);
		}
	}

	void GraphvizWriter::writeEdge(const Edge *e)
	{
		size_t source_id = (size_t) e->source_node;
		ost << "\tv" << source_id;
		ost << " -> v";
		size_t destination_id = (size_t) e->destination_node;
		ost << destination_id;
		ost << " [label=\"" << e->label;
		for(Edge::property_iterator p = e->properties_begin(), E = e->properties_end(); p != E; ++p) {
			ost << "\\n" << p->first << '=' << p->second;
		}
		ost << "\"]\n";
	}

	void GraphvizWriter::writeEdges(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::edge_iterator e = octopus_graph.edge_begin(), ee = octopus_graph.edge_end(); e != ee; ++e) {
			writeEdge(*e);
		}
	}

	void GraphvizWriter::writeOctopusGraph(OctopusGraph &octopus_graph)
	{
		writeHeader();
		writeNodes(octopus_graph);
		writeEdges(octopus_graph);
		writeFooter();
	}

}
