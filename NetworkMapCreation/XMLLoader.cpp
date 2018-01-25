#include "XMLLoader.h"
#include "IPHelper.h"
#include <fstream>

tinyxml2::XMLElement* XmlLoader::findWorksheet(tinyxml2::XMLDocument* doc, const wxString& sheetName) {
	tinyxml2::XMLElement* node = doc->FirstChildElement("Workbook")->FirstChildElement("Worksheet");
	while (node != nullptr) {
		const char* name = node->Attribute("ss:Name");
		if (name != nullptr && sheetName.compare(name) == 0) {
			return node;
		}
		node = node->NextSiblingElement("Worksheet");
	}
	return nullptr;
}

void XmlLoader::loadArpCache(tinyxml2::XMLDocument* doc) {
	tinyxml2::XMLElement* arpSheet = findWorksheet(doc, "ARP Cache");
	if (arpSheet != nullptr) {
		tinyxml2::XMLElement* row = arpSheet->FirstChildElement("Table")->FirstChildElement("Row");
		bool isHeader = true;
		while (row != nullptr) {
			tinyxml2::XMLElement* cell = row->FirstChildElement("Cell");
			ARPCacheEntry* cacheEntry = new ARPCacheEntry;
			int cellid = 0;
			while (cell != nullptr) {
				tinyxml2::XMLElement* data = cell->FirstChildElement("Data");
				if (data != nullptr) {
					wxString dataStr = data->FirstChild()->ToText()->Value();
					if (isHeader) {
						if (dataStr.compare("MAC Address") == 0) {
							ARPCacheEntry::macAddressXMLId = cellid;
						} else if (dataStr.compare("IP Address") == 0) {
							ARPCacheEntry::ipAddressXMLId = cellid;
						} else if (dataStr.compare("Notes") == 0) {
							ARPCacheEntry::notesXMLId = cellid;
						}
					} else {
						if (ARPCacheEntry::macAddressXMLId == cellid) {
							cacheEntry->macAddress = dataStr;
						} else if (ARPCacheEntry::ipAddressXMLId == cellid) {
							cacheEntry->ipAddress = dataStr;
						} else if (ARPCacheEntry::notesXMLId == cellid) {
							cacheEntry->notes = dataStr;
						}
					}
				}
				cell = cell->NextSiblingElement("Cell");
				cellid++;
			}
			if (!isHeader) {
				arpCache.push_back(cacheEntry);
			} else {
				delete cacheEntry;
			}
			row = row->NextSiblingElement("Row");
			isHeader = false;
		}
	}
}

void XmlLoader::loadBridges(tinyxml2::XMLDocument* doc) {
	tinyxml2::XMLElement* arpSheet = findWorksheet(doc, "Bridge Table");
	if (arpSheet != nullptr) {
		tinyxml2::XMLElement* row = arpSheet->FirstChildElement("Table")->FirstChildElement("Row");
		bool isHeader = true;
		while (row != nullptr) {
			tinyxml2::XMLElement* cell = row->FirstChildElement("Cell");
			BridgeTableEntry* bridgeEntry = new BridgeTableEntry;
			int cellid = 0;
			while (cell != nullptr) {
				tinyxml2::XMLElement* data = cell->FirstChildElement("Data");
				if (data != nullptr) {
					wxString dataStr = data->FirstChild()->ToText()->Value();
					if (isHeader) {
						if (dataStr.compare("MAC Address") == 0) {
							BridgeTableEntry::macAddressXMLId = cellid;
						} else if (dataStr.compare("Switch Name") == 0) {
							BridgeTableEntry::switchNameXMLId = cellid;
						} else if (dataStr.compare("Switch IP Address") == 0) {
							BridgeTableEntry::switchIPXMLId = cellid;
						} else if (dataStr.compare("Interface") == 0) {
							BridgeTableEntry::interfaceNameXMLId = cellid;
						} else if (dataStr.compare("VLAN") == 0) {
							BridgeTableEntry::vlanXMLId = cellid;
						} else if (dataStr.compare("Description") == 0) {
							BridgeTableEntry::descriptionXMLId = cellid;
						}
					} else {
						if (BridgeTableEntry::macAddressXMLId == cellid) {
							bridgeEntry->macAddress = dataStr;
						} else if (BridgeTableEntry::switchNameXMLId == cellid) {
							bridgeEntry->switchName = dataStr;
						} else if (BridgeTableEntry::switchIPXMLId == cellid) {
							bridgeEntry->switchIP = dataStr;
						} else if (BridgeTableEntry::interfaceNameXMLId == cellid) {
							bridgeEntry->interfaceName = dataStr;
						} else if (BridgeTableEntry::vlanXMLId == cellid) {
							bridgeEntry->vlan = dataStr;
						} else if (BridgeTableEntry::descriptionXMLId == cellid) {
							bridgeEntry->description = dataStr;
						}
					}
				}
				cell = cell->NextSiblingElement("Cell");
				cellid++;
			}
			if (!isHeader) {
				bridges.push_back(bridgeEntry);
			} else {
				delete bridgeEntry;
			}
			row = row->NextSiblingElement("Row");
			isHeader = false;
		}
	}
}

