#include "MainWindow.h"
#include "XMLLoader.h"
#include "GraphBuilder.h"
#include <wx/filedlg.h>
#include <wx/dialog.h>
#include <wx/filename.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <thread>
#include <chrono>

MainWindow::MainWindow(wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & pos, const wxSize & size, long style, const wxString & name)
	: wxFrame(parent, id, title, pos, size, style, name)
{
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	toolbar = new Toolbar(this, wxT("Toolbar"));
	vbox->Add(toolbar, 0, wxALIGN_TOP | wxEXPAND);

	render = new Render(this, wxID_ANY, wxT("Render"), nullptr);
	vbox->Add(render, 1, wxALIGN_TOP | wxEXPAND);

	SetSizer(vbox);

	Connect(wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnFileOpen), nullptr, this);
	Connect(wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnFileSave), nullptr, this);
	Connect(wxID_SAVEAS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnFileSaveAs), nullptr, this);
	//Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindow::OnClose), nullptr, this);

	graph = nullptr;
	loader = nullptr;
}

void MainWindow::OnClose(wxCloseEvent& event) {
	if (loader != nullptr) {
		delete loader;
		loader = nullptr;
	}
	if (graph != nullptr) {
		delete graph;
		graph = nullptr;
	}
	event.Skip();
	event.ShouldPropagate();
}

void MainWindow::OnFileOpen(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, wxT("Open XML file"), "", "", "XML files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wxString path = openFileDialog.GetPath();
	openFileDialog.Destroy();

	wxFileName xmlFile(path);

	wxFrame* dialog = new wxFrame(this, wxID_ANY, "Load", wxDefaultPosition, wxSize(450, 200), wxFULL_REPAINT_ON_RESIZE | (wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX)));
	wxBoxSizer *vbox = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* panel = new wxPanel(dialog);
	wxBoxSizer *vboxpanel = new wxBoxSizer(wxVERTICAL);
	wxStaticText* label = new wxStaticText(panel, wxID_ANY, wxString::Format(wxT("Loading %s, please wait"), xmlFile.GetFullName()), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	vboxpanel->AddStretchSpacer(1);
	vboxpanel->Add(label, 1, wxEXPAND);
	panel->SetSizer(vboxpanel);
	vbox->Add(panel, 1, wxALIGN_CENTER | wxEXPAND);
	dialog->SetSizer(vbox);
	dialog->CenterOnParent();
	dialog->Raise();
	dialog->Show();
	dialog->Update();

	if (loader != nullptr) {
		delete loader;
		loader = nullptr;
	}
	if (graph != nullptr) {
		delete graph;
		graph = nullptr;
	}
	loader = new XmlLoader(path);
	graph = GraphBuilder::buildGraph(loader);
	graph = GraphBuilder::layoutGraphSugiyama(graph);
	//graph = GraphBuilder::layoutGraphFMMM(graph);

	dialog->Close();
	openFileDialog.Destroy();
	dialog->Destroy();
	delete dialog;

	int maxX = 0;
	int maxY = 0;
	for (int i = 0; i < graph->nodesCount; i++) {
		if (maxX < graph->nodes[i]->x) {
			maxX = graph->nodes[i]->x;
		}
		if (maxY < graph->nodes[i]->y) {
			maxY = graph->nodes[i]->y;
		}
	}
	render->setMaxDimensions(maxX, maxY);
	render->setGraph(graph);
	render->paintNow(false);
}

void MainWindow::OnFileSave(wxCommandEvent& event) {
	OnFileSaveCustom(true);
}

void MainWindow::OnFileSaveAs(wxCommandEvent& event) {
	OnFileSaveCustom(false);
}

void MainWindow::OnFileSaveCustom(bool saveAsPNG) {
	wxString dialogStr1, dialogStr2;
	if (saveAsPNG) {
		dialogStr1 = wxT("Save PNG file");
		dialogStr2 = wxT("PNG files (*.png)|*.png");
	} else {
		dialogStr1 = wxT("Save JPEG file");
		dialogStr2 = wxT("JPEG files (*.jpg)|*.jpg");
	}
	wxFileDialog openFileDialog(this, dialogStr1, "", "", dialogStr2, wxFD_SAVE);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wxString path = openFileDialog.GetPath();
	openFileDialog.Destroy();

	wxFileName pngFile(path);
	if (pngFile.Exists()) {
		wxMessageDialog* dlg = new wxMessageDialog(this, wxString::Format(wxT("File %s already exists, overwrite?"), pngFile.GetFullName()), "Overwrite?", wxYES_NO);
		if (dlg->ShowModal() == wxID_NO) {
			return;
		}
		dlg->Destroy();
		delete dlg;
	}

	wxFrame* dialog = new wxFrame(this, wxID_ANY, "Save", wxDefaultPosition, wxSize(450, 200), wxFULL_REPAINT_ON_RESIZE | (wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX)));
	wxBoxSizer *vbox = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* panel = new wxPanel(dialog);
	wxBoxSizer *vboxpanel = new wxBoxSizer(wxVERTICAL);
	wxStaticText* label = new wxStaticText(panel, wxID_ANY, wxString::Format(wxT("Saving %s, please wait"), pngFile.GetFullName()), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	vboxpanel->AddStretchSpacer(1);
	vboxpanel->Add(label, 1, wxEXPAND);
	panel->SetSizer(vboxpanel);

	vbox->Add(panel, 1, wxALIGN_CENTER | wxEXPAND);
	dialog->SetSizer(vbox);
	dialog->CenterOnParent();
	dialog->Raise();
	dialog->Show();
	dialog->Update();

	render->renderToFile(path, saveAsPNG);

	dialog->Close();
	openFileDialog.Destroy();
	dialog->Destroy();
	delete dialog;
}