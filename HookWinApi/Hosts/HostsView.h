
// HostsView.h : CHostsView 类的接口
//

#pragma once


class CHostsView : public CView
{
protected: // 仅从序列化创建
	CHostsView();
	DECLARE_DYNCREATE(CHostsView)

// 特性
public:
	CHostsDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHostsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HostsView.cpp 中的调试版本
inline CHostsDoc* CHostsView::GetDocument() const
   { return reinterpret_cast<CHostsDoc*>(m_pDocument); }
#endif

