
// mysniffDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mysniff.h"
#include "mysniffDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include "string"
#include "Winuser.h" // MESSAGE
// winCap Suport
#include "pcap.h"

#include "iostream"
#include "string"

# include "utils.h"
//#define _DEBUG

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

														// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmysniffDlg 对话框



CmysniffDlg::CmysniffDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYSNIFF_DIALOG, pParent), m_snifferCore(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmysniffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_editTCP);
	DDX_Control(pDX, IDC_EDIT2, m_editUDP);
	DDX_Control(pDX, IDC_EDIT4, m_editICMP);
	DDX_Control(pDX, IDC_EDIT6, m_editHTTP);
	DDX_Control(pDX, IDC_EDIT5, m_editARP);
	DDX_Control(pDX, IDC_EDIT7, m_editICMPv6);
	DDX_Control(pDX, IDC_EDIT9, m_editIPv4);
	DDX_Control(pDX, IDC_EDIT8, m_editIPv6);
	DDX_Control(pDX, IDC_EDIT10, m_editOther);
	DDX_Control(pDX, IDC_EDIT11, m_editSum);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON3, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON4, m_buttonStop);
	DDX_Control(pDX, IDC_BUTTON5, m_buttonSave);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonRead);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
//	DDX_Control(pDX, IDC_COMBO2, m_comboBoxRule);
	DDX_Control(pDX, IDC_EDIT12, m_CMyEdit);
}

BEGIN_MESSAGE_MAP(CmysniffDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CmysniffDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CmysniffDlg::OnBnClickedButton2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CmysniffDlg::OnTvnSelchangedTree1)
//	ON_CBN_SELCHANGE(IDC_COMBO2, &CmysniffDlg::OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT3, &CmysniffDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT2, &CmysniffDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CmysniffDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CmysniffDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CmysniffDlg::OnBnClickedButton4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CmysniffDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON5, &CmysniffDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CmysniffDlg::OnNMCustomdrawList1)
	ON_EN_CHANGE(IDC_EDIT12, &CmysniffDlg::OnEnChangeEdit12)
	ON_BN_CLICKED(IDC_BUTTON1, &CmysniffDlg::OnClickedButtonHelp)
END_MESSAGE_MAP()


// CmysniffDlg 消息处理程序

BOOL CmysniffDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// TODO: 优化添加column的代码
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCtrl.InsertColumn(0, _T("编号"), 2, 80); //1表示右，2表示中，3表示左
	m_listCtrl.InsertColumn(1, _T("时间"), 2, 260);
	m_listCtrl.InsertColumn(2, _T("长度"), 2, 90);
	m_listCtrl.InsertColumn(3, _T("源MAC地址"), 2, 220);
	m_listCtrl.InsertColumn(4, _T("目标MAC地址"), 2, 220);
	m_listCtrl.InsertColumn(5, _T("协议"), 2, 90);
	m_listCtrl.InsertColumn(6, _T("源IP地址"), 2, 220);
	m_listCtrl.InsertColumn(7, _T("目标IP地址"), 2, 220);

	m_comboBox.AddString(_T("请选择一个网卡接口（必选）"));
	m_comboBox.SetCurSel(0);
	//m_comboBoxRule.AddString(_T("请选择过滤规则（可选）"));

	//m_comboBoxRule.SetCurSel(0);

	//m_comboBoxRule.AddString(_T("ether"));
	//m_comboBoxRule.AddString(_T("tr"));
	//m_comboBoxRule.AddString(_T("ip"));
	//m_comboBoxRule.AddString(_T("ip6"));
	//m_comboBoxRule.AddString(_T("arp"));
	//m_comboBoxRule.AddString(_T("rarp"));
	//m_comboBoxRule.AddString(_T("decnet"));
	//m_comboBoxRule.AddString(_T("tcp"));
	//m_comboBoxRule.AddString(_T("udp"));
	//m_comboBoxRule.AddString(_T("xxx"));

	m_CMyEdit.setPrompt();

	//  fddi(ether), tr, ip, ip6, arp, rarp, decnet, tcp and udp

	// 初始化设置“结束”和“保存”按钮不可用
	m_buttonStop.EnableWindow(FALSE);
	m_buttonSave.EnableWindow(FALSE);


	// 初始化网络适配器列表
	if (m_snifferCore.snif_initCap() < 0) {
		return FALSE;
	}

	/*更新GUI中网络适配器列表*/

	for (auto dev = m_snifferCore.getAvaliableDevs(); dev; dev = dev->next) {
		m_comboBox.AddString(CString(dev->description));
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmysniffDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmysniffDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标 
//显示。
HCURSOR CmysniffDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 网卡选择下拉框
void CmysniffDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSel = m_comboBox.GetCurSel();


	CString selText;
	m_comboBox.GetWindowText(selText);

	// 设置当前处理的dev
	auto curDev = m_snifferCore.adapterName2dev(CString2string(selText));
	m_snifferCore.setChoosedIf(curDev);

	if (m_snifferCore.getChoosedIf()) {
		//assert(std::string(m_snifferCore.getChoosedIf()->description) == CString2string(selText));
		ASSERT(std::string(m_snifferCore.getChoosedIf()->description) == CString2string(selText));
	}
}


