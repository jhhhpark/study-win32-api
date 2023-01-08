
// MFCExamView.cpp : implementation of the CMFCExamView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCExam.h"
#endif

#include "MFCExamDoc.h"
#include "MFCExamView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCExamView

IMPLEMENT_DYNCREATE(CMFCExamView, CView)

BEGIN_MESSAGE_MAP(CMFCExamView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCExamView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCExamView construction/destruction

CMFCExamView::CMFCExamView() noexcept
{
	// TODO: add construction code here

}

CMFCExamView::~CMFCExamView()
{
}

BOOL CMFCExamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCExamView drawing

void CMFCExamView::OnDraw(CDC* pDC)
{
	CMFCExamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code for native data here
	pDC->TextOut(10, 10, TEXT("MFC로 만든 예제입니다"));
}


// CMFCExamView printing


void CMFCExamView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCExamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCExamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCExamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCExamView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCExamView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCExamView diagnostics

#ifdef _DEBUG
void CMFCExamView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCExamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCExamDoc* CMFCExamView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCExamDoc)));
	return (CMFCExamDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCExamView message handlers
