
// mysniffDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "snifferCore.h"
#include "functional"
#include "string"
#include "myCEdit.h"

using namespace std;

// 前置声明，解决snifferCore.h 和 mysniffDlg.h 出现的相互包含的循环依赖问题
class CmysniffDlg;

// CmysniffDlg 对话框
class CmysniffDlg : public CDialogEx
{


public:
	CmysniffDlg(CWnd* pParent = NULL);	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSNIFF_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP();

// 控件事件处理函数
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	// 控件变量相关
	CEdit m_editTCP;
	CEdit m_editUDP;
	CEdit m_editICMP;
	CEdit m_editHTTP;
	CEdit m_editARP;
	CEdit m_editICMPv6;
	CEdit m_editIPv4;
	CEdit m_editIPv6;
	CEdit m_editOther;
	CEdit m_editSum;

	CListCtrl m_listCtrl;  // 中间的框
	CTreeCtrl m_treeCtrl;  // 左侧的较大的框
	CEdit m_edit;          // 右侧较大的框

	// 按钮 
	CButton m_buttonStart;
	CButton m_buttonStop;
	CButton m_buttonSave;
	CButton m_buttonRead;
	
	// 设备列表下拉框 和 过滤规则下下拉框
	CComboBox m_comboBox;
//	CComboBox m_comboBoxRule;

// 抓包分析类（Core)
	SnifferCore m_snifferCore;


// 界面数据更新相关
public:
	void UpdateGui(const pktCount* npkt, const datapkt* hdrspack);

private:
	void update_listCtrl(const pktCount* npkt, const datapkt* hdrsPack);
	void updateNPacket(const pktCount* npkt);
	void updateTree(int index, const pktCount* npkt, const datapkt* hdrsPack);
	void updateEdit(datapkt* hdrsPack);

public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	string getFilterRule();
// 	CMyEdit m_cmyEdit;
	CMyEdit m_CMyEdit;
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnClickedButtonHelp();
};