bool XmlLoader::filterIP(IPAddressEntry* ip) {
	return ip->ipAddress == 2130706432 || ip->ipAddress == 2147483648 || ip->subnet == 2130706432 || ip->subnet == 2147483648 || ip->ipAddress == 0 || ip->subnet == 0;
}

void XmlLoader::loadIPAddresses(tinyxml2::XMLDocument* doc) {
	tinyxml2::XMLElement* arpSheet = findWorksheet(doc, "IP Addresses");
	if (arpSheet != nullptr) {
		tinyxml2::XMLElement* row = arpSheet->FirstChildElement("Table")->FirstChildElement("Row");
		bool isHeader = true;
		while (row != nullptr) {
			tinyxml2::XMLElement* cell = row->FirstChildElement("Cell");
			IPAddressEntry* ipAddressEntry = new IPAddressEntry;
			ipAddressEntry->isValid = true;
			int cellid = 0;
			while (cell != nullptr) {
				tinyxml2::XMLElement* data = cell->FirstChildElement("Data");
				if (data != nullptr) {
					wxString dataStr = L"";
					if (!(data->FirstChild() == nullptr || data->FirstChild()->ToText() == nullptr)) {
						dataStr = data->FirstChild()->ToText()->Value();
					}
					if (isHeader) {
						if (dataStr.compare("Device Name") == 0) {
							IPAddressEntry::deviceNameXMLId = cellid;
						} else if (dataStr.compare("Interface") == 0) {
							IPAddressEntry::interfaceNameXMLId = cellid;
						} else if (dataStr.compare("MAC Address") == 0) {
							IPAddressEntry::macAddressXMLId = cellid;
						} else if (dataStr.compare("IP Address") == 0) {
							IPAddressEntry::ipAddressXMLId = cellid;
						} else if (dataStr.compare("Mask") == 0) {
							IPAddressEntry::maskXMLId = cellid;
						} else if (dataStr.compare("Subnet") == 0) {
							IPAddressEntry::subnetXMLId = cellid;
						}
					} else {
						if (IPAddressEntry::deviceNameXMLId == cellid) {
							ipAddressEntry->deviceName = dataStr;
						} else if (IPAddressEntry::interfaceNameXMLId == cellid) {
							ipAddressEntry->interfaceName = dataStr;
						} else if (IPAddressEntry::macAddressXMLId == cellid) {
							ipAddressEntry->macAddress = dataStr;
						} else if (IPAddressEntry::ipAddressXMLId == cellid) {
							ipAddressEntry->ipAddress = IPHelper::stringToIP(dataStr);
						} else if (IPAddressEntry::maskXMLId == cellid) {
							ipAddressEntry->mask = IPHelper::stringToIP(dataStr);
						} else if (IPAddressEntry::subnetXMLId == cellid) {
							ipAddressEntry->subnet = IPHelper::stringToIP(dataStr);
						}
					}
				}
				cell = cell->NextSiblingElement("Cell");
				cellid++;
			}
			ipAddressEntry->isValid  = !(ipAddressEntry->ipAddress == 0 || ipAddressEntry->mask == 0 || ipAddressEntry->subnet == 0);
			if ((!isHeader) && (!filterIP(ipAddressEntry))) {
				ipAddresses.push_back(ipAddressEntry);
			} else {
				delete ipAddressEntry;
			}
			row = row->NextSiblingElement("Row");
			isHeader = false;
		}
	}
}

