#ifndef IP_ROUTE_ENTRY_H_
#define IP_ROUTE_ENTRY_H_

#include <wx/wx.h>

class IPRouteEntry
{
public:
	static int routerNameXMLId;
	static int routerIPXMLId;
	static int interfaceNameXMLId;
	static int routeXMLId;
	static int maskXMLId;
	static int nextHopXMLId;

	wxString routerName;
	wxString routerIP;
	wxString interfaceName;
	wxString route;
	wxString mask;
	wxString nextHop;
};

#endif
