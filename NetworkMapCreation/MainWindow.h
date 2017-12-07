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

using namespace std;

class MainWindow : public wxFrame
{
private:
	Toolbar* toolbar;

public:
	MainWindow(wxWindow *parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
	void OnFileOpen(wxCommandEvent& event);
	GraphNode* CreateGraph();
};

#endif

