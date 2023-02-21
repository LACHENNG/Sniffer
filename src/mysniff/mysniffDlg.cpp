
// mysniffDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

														// ʵ��
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


// CmysniffDlg �Ի���



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


// CmysniffDlg ��Ϣ�������

BOOL CmysniffDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// TODO: �Ż����column�Ĵ���
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCtrl.InsertColumn(0, _T("���"), 2, 80); //1��ʾ�ң�2��ʾ�У�3��ʾ��
	m_listCtrl.InsertColumn(1, _T("ʱ��"), 2, 260);
	m_listCtrl.InsertColumn(2, _T("����"), 2, 90);
	m_listCtrl.InsertColumn(3, _T("ԴMAC��ַ"), 2, 220);
	m_listCtrl.InsertColumn(4, _T("Ŀ��MAC��ַ"), 2, 220);
	m_listCtrl.InsertColumn(5, _T("Э��"), 2, 90);
	m_listCtrl.InsertColumn(6, _T("ԴIP��ַ"), 2, 220);
	m_listCtrl.InsertColumn(7, _T("Ŀ��IP��ַ"), 2, 220);

	m_comboBox.AddString(_T("��ѡ��һ�������ӿڣ���ѡ��"));
	m_comboBox.SetCurSel(0);
	//m_comboBoxRule.AddString(_T("��ѡ����˹��򣨿�ѡ��"));

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

	// ��ʼ�����á��������͡����桱��ť������
	m_buttonStop.EnableWindow(FALSE);
	m_buttonSave.EnableWindow(FALSE);


	// ��ʼ�������������б�
	if (m_snifferCore.snif_initCap() < 0) {
		return FALSE;
	}

	/*����GUI�������������б�*/

	for (auto dev = m_snifferCore.getAvaliableDevs(); dev; dev = dev->next) {
		m_comboBox.AddString(CString(dev->description));
	}


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmysniffDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù�� 
//��ʾ��
HCURSOR CmysniffDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// ����ѡ��������
void CmysniffDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int curSel = m_comboBox.GetCurSel();


	CString selText;
	m_comboBox.GetWindowText(selText);

	// ���õ�ǰ�����dev
	auto curDev = m_snifferCore.adapterName2dev(CString2string(selText));
	m_snifferCore.setChoosedIf(curDev);

	if (m_snifferCore.getChoosedIf()) {
		//assert(std::string(m_snifferCore.getChoosedIf()->description) == CString2string(selText));
		ASSERT(std::string(m_snifferCore.getChoosedIf()->description) == CString2string(selText));
	}
}


// ѡ��filter��������
//void CmysniffDlg::OnCbnSelchangeCombo2()
//{
//
//	CString str_rule;
//	//m_comboBox.GetWindowText(selText);
//	m_comboBoxRule.GetWindowText(str_rule);
//	// ���õ�ǰ�����dev
//	auto rule = CString2string(str_rule);
//	//auto curDev = m_snifferCore.adapterName2dev(CString2string(str_rule));
//	//m_snifferCore.setChoosedIf(curDev);
//	m_snifferCore.setChoosedRule(rule);
//	printf("CURRENT CHOOSED RULE: %s\n\n\n\n\n\n", rule.c_str());
//
//}

void CmysniffDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

