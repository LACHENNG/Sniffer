#pragma once
/*
* Copyright (c) ChenLang. NWPU
* Description: ����WinPcak���񵽵�frame, ֧��
1. ʱ����� 
2. Դ��Ŀ�� MAC ��ַ����
3. Դ��Ŀ�� IP ��ַ����
4. IP ͷ����
5. UDP��TCP����
6. Ethernetͷ����
7. Э�����ƽ���
*/

/*
IP Э��أ� 
1. IP : TCP/IP ����
2. TCP: ���IP���ݰ������Ѿ���װ�õ�TCP���ݰ�����ôIP���������ϴ���TCP
3. UDP, ��TCPͬ��
4. ICMP: ��IPλ��ͬһ�㣬����IP�Ŀ�����Ϣ
*/

//  WinPcap 
#include "stdafx.h"
#include "netprotocol.h"
#include "pcap.h"
#include "winsock2.h"
#include "vector"
using namespace std;

/* ���ܲ��񵽵� char* data ����֡���ݣ����н����� ���Ի��ͳ�����ݣ��͸���Э��ͷ��ָ�룬�ֱ����
	pktCount �� header_Pack��
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
	// ��ȡָ��idx�����ݰ�
	const pair<pktCount, headerPack>& getAt(int idx); 
	pktCount getStatistics();
	headerPack getParsedHeaderPack();

private:
	// ����
	const u_char * m_pkt_data;
	const PKTHDR * m_pacp_header;
	// �������
	pktCount m_pkt_counter;
	headerPack m_hdr_pack; 
	// ÿ�ν����õ������ݶ����浽����
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

