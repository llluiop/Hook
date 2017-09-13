
// HostView.cpp : CHostView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHostView ����/����

CHostView::CHostView()
{
	// TODO: �ڴ˴���ӹ������

}

CHostView::~CHostView()
{
}

BOOL CHostView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CHostView ����

void CHostView::OnDraw(CDC* /*pDC*/)
{
	CHostDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CHostView ��ӡ

BOOL CHostView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CHostView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CHostView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CHostView ���

#ifdef _DEBUG
void CHostView::AssertValid() const
{
	CView::AssertValid();
}

void CHostView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHostDoc* CHostView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHostDoc)));
	return (CHostDoc*)m_pDocument;
}
#endif //_DEBUG


// CHostView ��Ϣ�������
