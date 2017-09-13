
// HostView.h : CHostView ��Ľӿ�
//

#pragma once


class CHostView : public CView
{
protected: // �������л�����
	CHostView();
	DECLARE_DYNCREATE(CHostView)

// ����
public:
	CHostDoc* GetDocument() const;

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
	virtual ~CHostView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HostView.cpp �еĵ��԰汾
inline CHostDoc* CHostView::GetDocument() const
   { return reinterpret_cast<CHostDoc*>(m_pDocument); }
#endif