// 选择filter的下拉框
//void CmysniffDlg::OnCbnSelchangeCombo2()
//{
//
//	CString str_rule;
//	//m_comboBox.GetWindowText(selText);
//	m_comboBoxRule.GetWindowText(str_rule);
//	// 设置当前处理的dev
//	auto rule = CString2string(str_rule);
//	//auto curDev = m_snifferCore.adapterName2dev(CString2string(str_rule));
//	//m_snifferCore.setChoosedIf(curDev);
//	m_snifferCore.setChoosedRule(rule);
//	printf("CURRENT CHOOSED RULE: %s\n\n\n\n\n\n", rule.c_str());
//
//}

void CmysniffDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 读取 按钮
void CmysniffDlg::OnBnClickedButton2()
{
	// 打开文件对话框
	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir = L"D:\\";	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	CString filter = L"文件 (*.dmp)||";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"D:\\";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath;
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	else {
		return ;
	}

	
	/* Create the source string according to the new WinPcap syntax */
	CFileDialog fdlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All files(*.dmp)");
	fdlg.m_ofn.lpstrTitle = L"Open File";
	fdlg.m_ofn.lpstrInitialDir = L"D:\\";

	char source[PCAP_BUF_SIZE];
	char errbuf[PCAP_ERRBUF_SIZE];
	if (pcap_createsrcstr(source,         // variable that will keep the source string
		PCAP_SRC_FILE,  // we want to open a file
		NULL,           // remote host
		NULL,           // port on the remote host
		CT2A(filePath),        // name of the file we want to open
		errbuf          // error buffer
		) != 0)
	{
		fprintf(stderr, "\nError creating a source string\n");
		MessageBox(_T("Error creating a source string"), _T("错误"));
		return ;
	}

	pcap_t *fp;
	/* Open the capture file */
	if ((fp = pcap_open(source,         // name of the device
		65536,          // portion of the packet to capture
						// 65536 guarantees that the whole packet will be captured on all the link layers
		PCAP_OPENFLAG_PROMISCUOUS,     // promiscuous mode
		1000,              // read timeout
		NULL,              // authentication on the remote machine
		errbuf         // error buffer
		)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the file %s.\n", source);
		MessageBox(_T("Unable to open file"), _T("错误"));
		return ;
	}
	


	// 重置GUI状态
	m_treeCtrl.DeleteAllItems();
	m_listCtrl.DeleteAllItems();
	m_edit.SetWindowText(_T("")); 
	// 计数统计重置
	pktCount pktcnt;
	CmysniffDlg::updateNPacket(&pktcnt);
	
	// 启动抓包线程
	m_snifferCore.m_snif_CreateCapThread();

	// setup the opened interface handle (in this case, this handle is create from file)
	m_snifferCore.setOpenIfHandle(fp);
	m_buttonRead.EnableWindow(FALSE);
	m_buttonSave.EnableWindow(FALSE);
	m_buttonStart.EnableWindow(FALSE);
	//WaitForSingleObject(this->m_snifferCore.getOpenedIfHandle(), INFINITE);	//一直等待，直到子线程返回
	//CloseHandle(this->m_snifferCore.getOpenedIfHandle());
}


void CmysniffDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CmysniffDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CmysniffDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CmysniffDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

}

