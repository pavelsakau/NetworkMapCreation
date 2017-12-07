#include "MainWindow.h"
#include "XMLLoader.h"
#include "Render.h"
#include <wx/filedlg.h>

MainWindow::MainWindow(wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & pos, const wxSize & size, long style, const wxString & name)
	: wxFrame(parent, id, title, pos, size, style, name)
{
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	toolbar = new Toolbar(this, wxT("Toolbar"));
	vbox->Add(toolbar, 0, wxALIGN_TOP | wxEXPAND);

	GraphNode* graph = CreateGraph();
	Render* render = new Render(this, wxID_ANY, wxT("Render"), graph, 14);
	vbox->Add(render, 1, wxALIGN_TOP | wxEXPAND);

	SetSizer(vbox);

	Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnFileOpen), nullptr, this);
}

GraphNode* MainWindow::CreateGraph() {
	GraphNode* graph = new GraphNode[14];
	graph[0].x = 200;
	graph[0].y = 200;
	graph[0].type = GraphNode::ROUTER;
	graph[0].addAdjancedNode(&graph[1]);

	graph[1].x = 370;
	graph[1].y = 240;
	graph[1].type = GraphNode::CLOUD;
	graph[1].addAdjancedNode(&graph[0])->addAdjancedNode(&graph[2]);

	graph[2].x = 600;
	graph[2].y = 300;
	graph[2].type = GraphNode::ROUTER;
	graph[2].addAdjancedNode(&graph[3])->addAdjancedNode(&graph[7]);

	graph[3].x = 400;
	graph[3].y = 400;
	graph[3].type = GraphNode::CLOUD;
	graph[3].addAdjancedNode(&graph[2])->addAdjancedNode(&graph[4]);

	graph[4].x = 200;
	graph[4].y = 630;
	graph[4].type = GraphNode::ROUTER;
	graph[4].addAdjancedNode(&graph[3])->addAdjancedNode(&graph[5]);

	graph[5].x = 550;
	graph[5].y = 640;
	graph[5].type = GraphNode::CLOUD;
	graph[5].addAdjancedNode(&graph[4])->addAdjancedNode(&graph[6]);

	graph[6].x = 650;
	graph[6].y = 570;
	graph[6].type = GraphNode::ROUTER;
	graph[6].addAdjancedNode(&graph[5])->addAdjancedNode(&graph[7])->addAdjancedNode(&graph[10]);

	graph[7].x = 780;
	graph[7].y = 280;
	graph[7].type = GraphNode::CLOUD;
	graph[7].addAdjancedNode(&graph[2])->addAdjancedNode(&graph[6])->addAdjancedNode(&graph[11]);

	graph[8].x = 1100;
	graph[8].y = 630;
	graph[8].type = GraphNode::CLOUD;
	graph[8].addAdjancedNode(&graph[9]);

	graph[9].x = 835;
	graph[9].y = 635;
	graph[9].type = GraphNode::ROUTER;
	graph[9].addAdjancedNode(&graph[8])->addAdjancedNode(&graph[10]);

	graph[10].x = 870;
	graph[10].y = 510;
	graph[10].type = GraphNode::CLOUD;
	graph[10].addAdjancedNode(&graph[6])->addAdjancedNode(&graph[9])->addAdjancedNode(&graph[11]);

	graph[11].x = 1050;
	graph[11].y = 310;
	graph[11].type = GraphNode::ROUTER;
	graph[11].addAdjancedNode(&graph[7])->addAdjancedNode(&graph[10])->addAdjancedNode(&graph[12]);

	graph[12].x = 870;
	graph[12].y = 230;
	graph[12].type = GraphNode::CLOUD;
	graph[12].addAdjancedNode(&graph[11])->addAdjancedNode(&graph[13]);

	graph[13].x = 680;
	graph[13].y = 210;
	graph[13].type = GraphNode::ROUTER;
	graph[13].addAdjancedNode(&graph[12]);

	return graph;
}

void MainWindow::OnFileOpen(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, wxT("Open XML file"), "", "", "XML files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	XmlLoader loader(openFileDialog.GetPath());
}
