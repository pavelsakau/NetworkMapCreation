#include "Render.h"
#include <wx/dcbuffer.h>

Render::Render(wxWindow* parent, wxWindowID id, const wxString& title, GraphNode* graph, int N) : N(N), graph(graph), wxPanel(parent, id)
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(Render::paintEvent));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Render::OnSize));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Render::OnMouseWheel));
	Connect(wxEVT_MOTION, wxMouseEventHandler(Render::OnMouseMotion));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(Render::OnMouseEnter));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(Render::OnMouseLeave));
	imageRouter.LoadFile(wxT("Router2.png"), wxBITMAP_TYPE_PNG);
	imageCloud.LoadFile(wxT("Cloud2.png"), wxBITMAP_TYPE_PNG);

	routerW = imageRouter.GetWidth();
	routerH = imageRouter.GetHeight();

	cloudW = imageCloud.GetWidth();
	cloudH = imageCloud.GetHeight();

	Px = 100;
	Py = 100;
	scale = 1.0;
	prevScale = scale;
	zoom = 0.9;
}

void Render::OnMouseMotion(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
		int deltaX = mouseX - event.GetX();
		int deltaY = mouseY - event.GetY();

		Px = Px + deltaX/scale;
		Py = Py + deltaY/scale;
		paintNow(false);
	}
	mouseX = event.GetX();
	mouseY = event.GetY();
}

void Render::OnMouseEnter(wxMouseEvent& event) {
	mouseX = event.GetX();
	mouseY = event.GetY();
}

void Render::OnMouseLeave(wxMouseEvent& event) {
	mouseX = event.GetX();
	mouseY = event.GetY();
}

void Render::OnMouseWheel(wxMouseEvent& event) {
	if (event.GetWheelAxis() == wxMOUSE_WHEEL_VERTICAL) {
		prevScale = scale;
		if (event.GetWheelRotation() > 0) {
			scale = scale*zoom;
		} else {
			scale = scale*(1.0/zoom);
			if (scale > 1.0) {
				scale = 1.0;
			}
		}
	}
	paintNow(true);
}

void Render::paintEvent(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this, wxBUFFER_VIRTUAL_AREA);
	//wxPaintDC dc(this);
	render(dc, false);
}

void Render::paintNow(bool zooming)
{
	//wxBufferedPaintDC dc(this, wxBUFFER_VIRTUAL_AREA);
	wxClientDC clientDC(this);
	wxBufferedDC dc((wxDC*)&clientDC);
	render(dc, zooming);
}

void Render::OnSize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void Render::render(wxDC& dc, bool zooming)
{
	dc.Clear();
	int width, height;
	dc.GetSize(&width, &height);

	int rectWidthPrev = width/prevScale;
	int rectHeightPrev = height/prevScale;

	int rectWidth = width/scale;
	int rectHeight = height/scale;

	if (zooming) {
		Px += (rectWidthPrev - rectWidth)/2;
		Py += (rectHeightPrev - rectHeight)/2;
	}

	for (int i = 0; i < N; i++) {
		if (InsideDrawRect(&graph[i], rectWidth, rectHeight)) {
			graph[i].visible = true;
		} else {
			graph[i].visible = false;
		}
	}

	for (int i = 0; i < N; i++) {
		if (graph[i].visible) {
			int coordX = (graph[i].x - Px)*scale;
			int coordY = (graph[i].y - Py)*scale;
			GraphNode::NodeAdj* curAdj = graph[i].adjanced;
			while (curAdj != nullptr) {
				if (curAdj->node->visible) {
					int coordX2 = (curAdj->node->x - Px)*scale;
					int coordY2 = (curAdj->node->y - Py)*scale;
					dc.DrawLine(wxPoint(coordX, coordY), wxPoint(coordX2, coordY2));
				}
				curAdj = curAdj->next;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (graph[i].visible) {
			int imgW = routerW/2;
			int imgH = routerH/2;
			if (graph[i].type == GraphNode::ROUTER) {
				resized = wxBitmap(imageRouter.Scale(routerW*scale, routerH*scale));
			} else {
				resized = wxBitmap(imageCloud.Scale(cloudW*scale, cloudH*scale));
				imgW = cloudW/2;
				imgH = cloudH/2;
			}
			int coordX = (graph[i].x - Px - imgW)*scale;
			int coordY = (graph[i].y - Py - imgH)*scale;
			dc.DrawBitmap(resized, coordX, coordY, true);
		}
	}
}

bool Render::InsideDrawRect(GraphNode* node, int w, int h) {
	int imgW = routerW/2;
	int imgH = routerH/2;

	if (node->type == GraphNode::CLOUD) {
		imgW = cloudW/2;
		imgH = cloudH/2;
	}

	return InsideRect(node->x - imgW, node->y - imgH, Px, Py, w, h) || InsideRect(node->x + imgW, node->y + imgH, Px, Py, w, h) || 
		InsideRect(node->x - imgW, node->y + imgH, Px, Py, w, h) || InsideRect(node->x + imgW, node->y - imgH, Px, Py, w, h);
}

bool Render::InsideRect(int x0, int y0, int x, int y, int w, int h) {
	return (x < x0 && y < y0 && x + w > x0 && y + h > y0);
}