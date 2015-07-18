
// llkDlg.h : 头文件
//

#pragma once


// CllkDlg 对话框
class CllkDlg : public CDialogEx
{
// 构造
public:
	CllkDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LLK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartgame();
	int m_PointX;
	int m_PointY;
	int m_nSeatNum;
	afx_msg void OnBnClickedSeatnum();
	afx_msg void OnBnClickedReadchessdata();
};
