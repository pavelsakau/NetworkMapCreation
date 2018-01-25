#include "Application.h"
#include "resource.h"
#include "BitmapHelper.h"

bool Application::OnInit()
{
	wxInitAllImageHandlers();

	int desktopWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X) ;
	int desktopHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) ;
	mainWindow = new MainWindow(nullptr, wxID_ANY, "Network map creator", wxDefaultPosition, wxSize(desktopWidth*0.6, desktopHeight*0.6));

	mainWindow->Center();
	mainWindow->Raise();
	mainWindow->Show();

	mainWindow->SetIcon(wxIcon(BitmapHelper::GetResourceById(APPLICATIONICON), wxBITMAP_TYPE_ICO_RESOURCE));

	mainWindow->OnFileOpen(wxCommandEvent());

	return true;
}

IMPLEMENT_APP(Application);