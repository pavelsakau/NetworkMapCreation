#include "Render.h"
#include <wx/dcbuffer.h>
#include <math.h>
#include "BitmapHelper.h"
#include "resource.h"

int Render::cloudW = 0;
int Render::cloudH = 0;
int Render::routerW = 0;
int Render::routerH = 0;
int Render::cloudUnknownW = 0;
int Render::cloudUnknownH = 0;

Render::Render(wxWindow* parent, wxWindowID id, const wxString& title, GraphSpace::Graph* graph) : graph(graph), originalFont(nullptr), wxPanel(parent, id)
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(Render::paintEvent));
	Connect(wxEVT_SIZE, wxSizeEventHandler(Render::OnSize));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Render::OnMouseWheel));
	Connect(wxEVT_MOTION, wxMouseEventHandler(Render::OnMouseMotion));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(Render::OnMouseEnter));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(Render::OnMouseLeave));

	wxBitmap router(BitmapHelper::GetResourceById(ROUTERPNG), wxBITMAP_TYPE_PNG_RESOURCE);
	wxBitmap cloud(BitmapHelper::GetResourceById(CLOUDPNG), wxBITMAP_TYPE_PNG_RESOURCE);
	//wxBitmap cloudUnknown(BitmapHelper::GetResourceById(CLOUDUNKNOWNPNG), wxBITMAP_TYPE_PNG_RESOURCE);
	wxBitmap cloudUnknown(BitmapHelper::GetResourceById(CLOUDUNKNOWNYELLOWPNG), wxBITMAP_TYPE_PNG_RESOURCE);

	imageRouter = router.ConvertToImage();
	imageCloud = cloud.ConvertToImage();
	imageCloudUnknown = cloudUnknown.ConvertToImage();

	routerW = imageRouter.GetWidth();
	routerH = imageRouter.GetHeight();

	cloudW = imageCloud.GetWidth();
	cloudH = imageCloud.GetHeight();

	cloudUnknownW = imageCloudUnknown.GetWidth();
	cloudUnknownH = imageCloudUnknown.GetHeight();

	Px = 1000;
	Py = 1000;
	scale = 1.0;
	prevScale = scale;
	zoom = 0.9;
}

