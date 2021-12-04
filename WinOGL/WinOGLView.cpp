
// WinOGLView.cpp : CWinOGLView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinOGLView

IMPLEMENT_DYNCREATE(CWinOGLView, CView)

BEGIN_MESSAGE_MAP(CWinOGLView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_XYZ, &CWinOGLView::OnXyz)
	ON_UPDATE_COMMAND_UI(ID_XYZ, &CWinOGLView::OnUpdateXyz)
	ON_COMMAND(ID_EDIT_SELECT, &CWinOGLView::OnEditSelect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT, &CWinOGLView::OnUpdateEditSelect)
	ON_WM_MOUSEMOVE()
//	ON_WM_RBUTTONDOWN()
//	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_ALL_DELETE, &CWinOGLView::OnAllDelete)
//	ON_UPDATE_COMMAND_UI(ID_ALL_DELETE, &CWinOGLView::OnUpdateAllDelete)
ON_WM_RBUTTONDOWN()
//ON_UPDATE_COMMAND_UI(ID_ALL_DELETE, &CWinOGLView::OnUpdateAllDelete)
ON_COMMAND(ID_SQUARE, &CWinOGLView::OnSquare)
ON_UPDATE_COMMAND_UI(ID_SQUARE, &CWinOGLView::OnUpdateSquare)
//ON_COMMAND(ID_STRAIGHT, &CWinOGLView::OnStraight)
//ON_UPDATE_COMMAND_UI(ID_STRAIGHT, &CWinOGLView::OnUpdateStraight)
ON_COMMAND(ID_STRAIGHT, &CWinOGLView::OnStraight)
ON_UPDATE_COMMAND_UI(ID_STRAIGHT, &CWinOGLView::OnUpdateStraight)
//ON_WM_MOUSEHWHEEL()
ON_WM_MBUTTONDOWN()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	//左ボタンをクリックしたときのx,y座標
	clickX_L = 0;
	clickY_L = 0;

	clickX = 0;
	clickY = 0;

	//マウスが移動中のときのx,y座標
	clickX_M = 0;
	clickY_M = 0;

}

CWinOGLView::~CWinOGLView()
{
}

BOOL CWinOGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CWinOGLView 描画

void CWinOGLView::OnDraw(CDC* pDC)
{
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	wglMakeCurrent(pDC -> m_hDC, m_hRC);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */);
	
	AC.Draw(); //問8.1

	glFlush();
	SwapBuffers(pDC -> m_hDC);

	wglMakeCurrent(pDC -> m_hDC, NULL);

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CWinOGLView の診断

#ifdef _DEBUG
void CWinOGLView::AssertValid() const
{
	CView::AssertValid();
}

void CWinOGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinOGLDoc* CWinOGLView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinOGLDoc)));
	return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinOGLView メッセージ ハンドラーa


void CWinOGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX_L = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX_L = clickX_L * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width()> rect.Height()) { //横長の時
		clickX_L = clickX_L * ((float)rect.Width()/ rect.Height());
	}

	clickY_L = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY_L = clickY_L * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY_L = clickY_L * ((float)rect.Height() / rect.Width());
	}

	LButtonDownFlag = true;

	RedrawWindow();

	CView::OnLButtonDown(nFlags, point);

}

void CWinOGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX = clickX * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		clickX = clickX * ((float)rect.Width() / rect.Height());
	}

	clickY = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY = clickY * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY = clickY * ((float)rect.Height() / rect.Width());
	}

	//編集ボタンが押されていない場合、通常モード
	if (AC.SelectButtonFlag == false) {
		if (AC.SquareButtonFlag == false && AC.StraightButtonFlag==false) {
			AC.CreateShape(clickX, clickY); //問8.2
		}
		else if(AC.SquareButtonFlag == true){
			AC.DrawSquare(clickX_L, clickY_L, clickX, clickY);
		}
		else {
			AC.DrawStraight(clickX, clickY);
		}
	}
	else {
		//マウスが動いていない場合、選択モード
		if (AC.GetVertexMoveNowJudge() == false && AC.GetShapeMoveNowJudge() == false) {
			if (AC.SelectVertex(clickX, clickY) != 1) {
				if (AC.SelectLine(clickX, clickY) != 1) {
					AC.SelectShape(clickX, clickY);
				}
			}
		}//点移動後、交差している場合
		else if (AC.GetVertexMoveNowJudge() == true) {
			if (AC.VMoveCrossJudge() == true) {
				AC.VMoveCancel();
			}
		}//Shape移動後、交差している場合
		else if (AC.GetShapeMoveNowJudge() == true) {
			if (AC.ShapeMoveCrossJudge() == true) {
				AC.ShapeMoveCancel();
			}
		}
	}


	LButtonDownFlag = false;
	AC.ResetVertexMoveNowJudge();
	AC.ResetShapeMoveNowJudge();
	AC.ResetHoldV();
	AC.ResetHoldS();
	AC.ResetAlreadySelectVertexFlag();
	AC.ResetAlreadySelectShapeFlag();

	RedrawWindow();

	CView::OnLButtonUp(nFlags, point);
}


void CWinOGLView::OnMouseMove(UINT nFlags, CPoint point)
{

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX_M = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX_M = clickX_M * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		clickX_M = clickX_M * ((float)rect.Width() / rect.Height());
	}

	clickY_M = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY_M = clickY_M * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY_M = clickY_M * ((float)rect.Height() / rect.Width());
	}

	if (AC.SelectButtonFlag == true) {
		if (LButtonDownFlag == true) {		
			if (AC.DrawMoveShape(clickX_L, clickY_L, clickX_M, clickY_M) == false) {
				AC.DrawMoveVertex(clickX_L, clickY_L, clickX_M, clickY_M);
			}
		}
	}

	RedrawWindow();

	CView::OnMouseMove(nFlags, point);
}


void CWinOGLView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX = clickX * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		clickX = clickX * ((float)rect.Width() / rect.Height());
	}

	clickY = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY = clickY * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY = clickY * ((float)rect.Height() / rect.Width());
	}

	//編集ボタンが押されている場合のみ有効
	if (AC.SelectButtonFlag == true) {
		AC.InsertVertex(clickX, clickY);
	}

	RedrawWindow();

	CView::OnLButtonDblClk(nFlags, point);
}


void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX = clickX * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		clickX = clickX * ((float)rect.Width() / rect.Height());
	}

	clickY = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY = clickY * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY = clickY * ((float)rect.Height() / rect.Width());
	}

	//編集ボタンが押されている場合のみ有効
	if (AC.SelectButtonFlag == true) {
		if (AC.DeleteVertex(clickX, clickY) != 1) {
			if (AC.GetWheelButtonFlag() == false && AC.GetShapeMoveNowJudge() == false) { //図形が赤色ではない場合
				AC.DeleteShape(clickX, clickY);
			}
		}
	}

	RedrawWindow();

	CView::OnRButtonDown(nFlags, point);
}


int CWinOGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		24,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	CClientDC clientDC(this);
	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC, &pfd);
	SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(clientDC.m_hDC);

	return 0;
}


void CWinOGLView::OnDestroy()
{
	CView::OnDestroy();
	AC.Reset_shape_head2();
	wglDeleteContext(m_hRC);

}


BOOL CWinOGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return true;
}


void CWinOGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CClientDC clientDC(this);
	wglMakeCurrent(clientDC.m_hDC, m_hRC);

	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 問6.2 で考える内容
	double hi;

	if (cx > cy) {
		hi = (double)cx / cy;
		glOrtho(-1 * hi, 1 * hi, -1, 1, -100, 100);
	}
	else {
		hi = (double)cy / cx;
		glOrtho(-1, 1, -1 * hi, 1 * hi, -100, 100);
	}


	glMatrixMode(GL_MODELVIEW);

	RedrawWindow();
	wglMakeCurrent(clientDC.m_hDC, NULL);
	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