// 开始按钮
void CmysniffDlg::OnBnClickedButton3()
{
	// 计数统计重置
	pktCount pktcnt;
	CmysniffDlg::updateNPacket(&pktcnt);


	// 列表Item重新计数
	m_listCtrl.DeleteAllItems();
	m_snifferCore.setnpkt(1);
	int status = m_snifferCore.snif_startCap();
	if (status == -1) {
		printf("Error in snif_startCap\n");
		return;
	}
	
	m_comboBox.EnableWindow(FALSE);
	// m_comboBoxRule.EnableWindow(FALSE);
	m_buttonStart.EnableWindow(FALSE);
	m_buttonStop.EnableWindow(TRUE);
	m_buttonRead.EnableWindow(FALSE);
	m_buttonSave.EnableWindow(FALSE);
}


// Gui界面数据更新, 被snifferCore主动调用
void CmysniffDlg::UpdateGui(const pktCount* npkt, const datapkt* hdrspack)
{
	this->update_listCtrl(npkt, hdrspack);
	this->updateNPacket(npkt);
}

// 更新中间位置的列表
void CmysniffDlg::update_listCtrl(const pktCount * npkt, const datapkt * hdrs_pack)
{
	char strbuf[32];
	CString num, ts, len, s_mac, d_mac, proto, s_ip, d_ip;
	
	u_char* mac_arr;
	num.Format(_T("%d"), m_snifferCore.getnpkt());
	len.Format(_T("%d"), hdrs_pack->pcaph->len);
	mac_arr = hdrs_pack->ethh->s_mac;
	s_mac.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
	mac_arr = hdrs_pack->ethh->d_mac;
	d_mac.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
	proto.Format(_T("%S"), hdrs_pack->pktType);
	
	// ts
	/* convert the timestamp to readable format*/
	struct tm* ltime;
	time_t t = hdrs_pack->pcaph->ts.tv_sec;
	ltime = localtime(&t);
	strftime(strbuf, sizeof(strbuf), "%Y/%m/%d %H:%M:%S", ltime);
	ts = CString(strbuf);
	//// ip
	auto code = ntohs(hdrs_pack->ethh->proto);
	if (code == ETH_PROTOCOL_ARP) {
		s_ip.Format(_T("%d.%d.%d.%d"), hdrs_pack->arph->saddr.byte1, hdrs_pack->arph->saddr.byte2, hdrs_pack->arph->saddr.byte3, hdrs_pack->arph->saddr.byte4);
		d_ip.Format(_T("%d.%d.%d.%d"), hdrs_pack->arph->daddr.byte1, hdrs_pack->arph->daddr.byte2, hdrs_pack->arph->daddr.byte3, hdrs_pack->arph->daddr.byte4);
	}
	else if(code == ETH_PROTOCOL_IP) {
		struct in_addr ip;
		ip.S_un.S_addr = *((u_long*)(void*)(&hdrs_pack->iph->saddr));
		s_ip = CString(inet_ntoa(ip));
		ip.S_un.S_addr = *((u_long*)(void*)(&hdrs_pack->iph->daddr));
		d_ip = CString(inet_ntoa(ip));
	}
	else if (code == ETH_PROTOCOL_IPV6) {
		for (int i = 0; i < 7; i++) {
			s_ip.AppendFormat(_T("%02x:", hdrs_pack->iph6->saddr[i]));
			d_ip.AppendFormat(_T("%02x:", hdrs_pack->iph6->daddr[i]));
		}
		s_ip.AppendFormat(_T("%02x", hdrs_pack->iph6->saddr[7]));
		d_ip.AppendFormat(_T("%02x", hdrs_pack->iph6->daddr[7]));
	}
	// ListControl
	int nitem = m_listCtrl.InsertItem(m_snifferCore.getnpkt(), num);
	m_listCtrl.SetItemText(nitem, 1, ts);
	m_listCtrl.SetItemText(nitem, 2, len);
	m_listCtrl.SetItemText(nitem, 3, s_mac);
	m_listCtrl.SetItemText(nitem, 4, d_mac);
	m_listCtrl.SetItemText(nitem, 5, proto);
	m_listCtrl.SetItemText(nitem, 6, s_ip);
	m_listCtrl.SetItemText(nitem, 7, d_ip);
	
}

