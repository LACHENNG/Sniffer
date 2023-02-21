
/*
* Copyright (c) Lang NWPU
* Description:
* Implement Core of sniffer backend with various functionality suppor
1. ��������ж���������û���ѡ��������
2. ����Э�����͡�Դ��ַ��Ŀ�ĵ�ַ���ˡ���������֡����������ݶΣ���
3. ʵ��IP ��Ƭ�����飻
4. ֧�ַ�������ı������ȡ��
5. ������̫����֡��IPv4��TCP��UDP��HTPP��ARP��ICMP Э�����ݣ�
*/


#include "stdafx.h"
#include "snifferCore.h"
#include "winsock2.h"
#include "Winuser.h" // MESSAGE
#include "string"
#include "iostream"
#include "utils.h"
#include "netprotocol.h"
#include "mysniffDlg.h" 
#include "winnt.h"
// ��̫�����ݽ�����֧��
#include "frame_parser.h"


SnifferCore::SnifferCore(CmysniffDlg * this_of_gui): m_pSniffDlg(this_of_gui), m_alldevs(NULL), m_dev(NULL), m_opened_if_handle(NULL)
{

}


SnifferCore::~SnifferCore()
{
	/* We don't need any more the device list. Free it */
	pcap_freealldevs(m_alldevs);
}


int SnifferCore::snif_initCap() {
	// ������¼�ҵ��������豸
	int devCount = 0;
	char * errbuff = new char[PCAP_ERRBUF_SIZE];
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &m_alldevs, errbuff) == -1)
	if (pcap_findalldevs(&m_alldevs, errbuff) == -1)
	{
		int x = MessageBox(GetForegroundWindow(), _T("û���ҵ������豸����ȷ��Npcap-1.60�����Ѿ���װ��"), _T("����"), 2);
		if (x == 3) { // ��ֹ
			PostMessage(NULL, WM_QUIT, 0, 0);
		}
		if (x == 4) { // ����
			while (1) {
				x = MessageBox(GetForegroundWindow(), _T("û���ҵ������豸����ȷ��Npcap-1.60�����Ѿ���װ��"), _T("����"), 2);
				if (x != 4) break;
			}
			if (x == 3) {
				PostMessage(NULL, WM_QUIT, 0, 0);
			}
		}
	}

	for (auto dev = m_alldevs; dev; dev = dev->next) {
		printf("\n%d : ��������: %s\n", ++devCount, dev->name);
		if (dev->description)
			printf("\t������%s��\n", dev->description);

		m_adapterName2dev[std::string(dev->description)] = dev;	
	}
	
	if (devCount > 0) return devCount;
	return -1; // No adapter found
}


pcap_if_t* SnifferCore::getAvaliableDevs() const {
	return m_alldevs;
}

pcap_if_t* SnifferCore::getChoosedIf() const {
	return m_dev;
}

pcap_if_t * SnifferCore::adapterName2dev(const std::string adpName) const
{
	if (m_adapterName2dev.find(adpName) != m_adapterName2dev.end()) {
		//return m_adapterName2dev[adpName];
		auto iter = m_adapterName2dev.find(adpName);
		//std::cout << std::string(iter->second->description) << "IN adpName2dev" << std::endl;
		return iter->second;
	}
	return nullptr;
}

pcap_t * SnifferCore::getOpenedIfHandle() const
{
	return m_opened_if_handle;
}
pcap_dumper_t * SnifferCore::getDumper() const
{
	return this->dumpfile;
}
const char * SnifferCore::getDefaltDumpFilePath() const
{
	return this->default_dump_file;
}
void  SnifferCore::setThreadHandle(HANDLE thread_handle)
{
	m_threadHandle = thread_handle;
}

void SnifferCore::setnpkt(int n)
{
	this->m_npkt = n;
}

int SnifferCore::getnpkt() const
{
	if (this->m_npkt <= 0) return 1;
	return this->m_npkt;
}

HANDLE SnifferCore::getThreadHandle()
{
	return m_threadHandle;
}

void SnifferCore::setChoosedIf(pcap_if_t * dev)
{
	m_dev = dev;
}

void SnifferCore::setChoosedRule(string rule)
{
	if (rule == "��ѡ����˹��򣨿�ѡ��") {
		this->m_filter_rule = "";
	}else
		this->m_filter_rule = rule;
}

void SnifferCore::setOpenIfHandle(pcap_t* opend)
{
	m_opened_if_handle = opend;
}

string SnifferCore::getChoosedRule()
{
	return this->m_pSniffDlg->getFilterRule();
	//return this->m_filter_rule;
}
int SnifferCore::m_snif_CreateCapThread()
{
	// �رվɵ�ץ���߳�
	if (m_threadHandle != NULL) {
		CloseHandle(m_threadHandle);
	}

	// ����ץ���߳�
	LPDWORD threadCap = NULL;
	auto threadHandle = CreateThread(NULL, 0, m_snif_CapThreadFun, this, 0, threadCap);

	if (threadHandle == NULL) {
		int code = GetLastError();
		CString str;
		str.Format(_T("�����̴߳��󣬴���Ϊ%d."), code);
		MessageBox(GetForegroundWindow(), str, _T("�����̴߳��󣬴������%d."), 0);
		return -1;

	}
	this->setThreadHandle(threadHandle);

	return 0;
}

