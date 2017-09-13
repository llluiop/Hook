
// HostView.cpp : CHostView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Host.h"
#endif

#include "HostDoc.h"
#include "HostView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHostView

IMPLEMENT_DYNCREATE(CHostView, CView)

BEGIN_MESSAGE_MAP(CHostView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHostView 构造/析构

CHostView::CHostView()
{
	// TODO: 在此处添加构造代码

}

CHostView::~CHostView()
{
}

BOOL CHostView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHostView 绘制

void CHostView::OnDraw(CDC* /*pDC*/)
{
	CHostDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CHostView 打印

BOOL CHostView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHostView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHostView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHostView 诊断

#ifdef _DEBUG
void CHostView::AssertValid() const
{
	CView::AssertValid();
}

void CHostView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHostDoc* CHostView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHostDoc)));
	return (CHostDoc*)m_pDocument;
}
#endif //_DEBUG


// CHostView 消息处理程序
