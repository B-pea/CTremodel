
// CTReModelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CTReModel.h"
#include "CTReModelDlg.h"
#include "afxdialogex.h"

#include "vtkProperty.h"   
#include "vtkPolyDataNormals.h"   
#include "vtkContourFilter.h"   
#include "vtkMarchingCubes.h"   
#include "vtkDICOMImageReader.h"   
#include "vtkImageCast.h"   
#include "vtkDecimatePro.h"   
#include "vtkStripper.h"   
#include "vtkImageShrink3D.h"   
#include "vtkSmoothPolyDataFilter.h"    
#include "vtkTriangleFilter.h"   
#include "vtkImageData.h"   

#include "vtkPolyDataWriter.h"   
#include <vtkBYUWriter.h>    
#include "vtkXMLPolyDataWriter.h"   
#include "vtkDataSetWriter.h"   
#include <vtkMCubesWriter.h>    
#include "vtkSTLWriter.h"   
#include <vtkCGMWriter.h>   

#include "stdafx.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDICOMImageReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"

#include "vtkPolyDataReader.h"
#include "vtkSTLWriter.h"
#include "vtkTriangleFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCTReModelDlg 对话框




CCTReModelDlg::CCTReModelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCTReModelDlg::IDD, pParent)
	, m_input_dir(_T(""))
	, m_output_dir(_T(""))
	
	, m_fileName_flag(0)
	
	, m_handle_tip(_T(""))
	
	, m_fileName(_T(""))
	, m_choose(0)
	, m_handle_flag(0)
	, m_handle_flag2(0)
	, m_handle_flag3(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTReModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_input_dir);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_output_dir);
	DDX_Text(pDX, IDC_STATIC_TIP, m_handle_tip);
	DDX_Text(pDX, IDC_EDIT_NAME, m_fileName);
}

BEGIN_MESSAGE_MAP(CCTReModelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCTReModelDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CCTReModelDlg::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CCTReModelDlg::OnBnClickedButtonDone)

	ON_BN_CLICKED(IDC_RADIO_SKIN, &CCTReModelDlg::OnBnClickedRadioSkin)
	ON_BN_CLICKED(IDC_RADIO_BONE, &CCTReModelDlg::OnBnClickedRadioBone)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, &CCTReModelDlg::OnBnClickedButtonDebug)
END_MESSAGE_MAP()


// CCTReModelDlg 消息处理程序

BOOL CCTReModelDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	char m_file_name[80] = "\\myPoint"; 
	m_fileName = m_file_name;
	SetDlgItemText(IDC_EDIT_NAME,m_fileName);

	m_handle_tip = "欢迎使用CT程序重建系统！";
	SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);

	m_input_dir = "D:\\CTdata";
	SetDlgItemText(IDC_EDIT_INPUT,m_input_dir);
	m_output_dir = "C:\\Users\\meng-PC\\Desktop";
	SetDlgItemText(IDC_EDIT_OUTPUT,m_output_dir);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCTReModelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCTReModelDlg::OnPaint()
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
		/*CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);
		BITMAP bitmap;  
		bmpBackground.GetBitmap(&bitmap);  
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);  
		dc.StretchBlt(0,0,rect.Width(), rect.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY); */ 

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCTReModelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCTReModelDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//char szSelected[MAX_PATH];
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = "选择输入文件路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;

	if ((pidl = SHBrowseForFolder(&bi))!= NULL)
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl,szSelected)))
		{
			m_input_dir = szSelected;
			m_handle_flag = 1;
		}
	}
	SetDlgItemText(IDC_EDIT_INPUT,m_input_dir);

	strcat(szSelected,m_file_name);
}


void CCTReModelDlg::OnBnClickedButtonOutput()
{
	// TODO: 在此添加控件通知处理程序代码
	//char szSelected[MAX_PATH];

	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = "选择输出文件路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;

	if ((pidl = SHBrowseForFolder(&bi))!= NULL)
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl,szSelected)))
		{
			m_output_dir = szSelected;
			m_handle_flag2 = 1;
		}
	}

	SetDlgItemText(IDC_EDIT_OUTPUT,m_output_dir);
	
	
}