void CWinOGLView::OnXyz()
{
	
	if (AC.AxisFlag == true) {
		AC.AxisFlag = false;
	}
	else {
		AC.AxisFlag = true;
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateXyz(CCmdUI* pCmdUI)
{
	//AxisFlagがtrueの時、ボタンが沈む
	if (AC.AxisFlag==true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnEditSelect()
{
	if (AC.GetShapeCloseFlag() == true) {
		if (AC.GetNoVertex() == false) {
			if (AC.SelectButtonFlag == true) {
				AC.SelectButtonFlag = false;
				AC.NotSelectFlagReset();
			}
			else {
				AC.SelectButtonFlag = true;
				AC.SquareButtonFlag = false;
				AC.StraightButtonFlag = false;
			}
		}
	}

	RedrawWindow();
}

void CWinOGLView::OnUpdateEditSelect(CCmdUI* pCmdUI)
{
	//SelectButtonFlagがtrueの時、ボタンが沈む
	if (AC.SelectButtonFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}

void CWinOGLView::OnSquare()
{
	if (AC.GetShapeCloseFlag() == true || AC.GetNoVertex() == true) {
		if (AC.SquareButtonFlag == false) {
			if (AC.SelectButtonFlag == true) {
				AC.SelectButtonFlag = false;
				AC.NotSelectFlagReset();
			}
			if (AC.StraightButtonFlag == true) {
				AC.StraightButtonFlag = false;
			}
			AC.SquareButtonFlag = true;
		}
		else {
			AC.SquareButtonFlag = false;
		}
	}

	RedrawWindow();
}

void CWinOGLView::OnUpdateSquare(CCmdUI* pCmdUI)
{
	if (AC.SquareButtonFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnStraight()
{

	if (AC.StraightButtonFlag == false) {
		if (AC.SelectButtonFlag == true) {
			AC.SelectButtonFlag = false;
			AC.NotSelectFlagReset();
		}
		if (AC.SquareButtonFlag == true) {
			AC.SquareButtonFlag = false;
		}
		AC.StraightButtonFlag = true;
	}
	else {
		AC.StraightButtonFlag = false;
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateStraight(CCmdUI* pCmdUI)
{
	if (AC.StraightButtonFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnAllDelete()
{
	if (AC.GetWheelButtonFlag()==false) {
		AC.AllDelete();
		AC.SetShapeCloseFlag(false);
		if (AC.SelectButtonFlag == true) {
			AC.SelectButtonFlag = false;
		}
	}
	RedrawWindow();
}

void CWinOGLView::OnMButtonDown(UINT nFlags, CPoint point)
{

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX_C = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX_C = clickX_C * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		clickX_C = clickX_C * ((float)rect.Width() / rect.Height());
	}

	clickY_C = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY_C = clickY_C * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY_C = clickY_C * ((float)rect.Height() / rect.Width());
	}

	if (AC.SelectButtonFlag == true) {
		if (AC.GetWheelButtonFlag() == false) {
			AC.SetclickXY_C(clickX_C, clickY_C);
			AC.SetWheelButtonFlag(true);
		}
		else {
			AC.SetWheelButtonFlag(false);
		}
	}

	RedrawWindow();

	CView::OnMButtonDown(nFlags, point);
}


BOOL CWinOGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//基点が追加されていたら
	if (AC.GetWheelButtonFlag() == true) {
		AC.DrawExpansionShape(zDelta);
		if (AC.ShapeMoveCrossJudge() == true) {
			AC.ShapeMoveCancel();
		}else if(AC.ExpansionJudge()==true){
			AC.ShapeMoveCancel();
		}
	}

	AC.ResetHoldS();

	RedrawWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
