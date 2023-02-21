#include "stdafx.h"
#include "unordered_map"
#include "netprotocol.h"



MAP_ETH2STR ETH_PROT2STR = {{ ETH_PROTOCOL_PUP, CString("Xerox PUP")},
							{ ETH_PROTOCOL_SPRITE , CString("Sprite")},
							{ ETH_PROTOCOL_IP, CString("IP")},
							{ ETH_PROTOCOL_AT , CString("ApleTalk")},
							{ ETH_PROTOCOL_AARP , CString("AARP")},
							{ ETH_PROTOCOL_VLAN , CString("802.1QVLAN")},
							{ ETH_PROTOCOL_IPX , CString("IPX")},
							{ ETH_PROTOCOL_LOOPBACK, CString("LoopBack")},
							{ ETH_PROTOCOL_ARP, CString("ARP")},
							{ ETH_PROTOCOL_IPV6, CString("IPv6")},
						   };


MAP_IP2STR IP_PROT2STR =  {	{ IP_PROTOCOL_ICMP, CString("ICMP") },
							{ IP_PROTOCOL_IGMP , CString("IGMP")},
							{ IP_PROTOCOL_TCP, CString("TCP")},
							{ IP_PROTOCOL_UDP , CString("UDP")},
							{ IP_PROTOCOL_IGRP ,CString("IGRP")},
							{ IP_PROTOCOL_OSPF , CString("OSPF")},
						};
