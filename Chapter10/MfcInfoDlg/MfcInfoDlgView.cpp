
// MfcInfoDlgView.cpp : implementation of the CMfcInfoDlgView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MfcInfoDlg.h"
#endif

#include "MfcInfoDlgDoc.h"
#include "MfcInfoDlgView.h"
#include "CInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMfcInfoDlgView

IMPLEMENT_DYNCREATE(CMfcInfoDlgView, CView)

BEGIN_MESSAGE_MAP(CMfcInfoDlgView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMfcInfoDlgView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMfcInfoDlgView construction/destruction

CMfcInfoDlgView::CMfcInfoDlgView() noexcept
{
	// TODO: add construction code here

}

CMfcInfoDlgView::~CMfcInfoDlgView()
{
}

BOOL CMfcInfoDlgView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMfcInfoDlgView drawing

void CMfcInfoDlgView::OnDraw(CDC* pDC)
{
	CMfcInfoDlgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	pDC->TextOutA(pDoc->m_x, pDoc->m_y, pDoc->m_str);
}


// CMfcInfoDlgView printing


void CMfcInfoDlgView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMfcInfoDlgView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMfcInfoDlgView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMfcInfoDlgView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMfcInfoDlgView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMfcInfoDlgView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMfcInfoDlgView diagnostics

#ifdef _DEBUG
void CMfcInfoDlgView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcInfoDlgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcInfoDlgDoc* CMfcInfoDlgView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcInfoDlgDoc)));
	return (CMfcInfoDlgDoc*)m_pDocument;
}
#endif //_DEBUG


// CMfcInfoDlgView message handlers


void CMfcInfoDlgView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMfcInfoDlgDoc* pDoc = GetDocument();
	CInfoDlg dlg;
	dlg.m_x = pDoc->m_x;
	dlg.m_y = pDoc->m_y;
	dlg.m_str = pDoc->m_str;
	if (dlg.DoModal() == IDOK)
	{
		pDoc->m_x = dlg.m_x;
		pDoc->m_y = dlg.m_y;
		pDoc->m_str = dlg.m_str;
		Invalidate();
	}

	CView::OnLButtonDown(nFlags, point);
}
