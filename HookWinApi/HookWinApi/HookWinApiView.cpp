
// HookWinApiView.cpp : CHookWinApiView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "HookWinApi.h"
#endif

#include "HookWinApiDoc.h"
#include "HookWinApiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookWinApiView

IMPLEMENT_DYNCREATE(CHookWinApiView, CView)

BEGIN_MESSAGE_MAP(CHookWinApiView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHookWinApiView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CHookWinApiView ����/����

CHookWinApiView::CHookWinApiView()
{
	// TODO: �ڴ˴���ӹ������

}

CHookWinApiView::~CHookWinApiView()
{
}

BOOL CHookWinApiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CHookWinApiView ����

void CHookWinApiView::OnDraw(CDC* /*pDC*/)
{
	CHookWinApiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CHookWinApiView ��ӡ


void CHookWinApiView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHookWinApiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CHookWinApiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CHookWinApiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CHookWinApiView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHookWinApiView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHookWinApiView ���

#ifdef _DEBUG
void CHookWinApiView::AssertValid() const
{
	CView::AssertValid();
}

void CHookWinApiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHookWinApiDoc* CHookWinApiView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHookWinApiDoc)));
	return (CHookWinApiDoc*)m_pDocument;
}
#endif //_DEBUG


// CHookWinApiView ��Ϣ�������
