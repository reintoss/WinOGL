﻿
// WinOGLView.h : CWinOGLView クラスのインターフェイス
//

#pragma once
#include <gl/GL.h>
#include "AdminControl.h"
#include "pch.h"


class CWinOGLView : public CView
{
protected: // シリアル化からのみ作成します。
	CWinOGLView() noexcept;
	DECLARE_DYNCREATE(CWinOGLView)

// 属性
public:
	CWinOGLDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CWinOGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	HGLRC m_hRC;
	CAdminControl AC; //問8.1
	CVertex V;
	CShape S;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:

	//左ボタンをクリックしたときのx,y座標
	float clickX_L = 0;
	float clickY_L = 0;

	float clickX = 0;
	float clickY = 0;

	//マウスが移動中のときのx,y座標
	float clickX_M = 0;
	float clickY_M = 0;

	bool LButtonDownFlag = false;

public:
	afx_msg void OnXyz();
	afx_msg void OnUpdateXyz(CCmdUI* pCmdUI);
	afx_msg void OnEditSelect();
	afx_msg void OnUpdateEditSelect(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnAllDelete();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSquare();
	afx_msg void OnUpdateSquare(CCmdUI* pCmdUI);
	afx_msg void OnStraight();
	afx_msg void OnUpdateStraight(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument() const
   { return reinterpret_cast<CWinOGLDoc*>(m_pDocument); }
#endif

