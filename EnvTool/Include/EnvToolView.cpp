
// EnvToolView.cpp: CEnvToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EnvTool.h"
#endif

#include "EnvToolDoc.h"
#include "EnvToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEnvToolView

IMPLEMENT_DYNCREATE(CEnvToolView, CView)

BEGIN_MESSAGE_MAP(CEnvToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CEnvToolView 생성/소멸

CEnvToolView::CEnvToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEnvToolView::~CEnvToolView()
{
}

BOOL CEnvToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEnvToolView 그리기

void CEnvToolView::OnDraw(CDC* /*pDC*/)
{
	CEnvToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEnvToolView 인쇄

BOOL CEnvToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEnvToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEnvToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEnvToolView 진단

#ifdef _DEBUG
void CEnvToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEnvToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEnvToolDoc* CEnvToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEnvToolDoc)));
	return (CEnvToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEnvToolView 메시지 처리기
