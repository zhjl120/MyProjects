
// llkDlg.h : ͷ�ļ�
//

#pragma once


// CllkDlg �Ի���
class CllkDlg : public CDialogEx
{
// ����
public:
	CllkDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LLK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
