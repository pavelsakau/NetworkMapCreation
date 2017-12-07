#ifndef XML_LOADER_H_
#define XML_LOADER_H_

#include <wx/wx.h>
#include "tinyxml2.h"
#include "ARPCacheEntry.h"
#include "BridgeTableEntry.h"
#include "IPAddressEntry.h"
#include "IPRouteEntry.h"
#include <map>
#include <vector>

using namespace std;

class XmlLoader
{
	vector<ARPCacheEntry* > arpCache;
	vector<BridgeTableEntry* > bridges;
	vector<IPAddressEntry* > ipAddresses;
	vector<IPRouteEntry* > ipRoutes;

public:

	XmlLoader(const wxString& filename);
	tinyxml2::XMLElement* findWorksheet(tinyxml2::XMLDocument* doc, const wxString& sheetName);
	void loadArpCache(tinyxml2::XMLDocument* doc);
	void loadBridges(tinyxml2::XMLDocument* doc);
	void loadIPAddresses(tinyxml2::XMLDocument* doc);
	void loadIPRoutes(tinyxml2::XMLDocument* doc);
};

#endif
