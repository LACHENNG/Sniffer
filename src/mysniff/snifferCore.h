#pragma once
/*
* Copyright (c) lang@NWPU
* Description:
* Core head of sniffer backend with various functionality support
1. 如果主机有多块网卡，用户可选定网卡；
2. 按照协议类型、源地址和目的地址过滤、查找数据帧（分组或数据段）；
3. 实现IP 分片的重组；
4. 支持分析结果的保存与读取；
5. 解析以太数据帧、IPv4、TCP、UDP、HTPP、ARP、ICMP 协议数据；
*/
//  WinPcap Support 
#include "pcap.h"
#include "remote-ext.h" // 包含wpcap库
#include "string"
#include "unordered_map"
#include "netprotocol.h"
#include "frame_parser.h"
// 前置声明，解决snifferCore.h 和 mysniffDlg.h 出现的相互包含的循环依赖问题
class CmysniffDlg; // 注意与此同时不要include "mysniffDlg.h"，否则还是会出现相互包含，循环引用的问题

class SnifferCore {
public:
	// device init
	// SnifferCore();
	SnifferCore(CmysniffDlg* this_of_gui);

	~SnifferCore();
	int snif_initCap();
	int snif_setupFilter();

	// Functionalities
	int snif_startCap();

	// Getters And Setters
	pcap_if_t* getAvaliableDevs() const;
	pcap_if_t* getChoosedIf() const;
	pcap_if_t* adapterName2dev(const std::string adpName) const;
	pcap_t * getOpenedIfHandle() const; // If: interface
	pcap_dumper_t * getDumper() const;
	const char * getDefaltDumpFilePath() const;
	HANDLE getThreadHandle();
	void  SnifferCore::setThreadHandle(HANDLE thread_handle);

	void setnpkt(int n);
	int getnpkt() const;
	void setChoosedIf(pcap_if_t * dev);  
	void setChoosedRule(string rule);
	void setOpenIfHandle(pcap_t * opend);
	string getChoosedRule();
	//
	DataParser data_parser;
	int m_snif_CreateCapThread();  // Thread Creation

private: 
	// private functions 
	static DWORD WINAPI m_snif_CapThreadFun(LPVOID lpParameter);  // Thread Proc func, static means no this pointer passed to function implicitly 

	// updates data to gui( m_pSniffDlg )
	void tellGuiToUpdate(const pktCount* nPacket, const headerPack* hdrPack);

private:
	// adapters 
	pcap_if_t *m_alldevs; // all
	pcap_if_t *m_dev;     // currently choosed
	string m_filter_rule;   

	std::unordered_map<std::string, pcap_if_t*> m_adapterName2dev;
	
	// file dump related
	std::string filepath;
	std::string filename;

	//Statistics
	int devCount = 0;
	int m_npkt = 0;  

	//pointer to gui client
	CmysniffDlg* m_pSniffDlg;

	//struct pktcount m_npacket;

	// handles 
	pcap_t * m_opened_if_handle;  // pcap opened live
	HANDLE m_threadHandle = NULL;  // capture thread
	
	// dump 
	pcap_dumper_t* dumpfile;
	const char * default_dump_file = "./data_dump";

};

