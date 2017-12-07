#ifndef ARPCACHE_ENTRY_H_
#define ARPCACHE_ENTRY_H_

#include <wx/wx.h>

class ARPCacheEntry
{
public:

	static int macAddressXMLId;
	static int ipAddressXMLId;
	static int notesXMLId;

	wxString macAddress;
	wxString ipAddress;
	wxString notes;
};

#endif
