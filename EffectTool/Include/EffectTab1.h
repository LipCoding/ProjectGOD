#pragma once

#include "GameObject/GameObject.h"
#include "Component/Effect.h"

PG_USING

// CEffectTab1 대화 상자

class CEffectTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTab1)

public:
	CEffectTab1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEffectTab1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/* Getter, Setter */
	void SetTargetObject(CGameObject *target)
	{
		m_pTargetObject = target;
		m_bFirstTargetCheck = true;
	}
private:
	/* Fade In */
	float m_fFadeInStaticStartTime;
	float m_fFadeInStaticEndTime;
	float m_fFadeInStaticDegree;

	float m_fFadeInStaticTime;

	CEdit m_editFadeInStartTime;
	CEdit m_editFadeInEndTime;
	CEdit m_editFadeInDegree;

	/* Fade Out */
	float m_fFadeOutStaticStartTime;
	float m_fFadeOutStaticEndTime;
	float m_fFadeOutStaticDegree;

	float m_fFadeOutStaticTime;

	CEdit m_editFadeOutStartTime;
	CEdit m_editFadeOutEndTime;
	CEdit m_editFadeOutDegree;

	/* UV Sprite */
	float m_fUVSpriteStaticStartTime;
	float m_fUVSpriteStaticEndTime;
	int   m_iUVSpriteStaticNum;

	float m_fUVSpriteTime;

	CEdit m_editUVSpriteStartTime;
	CEdit m_editUVSpriteEndTime;
	CEdit m_editUVSpriteNum;
	
	/* Check */
	CButton m_checkFadeIn;
	CButton m_checkFadeOut;
	CButton m_checkUVSprite;

public:
	void UpdateForm();

private:
	/* */
	void InitForm();

	/* Total */
	void InitFormValue();
	/* Fade */
	void InitFadeIn();
	void InitFadeOut();
	/* UV */
	void InitUVSprite();

	void UpdateFade();
	void UpdateUV();
	void UpdateTime();

private:
	CGameObject *m_pTargetObject = nullptr;
	bool m_bFirstTargetCheck = true;

private:
	void AddFadeIn(class CEffect *pEffect);
	void AddFadeOut(class CEffect *pEffect);
	void AddUVSprite(class CEffect *pEffect);
public:
	afx_msg void OnBnClickedButtonFadeInInput();
	afx_msg void OnBnClickedButtonFadeInPlay();
	afx_msg void OnBnClickedButtonFadeInStop();
	afx_msg void OnBnClickedButtonFadeOutInput();
	afx_msg void OnBnClickedButtonFadeOutPlay();
	afx_msg void OnBnClickedButtonFadeOutStop();
	afx_msg void OnBnClickedCheckFadeIn();
	afx_msg void OnBnClickedCheckFadeOut();

	afx_msg void OnBnClickedButtonUvSpriteInput();
	afx_msg void OnBnClickedButtonUvSpritePlay();
	afx_msg void OnBnClickedButtonUvSpriteStop();
	afx_msg void OnBnClickedCheckUvSprite();
};