DWORD SnifferCore::m_snif_CapThreadFun(LPVOID lpParameter) 
{
 	// printf("Thread Function Called\n");
	SnifferCore* _this = (SnifferCore*)lpParameter;
	struct pcap_pkthdr* pkt_header;   // generic header add by pcap
	const u_char *pkt_data;
	
	int code = 0;
	DataParser _parser;
	_this->data_parser = _parser;
	_this->setnpkt(0);
	/* Retrieve the packets */
	while ((code = pcap_next_ex(_this->getOpenedIfHandle(), &pkt_header, &pkt_data)) >= 0) {
		/* save the packet on the dump file */
		if((u_char*)_this->dumpfile != NULL)
			pcap_dump((u_char*)_this->dumpfile, pkt_header, pkt_data);

		/* Timeout elapsed */
		if (code == 0) {
			continue;
		}	
		
		// ���ݽ���
		_this->data_parser.set(pkt_header, pkt_data);
		_this->data_parser.parse();

		// �������
		pktCount nPacket = _this->data_parser.getStatistics();
		headerPack hdrPack = _this->data_parser.getParsedHeaderPack();
		// ��ӡ
	/*	cout << endl << "-------------------------------";
		printf("Name: %s\n Total:%d ", hdrPack.pktType, nPacket.n_sum);
		cout << endl << "-------------------------------" << endl;
		*/
		// update GUI
		_this->tellGuiToUpdate(&nPacket, &hdrPack);

		/* m_npkt++;*/
		_this->setnpkt(_this->getnpkt()+1);
	}

	// �ָ���ť״̬
	_this->m_pSniffDlg->m_buttonRead.EnableWindow(TRUE);
	_this->m_pSniffDlg->m_buttonStart.EnableWindow(TRUE);
	_this->m_pSniffDlg->m_buttonSave.EnableWindow(TRUE);
	return 0;
}

void SnifferCore::tellGuiToUpdate(const pktCount* nPacket, const headerPack* hdrPack) {
	this->m_pSniffDlg->UpdateGui(nPacket, hdrPack);
}


int SnifferCore::snif_startCap() {

	// ����������ʼ��������
	pcap_if_t * curAdapter = this->getChoosedIf();
	if (curAdapter == NULL) {
		MessageBox(GetForegroundWindow(), _T("����ѡ��һ��Ҫ�����������ӿ�"), _T("��ʾ"), 1);
		return -1;
	}

	char * errbuf = new char[PCAP_ERRBUF_SIZE];
	if ((m_opened_if_handle = pcap_open_live(curAdapter->name,   // �豸��
		65536,												  // �������ݰ�����
		PCAP_OPENFLAG_PROMISCUOUS,                                                    // ����ģʽ
		1000,                                                 // ��ʱ����
		errbuf)) == NULL)
	{                                   // ������Ϣ

		MessageBox(GetForegroundWindow(), _T("�޷��򿪽ӿڣ�") + CString(curAdapter->description)
			+ CString("\n�������飺") + CString(errbuf), _T("����"), 1);
		return -1;
	}

	if (pcap_datalink(m_opened_if_handle) != DLT_EN10MB) {
		MessageBox(GetForegroundWindow(), _T("��֧�ּ�����̫���磬����������ӿڽ��м�����"), _T("��ʾ"), 1);
		return -1;
	}

	// ���ù�����
	if (this->snif_setupFilter() != 0) {
		return -1;
	}

	// opens a dump file and associates it with the interface 
	dumpfile = pcap_dump_open(this->getOpenedIfHandle(), default_dump_file);

	// �½�ץ���̣߳��ڸ��߳��������ͳ�Ƶȣ�
	return this->m_snif_CreateCapThread();
}

int SnifferCore::snif_setupFilter() {

	// ����filter =====================================================
	struct bpf_program fcode;
	auto curIf = this->getChoosedIf();
	int netmask;
	if (curIf->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask = ((struct sockaddr_in *)(curIf->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask = 0xffffff;
	//compile the filter
//	auto tmp = this->getChoosedRule();
	if (pcap_compile(this->getOpenedIfHandle(), &fcode, const_cast<char*>(this->getChoosedRule().c_str()), 1, netmask) <0)
	{
		MessageBox(GetForegroundWindow(), _T("�޷�������˹���.������˹����﷨"), _T("����"), 1);
	//	printf("\nUnable to compile the packet filter. Check the syntax.\n");
		return -1;
	}

	//set the filter
	if (pcap_setfilter(this->getOpenedIfHandle(), &fcode)<0)
	{
		MessageBox(GetForegroundWindow(), _T("���ù��˹���ʱ���ִ���"), _T("��ʾ"), 1);
		fprintf(stderr, "\nError setting the filter.\n");
		return -1;
	}
	return 0;
}