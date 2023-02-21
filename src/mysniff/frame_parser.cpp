#include "stdafx.h"
#include "frame_parser.h"
#include "netprotocol.h"
#include "assert.h"
#include "utils.h"


DataParser::DataParser() : m_pkt_data(NULL), m_pacp_header(NULL)
{
	memset(&this->m_pkt_counter, 0, sizeof(decltype(this->m_pkt_counter)));
	memset(&this->m_hdr_pack, 0, sizeof(decltype(this->m_hdr_pack)));

}

DataParser::DataParser(const u_char * pkt_data, PKTHDR * pkt_header) : m_pkt_data(pkt_data), m_pacp_header(pkt_header)
{


	memset(&this->m_pkt_counter, 0, sizeof(decltype(this->m_pkt_counter)));
	memset(&this->m_hdr_pack, 0, sizeof(decltype(this->m_hdr_pack)));
}

// 解析ETH头，剩下的交给下一层处理
int DataParser::parse()
{ 
	//保存Winpcap给的头部信息(拷贝一份）
	m_hdr_pack.pcaph = shallowCopy(const_cast<PKTHDR*>(m_pacp_header));

	// 当前头部信息
	auto ethdr = (eth_header*)m_pkt_data;
	auto protocol = ntohs(ethdr->proto); 
	m_pkt_counter.n_sum += 1;
	// 保存相关数据
	m_hdr_pack.ethh = shallowCopy(ethdr);
	// 继续解析下一层
	switch (protocol) {
		case ETH_PROTOCOL_IP:
			parse_ip((u_char*)ethdr + ETH_HEAD_LEN);
			break;
		case ETH_PROTOCOL_IPV6:
			parse_ipv6((u_char*)ethdr + ETH_HEAD_LEN);
			break;
		case ETH_PROTOCOL_ARP:
			parse_arp((u_char*)ethdr + ETH_HEAD_LEN);
			break;
		default:
			// currently  un-supported 
			break;
	}
	
	m_idx2data.push_back(pair<pktCount, headerPack>(m_pkt_counter, m_hdr_pack));
	return 0;
}

void DataParser::setPktHeader(const PKTHDR * new_pkthdr)
{
	this->m_pacp_header = new_pkthdr;
}

void DataParser::setPktdata(const u_char* new_pkt_data)
{
	this->m_pkt_data = new_pkt_data;
}

void DataParser::set(const PKTHDR * new_pkthdr, const u_char * new_pkt_data)
{
	m_pacp_header = new_pkthdr;
	m_pkt_data = new_pkt_data;
}

const pair<pktCount, headerPack>& DataParser::getAt(int idx)
{
	assert(idx >= 0 && idx < m_idx2data.size());
	return m_idx2data[idx];
}

int DataParser::parse_ip(const u_char* pkt) {
	// 当前头部信息
	auto iphdr = (ip_header*)pkt; 
	int ip_head_len = (iphdr->ver_ihl & 0xf) * 4;
	auto protocol = iphdr->proto;
	// 保存当前解析结果(拷贝一份到程序自己的堆空间）
	m_hdr_pack.iph = shallowCopy(iphdr);
	m_pkt_counter.n_ip++;
	// 继续解析下一层
	switch (protocol) {
		case IP_PROTOCOL_TCP:
			return parse_tcp((u_char*)pkt + ip_head_len);
		case IP_PROTOCOL_UDP:
			return parse_udp((u_char*)pkt + ip_head_len);
		case IP_PROTOCOL_ICMP:
			return parse_icmp((u_char*)pkt + ip_head_len);
		default:
			// currently  un-supported 
			return -1;
	}
	return 0;
}

