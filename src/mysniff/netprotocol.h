/*
* Copyright (c) Lang@NWPU.
* Description: ����Э��ͷ�Ľṹ�嶨��
*/
#pragma once
#include "WinSock2.h"
#include "unordered_map"
#include "string"
#include "pcap.h"
#include "memory"
using namespace std;

// WinPcap����������֮ǰ��ӵ�ͷ
typedef struct pcap_pkthdr pktHeader;

// intel x86/64 is little_endian
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN (1234)  
#endif

/* Ethernet protocol ID's */
#define	ETH_PROTOCOL_PUP		0x0200          /* Xerox PUP */
#define ETH_PROTOCOL_SPRITE  	0x0500		/* Sprite */
#define	ETH_PROTOCOL_IP		    0x0800		/* IP ��*/ 
#define	ETH_PROTOCOL_ARP		0x0806		/* Address resolution�� */
#define	ETH_PROTOCOL_REVARP  	0x8035		/* Reverse ARP */
#define ETH_PROTOCOL_AT		    0x809B		/* AppleTalk protocol */
#define ETH_PROTOCOL_AARP		0x80F3		/* AppleTalk ARP */
#define	ETH_PROTOCOL_VLAN		0x8100		/* IEEE 802.1Q VLAN tagging */
#define ETH_PROTOCOL_IPX		0x8137		/* IPX */
#define	ETH_PROTOCOL_IPV6		0x86dd		/* IP protocol version 6 */
#define ETH_PROTOCOL_LOOPBACK	0x9000		/* used to test interfaces */


/* 14 bytes Ethernet header */
const int ETH_MLEN = 6;
#pragma pack(push,1)
typedef struct eth_header {
	unsigned char d_mac[ETH_MLEN]; /* destination eth addr(mac address) */  
	unsigned char s_mac[ETH_MLEN]; /* source ether addr(mac address)*/
	unsigned short proto;          /* packet type ID field*/
}eth_header;
#pragma pack(pop)
const int ETH_HEAD_LEN = sizeof(eth_header);
typedef std::unordered_map<unsigned short, CString> MAP_ETH2STR;
extern MAP_ETH2STR ETH_PROT2STR;// ����ڶ��Դ�ļ��������ͬһ�����ֵ�ȫ�ֱ����Ķ��壬�������ض��壻���ԣ����ڶ���ļ�����һ��ȫ�ֱ�����ֻ��Ҫ��һ��ͷ�ļ���������������� extern int i��Ȼ��������һ��Դ�ļ��У�ֻ����һ����ͨ������.hͬ����Դ�ļ����������������int i = 1��������


/* 4 bytes IP address */
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header {
	u_char ver_ihl;          // Version(4 bits) + Internet header length (4 bits)(IHL: actually means num of lines, 4 bytes(32 bits) per line)
	u_char tos;              // Type of service 
	u_short tlen;            // Total length 
	u_short identification; //  Identification (��������ֶ�һ�����ڷ�Ƭ/���飩     
	u_short flags_fo;       //  Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
	ip_address  saddr;      // Source address
	ip_address  daddr;      // Destination address
	u_int   op_pad;         // Option + Padding
}ip_header;


/* IP protocol ID's */
#define IP_PROTOCOL_ICMP   1
#define IP_PROTOCOL_IGMP   2
#define IP_PROTOCOL_TCP    6
#define IP_PROTOCOL_UDP   17
#define IP_PROTOCOL_IGRP  88
#define IP_PROTOCOL_OSPF  89

#define IPv6_PROTOCOL_ICMP   0x3a
#define IPv6_PROTOCOL_UDP    0x11
#define IPv6_PROTOCOL_TCP    0x06


typedef std::unordered_map<unsigned short, CString> MAP_IP2STR;
extern MAP_IP2STR IP_PROT2STR;


/*UDP header*/
typedef struct udp_header {
	u_short sport;          // Source port
	u_short dport;          // Destination port 
	u_short len;            // Datagram length
	u_short crc;            // Checksum 
}udp_header;


/*TCP header*/
typedef struct tcp_header {
	u_short sport;           // Source port (16 bits)
	u_short dport;           // Destination port
	u_int seq_num;           // Sequence number      
	u_int ack_num;         // acknowledgment sequence number
	u_short res:4,
		doff : 4,
		fin : 1,
		syn : 1,
		rst : 1,
		psh : 1,
		ack : 1,
		urg : 1,
		ece : 1,
		cwr : 1;

#if LITTLE_ENDIAN   
	unsigned char reserved_1 : 4; //����6λ�е�4λ�ײ�����   
	unsigned char thl : 4;        //tcpͷ������   
	unsigned char flag : 6;       //6λ��־   
	unsigned char reseverd_2 : 2; //����6λ�е�2λ   
#else   
	unsigned char thl : 4;        //tcpͷ������   
	unsigned char reserved_1 : 4; //����6λ�е�4λ�ײ�����   
	unsigned char reseverd_2 : 2; //����6λ�е�2λ   
	unsigned char flag : 6;       //6λ��־    
#endif 
	u_short window;
	u_short checksum;
	u_short urg_ptr;
	u_int option;
}tcp_header;

