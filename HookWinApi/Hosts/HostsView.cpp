
// HostsView.cpp : CHostsView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHostsView ����/����

CHostsView::CHostsView()
{
	// TODO: �ڴ˴���ӹ������

}

CHostsView::~CHostsView()
{
}

BOOL CHostsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CHostsView ����

void CHostsView::OnDraw(CDC* /*pDC*/)
{
	CHostsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CHostsView ��ӡ

BOOL CHostsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CHostsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CHostsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CHostsView ���

#ifdef _DEBUG
void CHostsView::AssertValid() const
{
	CView::AssertValid();
}

void CHostsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHostsDoc* CHostsView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHostsDoc)));
	return (CHostsDoc*)m_pDocument;
}
#endif //_DEBUG


// CHostsView ��Ϣ�������
