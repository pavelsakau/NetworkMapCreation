#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <wx/wx.h>
#include "MainWindow.h"

class Application : public wxApp
{
private:
	MainWindow* mainWindow;

public:
	bool OnInit();
};

#endif
