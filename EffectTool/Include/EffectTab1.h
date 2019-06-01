#pragma once

#include "GameObject/GameObject.h"
#include "Component/Effect.h"

PG_USING

// CEffectTab1 대화 상자

class CEffectTab1 : public CDialogEx
{
public:
	enum SpriteType
	{
		SPRITE_TYPE_FRAME,
		SPRITE_TYPE_ATLAS,
		SPRITE_TYPE_END
	};

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
	
	int m_iUVSpriteStaticWidth;
	int m_iUVSpriteStaticHeight;
	int m_iUVSpriteStaticMax_X;
	int m_iUVSpriteStaticMax_Y;

	CEdit m_editUVSpriteWidth;
	CEdit m_editUVSpriteHeight;
	CEdit m_editUVSpriteMax_X;
	CEdit m_editUVSpriteMax_Y;

	/* UV Move */
	float m_fMoveUVStaticStartTime;
	float m_fMoveUVStaticEndTime;
	float m_fMoveUVStaticDirX;
	float m_fMoveUVStaticDirY;

	float m_fMoveUVStaticTime;

	CEdit m_editMoveUVStartTime;
	CEdit m_editMoveUVEndTime;
	CEdit m_editMoveUVDirX;
	CEdit m_editMoveUVDirY;


	/* Check */
	CButton m_checkFadeIn;
	CButton m_checkFadeOut;
	CButton m_checkUVSprite;
	CButton m_checkUVMove;

	CButton m_checkInfiniteFadeIn;
	CButton m_checkInfiniteFadeOut;
	CButton m_checkInfiniteUVMove;
	CButton m_checkInfiniteUVSprite;

	int m_iRadioSpriteType;

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
	void InitUVMove();

	void UpdateFade();
	void UpdateUV();
	void UpdateTime();

	/**/

private:
	CGameObject *m_pTargetObject = nullptr;
	bool m_bFirstTargetCheck = true;

private:
	void AddFadeIn(class CEffect *pEffect);
	void AddFadeOut(class CEffect *pEffect);
	bool AddUVSprite(class CEffect *pEffect);
	bool AddUVMove(class CEffect *pEffect);

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
	afx_msg void OnBnClickedButtonUvMoveInput();
	afx_msg void OnBnClickedButtonUvMovePlay();
	afx_msg void OnBnClickedButtonUvMoveStop();
	afx_msg void OnBnClickedCheckUv();
	afx_msg void OnBnClickedCheckInfiniteFadeIn();
	afx_msg void OnBnClickedCheckInfiniteFadeOut();
	afx_msg void OnBnClickedCheckInfiniteUvMove();

	afx_msg void OnBnClickedRadioSpriteType1();
	afx_msg void OnBnClickedRadioSpriteType2();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheckInfiniteUvSprite();
};
