
// CTReModelDlg.h : ͷ�ļ�
//

#pragma once


// CCTReModelDlg �Ի���
class CCTReModelDlg : public CDialogEx
{
// ����
public:
	CCTReModelDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CTREMODEL_DIALOG };

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
	CString m_input_dir;
	CString m_output_dir;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnBnClickedButtonDone();

	char szSelected[80];
	int m_fileName_flag;
	char m_final_fileName[80];
	CString m_handle_tip;
	char m_file_name[80];
	CString m_fileName;

	afx_msg void OnBnClickedRadioSkin();
	int m_choose;
	afx_msg void OnBnClickedRadioBone();
	int m_handle_flag;
	int m_handle_flag2;
	int m_handle_flag3;
	afx_msg void OnBnClickedButtonDebug();
};
