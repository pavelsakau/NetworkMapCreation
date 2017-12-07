#ifndef GRAPH_NODE_H_
#define GRAPH_NODE_H_

class GraphNode {
public:
	enum NodeType {ROUTER, CLOUD};
	GraphNode() : adjanced(nullptr) {};
	GraphNode(const int& x,const int& y, const wxString& text, const NodeType& type) : x(x), y(y), text(text), type(type), adjanced(nullptr) {};
	struct NodeAdj {
		GraphNode* node;
		NodeAdj* next;
	};

	int x, y;
	wxString text;
	NodeType type;
	NodeAdj* adjanced;
	bool visible;
	GraphNode* addAdjancedNode(GraphNode* node) {
		NodeAdj* newAdjanced = new NodeAdj;
		newAdjanced->next = adjanced;
		newAdjanced->node = node;
		adjanced = newAdjanced;
		return this;
	};
};

#endif
