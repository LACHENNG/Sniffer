
/*
* Copyright (c) Lang NWPU
* Description:
* Implement Core of sniffer backend with various functionality suppor
1. 如果主机有多块网卡，用户可选定网卡；
2. 按照协议类型、源地址和目的地址过滤、查找数据帧（分组或数据段）；
3. 实现IP 分片的重组；
4. 支持分析结果的保存与读取；
5. 解析以太数据帧、IPv4、TCP、UDP、HTPP、ARP、ICMP 协议数据；
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
// 以太网数据解析器支持
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
	// 用来记录找到的网卡设备
	int devCount = 0;
	char * errbuff = new char[PCAP_ERRBUF_SIZE];
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &m_alldevs, errbuff) == -1)
	if (pcap_findalldevs(&m_alldevs, errbuff) == -1)
	{
		int x = MessageBox(GetForegroundWindow(), _T("没有找到网卡设备，请确认Npcap-1.60驱动已经安装！"), _T("错误"), 2);
		if (x == 3) { // 终止
			PostMessage(NULL, WM_QUIT, 0, 0);
		}
		if (x == 4) { // 重试
			while (1) {
				x = MessageBox(GetForegroundWindow(), _T("没有找到网卡设备，请确认Npcap-1.60驱动已经安装！"), _T("错误"), 2);
				if (x != 4) break;
			}
			if (x == 3) {
				PostMessage(NULL, WM_QUIT, 0, 0);
			}
		}
	}

	for (auto dev = m_alldevs; dev; dev = dev->next) {
		printf("\n%d : 网卡名称: %s\n", ++devCount, dev->name);
		if (dev->description)
			printf("\t描述（%s）\n", dev->description);

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
	if (rule == "请选择过滤规则（可选）") {
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
	// 关闭旧的抓包线程
	if (m_threadHandle != NULL) {
		CloseHandle(m_threadHandle);
	}

	// 启动抓包线程
	LPDWORD threadCap = NULL;
	auto threadHandle = CreateThread(NULL, 0, m_snif_CapThreadFun, this, 0, threadCap);

	if (threadHandle == NULL) {
		int code = GetLastError();
		CString str;
		str.Format(_T("创建线程错误，代码为%d."), code);
		MessageBox(GetForegroundWindow(), str, _T("创建线程错误，错误代码%d."), 0);
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
		
		// 数据解析
		_this->data_parser.set(pkt_header, pkt_data);
		_this->data_parser.parse();

		// 解析结果
		pktCount nPacket = _this->data_parser.getStatistics();
		headerPack hdrPack = _this->data_parser.getParsedHeaderPack();
		// 打印
	/*	cout << endl << "-------------------------------";
		printf("Name: %s\n Total:%d ", hdrPack.pktType, nPacket.n_sum);
		cout << endl << "-------------------------------" << endl;
		*/
		// update GUI
		_this->tellGuiToUpdate(&nPacket, &hdrPack);

		/* m_npkt++;*/
		_this->setnpkt(_this->getnpkt()+1);
	}

	// 恢复按钮状态
	_this->m_pSniffDlg->m_buttonRead.EnableWindow(TRUE);
	_this->m_pSniffDlg->m_buttonStart.EnableWindow(TRUE);
	_this->m_pSniffDlg->m_buttonSave.EnableWindow(TRUE);
	return 0;
}

void SnifferCore::tellGuiToUpdate(const pktCount* nPacket, const headerPack* hdrPack) {
	this->m_pSniffDlg->UpdateGui(nPacket, hdrPack);
}


int SnifferCore::snif_startCap() {

	// 打开网卡、初始化过滤器
	pcap_if_t * curAdapter = this->getChoosedIf();
	if (curAdapter == NULL) {
		MessageBox(GetForegroundWindow(), _T("请先选择一个要监听的网卡接口"), _T("提示"), 1);
		return -1;
	}

	char * errbuf = new char[PCAP_ERRBUF_SIZE];
	if ((m_opened_if_handle = pcap_open_live(curAdapter->name,   // 设备名
		65536,												  // 捕获数据包长度
		PCAP_OPENFLAG_PROMISCUOUS,                                                    // 混杂模式
		1000,                                                 // 超时设置
		errbuf)) == NULL)
	{                                   // 错误信息

		MessageBox(GetForegroundWindow(), _T("无法打开接口：") + CString(curAdapter->description)
			+ CString("\n错误详情：") + CString(errbuf), _T("错误"), 1);
		return -1;
	}

	if (pcap_datalink(m_opened_if_handle) != DLT_EN10MB) {
		MessageBox(GetForegroundWindow(), _T("仅支持监听以太网络，请更换其它接口进行监听！"), _T("提示"), 1);
		return -1;
	}

	// 设置过滤器
	if (this->snif_setupFilter() != 0) {
		return -1;
	}

	// opens a dump file and associates it with the interface 
	dumpfile = pcap_dump_open(this->getOpenedIfHandle(), default_dump_file);

	// 新建抓包线程（在该线程里解析、统计等）
	return this->m_snif_CreateCapThread();
}

int SnifferCore::snif_setupFilter() {

	// 设置filter =====================================================
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
		MessageBox(GetForegroundWindow(), _T("无法编译过滤规则.请检查过滤规则语法"), _T("错误"), 1);
	//	printf("\nUnable to compile the packet filter. Check the syntax.\n");
		return -1;
	}

	//set the filter
	if (pcap_setfilter(this->getOpenedIfHandle(), &fcode)<0)
	{
		MessageBox(GetForegroundWindow(), _T("设置过滤规则时出现错误"), _T("提示"), 1);
		fprintf(stderr, "\nError setting the filter.\n");
		return -1;
	}
	return 0;
}