#ifndef RENDER_H_
#define RENDER_H_

#include <wx/wx.h>
#include <wx/sizer.h>
#include "GraphNode.h"

class Render : public wxPanel {
	wxImage imageCloud;
	int cloudW, cloudH;
	wxImage imageRouter;
	int routerW, routerH;
	wxBitmap resized;
	int w, h;
	int N;
	int Px, Py;
	int mouseX, mouseY;
	double scale;
	double prevScale;
	double zoom;
	GraphNode* graph;

public:
	Render(wxWindow* parent, wxWindowID id, const wxString& title, GraphNode* graph, int N);

	void paintEvent(wxPaintEvent & evt);
	void paintNow(bool zooming);
	void OnSize(wxSizeEvent& event);
	void render(wxDC& dc, bool zooming);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);

	bool InsideDrawRect(GraphNode* node, int w, int h);
	bool InsideRect(int x0, int y0, int x, int y, int w, int h);
};

#endif