void CCTReModelDlg::OnBnClickedButtonDone()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_handle_flag == 0)
	{
		m_handle_tip = "请设置输入文件路径！";
		SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);
		return;
	}
	if (m_handle_flag2 == 0)
	{
		m_handle_tip = "请设置输出文件名称及路径！如再次生成，请勿重复名称！";
		SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);
		return;
	}
	if (m_handle_flag3== 0)
	{
		m_handle_tip = "请选择建模目标！";
		SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);
		return;
	}
	UpdateData(TRUE); 
	strcat(szSelected,m_fileName);
	strcat(szSelected,".g");


	m_handle_tip = "处理中...";
	SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);

	vtkDICOMImageReader   *reader =  vtkDICOMImageReader::New();   
	reader->SetDataByteOrderToLittleEndian();   
	reader->SetDirectoryName(m_input_dir);   
	reader->Update();   

	vtkImageShrink3D *shrink=vtkImageShrink3D::New();   
	shrink->SetInput((vtkDataObject *)reader->GetOutput());   
	shrink->SetShrinkFactors(4,4,1);   
	shrink->AveragingOn();   

	vtkContourFilter *skinExtractor = vtkContourFilter::New();   
	skinExtractor->SetInputConnection(shrink->GetOutputPort());   
	skinExtractor->SetValue(0,m_choose);   //选择皮肤还是骨骼？

	vtkDecimatePro *deci=vtkDecimatePro::New();   
	deci->SetInputConnection(skinExtractor->GetOutputPort());   
	deci->SetTargetReduction(0.3);   

	vtkSmoothPolyDataFilter *smooth=vtkSmoothPolyDataFilter::New();    
	smooth->SetInputConnection(deci->GetOutputPort());   
	smooth->SetNumberOfIterations(200)  ;   

	vtkPolyDataNormals *skinNormals = vtkPolyDataNormals::New();   
	skinNormals->SetInputConnection(smooth->GetOutputPort());   
	skinNormals->SetFeatureAngle(60.0);   

	vtkStripper *stripper=vtkStripper::New();    
	stripper->SetInput(skinNormals->GetOutput()); 

	vtkBYUWriter *byu=vtkBYUWriter::New();   
	byu-> SetGeometryFileName(szSelected);   
//	byu-> SetScalarFileName("E:/CT/BYUWriter.s");   
//	byu-> SetDisplacementFileName ("E://CT/BYUWriter.d");   
	byu-> SetInputConnection(stripper ->GetOutputPort());   
	byu-> Write();  

	SetDlgItemText(IDC_STATIC_TIP,"处理完成！再次生成请更改文件名或路径！");
	
	m_handle_flag = 0;
	m_handle_flag2 = 0;
	////////////////////////////////////////////显示
	vtkRenderer *aRenderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(aRenderer);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkPolyDataMapper *skinMapper = vtkPolyDataMapper::New();
	skinMapper->SetInputConnection(skinNormals->GetOutputPort());
	skinMapper->ScalarVisibilityOff();
	vtkActor *skin = vtkActor::New();
	skin->SetMapper(skinMapper);

	vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
	outlineData->SetInputConnection(reader->GetOutputPort());
	vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());
	vtkActor *outline = vtkActor::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0,0,0);

	vtkCamera *aCamera = vtkCamera::New();
	aCamera->SetViewUp (0, 0, -1);
	aCamera->SetPosition (0, 1, 0);
	aCamera->SetFocalPoint (0, 0, 0);
	aCamera->ComputeViewPlaneNormal();

	//aRenderer->AddActor(outline);
	aRenderer->AddActor(skin);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera ();
	aCamera->Dolly(1.5);

	aRenderer->SetBackground(1,1,1);
	renWin->SetSize(640, 480);

	aRenderer->ResetCameraClippingRange ();

	iren->Initialize();
	iren->Start();
}


void CCTReModelDlg::OnBnClickedRadioSkin()
{
	// TODO: 在此添加控件通知处理程序代码
	m_choose = -500;
	m_handle_flag3 =1;
}


void CCTReModelDlg::OnBnClickedRadioBone()
{
	// TODO: 在此添加控件通知处理程序代码
	m_choose = 500;
	m_handle_flag3 =1;
}


void CCTReModelDlg::OnBnClickedButtonDebug()
{
	// TODO: 在此添加控件通知处理程序代码
	m_handle_flag = 1;
	m_handle_flag2 = 1;
}
