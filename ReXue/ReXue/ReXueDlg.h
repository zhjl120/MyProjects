
// ReXueDlg.h : ͷ�ļ�
//

#pragma once
#include "DexterModule.h"


// CReXueDlg �Ի���
class CReXueDlg : public CDialogEx
{
// ����
public:
	CReXueDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REXUE_DIALOG };

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
	int m_nRedValue;
	int m_nBlueValue;
	int m_nAngerValue;
	float m_fExperienceValue;
	afx_msg void OnGetProperty();
};
