#include "Toolbar.h"
#include "BitmapHelper.h"
#include "resource.h"
#include <wx/sysopt.h>

Toolbar::Toolbar(wxWindow* parent, const wxString& title) : wxPanel(parent, wxID_ANY)
{
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT | wxTB_NODIVIDER);

	wxBitmap publish(BitmapHelper::GetResourceById(OPENFILEICON), wxBITMAP_TYPE_ICO_RESOURCE);
	//wxBitmap screenshot(BitmapHelper::GetResourceById(PICICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap savetopng(BitmapHelper::GetResourceById(PNGSAVEICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap savetojpeg(BitmapHelper::GetResourceById(JPEGSAVEICON), wxBITMAP_TYPE_ICO_RESOURCE);

	//toolbar->SetToolBitmapSize(wxSize(46, 46));
	toolbar->AddTool(wxID_OPEN, wxT("Open file"), publish, wxT("Open file"));
	toolbar->AddTool(wxID_SAVE, wxT("Save PNG"), savetopng, wxT("Save PNG"));
	toolbar->AddTool(wxID_SAVEAS, wxT("Save JPEG"), savetojpeg, wxT("Save JPEG"));
	toolbar->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Realize();
	hbox->Add(toolbar, wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxRIGHT));
	//hbox->AddStretchSpacer(7);

	SetSizer(hbox);
}

void Toolbar::EnableTool(int toolid, bool enable) {
	toolbar->EnableTool(toolid, enable);
}

void Toolbar::OnToolbarClick(wxCommandEvent& event) {
	event.Skip();
	event.ShouldPropagate();
}