int DataParser::parse_ipv6(const u_char* pkt) {
	// 当前头部信息
	auto ipv6hdr = (ipv6_header*)pkt;
	auto protocol = ipv6hdr->nh;
	m_pkt_counter.n_ip6++;
	// 保存当前解析结果(拷贝一份到程序自己的堆空间）
	m_hdr_pack.iph6 = shallowCopy(ipv6hdr);
	// 继续解析下一层
	switch (protocol) {
		case IPv6_PROTOCOL_TCP:
			return parse_tcp((u_char*)ipv6hdr + IPV6_HEAD_LEN);
		case IPv6_PROTOCOL_UDP:
			return parse_udp((u_char*)ipv6hdr + IPV6_HEAD_LEN);
		case IPv6_PROTOCOL_ICMP:
			return parse_icmpv6((u_char*)ipv6hdr + IPV6_HEAD_LEN);
		default:
			// currently  un-supported 
		return -1;
	}
	return 0;
}

int DataParser::parse_arp(const u_char* pkt) {
	// 当前头部信息
	auto arphdr = (arp_header*)pkt;
	// 保存当前解析结果
	m_hdr_pack.arph = shallowCopy(arphdr);
	m_pkt_counter.n_arp++;
	//strcpy(m_hdr_pack.pktType, "ARP");
	strcpy(m_hdr_pack.pktType, PKTTYPE_ARP);
	
	return 1;
}
//
int DataParser::parse_udp(const u_char* pkt) {
	
	// 当前头部信息
	udp_header* udphdr = (udp_header*)pkt;
	// 保存当前解析结果
	m_hdr_pack.udph = shallowCopy(udphdr);
	m_pkt_counter.n_udp++;
	//strcpy(m_hdr_pack.pktType, "UDP");
	strcpy(m_hdr_pack.pktType, PKTTYPE_UDP);
	
	return 1;

}
int DataParser::parse_tcp(const u_char* pkt) {
	// 当前头部信息
	tcp_header* tcphdr = (tcp_header*)pkt;
	// 保存当前解析结果
	m_hdr_pack.tcph = shallowCopy(tcphdr);
	//m_hdr_pack.tcph = tcphdr;
	m_pkt_counter.n_tcp++;
	//strcpy(m_hdr_pack.pktType, "TCP");
	strcpy(m_hdr_pack.pktType, PKTTYPE_TCP);

	// TODO: 解析下一层
	if (ntohs(tcphdr->dport) == 80 || ntohs(tcphdr->sport) == 80) {
		m_pkt_counter.n_http++;
		//strcpy(m_hdr_pack.pktType, "HTTP");
		strcpy(m_hdr_pack.pktType, PKTTYPE_HTTP);
	}
	
	return 1;
}
int DataParser::parse_icmp(const u_char* pkt) {
	// 当前头部信息
	icmp_header* icmphdr = (icmp_header*)pkt;
	// 保存当前解析结果
	m_hdr_pack.icmph = shallowCopy(icmphdr);
	

	m_pkt_counter.n_icmp++;
	//strcpy(m_hdr_pack.pktType, "ICMP");
	strcpy(m_hdr_pack.pktType, PKTTYPE_ICMP);
	
	return 1;
}
int DataParser::parse_icmpv6(const u_char* pkt) {
	// 当前头部信息
	icmp_header6* icmphdr6 = (icmp_header6*)pkt;

	m_hdr_pack.icmph6 = shallowCopy(icmphdr6);

	m_pkt_counter.n_icmp6++;
	//strcpy(m_hdr_pack.pktType, "ICMPv6");
	strcpy(m_hdr_pack.pktType, PKTTYPE_ICMPv6);
	
	return 1;
}
// 
int DataParser::parse_http(const u_char* pkt) {
	//strcpy(m_hdr_pack.pktType, "HTTP");
	strcpy(m_hdr_pack.pktType, PKTTYPE_HTTP);
	// TODO ： 保存当前解析结果
	m_pkt_counter.n_http++;
	return 1;
}
pktCount DataParser::getStatistics() {
	return m_pkt_counter;
}
headerPack DataParser::getParsedHeaderPack() {
	return m_hdr_pack;
}