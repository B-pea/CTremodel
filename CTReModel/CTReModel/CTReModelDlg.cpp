
// CTReModelDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCTReModelDlg �Ի���




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


// CCTReModelDlg ��Ϣ�������

BOOL CCTReModelDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	char m_file_name[80] = "\\myPoint"; 
	m_fileName = m_file_name;
	SetDlgItemText(IDC_EDIT_NAME,m_fileName);

	m_handle_tip = "��ӭʹ��CT�����ؽ�ϵͳ��";
	SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);

	m_input_dir = "D:\\CTdata";
	SetDlgItemText(IDC_EDIT_INPUT,m_input_dir);
	m_output_dir = "C:\\Users\\meng-PC\\Desktop";
	SetDlgItemText(IDC_EDIT_OUTPUT,m_output_dir);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCTReModelDlg::OnPaint()
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCTReModelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCTReModelDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//char szSelected[MAX_PATH];
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = "ѡ�������ļ�·��";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//char szSelected[MAX_PATH];

	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = "ѡ������ļ�·��";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_handle_flag == 0)
	{
		m_handle_tip = "�����������ļ�·����";
		SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);
		return;
	}
	if (m_handle_flag2 == 0)
	{
		m_handle_tip = "����������ļ����Ƽ�·�������ٴ����ɣ������ظ����ƣ�";
		SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);
		return;
	}
	if (m_handle_flag3== 0)
	{
		m_handle_tip = "��ѡ��ģĿ�꣡";
		SetDlgItemText(IDC_STATIC_TIP,m_handle_tip);
		return;
	}
	UpdateData(TRUE); 
	strcat(szSelected,m_fileName);
	strcat(szSelected,".g");


	m_handle_tip = "������...";
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
	skinExtractor->SetValue(0,m_choose);   //ѡ��Ƥ�����ǹ�����

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

	SetDlgItemText(IDC_STATIC_TIP,"������ɣ��ٴ�����������ļ�����·����");
	
	m_handle_flag = 0;
	m_handle_flag2 = 0;
	////////////////////////////////////////////��ʾ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_choose = -500;
	m_handle_flag3 =1;
}


void CCTReModelDlg::OnBnClickedRadioBone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_choose = 500;
	m_handle_flag3 =1;
}


void CCTReModelDlg::OnBnClickedButtonDebug()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_handle_flag = 1;
	m_handle_flag2 = 1;
}
