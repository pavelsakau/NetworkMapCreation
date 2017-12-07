#ifndef IP_ADDRESS_ENTRY_H_
#define IP_ADDRESS_ENTRY_H_

#include <wx/wx.h>

class IPAddressEntry
{
public:
	static int deviceNameXMLId;
	static int interfaceNameXMLId;
	static int macAddressXMLId;
	static int ipAddressXMLId;
	static int maskXMLId;
	static int subnetXMLId;

	wxString deviceName;
	wxString interfaceName;
	wxString macAddress;
	wxString ipAddress;
	wxString mask;
	wxString subnet;
};

#endif