// ��ȡ ��ť
void CmysniffDlg::OnBnClickedButton2()
{
	// ���ļ��Ի���
	BOOL isOpen = TRUE;		//�Ƿ��(����Ϊ����)
	CString defaultDir = L"D:\\";	//Ĭ�ϴ򿪵��ļ�·��
	CString fileName = L"";			//Ĭ�ϴ򿪵��ļ���
	CString filter = L"�ļ� (*.dmp)||";	//�ļ����ǵ�����
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
		MessageBox(_T("Error creating a source string"), _T("����"));
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
		MessageBox(_T("Unable to open file"), _T("����"));
		return ;
	}
	


	// ����GUI״̬
	m_treeCtrl.DeleteAllItems();
	m_listCtrl.DeleteAllItems();
	m_edit.SetWindowText(_T("")); 
	// ����ͳ������
	pktCount pktcnt;
	CmysniffDlg::updateNPacket(&pktcnt);
	
	// ����ץ���߳�
	m_snifferCore.m_snif_CreateCapThread();

	// setup the opened interface handle (in this case, this handle is create from file)
	m_snifferCore.setOpenIfHandle(fp);
	m_buttonRead.EnableWindow(FALSE);
	m_buttonSave.EnableWindow(FALSE);
	m_buttonStart.EnableWindow(FALSE);
	//WaitForSingleObject(this->m_snifferCore.getOpenedIfHandle(), INFINITE);	//һֱ�ȴ���ֱ�����̷߳���
	//CloseHandle(this->m_snifferCore.getOpenedIfHandle());
}


void CmysniffDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CmysniffDlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CmysniffDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CmysniffDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}

