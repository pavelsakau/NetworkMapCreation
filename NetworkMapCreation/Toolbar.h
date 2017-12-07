#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/radiobut.h>

class Toolbar : public wxPanel
{
private:
	wxToolBar* toolbar;

public:
	Toolbar(wxWindow* parent, const wxString& title);
	void OnToolbarClick(wxCommandEvent& event);
	void EnableTool(int toolid, bool enable);
};

#endif

