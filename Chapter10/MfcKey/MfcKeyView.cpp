
// MfcKeyView.cpp : implementation of the CMfcKeyView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MfcKey.h"
#endif

#include "MfcKeyDoc.h"
#include "MfcKeyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMfcKeyView

IMPLEMENT_DYNCREATE(CMfcKeyView, CView)

BEGIN_MESSAGE_MAP(CMfcKeyView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMfcKeyView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CMfcKeyView construction/destruction

CMfcKeyView::CMfcKeyView() noexcept
{
	// TODO: add construction code here

}

CMfcKeyView::~CMfcKeyView()
{
}

BOOL CMfcKeyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMfcKeyView drawing

void CMfcKeyView::OnDraw(CDC* pDC)
{
	CMfcKeyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	pDC->TextOut(100, 100, pDoc->str);
}


// CMfcKeyView printing


void CMfcKeyView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMfcKeyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMfcKeyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMfcKeyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMfcKeyView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMfcKeyView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMfcKeyView diagnostics

#ifdef _DEBUG
void CMfcKeyView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcKeyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcKeyDoc* CMfcKeyView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcKeyDoc)));
	return (CMfcKeyDoc*)m_pDocument;
}
#endif //_DEBUG


// CMfcKeyView message handlers


void CMfcKeyView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CMfcKeyDoc* pDoc = GetDocument();
	pDoc->str += (TCHAR)nChar;
	Invalidate();

	CView::OnChar(nChar, nRepCnt, nFlags);
}
