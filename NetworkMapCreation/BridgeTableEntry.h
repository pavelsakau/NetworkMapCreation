#ifndef BRIDGE_TABLE_ENTRY_H_
#define BRIDGE_TABLE_ENTRY_H_

#include <wx/wx.h>

class BridgeTableEntry
{
public:
	static int macAddressXMLId;
	static int switchNameXMLId;
	static int switchIPXMLId;
	static int interfaceNameXMLId;
	static int vlanXMLId;
	static int descriptionXMLId;

	wxString macAddress;
	wxString switchName;
	wxString switchIP;
	wxString interfaceName;
	wxString vlan;
	wxString description;
};

#endif
