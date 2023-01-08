
// MfcInfoDlgDoc.h : interface of the CMfcInfoDlgDoc class
//


#pragma once


class CMfcInfoDlgDoc : public CDocument
{
protected: // create from serialization only
	CMfcInfoDlgDoc() noexcept;
	DECLARE_DYNCREATE(CMfcInfoDlgDoc)

// Attributes
public:
	CString m_str;
	int m_y;
	int m_x;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMfcInfoDlgDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
