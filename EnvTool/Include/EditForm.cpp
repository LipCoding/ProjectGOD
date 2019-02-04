// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "EditForm.h"


// EditForm

IMPLEMENT_DYNCREATE(EditForm, CFormView)

EditForm::EditForm()
	: CFormView(IDD_DIALOG_FORM)
{

}

EditForm::~EditForm()
{
}

BEGIN_MESSAGE_MAP(EditForm, CView)
	ON_BN_CLICKED(IDC_BUTTON1, &EditForm::OnBnClickedButton1)
END_MESSAGE_MAP()

// EditForm 진단

#ifdef _DEBUG
void EditForm::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void EditForm::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// EditForm 메시지 처리기

void EditForm::DoDataExchange(CDataExchange * pDX)
{
	CFormView::DoDataExchange(pDX);
}


void EditForm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
