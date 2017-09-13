
// HookWinApiView.h : CHookWinApiView ��Ľӿ�
//

#pragma once


class CHookWinApiView : public CView
{
protected: // �������л�����
	CHookWinApiView();
	DECLARE_DYNCREATE(CHookWinApiView)

// ����
public:
	CHookWinApiDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CHookWinApiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HookWinApiView.cpp �еĵ��԰汾
inline CHookWinApiDoc* CHookWinApiView::GetDocument() const
   { return reinterpret_cast<CHookWinApiDoc*>(m_pDocument); }
#endif