void CmysniffDlg::updateNPacket(const pktCount* npkt)
{
	CString buf;
	buf.Format(_T("%d"), npkt->n_sum);
	this->m_editSum.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_arp);
	this->m_editARP.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_http);
	this->m_editHTTP.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_icmp);
	this->m_editICMP.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_icmp6);
	this->m_editICMPv6.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_ip);
	this->m_editIPv4.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_ip6);
	this->m_editIPv6.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_tcp);
	this->m_editTCP.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_udp);
	this->m_editUDP.SetWindowText(buf);

	buf.Format(_T("%d"), npkt->n_other);
	this->m_editOther.SetWindowText(buf);
}

void CmysniffDlg::updateTree(int index, const pktCount * npkt, const datapkt * hdrsPack)
{	
	this->m_treeCtrl.DeleteAllItems();
	// POSITION pos;
	CString strbuf;
	auto iph = hdrsPack->iph;
	//printf("\n(in update Tree) Select: %d\n", index);
	//printf("in func tree : %d.%d.%d.%d\n", iph->saddr.byte1, iph->saddr.byte2, iph->saddr.byte3, iph->saddr.byte4);
	//printf("in func tree: %d.%d.%d.%d\n", iph->daddr.byte1, iph->daddr.byte2, iph->daddr.byte3, iph->daddr.byte4);

	HTREEITEM root = this->m_treeCtrl.GetRootItem();
	strbuf.Format(_T("接受到的第%d个数据包"), index + 1);

	HTREEITEM head = this->m_treeCtrl.InsertItem(strbuf, root);

	/* 链路层 */
	HTREEITEM frame = this->m_treeCtrl.InsertItem(_T("链路层数据"), head);
	// 源 mac 
	strbuf.Format(_T("源MAC: "));
	auto mac_arr = hdrsPack->ethh->s_mac;
	strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
	this->m_treeCtrl.InsertItem(strbuf, frame);
	// 目的 mac 
	strbuf.Format(_T("目的MAC: "));
	mac_arr = hdrsPack->ethh->d_mac;
	strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
	this->m_treeCtrl.InsertItem(strbuf, frame);
	// 类型
	strbuf.Format(_T("类型码：0x%04x"), ntohs(hdrsPack->ethh->proto));
	this->m_treeCtrl.InsertItem(strbuf, frame);
	
	
	// 处理下一层协议数据 IP、ARP、IPv6...
	auto proto = ntohs(hdrsPack->ethh->proto);
	if (ETH_PROTOCOL_ARP == proto) {
		cout << "Enter ETH_PROTOCOL_ARP" << endl;
		auto arphdr = hdrsPack->arph;
		HTREEITEM arp = this->m_treeCtrl.InsertItem(_T("ARP协议头"), head);
		strbuf.Format(_T("硬件类型: %hu", ntohs(arphdr->hardware_type)));
		this->m_treeCtrl.InsertItem(strbuf, arp);
		strbuf.Format(_T("协议类型: 0x%hx", ntohs(arphdr->proto_type)));
		this->m_treeCtrl.InsertItem(strbuf, arp);
	
		strbuf.Format(_T("硬件地址长度: %u", (u_int)arphdr->hlen & 0xff));
		cout << arphdr->hlen << " hardware len " << endl;
		this->m_treeCtrl.InsertItem(strbuf, arp);
		strbuf.Format(_T("协议地址长度: %u", (u_int)arphdr->plen & 0xff));
		this->m_treeCtrl.InsertItem(strbuf, arp);
		strbuf.Format(_T("操作码: 0x%hx", ntohs(arphdr->op_code)));
		this->m_treeCtrl.InsertItem(strbuf, arp);
		// 收发双方Mac、 Ip 
		strbuf.Format(_T("发送方MAC: "));
		mac_arr = hdrsPack->arph->src_mac;
		strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
		this->m_treeCtrl.InsertItem(strbuf, arp);

		strbuf.Format(_T("发送方ip: "));
		auto iparr = hdrsPack->arph->saddr;
		strbuf.AppendFormat(_T("%d.%d.%d.%d"), iparr.byte1, iparr.byte2, iparr.byte3, iparr.byte4);
		this->m_treeCtrl.InsertItem(strbuf, arp);

		strbuf.Format(_T("接受方MAC: "));
		mac_arr = hdrsPack->arph->dest_mac;
		strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
		this->m_treeCtrl.InsertItem(strbuf, arp);

		strbuf.Format(_T("接收方ip: "));
		iparr = hdrsPack->arph->daddr;
		strbuf.AppendFormat(_T("%d.%d.%d.%d"), iparr.byte1, iparr.byte2, iparr.byte3, iparr.byte4);
		this->m_treeCtrl.InsertItem(strbuf, arp);
	}
	else if (ETH_PROTOCOL_IP == proto) {
		HTREEITEM ip = this->m_treeCtrl.InsertItem(_T("IP协议头"), head);
		auto iphdr = hdrsPack->iph;
		strbuf.Format(_T("版本: %d"), (iphdr->ver_ihl >> 4) & 0xf);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("IP头部长度: %d(bytes)"), (iphdr->ver_ihl & 0xf) * 4 );
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("服务类型: %d"), iphdr->tos);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("总长度: %d"), ntohs(iphdr->tlen));
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("标识: %d"), ntohs(iphdr->identification));
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("段偏移: %d"), ntohs(iphdr->flags_fo) & 0x1fff);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("生存期: %d"), iphdr->ttl);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("协议: %d"), iphdr->proto);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("头部校验和: %d"), ntohs(iphdr->crc));
		this->m_treeCtrl.InsertItem(strbuf, ip);

		strbuf.Format(_T("源IP: "));
		struct in_addr in;
		in.S_un.S_addr =*((u_long*)(void*) &iphdr->saddr);
		strbuf.AppendFormat(CString(inet_ntoa(in)));
		this->m_treeCtrl.InsertItem(strbuf, ip);

		strbuf.Format(_T("目标IP: "));
		in.S_un.S_addr = *((u_long*)(void*)&iphdr->daddr);
		strbuf.AppendFormat(CString(inet_ntoa(in)));
		this->m_treeCtrl.InsertItem(strbuf, ip);

		/* Transport Layer: ICMP、 UDP 、 TCP */
		if (IP_PROTOCOL_ICMP == hdrsPack->iph->proto) {
			HTREEITEM icmp = this->m_treeCtrl.InsertItem(_T("ICMP协议头"), head);

			strbuf.Format(_T("类型: %d"), hdrsPack->icmph->type);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
			strbuf.Format(_T("代码: %d"), hdrsPack->icmph->code);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
			strbuf.Format(_T("序号: %d"), hdrsPack->icmph->seq);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
			strbuf.Format(_T("校验和: %d"), hdrsPack->icmph->chksum);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
		}
		else if (IP_PROTOCOL_TCP == hdrsPack->iph->proto) {
			HTREEITEM tcp = this->m_treeCtrl.InsertItem(_T("TCP协议头"), head);
			strbuf.Format(_T("源端口: %d"), ntohs(hdrsPack->tcph->sport));
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("目标端口: %d"), ntohs(hdrsPack->tcph->dport));
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("序号: %u"), hdrsPack->tcph->seq_num);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("确认号: %u"), hdrsPack->tcph->ack_num);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("头部长度: %d"), hdrsPack->tcph->thl);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			
			HTREEITEM flag = this->m_treeCtrl.InsertItem(_T("标志位"), tcp);
			strbuf.Format(_T("cwr: %d"), hdrsPack->tcph->cwr);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			strbuf.Format(_T("ece: %d"), hdrsPack->tcph->ece);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			
			strbuf.Format(_T("urg: %d"), hdrsPack->tcph->urg);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			strbuf.Format(_T("ack: %d"), hdrsPack->tcph->ack);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			strbuf.Format(_T("psh: %d"), hdrsPack->tcph->psh);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			strbuf.Format(_T("rst: %d"), hdrsPack->tcph->rst);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			strbuf.Format(_T("syn: %d"), hdrsPack->tcph->syn);
			this->m_treeCtrl.InsertItem(strbuf, flag);
			strbuf.Format(_T("fin: %d"), hdrsPack->tcph->fin);
			this->m_treeCtrl.InsertItem(strbuf, flag);

			strbuf.Format(_T("	紧急指针: 0x%hx"), hdrsPack->tcph->urg_ptr);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("	校验和: %hd"), hdrsPack->tcph->checksum);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
		}
		else if (IP_PROTOCOL_UDP == hdrsPack->iph->proto) {
			HTREEITEM udp = this->m_treeCtrl.InsertItem(_T("UDP协议头"), head);
			strbuf.Format(_T("源端口: %d"), hdrsPack->udph->sport);
			this->m_treeCtrl.InsertItem(strbuf, udp);
			strbuf.Format(_T("目标端口: %d"), hdrsPack->udph->dport);
			this->m_treeCtrl.InsertItem(strbuf, udp);
			strbuf.Format(_T("总长度: %d"), hdrsPack->udph->len);
			this->m_treeCtrl.InsertItem(strbuf, udp);

			strbuf.Format(_T("校验和: %d"), hdrsPack->udph->crc);
			this->m_treeCtrl.InsertItem(strbuf, udp);
		}
	}
	else if (ETH_PROTOCOL_IPV6 == proto) {
		cout << "IPv6 head parse not supported" << endl;
	}
	else {
		cout << "尚未实现的Ethernet Protocol类型 " << hdrsPack->ethh->proto << endl;
	}

}

