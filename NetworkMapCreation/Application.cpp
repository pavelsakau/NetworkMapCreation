#include "Application.h"

bool Application::OnInit()
{
	wxInitAllImageHandlers();

	int desktopWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X) ;
	int desktopHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) ;
	mainWindow = new MainWindow(nullptr, wxID_ANY, "Batch File Creator Tool", wxDefaultPosition, wxSize(desktopWidth*0.6, desktopHeight*0.6));

	mainWindow->Center();
	mainWindow->Raise();
	mainWindow->Show();

	return true;
}

IMPLEMENT_APP(Application);