vector<IPAddressEntry* > XmlLoader::getIpAddresses() {
	return ipAddresses;
}

vector<IPRouteEntry* > XmlLoader::getRoutes() {
	return ipRoutes;
}

void XmlLoader::loadIPRoutes(tinyxml2::XMLDocument* doc) {
	tinyxml2::XMLElement* arpSheet = findWorksheet(doc, "IP Routes");
	if (arpSheet != nullptr) {
		tinyxml2::XMLElement* row = arpSheet->FirstChildElement("Table")->FirstChildElement("Row");
		bool isHeader = true;
		while (row != nullptr) {
			tinyxml2::XMLElement* cell = row->FirstChildElement("Cell");
			IPRouteEntry* ipRouteEntry = new IPRouteEntry;
			int cellid = 0;
			while (cell != nullptr) {
				tinyxml2::XMLElement* data = cell->FirstChildElement("Data");
				if (data != nullptr) {
					wxString dataStr = L"";
					if (!(data->FirstChild() == nullptr || data->FirstChild()->ToText() == nullptr)) {
						dataStr = data->FirstChild()->ToText()->Value();
					}
					if (isHeader) {
						if (dataStr.compare("Router Name") == 0) {
							IPRouteEntry::routerNameXMLId = cellid;
						} else if (dataStr.compare("Router IP") == 0) {
							IPRouteEntry::routerIPXMLId = cellid;
						} else if (dataStr.compare("Interface") == 0) {
							IPRouteEntry::interfaceNameXMLId = cellid;
						} else if (dataStr.compare("Route") == 0) {
							IPRouteEntry::routeXMLId = cellid;
						} else if (dataStr.compare("Mask") == 0) {
							IPRouteEntry::maskXMLId = cellid;
						} else if (dataStr.compare("Next Hop") == 0) {
							IPRouteEntry::nextHopXMLId = cellid;
						}
					} else {
						if (IPRouteEntry::routerNameXMLId == cellid) {
							ipRouteEntry->routerName = dataStr;
						} else if (IPRouteEntry::routerIPXMLId == cellid) {
							ipRouteEntry->routerIP = IPHelper::stringToIP(dataStr);
						} else if (IPRouteEntry::interfaceNameXMLId == cellid) {
							ipRouteEntry->interfaceName = dataStr;
						} else if (IPRouteEntry::routeXMLId == cellid) {
							ipRouteEntry->route = IPHelper::stringToIP(dataStr);
						} else if (IPRouteEntry::maskXMLId == cellid) {
							ipRouteEntry->mask = IPHelper::stringToIP(dataStr);
						} else if (IPRouteEntry::nextHopXMLId == cellid) {
							ipRouteEntry->nextHop = IPHelper::stringToIP(dataStr);
						}
					}
				}
				cell = cell->NextSiblingElement("Cell");
				cellid++;
			}
			if (!isHeader) {
				ipRoutes.push_back(ipRouteEntry);
			} else {
				delete ipRouteEntry;
			}
			row = row->NextSiblingElement("Row");
			isHeader = false;
		}
	}
}

XmlLoader::~XmlLoader() {
	int totalIPAddresses = ipAddresses.size();
	for (int i = 0; i < totalIPAddresses; i++) {
		delete ipAddresses[i];
	}
	ipAddresses.clear();
	int totalIPRoutes = ipRoutes.size();
	for (int i = 0; i < totalIPRoutes; i++) {
		delete ipRoutes[i];
	}
	ipRoutes.clear();
}

XmlLoader::XmlLoader(const wxString & filename)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);
	loadIPAddresses(&doc);
	loadIPRoutes(&doc);
}
