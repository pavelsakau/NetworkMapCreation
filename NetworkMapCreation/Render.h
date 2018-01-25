#ifndef RENDER_H_
#define RENDER_H_

#include <wx/wx.h>
#include <wx/sizer.h>
#include "Graph.h"
#include <map>

using namespace std;

class Render : public wxPanel {
	wxImage imageCloud;
	wxImage imageRouter;
	wxImage imageCloudUnknown;
	wxBitmap resized;
	int w, h;
	int Px, Py;
	int mouseX, mouseY;
	double scale;
	double prevScale;
	double zoom;
	GraphSpace::Graph* graph;
	wxFont* originalFont;
	int maxX;
	int maxY;

	map<int, wxBitmap*> routerBitmapCache;
	map<int, wxBitmap*> cloudBitmapCache;
	map<int, wxBitmap*> cloudUnknownBitmapCache;

	static double dist(wxPoint a, wxPoint b);
	static double getLineDist(wxPoint a, wxPoint b, wxPoint center);

public:
	static int cloudW, cloudH;
	static int routerW, routerH;
	static int cloudUnknownW, cloudUnknownH;
	Render(wxWindow* parent, wxWindowID id, const wxString& title, GraphSpace::Graph* graph);

	void paintEvent(wxPaintEvent & evt);
	void paintNow(bool zooming);
	void OnSize(wxSizeEvent& event);
	void render(wxDC& dc, bool zooming);
	void renderToFile(wxString filename, bool asPng);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);

	void setGraph(GraphSpace::Graph* graph);
	void setMaxDimensions(int maxX, int maxY);
	void setViewport(int px, int py);
	bool InsideDrawRect(GraphNode* node, int w, int h);
	static bool InsideRect(int x0, int y0, int x, int y, int w, int h);
};

#endif
