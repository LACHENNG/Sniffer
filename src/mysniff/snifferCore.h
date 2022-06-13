#pragma once
/*
* Copyright (c) lang@NWPU
* Description:
* Core head of sniffer backend with various functionality support
1. ��������ж���������û���ѡ��������
2. ����Э�����͡�Դ��ַ��Ŀ�ĵ�ַ���ˡ���������֡����������ݶΣ���
3. ʵ��IP ��Ƭ�����飻
4. ֧�ַ�������ı������ȡ��
5. ������̫����֡��IPv4��TCP��UDP��HTPP��ARP��ICMP Э�����ݣ�
*/
//  WinPcap Support 
#include "pcap.h"
#include "remote-ext.h" // ����wpcap��
#include "string"
#include "unordered_map"
#include "netprotocol.h"
#include "frame_parser.h"
// ǰ�����������snifferCore.h �� mysniffDlg.h ���ֵ��໥������ѭ����������
class CmysniffDlg; // ע�����ͬʱ��Ҫinclude "mysniffDlg.h"�������ǻ�����໥������ѭ�����õ�����

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

