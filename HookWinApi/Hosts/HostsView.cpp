
// HostsView.cpp : CHostsView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Hosts.h"
#endif

#include "HostsDoc.h"
#include "HostsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHostsView

IMPLEMENT_DYNCREATE(CHostsView, CView)

BEGIN_MESSAGE_MAP(CHostsView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHostsView 构造/析构

CHostsView::CHostsView()
{
	// TODO: 在此处添加构造代码

}

CHostsView::~CHostsView()
{
}

BOOL CHostsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHostsView 绘制

void CHostsView::OnDraw(CDC* /*pDC*/)
{
	CHostsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CHostsView 打印

BOOL CHostsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHostsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHostsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHostsView 诊断

#ifdef _DEBUG
void CHostsView::AssertValid() const
{
	CView::AssertValid();
}

void CHostsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHostsDoc* CHostsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHostsDoc)));
	return (CHostsDoc*)m_pDocument;
}
#endif //_DEBUG


// CHostsView 消息处理程序