void Render::setGraph(GraphSpace::Graph* graph) {
	this->graph = graph;

	wxClientDC clientDC(this);
	int width, height;
	clientDC.GetSize(&width, &height);
	double startScale = (double)width/(double)maxX;
	double minPossibleScale = 0.1;
	if (startScale > 1.0) {
		startScale = 1.0;
	} 
	if ((double)height/(double)maxY < startScale && ((double)height/(double)maxY >= minPossibleScale)) {
		startScale = (double)height/(double)maxY;
		if (startScale > 1.0) {
			startScale = 1.0;
		}
	}
	if (startScale < minPossibleScale) {
		startScale = minPossibleScale;
	}
	int rw = width/startScale;
	int rh = height/startScale;
	Px = (maxX - rw)/2;
	Py = (maxY - rh)/2;
	scale = startScale;
	prevScale = scale;
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
		if (!(event.GetWheelRotation() > 0)) {
			scale = scale*zoom;
			if (scale < 0.01) {
				scale = 0.01;
			}
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
	render(dc, false);
}

void Render::paintNow(bool zooming)
{
	wxClientDC clientDC(this);
	wxBufferedDC dc((wxDC*)&clientDC);
	render(dc, zooming);
}

void Render::OnSize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void Render::renderToFile(wxString filename, bool asPng) {
	bool settingScale = false;
	__int64 bitmapWidth = maxX + 150;
	__int64 bitmapHeigh = maxY + 150;
	__int64 bitmapPixelsCount = bitmapWidth*bitmapHeigh;

	__int64 maxPixelsCount = 700000000;

	double drawScale = 1.0;
	if (bitmapPixelsCount > maxPixelsCount) {
		drawScale = (double)maxPixelsCount/bitmapPixelsCount;
	}

	wxBitmap* drawingPngBitmap = new wxBitmap(wxSize((double)bitmapWidth*drawScale, (double)bitmapHeigh*drawScale));
	wxMemoryDC* memoryDC = new wxMemoryDC(*drawingPngBitmap);
	int savePx = Px;
	int savePy = Py;
	double saveScale = scale;
	double savePrevScale = prevScale;
	scale = drawScale;
	prevScale = drawScale;
	setViewport(0, 0);
	render(*memoryDC, false);
	Px = savePx;
	Py = savePy;
	scale = saveScale;
	prevScale = savePrevScale;
	if (asPng) {
		drawingPngBitmap->ConvertToImage().SaveFile(filename, wxBITMAP_TYPE_PNG);
	} else {
		drawingPngBitmap->ConvertToImage().SaveFile(filename, wxBITMAP_TYPE_JPEG);
	}

	delete memoryDC;
	delete drawingPngBitmap;
}

void Render::setViewport(int px, int py) {
	this->Px = px;
	this->Py = py;
}

double Render::dist(wxPoint a, wxPoint b) {
	return sqrt(((double)a.x - (double)b.x)*((double)a.x - (double)b.x) + ((double)a.y - (double)b.y)*((double)a.y - (double)b.y));
}

double Render::getLineDist(wxPoint a, wxPoint b, wxPoint center) {
	double d = dist(a, b);
	wxRealPoint n = wxRealPoint(-(b.y - a.y)/d, (b.x - a.x)/d);
	return (n.x*center.x + n.y*center.y - n.x*a.x - n.y*a.y)/(n.x*n.x + n.y*n.y);
}

void Render::render(wxDC& dc, bool zooming)
{
	if (originalFont == nullptr) {
		wxFont font = dc.GetFont();
		originalFont = new wxFont(*font.GetNativeFontInfo());
	}
	wxFont* fontResized;
	dc.Clear();
	if (graph != nullptr) {
		int width, height;

		dc.GetSize(&width, &height);

		int rectWidthPrev = width/prevScale;
		int rectHeightPrev = height/prevScale;

		int rectWidth = width/scale;
		int rectHeight = height/scale;

		fontResized = new wxFont(*originalFont);

		if (zooming) {
			Px += (rectWidthPrev - rectWidth)/2;
			Py += (rectHeightPrev - rectHeight)/2;
		}

		for (int i = 0; i < graph->nodesCount; i++) {
			if (InsideDrawRect(graph->nodes[i], rectWidth, rectHeight)) {
				graph->nodes[i]->visible = true;
			} else {
				graph->nodes[i]->visible = false;
			}
		}

		wxPoint rectCenter = wxPoint(Px + rectWidth/2, Py + rectHeight/2);
		double rectRadius = dist(rectCenter, wxPoint(Px, Py));
		for (int i = 0; i < graph->nodesCount; i++) {
			int coordX = (graph->nodes[i]->x - Px)*scale;
			int coordY = (graph->nodes[i]->y - Py)*scale;
			GraphNode::NodeAdj* curAdj = graph->nodes[i]->adjanced;
			while (curAdj != nullptr) {
				int coordX2 = (curAdj->node->x - Px)*scale;
				int coordY2 = (curAdj->node->y - Py)*scale;
				if (getLineDist(wxPoint(graph->nodes[i]->x, graph->nodes[i]->y), wxPoint(curAdj->node->x, curAdj->node->y), rectCenter) <= rectRadius) {
					dc.DrawLine(wxPoint(coordX, coordY), wxPoint(coordX2, coordY2));
				}
				curAdj = curAdj->next;
			}
		}

		if (scale > 0.2) {
			for (int i = 0; i < graph->nodesCount; i++) {
				if (graph->nodes[i]->type == GraphNode::CLOUD) {
					dc.SetFont(*fontResized);
					wxSize txtSize = dc.GetTextExtent(graph->nodes[i]->text);
					double cloudPerc = 0.57;
					double maxW = cloudW*scale*(cloudPerc);
					if (txtSize.GetWidth() > cloudW*scale*(cloudPerc)) {
						double fondScale = (cloudW*scale*(cloudPerc))/txtSize.GetWidth();
						wxFont* newFontResized = new wxFont(fontResized->Scaled(scale));
						delete fontResized;
						fontResized = newFontResized;
					}
					break;
				}
			}
		}

		for (int i = 0; i < graph->nodesCount; i++) {
			if (graph->nodes[i]->visible) {
				int imgW = routerW/2;
				int imgH = routerH/2;
				if (graph->nodes[i]->type == GraphNode::ROUTER) {
					map<int, wxBitmap*>::iterator it;
					int maxDimension = routerW*scale;
					if (routerW*scale > routerH*scale) {
						it = routerBitmapCache.find(routerW*scale);
					} else {
						it = routerBitmapCache.find(routerH*scale);
						maxDimension = routerH*scale;
					}
					if (it != routerBitmapCache.end()) {
						resized = *it->second;
					} else {
						wxBitmap* newBitmap = new wxBitmap(imageRouter.Scale(routerW*scale, routerH*scale));
						routerBitmapCache.insert(make_pair(maxDimension, newBitmap));
						resized = *newBitmap;
					}
				} else if (graph->nodes[i]->type == GraphNode::CLOUD && graph->nodes[i]->text.compare("?") != 0) {
					map<int, wxBitmap*>::iterator it;
					int maxDimension = cloudW*scale;
					if (cloudW*scale > cloudH*scale) {
						it = cloudBitmapCache.find(cloudW*scale);
					} else {
						it = cloudBitmapCache.find(cloudH*scale);
						maxDimension = cloudH*scale;
					}
					if (it != cloudBitmapCache.end()) {
						resized = *it->second;
					} else {
						wxBitmap* newBitmap = new wxBitmap(imageCloud.Scale(cloudW*scale, cloudH*scale));
						cloudBitmapCache.insert(make_pair(maxDimension, newBitmap));
						resized = *newBitmap;
					}
					imgW = cloudW/2;
					imgH = cloudH/2;
				} else if (graph->nodes[i]->type == GraphNode::UNKNOWN) {
					map<int, wxBitmap*>::iterator it;
					int maxDimension = cloudUnknownW*scale;
					if (cloudUnknownW*scale > cloudUnknownH*scale) {
						it = cloudUnknownBitmapCache.find(cloudUnknownW*scale);
					} else {
						it = cloudUnknownBitmapCache.find(cloudUnknownH*scale);
						maxDimension = cloudUnknownH*scale;
					}
					if (it != cloudUnknownBitmapCache.end()) {
						resized = *it->second;
					} else {
						wxBitmap* newBitmap = new wxBitmap(imageCloudUnknown.Scale(cloudUnknownW*scale, cloudUnknownH*scale));
						cloudUnknownBitmapCache.insert(make_pair(maxDimension, newBitmap));
						resized = *newBitmap;
					}
					imgW = cloudUnknownW/2;
					imgH = cloudUnknownH/2;
				}
				int coordX = (graph->nodes[i]->x - Px - imgW)*scale;
				int coordY = (graph->nodes[i]->y - Py - imgH)*scale;
				dc.DrawBitmap(resized, coordX, coordY, true);

				if (scale > 0.2) {
					dc.SetFont(*fontResized);
					if (graph->nodes[i]->type == GraphNode::ROUTER) {
						wxSize txtSize = dc.GetTextExtent(graph->nodes[i]->text);
						dc.DrawText(graph->nodes[i]->text, wxPoint((graph->nodes[i]->x - Px)*scale - txtSize.GetWidth()/2, coordY + routerH*scale*1.05));
					} else if (graph->nodes[i]->type == GraphNode::CLOUD) {
						dc.DrawText(graph->nodes[i]->text, wxPoint(coordX + cloudW*scale*0.21, coordY + cloudH*scale*0.45));
					}
				}
			}
		}
		delete fontResized;
	}
	if (originalFont != nullptr) {
		dc.SetFont(*originalFont);
	}
}

void Render::setMaxDimensions(int maxX, int maxY) {
	this->maxX = maxX;
	this->maxY = maxY;
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