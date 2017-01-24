#include "OctopusGraph.h"
#include "DGSWriter.h"

using namespace Octopus;

namespace GraphWriter {

	void DGSWriter::writeHeader()
	{
		ost << "DGS004\n";
		ost << "null 0 0\n"; 
	}

	void DGSWriter::writeFooter()
	{
	}

	void DGSWriter::writeEscaped(std::string const &s)
	{
		for(std::string::const_iterator i=s.begin(),e=s.end(); i != e; ++i) {
			char c = *i;
			switch(c) {
				case '\\':
				case '"':
					ost << '\\';
					break;
				default:
					break;
			}
			ost << c;
		}
	}

	void DGSWriter::writeNodePropertyValues(std::list<std::string> &properties)
	{
		int j = 0;
		for(std::list<std::string>::iterator v = properties.begin(), ve = properties.end(); v != ve; ++v, ++j) {
			if (j) {
				ost << ',';
			}
			ost << '"';
			writeEscaped(*v);
			ost << '"';
		}
	}

	void DGSWriter::writeNodeProperties(Node *n)
	{
		for(Node::property_iterator p = n->properties_begin(), E = n->properties_end(); p != E; ++p) {
			ost << " \"";
			writeEscaped(p->first);
		       	ost << "\"=";
			writeNodePropertyValues(p->second);
		}
	}

	void DGSWriter::writeNode(Node *n)
	{
		ost << "an " << (size_t) n;
		writeNodeProperties(n);
		ost << "\n";
	}

	void DGSWriter::writeNodes(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::node_iterator n = octopus_graph.node_begin(), ne = octopus_graph.node_end(); n != ne; ++n) {
			writeNode(*n);
		}
	}

	void DGSWriter::writeEdgeProperties(const Edge *e)
	{
		for(Edge::property_iterator p = e->properties_begin(), E = e->properties_end(); p != E; ++p) {
			ost << " \"";
			writeEscaped(p->first);
			ost << "\"=\"";
			writeEscaped(p->second);
			ost << '"';
		}
	}

	void DGSWriter::writeEdge(const Edge *e)
	{
		size_t source_id = (size_t) e->source_node;
		size_t destination_id = (size_t) e->destination_node;
		ost << "ae ";
		ost << '"';
		writeEscaped(e->label);
		ost << ':' << source_id << ':' << destination_id << '"';
		ost << ' ' << source_id << " > " << destination_id;
		writeEdgeProperties(e);
		ost << '\n';
	}


	void DGSWriter::writeEdges(OctopusGraph &octopus_graph)
	{
		for(OctopusGraph::edge_iterator e = octopus_graph.edge_begin(), ee = octopus_graph.edge_end(); e != ee; ++e) {
			writeEdge(*e);
		}
	}

	void DGSWriter::writeOctopusGraph(OctopusGraph &octopus_graph)
	{
		writeHeader();
		writeNodes(octopus_graph);
		writeEdges(octopus_graph);
		writeFooter();
	}
}
