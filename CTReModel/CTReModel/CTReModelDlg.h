
// CTReModelDlg.h : 头文件
//

#pragma once


// CCTReModelDlg 对话框
class CCTReModelDlg : public CDialogEx
{
// 构造
public:
	CCTReModelDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CTREMODEL_DIALOG };

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