void CmysniffDlg::updateEdit(datapkt * hdrsPack)
{
	u_char* data = (u_char *)hdrsPack;
	int len = hdrsPack->pcaph->len;

	CString buf;
	print_packet_hex(data, len, &buf);
	this->m_edit.SetWindowText(buf);

}

// 结束按钮
void CmysniffDlg::OnBnClickedButton4()
/*
* Close capture thread and dump file  and reset related buttoms` state
*/
{
	// closes the files associated with p and deallocates resources
	pcap_dump_close(this->m_snifferCore.getDumper());

	HANDLE thread_handle = this->m_snifferCore.getThreadHandle();
	if (NULL == thread_handle) {
		printf("NULL thread handle can not be terminated! \n");
		return;
	}
	if (TerminateThread(thread_handle, -1) == 0) {
		//TODO: ERROR MSG BOX
		MessageBox(_T("线程正忙，无法停止，请稍后再试"), _T("错误"));
		printf("ERROR: FAILED TO STOP THREAD\n");
		return;
	}

	m_comboBox.EnableWindow(TRUE);
//	m_comboBoxRule.EnableWindow(TRUE);
	this->m_snifferCore.setThreadHandle(NULL);
	this->m_buttonStart.EnableWindow(TRUE);
	this->m_buttonStop.EnableWindow(FALSE);
	this->m_buttonSave.EnableWindow(TRUE);
	this->m_buttonRead.EnableWindow(TRUE);

}

