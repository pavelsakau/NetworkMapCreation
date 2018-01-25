#ifndef GRAPH_NODE_H_
#define GRAPH_NODE_H_

class GraphNode {
public:
	enum NodeType {ROUTER, CLOUD, UNKNOWN};
	GraphNode() : x(0), y(0), text(""), type(NodeType::UNKNOWN), visible(true), adjanced(nullptr), index(0), component(-1), networkIp(0), mask(0) {};
	GraphNode(const int& x,const int& y, const wxString& text, const NodeType& type) : x(x), y(y), text(text), type(type), adjanced(nullptr), visible(true), index(0), component(-1), networkIp(0), mask(0) {};
	struct NodeAdj {
		GraphNode* node;
		NodeAdj* next;
	};
	struct Coords {
		int x, y, w, h;
		Coords() : x(0), y(0), w(0), h(0) {};
		Coords(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};
	};

	int x, y;
	wxString text;
	NodeType type;
	NodeAdj* adjanced;
	bool visible;
	int index;
	int component;
	unsigned int networkIp;
	unsigned int mask;
	GraphNode* addAdjancedNode(GraphNode* node) {
		NodeAdj* newAdjanced = new NodeAdj;
		newAdjanced->next = adjanced;
		newAdjanced->node = node;
		adjanced = newAdjanced;
		return this;
	};
};

#endif
