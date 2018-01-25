#ifndef IP_ADDRESS_ENTRY_H_
#define IP_ADDRESS_ENTRY_H_

#include <wx/wx.h>

class IPAddressEntry
{
public:
	bool isValid;

	static int deviceNameXMLId;
	static int interfaceNameXMLId;
	static int macAddressXMLId;
	static int ipAddressXMLId;
	static int maskXMLId;
	static int subnetXMLId;

	wxString deviceName;
	wxString interfaceName;
	wxString macAddress;
	unsigned int ipAddress;
	unsigned int mask;
	unsigned int subnet;
};

#endif