// ipv6

//����IPv6
const int IPV6_HEAD_LEN = 40;
typedef struct iphdr6
{
	u_int version : 4,				//�汾
		  flowtype : 8,			   //������
		  flowid : 20;				//����ǩ		
	u_short plen;					//��Ч�غɳ���
	u_char nh;						//��һ��ͷ��
	u_char hlim;					//������
	u_short saddr[8];			//Դ��ַ
	u_short daddr[8];			//Ŀ�ĵ�ַ
}ipv6_header;


// ARP ͷ
typedef struct arp_header {
	u_short hardware_type;			//Ӳ������
	u_short proto_type;				//Э������
	u_char hlen;				    //Ӳ����ַ����
	u_char plen;				    //Э���ַ����
	u_short op_code;			    //�����룬1Ϊ���� 2Ϊ�ظ�
	u_char src_mac[6];			    //���ͷ�MAC
	//u_char src_ip[4];				//���ͷ�IP
	ip_address  saddr;      // Source address
	u_char dest_mac[6];			    //���շ�MAC
	//u_char dest_ip[4];				//���շ�IP
	ip_address  daddr;      // Destination address
}arp_header;

// ICMP��v4)
typedef struct icmp_header {
	u_char type;            // 8 bits ����
	u_char code;            // ����
	u_char seq;             // ���к�
	u_char chksum;          // 8ΪλУ���
}icmp_header;

//����ICMPv6
typedef struct icmphdr6
{
	u_char type;			//8λ ����
	u_char code;			//8λ ����
	u_char seq;			    //���к� 8λ
	u_char chksum;		    //8λУ���
	u_char op_type;	        //ѡ�����
	u_char op_len;		    //ѡ�����
	u_char op_ethaddr[6];	//ѡ���·���ַ
}icmp_header6;


//�Ը��ְ����м���
typedef struct pktcount
{
	pktcount(): n_ip(0),n_ip6(0),n_arp(0),n_tcp(0),n_udp(0), n_icmp(0), n_icmp6(0), n_http(0), n_other(0), n_sum(0){}
	u_int n_ip;
	u_int n_ip6;
	u_int n_arp;
	u_int n_tcp;
	u_int n_udp;
	u_int n_icmp;
	u_int n_icmp6;
	u_int n_http;
	u_int n_other;
	u_int n_sum;
}pktCount;


// ע����ϵڶ���const(�����ñ����ǳ���������ͨ������
/*
This works because in C++ a name at namespace scope (including the global namespace) 
that is explicitly declared const and not explicitly declared extern has internal
linkage, so these variables would not cause duplicate symbols when you link together
translation units. Alternatively you could explicitly declare the constants as static.
*/
const char* const PKTTYPE_ARP = "ARP";
const char* const PKTTYPE_UDP = "UDP";
const char* const PKTTYPE_TCP = "TCP";
const char* const PKTTYPE_HTTP = "HTTP";
const char* const PKTTYPE_ICMP = "ICMP";
const char* const PKTTYPE_ICMPv6 = "ICMPv6";
const char* const PKTTYPE_IPv6 = "IPv6";

// ��������õ���heads pointers
// ʹ��shared_ptr �����Զ��ڴ����
typedef struct datapkt
{
	char  pktType[8];			     // ֡���������ͣ����ģ���У�������Ǳ������ܽ����������ϲ��Э�����ƣ�
									 //int time[6];					 //ʱ�� ? 
									 //int len;		
									 //���� ?
	shared_ptr<pktHeader> pcaph;          // winPcap����ͷ
	shared_ptr<eth_header> ethh;			 //��·���ͷ
	shared_ptr<arp_header> arph;			 //ARP��ͷ
	shared_ptr<ip_header> iph;				 //IP��ͷ
	shared_ptr<ipv6_header> iph6;			 //IPV6
	shared_ptr<icmp_header> icmph;		     //ICMP��ͷ
	shared_ptr<icmp_header6> icmph6;	     //ICMPv6��ͷ
	shared_ptr<udp_header> udph;		 	 //UDP��ͷ
	shared_ptr<tcp_header> tcph;		     //TCP��ͷ
	shared_ptr<void> apph;					 //Ӧ�ò��ͷ
}headerPack, *LPHeaderPack;


//typedef struct datapkt
//{
//	char  pktType[8];			     // ֡���������ͣ����ģ���У�������Ǳ������ܽ����������ϲ��Э�����ƣ�
//	//int time[6];					 //ʱ�� ? 
//	//int len;					     //���� ?
//	pktHeader* pcaph;          // winPcap����ͷ
//	eth_header* ethh;			 //��·���ͷ
//	arp_header* arph;			 //ARP��ͷ
//	ip_header* iph;				 //IP��ͷ
//	ipv6_header* iph6;			 //IPV6
//	icmp_header* icmph;		     //ICMP��ͷ
//	icmp_header6* icmph6;	     //ICMPv6��ͷ
//	udp_header* udph;		 	 //UDP��ͷ
//	tcp_header* tcph;		     //TCP��ͷ
//	void *apph;					 //Ӧ�ò��ͷ
//}headerPack;
//

//
