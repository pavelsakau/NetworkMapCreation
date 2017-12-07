#include "XMLLoader.h"
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

void XmlLoader::loadIPAddresses(tinyxml2::XMLDocument* doc) {
	tinyxml2::XMLElement* arpSheet = findWorksheet(doc, "IP Addresses");
	if (arpSheet != nullptr) {
		tinyxml2::XMLElement* row = arpSheet->FirstChildElement("Table")->FirstChildElement("Row");
		bool isHeader = true;
		while (row != nullptr) {
			tinyxml2::XMLElement* cell = row->FirstChildElement("Cell");
			IPAddressEntry* ipAddressEntry = new IPAddressEntry;
			int cellid = 0;
			while (cell != nullptr) {
				tinyxml2::XMLElement* data = cell->FirstChildElement("Data");
				if (data != nullptr) {
					wxString dataStr = data->FirstChild()->ToText()->Value();
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
							ipAddressEntry->ipAddress = dataStr;
						} else if (IPAddressEntry::maskXMLId == cellid) {
							ipAddressEntry->mask = dataStr;
						} else if (IPAddressEntry::subnetXMLId == cellid) {
							ipAddressEntry->subnet = dataStr;
						}
					}
				}
				cell = cell->NextSiblingElement("Cell");
				cellid++;
			}
			if (!isHeader) {
				ipAddresses.push_back(ipAddressEntry);
			} else {
				delete ipAddressEntry;
			}
			row = row->NextSiblingElement("Row");
			isHeader = false;
		}
	}
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
					wxString dataStr = data->FirstChild()->ToText()->Value();
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
							ipRouteEntry->routerIP = dataStr;
						} else if (IPRouteEntry::interfaceNameXMLId == cellid) {
							ipRouteEntry->interfaceName = dataStr;
						} else if (IPRouteEntry::routeXMLId == cellid) {
							ipRouteEntry->route = dataStr;
						} else if (IPRouteEntry::maskXMLId == cellid) {
							ipRouteEntry->mask = dataStr;
						} else if (IPRouteEntry::nextHopXMLId == cellid) {
							ipRouteEntry->nextHop = dataStr;
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

XmlLoader::XmlLoader(const wxString & filename)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);
	//loadArpCache(&doc);
	//loadBridges(&doc);
	loadIPAddresses(&doc);
	//loadIPRoutes(&doc);
	//ofstream f("output.txt");
	//f << "ARP Cache entries:" << endl;
	//for (int i = 0; i < arpCache.size(); i++) {
	//	f << arpCache[i]->ipAddress << " " << arpCache[i]->macAddress << " " << arpCache[i]->notes << endl;
	//}
	//f << "Bridge entries:" << endl;
	//for (int i = 0; i < bridges.size(); i++) {
	//	f << bridges[i]->description << " " << bridges[i]->interfaceName << " " << bridges[i]->macAddress << " " << bridges[i]->switchIP << " " << bridges[i]->switchName << " " << bridges[i]->vlan << endl;
	//}
	//f << "IP Address entries:" << endl;
	//for (int i = 0; i < ipAddresses.size(); i++) {
	//	f << ipAddresses[i]->deviceName << " " << ipAddresses[i]->interfaceName << " " << ipAddresses[i]->ipAddress << " " << ipAddresses[i]->macAddress << " " << ipAddresses[i]->mask << " " << ipAddresses[i]->subnet << endl;
	//}
	//f << "IP Route entries:" << endl;
	//for (int i = 0; i < ipRoutes.size(); i++) {
	//	f << ipRoutes[i]->interfaceName << " " << ipRoutes[i]->mask << " " << ipRoutes[i]->nextHop << " " << ipRoutes[i]->route << " " << ipRoutes[i]->routerIP << " " << ipRoutes[i]->routerName << endl;
	//}
	//f.close();
	//wxMessageBox(wxString::Format(wxT("ARP: %i, Bridges: %i, IP Addresses: %i, IPRoutes: %i"), arpCache.size(), bridges.size(), ipAddresses.size(), ipRoutes.size()));
	wxMessageBox(wxString::Format(wxT("Loaded %i IPs"), ipAddresses.size()));
}
