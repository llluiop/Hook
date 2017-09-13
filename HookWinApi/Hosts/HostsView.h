
// HostsView.h : CHostsView ��Ľӿ�
//

#pragma once


class CHostsView : public CView
{
protected: // �������л�����
	CHostsView();
	DECLARE_DYNCREATE(CHostsView)

// ����
public:
	CHostsDoc* GetDocument() const;

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
	virtual ~CHostsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HostsView.cpp �еĵ��԰汾
inline CHostsDoc* CHostsView::GetDocument() const
   { return reinterpret_cast<CHostsDoc*>(m_pDocument); }
#endif