// ��ʼ��ť
void CmysniffDlg::OnBnClickedButton3()
{
	// ����ͳ������
	pktCount pktcnt;
	CmysniffDlg::updateNPacket(&pktcnt);


	// �б�Item���¼���
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


// Gui�������ݸ���, ��snifferCore��������
void CmysniffDlg::UpdateGui(const pktCount* npkt, const datapkt* hdrspack)
{
	this->update_listCtrl(npkt, hdrspack);
	this->updateNPacket(npkt);
}

// �����м�λ�õ��б�
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
	strbuf.Format(_T("���ܵ��ĵ�%d�����ݰ�"), index + 1);

	HTREEITEM head = this->m_treeCtrl.InsertItem(strbuf, root);

	/* ��·�� */
	HTREEITEM frame = this->m_treeCtrl.InsertItem(_T("��·������"), head);
	// Դ mac 
	strbuf.Format(_T("ԴMAC: "));
	auto mac_arr = hdrsPack->ethh->s_mac;
	strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
	this->m_treeCtrl.InsertItem(strbuf, frame);
	// Ŀ�� mac 
	strbuf.Format(_T("Ŀ��MAC: "));
	mac_arr = hdrsPack->ethh->d_mac;
	strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
	this->m_treeCtrl.InsertItem(strbuf, frame);
	// ����
	strbuf.Format(_T("�����룺0x%04x"), ntohs(hdrsPack->ethh->proto));
	this->m_treeCtrl.InsertItem(strbuf, frame);
	
	
	// ������һ��Э������ IP��ARP��IPv6...
	auto proto = ntohs(hdrsPack->ethh->proto);
	if (ETH_PROTOCOL_ARP == proto) {
		cout << "Enter ETH_PROTOCOL_ARP" << endl;
		auto arphdr = hdrsPack->arph;
		HTREEITEM arp = this->m_treeCtrl.InsertItem(_T("ARPЭ��ͷ"), head);
		strbuf.Format(_T("Ӳ������: %hu", ntohs(arphdr->hardware_type)));
		this->m_treeCtrl.InsertItem(strbuf, arp);
		strbuf.Format(_T("Э������: 0x%hx", ntohs(arphdr->proto_type)));
		this->m_treeCtrl.InsertItem(strbuf, arp);
	
		strbuf.Format(_T("Ӳ����ַ����: %u", (u_int)arphdr->hlen & 0xff));
		cout << arphdr->hlen << " hardware len " << endl;
		this->m_treeCtrl.InsertItem(strbuf, arp);
		strbuf.Format(_T("Э���ַ����: %u", (u_int)arphdr->plen & 0xff));
		this->m_treeCtrl.InsertItem(strbuf, arp);
		strbuf.Format(_T("������: 0x%hx", ntohs(arphdr->op_code)));
		this->m_treeCtrl.InsertItem(strbuf, arp);
		// �շ�˫��Mac�� Ip 
		strbuf.Format(_T("���ͷ�MAC: "));
		mac_arr = hdrsPack->arph->src_mac;
		strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
		this->m_treeCtrl.InsertItem(strbuf, arp);

		strbuf.Format(_T("���ͷ�ip: "));
		auto iparr = hdrsPack->arph->saddr;
		strbuf.AppendFormat(_T("%d.%d.%d.%d"), iparr.byte1, iparr.byte2, iparr.byte3, iparr.byte4);
		this->m_treeCtrl.InsertItem(strbuf, arp);

		strbuf.Format(_T("���ܷ�MAC: "));
		mac_arr = hdrsPack->arph->dest_mac;
		strbuf.AppendFormat(_T("%02x:%02x:%02x:%02x:%02x:%02x"), mac_arr[0], mac_arr[1], mac_arr[2], mac_arr[3], mac_arr[4], mac_arr[5]);
		this->m_treeCtrl.InsertItem(strbuf, arp);

		strbuf.Format(_T("���շ�ip: "));
		iparr = hdrsPack->arph->daddr;
		strbuf.AppendFormat(_T("%d.%d.%d.%d"), iparr.byte1, iparr.byte2, iparr.byte3, iparr.byte4);
		this->m_treeCtrl.InsertItem(strbuf, arp);
	}
	else if (ETH_PROTOCOL_IP == proto) {
		HTREEITEM ip = this->m_treeCtrl.InsertItem(_T("IPЭ��ͷ"), head);
		auto iphdr = hdrsPack->iph;
		strbuf.Format(_T("�汾: %d"), (iphdr->ver_ihl >> 4) & 0xf);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("IPͷ������: %d(bytes)"), (iphdr->ver_ihl & 0xf) * 4 );
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("��������: %d"), iphdr->tos);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("�ܳ���: %d"), ntohs(iphdr->tlen));
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("��ʶ: %d"), ntohs(iphdr->identification));
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("��ƫ��: %d"), ntohs(iphdr->flags_fo) & 0x1fff);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("������: %d"), iphdr->ttl);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("Э��: %d"), iphdr->proto);
		this->m_treeCtrl.InsertItem(strbuf, ip);
		strbuf.Format(_T("ͷ��У���: %d"), ntohs(iphdr->crc));
		this->m_treeCtrl.InsertItem(strbuf, ip);

		strbuf.Format(_T("ԴIP: "));
		struct in_addr in;
		in.S_un.S_addr =*((u_long*)(void*) &iphdr->saddr);
		strbuf.AppendFormat(CString(inet_ntoa(in)));
		this->m_treeCtrl.InsertItem(strbuf, ip);

		strbuf.Format(_T("Ŀ��IP: "));
		in.S_un.S_addr = *((u_long*)(void*)&iphdr->daddr);
		strbuf.AppendFormat(CString(inet_ntoa(in)));
		this->m_treeCtrl.InsertItem(strbuf, ip);

		/* Transport Layer: ICMP�� UDP �� TCP */
		if (IP_PROTOCOL_ICMP == hdrsPack->iph->proto) {
			HTREEITEM icmp = this->m_treeCtrl.InsertItem(_T("ICMPЭ��ͷ"), head);

			strbuf.Format(_T("����: %d"), hdrsPack->icmph->type);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
			strbuf.Format(_T("����: %d"), hdrsPack->icmph->code);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
			strbuf.Format(_T("���: %d"), hdrsPack->icmph->seq);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
			strbuf.Format(_T("У���: %d"), hdrsPack->icmph->chksum);
			this->m_treeCtrl.InsertItem(strbuf, icmp);
		}
		else if (IP_PROTOCOL_TCP == hdrsPack->iph->proto) {
			HTREEITEM tcp = this->m_treeCtrl.InsertItem(_T("TCPЭ��ͷ"), head);
			strbuf.Format(_T("Դ�˿�: %d"), ntohs(hdrsPack->tcph->sport));
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("Ŀ��˿�: %d"), ntohs(hdrsPack->tcph->dport));
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("���: %u"), hdrsPack->tcph->seq_num);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("ȷ�Ϻ�: %u"), hdrsPack->tcph->ack_num);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("ͷ������: %d"), hdrsPack->tcph->thl);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			
			HTREEITEM flag = this->m_treeCtrl.InsertItem(_T("��־λ"), tcp);
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

			strbuf.Format(_T("	����ָ��: 0x%hx"), hdrsPack->tcph->urg_ptr);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
			strbuf.Format(_T("	У���: %hd"), hdrsPack->tcph->checksum);
			this->m_treeCtrl.InsertItem(strbuf, tcp);
		}
		else if (IP_PROTOCOL_UDP == hdrsPack->iph->proto) {
			HTREEITEM udp = this->m_treeCtrl.InsertItem(_T("UDPЭ��ͷ"), head);
			strbuf.Format(_T("Դ�˿�: %d"), hdrsPack->udph->sport);
			this->m_treeCtrl.InsertItem(strbuf, udp);
			strbuf.Format(_T("Ŀ��˿�: %d"), hdrsPack->udph->dport);
			this->m_treeCtrl.InsertItem(strbuf, udp);
			strbuf.Format(_T("�ܳ���: %d"), hdrsPack->udph->len);
			this->m_treeCtrl.InsertItem(strbuf, udp);

			strbuf.Format(_T("У���: %d"), hdrsPack->udph->crc);
			this->m_treeCtrl.InsertItem(strbuf, udp);
		}
	}
	else if (ETH_PROTOCOL_IPV6 == proto) {
		cout << "IPv6 head parse not supported" << endl;
	}
	else {
		cout << "��δʵ�ֵ�Ethernet Protocol���� " << hdrsPack->ethh->proto << endl;
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

// ������ť
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
		MessageBox(_T("�߳���æ���޷�ֹͣ�����Ժ�����"), _T("����"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index;
	index = this->m_listCtrl.GetHotItem();
	// ��ȡ�������һ���Ӧ�����ݣ�pktCount, headerPack)
	pair<pktCount, headerPack> data = m_snifferCore.data_parser.getAt(index);
	this->updateTree(index, &data.first, &data.second);

	this->updateEdit(&data.second);

	/*this->lixsniff_updateEdit(index);
	this->lixsniff_updateTree(index);*/
	// TODO END
	*pResult = 0;
}

// ���水ť
void CmysniffDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fdlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All files(*.dmp)");
	fdlg.m_ofn.lpstrTitle = L"Save File";
	fdlg.m_ofn.lpstrInitialDir = L"D:\\";
	if (fdlg.DoModal() == IDOK)
	{
		CString des_path = fdlg.GetPathName() + ".dmp";
		CString src_path = CString(this->m_snifferCore.getDefaltDumpFilePath());
		// copy dump  file to user specified path
		if (CopyFile(src_path, des_path, FALSE)) {
			MessageBox(_T("����ɹ�"), _T("��ʾ"));
		}
		else {
			MessageBox(_T("����ʧ�ܣ�������"), _T("��ʾ"));
		};
	}
}

void CmysniffDlg::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pNMCD = (LPNMLVCUSTOMDRAW)pNMHDR;
	*pResult = 0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if (CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->nmcd.dwDrawStage) {
		COLORREF crText;
		string buf;
		auto pos = pNMCD->nmcd.dwItemSpec;
		// ȡ��������¼�е�Э��ͷ���ݣ�
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
	return string(CT2A(str)) == "������˹���(Ĭ�ϲ�����)" ? " " : string(CT2A(str));
}

void CmysniffDlg::OnEnChangeEdit12()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CmysniffDlg::OnClickedButtonHelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString mhtPath = _T("WinPcap Filter Syntax.html");		//���������Ҫ�鿴���ļ�·��������ַ��http://**������exe�ļ�����д��test.exe����Ĭ�ϵ�ǰĿ¼��
 	ShellExecute(NULL, _T("open"), mhtPath, NULL, NULL, SW_SHOWNORMAL);	//��4�������ɴ��������в�������5��������ָ���ļ�Ŀ¼����6��������ΪSW_HIDE����ʾ����SW_SHOW��ʾ
}
