#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/dirdlg.h>
#include <wx/filefn.h> 
#include <wx/filename.h>
#include <wx/ffile.h>
#include "Toolbar.h"
#include "GraphNode.h"
#include "Render.h"
#include "XMLLoader.h"

using namespace std;

class MainWindow : public wxFrame
{
private:
	Toolbar* toolbar;
	Render* render;
	XmlLoader* loader;
	GraphSpace::Graph* graph;

public:
	MainWindow(wxWindow *parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
	void OnFileOpen(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileSaveAs(wxCommandEvent& event);
	void OnFileSaveCustom(bool saveAsPNG);
	void OnClose(wxCloseEvent& event);
};

#endif

