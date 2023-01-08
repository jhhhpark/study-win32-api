// CInfoDlg.cpp : implementation file
//

#include "pch.h"
#include "MfcInfoDlg.h"
#include "CInfoDlg.h"
#include "afxdialogex.h"


// CInfoDlg dialog

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CInfoDlg, pParent)
	, m_str(_T(""))
	, m_x(0)
	, m_y(0)
{
}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STR, m_str);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CInfoDlg message handlers
