
// MfcKeyView.h : interface of the CMfcKeyView class
//

#pragma once


class CMfcKeyView : public CView
{
protected: // create from serialization only
	CMfcKeyView() noexcept;
	DECLARE_DYNCREATE(CMfcKeyView)

// Attributes
public:
	CMfcKeyDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMfcKeyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in MfcKeyView.cpp
inline CMfcKeyDoc* CMfcKeyView::GetDocument() const
   { return reinterpret_cast<CMfcKeyDoc*>(m_pDocument); }
#endif

