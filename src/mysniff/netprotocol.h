/*
* Copyright (c) Lang@NWPU.
* Description: 各种协议头的结构体定义
*/
#pragma once
#include "WinSock2.h"
#include "unordered_map"
#include "string"
#include "pcap.h"
#include "memory"
using namespace std;

// WinPcap在所有数据之前添加的头
typedef struct pcap_pkthdr pktHeader;

// intel x86/64 is little_endian
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN (1234)  
#endif

/* Ethernet protocol ID's */
#define	ETH_PROTOCOL_PUP		0x0200          /* Xerox PUP */
#define ETH_PROTOCOL_SPRITE  	0x0500		/* Sprite */
#define	ETH_PROTOCOL_IP		    0x0800		/* IP ！*/ 
#define	ETH_PROTOCOL_ARP		0x0806		/* Address resolution！ */
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
extern MAP_ETH2STR ETH_PROT2STR;// 如果在多个源文件里面包含同一个名字的全局变量的定义，会引起重定义；所以，想在多个文件共用一个全局变量，只需要在一个头文件中声明这个变量： extern int i；然后在其中一个源文件中（只能是一个，通常是与.h同名的源文件）定义这个变量，int i = 1；不能用


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
	u_short identification; //  Identification (和下面的字段一起用于分片/重组）     
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
	unsigned char reserved_1 : 4; //保留6位中的4位首部长度   
	unsigned char thl : 4;        //tcp头部长度   
	unsigned char flag : 6;       //6位标志   
	unsigned char reseverd_2 : 2; //保留6位中的2位   
#else   
	unsigned char thl : 4;        //tcp头部长度   
	unsigned char reserved_1 : 4; //保留6位中的4位首部长度   
	unsigned char reseverd_2 : 2; //保留6位中的2位   
	unsigned char flag : 6;       //6位标志    
#endif 
	u_short window;
	u_short checksum;
	u_short urg_ptr;
	u_int option;
}tcp_header;

// ipv6

//定义IPv6
const int IPV6_HEAD_LEN = 40;
typedef struct iphdr6
{
	u_int version : 4,				//版本
		  flowtype : 8,			   //流类型
		  flowid : 20;				//流标签		
	u_short plen;					//有效载荷长度
	u_char nh;						//下一个头部
	u_char hlim;					//跳限制
	u_short saddr[8];			//源地址
	u_short daddr[8];			//目的地址
}ipv6_header;


// ARP 头
typedef struct arp_header {
	u_short hardware_type;			//硬件类型
	u_short proto_type;				//协议类型
	u_char hlen;				    //硬件地址长度
	u_char plen;				    //协议地址长度
	u_short op_code;			    //操作码，1为请求 2为回复
	u_char src_mac[6];			    //发送方MAC
	//u_char src_ip[4];				//发送方IP
	ip_address  saddr;      // Source address
	u_char dest_mac[6];			    //接收方MAC
	//u_char dest_ip[4];				//接收方IP
	ip_address  daddr;      // Destination address
}arp_header;

// ICMP（v4)
typedef struct icmp_header {
	u_char type;            // 8 bits 类型
	u_char code;            // 代码
	u_char seq;             // 序列号
	u_char chksum;          // 8为位校验和
}icmp_header;

//定义ICMPv6
typedef struct icmphdr6
{
	u_char type;			//8位 类型
	u_char code;			//8位 代码
	u_char seq;			    //序列号 8位
	u_char chksum;		    //8位校验和
	u_char op_type;	        //选项：类型
	u_char op_len;		    //选项：长度
	u_char op_ethaddr[6];	//选项：链路层地址
}icmp_header6;


//对各种包进行计数
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


// 注意加上第二个const(表明该变量是常量）才能通过链接
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

// 保存解析得到的heads pointers
// 使用shared_ptr 进行自动内存管理
typedef struct datapkt
{
	char  pktType[8];			     // 帧的最终类型（五层模型中，保存的是本程序能解析到的最上层的协议名称）
									 //int time[6];					 //时间 ? 
									 //int len;		
									 //长度 ?
	shared_ptr<pktHeader> pcaph;          // winPcap给的头
	shared_ptr<eth_header> ethh;			 //链路层包头
	shared_ptr<arp_header> arph;			 //ARP包头
	shared_ptr<ip_header> iph;				 //IP包头
	shared_ptr<ipv6_header> iph6;			 //IPV6
	shared_ptr<icmp_header> icmph;		     //ICMP包头
	shared_ptr<icmp_header6> icmph6;	     //ICMPv6包头
	shared_ptr<udp_header> udph;		 	 //UDP包头
	shared_ptr<tcp_header> tcph;		     //TCP包头
	shared_ptr<void> apph;					 //应用层包头
}headerPack, *LPHeaderPack;


//typedef struct datapkt
//{
//	char  pktType[8];			     // 帧的最终类型（五层模型中，保存的是本程序能解析到的最上层的协议名称）
//	//int time[6];					 //时间 ? 
//	//int len;					     //长度 ?
//	pktHeader* pcaph;          // winPcap给的头
//	eth_header* ethh;			 //链路层包头
//	arp_header* arph;			 //ARP包头
//	ip_header* iph;				 //IP包头
//	ipv6_header* iph6;			 //IPV6
//	icmp_header* icmph;		     //ICMP包头
//	icmp_header6* icmph6;	     //ICMPv6包头
//	udp_header* udph;		 	 //UDP包头
//	tcp_header* tcph;		     //TCP包头
//	void *apph;					 //应用层包头
//}headerPack;
//

//
