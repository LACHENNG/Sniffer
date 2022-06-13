#pragma once
/*
* Copyright (c) ChenLang. NWPU
* Description: 解析WinPcak捕获到的frame, 支持
1. 时间解析 
2. 源、目标 MAC 地址解析
3. 源、目标 IP 地址解析
4. IP 头解析
5. UDP、TCP解析
6. Ethernet头解析
7. 协议名称解析
*/

/*
IP 协议簇： 
1. IP : TCP/IP 心脏
2. TCP: 如果IP数据包中有已经封装好的TCP数据包，那么IP将它们向上传到TCP
3. UDP, 与TCP同层
4. ICMP: 与IP位于同一层，传送IP的控制信息
*/

//  WinPcap 
#include "stdafx.h"
#include "netprotocol.h"
#include "pcap.h"
#include "winsock2.h"
#include "vector"
using namespace std;

/* 接受捕获到的 char* data 网络帧数据，进行解析， 可以获得统计数据，和各层协议头的指针，分别放入
	pktCount 和 header_Pack中
*/
class DataParser {
	typedef struct pcap_pkthdr PKTHDR;

public:
	DataParser();
	DataParser(const u_char* pkt_data, PKTHDR * pkt_header);

	int parse(); 
	void setPktHeader(const PKTHDR* new_pkthdr);
	void setPktdata(const u_char* new_pkt_data);
	void set(const PKTHDR* new_pkthdr, const u_char* new_pkt_data);
	// 获取指定idx的数据包
	const pair<pktCount, headerPack>& getAt(int idx); 
	pktCount getStatistics();
	headerPack getParsedHeaderPack();

private:
	// 输入
	const u_char * m_pkt_data;
	const PKTHDR * m_pacp_header;
	// 解析结果
	pktCount m_pkt_counter;
	headerPack m_hdr_pack; 
	// 每次解析得到的数据都保存到数组
	vector<pair<pktCount, headerPack>> m_idx2data;

	// parse functions
	int parse_ip(const u_char* pkt);
	int parse_ipv6(const u_char* pkt);
	int parse_arp(const u_char* pkt);

	int parse_udp(const u_char* pkt);
	int parse_tcp(const u_char* pkt);
	int parse_icmp(const u_char* pkt);
	int parse_icmpv6(const u_char* pkt);

	int parse_http(const u_char* pkt);
};