// on click item 
void CmysniffDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int index;
	index = this->m_listCtrl.GetHotItem();
	// 获取点击的这一项对应的数据（pktCount, headerPack)
	pair<pktCount, headerPack> data = m_snifferCore.data_parser.getAt(index);
	this->updateTree(index, &data.first, &data.second);

	this->updateEdit(&data.second);

	/*this->lixsniff_updateEdit(index);
	this->lixsniff_updateTree(index);*/
	// TODO END
	*pResult = 0;
}

// 保存按钮
void CmysniffDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fdlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All files(*.dmp)");
	fdlg.m_ofn.lpstrTitle = L"Save File";
	fdlg.m_ofn.lpstrInitialDir = L"D:\\";
	if (fdlg.DoModal() == IDOK)
	{
		CString des_path = fdlg.GetPathName() + ".dmp";
		CString src_path = CString(this->m_snifferCore.getDefaltDumpFilePath());
		// copy dump  file to user specified path
		if (CopyFile(src_path, des_path, FALSE)) {
			MessageBox(_T("保存成功"), _T("提示"));
		}
		else {
			MessageBox(_T("保存失败，请重试"), _T("提示"));
		};
	}
}

void CmysniffDlg::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pNMCD = (LPNMLVCUSTOMDRAW)pNMHDR;
	*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
	
	if (CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->nmcd.dwDrawStage) {
		COLORREF crText;
		string buf;
		auto pos = pNMCD->nmcd.dwItemSpec;
		// 取出该条记录中的协议头数据，
		headerPack local_data = (this->m_snifferCore.data_parser.getAt(pos).second);
		// printf("Idx: %d, PackType: %s\n", pos, local_data.pktType);
		const char * pkttype = local_data.pktType;
		if (strcmp(pkttype, PKTTYPE_IPv6) == 0) {
			crText = RGB(110, 225, 252);
		}
		else if (strcmp(pkttype, PKTTYPE_UDP) == 0) {
			crText = RGB(204, 153, 204);
		}
		else if (strcmp(pkttype, PKTTYPE_TCP) == 0) {
			crText = RGB(164, 194, 224);
		}
		else if (strcmp(pkttype, PKTTYPE_ARP) == 0) {
			crText = RGB(224, 236, 226);
		}
		else if (strcmp(pkttype, PKTTYPE_ICMP) == 0) {
			crText = RGB(49, 166, 239);
		}
		else if (strcmp(pkttype, PKTTYPE_HTTP) == 0){
			crText = RGB(236, 231, 182);
		}
		else if (strcmp(pkttype, PKTTYPE_ICMPv6) == 0) {
			crText = RGB(189, 254, 77);
		}
		pNMCD->clrTextBk = crText;
		*pResult = CDRF_DODEFAULT;
		//POSITION pos = this->m_snifferCore.data_parser.getAt()
	}

}

string CmysniffDlg::getFilterRule()
{
	CString str;
	m_CMyEdit.GetWindowText(str);
	return string(CT2A(str)) == "输入过滤规则(默认不过滤)" ? " " : string(CT2A(str));
}

void CmysniffDlg::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CmysniffDlg::OnClickedButtonHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString mhtPath = _T("WinPcap Filter Syntax.html");		//这里添加需要查看的文件路径或者网址（http://**）或者exe文件。若写成test.exe，则默认当前目录下
 	ShellExecute(NULL, _T("open"), mhtPath, NULL, NULL, SW_SHOWNORMAL);	//第4个参数可传入命令行参数，第5个参数可指定文件目录，第6个参数可为SW_HIDE不显示或者SW_SHOW显示
}
