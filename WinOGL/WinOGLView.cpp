
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
//ON_WM_MBUTTONDBLCLK()
//ON_WM_RBUTTONDBLCLK()
ON_COMMAND(ID_COPY, &CWinOGLView::OnCopy)
ON_UPDATE_COMMAND_UI(ID_COPY, &CWinOGLView::OnUpdateCopy)
ON_COMMAND(ID_DRAWMODE, &CWinOGLView::OnDrawmode)
ON_UPDATE_COMMAND_UI(ID_DRAWMODE, &CWinOGLView::OnUpdateDrawmode)
ON_COMMAND(ID_CENTERBASE, &CWinOGLView::OnCenterbase)
ON_COMMAND(ID_BACK_VERTEX, &CWinOGLView::OnBackVertex)
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	//左ボタンが離された,ダブルクリックしたときのx,y座標
	clickX = 0;
	clickY = 0;

	//左ボタンが押下されたときのx,y座標
	clickX_L = 0;
	clickY_L = 0;

	//右ボタンが押下されたときのx,y座標
	clickX_R = 0;
	clickY_R = 0;

	//マウスが移動中のときのx,y座標
	clickX_M = 0;
	clickY_M = 0;

	//基点を追加したときのx,y座標(マウス中央ボタン,右ボタンが押下されたとき)
	BaseX = 0;
	BaseY = 0;

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

	if (AC.GetBasePointFlag() == false) { //基点がある場合は無効
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		clickX_L = (float)point.x / rect.Width(); //ex.1920を1とする
		clickX_L = clickX_L * 2 - 1; //区間[0,1]を[-1,0,1]にする
		if (rect.Width() > rect.Height()) { //横長の時
			clickX_L = clickX_L * ((float)rect.Width() / rect.Height());
		}

		clickY_L = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
		clickY_L = clickY_L * 2 - 1; //区間[0,1]を[-1,0,1]にする
		if (rect.Height() > rect.Width()) { //縦長の時
			clickY_L = clickY_L * ((float)rect.Height() / rect.Width());
		}

		LButtonDownFlag = true;
	}

	RedrawWindow();

	CView::OnLButtonDown(nFlags, point);

}

void CWinOGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (AC.GetBasePointFlag() == false) { //基点がある場合は無効
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
			if (AC.SquareButtonFlag == false && AC.StraightButtonFlag == false) {
				AC.CreateShape(clickX, clickY); //問8.2
			}
			else if (AC.SquareButtonFlag == true) {
				AC.DrawSquare(clickX_L, clickY_L, clickX, clickY);
			}
			else {
				AC.DrawStraight(clickX, clickY);
			}
		}
		else {
			//マウスが動いていない場合、選択モード
			if (AC.CopyButtonFlag == true) {
				AC.SelectShape(clickX, clickY);
				
			}
			else if (AC.GetVertexMoveNowJudge() == false && AC.GetShapeMoveNowJudge() == false) {
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
		AC.EndVertexFlag = false;
		AC.ResetHoldV();
		AC.ResetHoldS();
		AC.ResetAlreadySelectVertexFlag();
		AC.ResetAlreadySelectShapeFlag();
		AC.Reset_shape_head2();
	}

	RedrawWindow();

	CView::OnLButtonUp(nFlags, point);
}


void CWinOGLView::OnMouseMove(UINT nFlags, CPoint point)
{

	if (AC.GetBasePointFlag() == false && AC.CopyButtonFlag == false) { //基点がある場合またはコピーモードの場合は無効
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
	}

	RedrawWindow();

	CView::OnMouseMove(nFlags, point);
}


void CWinOGLView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	if (AC.GetBasePointFlag() == false) { //基点がある場合は無効
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
			if (AC.CopyButtonFlag == true) {
				if (AC.DrawCopyShape(clickX, clickY) == 1) { //コピーした形状を描画
					if (AC.ShapeMoveCrossJudge() == true) { //交差していた場合
						AC.DeleteCopyShape(); //コピーした形状を削除
					}
				}
			}
		}

	}
	RedrawWindow();

	AC.ResetHoldS();

	CView::OnLButtonDblClk(nFlags, point);
}


void CWinOGLView::OnMButtonDown(UINT nFlags, CPoint point)
{

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	BaseX = (float)point.x / rect.Width(); //ex.1920を1とする
	BaseX = BaseX * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		BaseX = BaseX * ((float)rect.Width() / rect.Height());
	}

	BaseY = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	BaseY = BaseY * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		BaseY = BaseY * ((float)rect.Height() / rect.Width());
	}

	if (AC.SelectButtonFlag == true) {
		if (AC.GetWheelButtonFlag() == false) {  //基点がない場合
			if (AC.GetRButtonFlag() == false) { //回転基点がなければ、拡大縮小基点を追加
				AC.SetBaseXY(BaseX, BaseY);
				AC.SetWheelButtonFlag(true);
				AC.SetRButtonFlag(false);
				AC.SetWheelUsedFlag(false);
			}
			else { //回転基点がある場合は、回転基点解除→交差判定
				AC.SetRButtonFlag(false);
				if (AC.shape_head2_NULLJudge() == false) { //shape_head2がNULLでなければ交差判定
					if (AC.ShapeMoveCrossJudge() == true) {
						AC.ShapeMoveCancel();
					}
				}
				AC.ResetHoldS();
				AC.Reset_shape_head2();
			}
		}
		else { //拡大縮小基点がある場合は、拡大縮小基点解除→交差判定
			AC.SetWheelButtonFlag(false);
			if (AC.shape_head2_NULLJudge() == false) { //shape_head2がNULLでなければ交差判定
				if (AC.ShapeMoveCrossJudge() == true) {
					AC.ShapeMoveCancel();
				}
				else if (AC.ExpansionJudge(rect) == true) {
					AC.ShapeMoveCancel();
				}
			}
			AC.ResetHoldS();
			AC.Reset_shape_head2();
		}
	}

	RedrawWindow();

	CView::OnMButtonDown(nFlags, point);
}


void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	clickX_R = (float)point.x / rect.Width(); //ex.1920を1とする
	clickX_R = clickX_R * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Width() > rect.Height()) { //横長の時
		clickX_R = clickX_R * ((float)rect.Width() / rect.Height());
	}

	clickY_R = (float)(rect.Height() - point.y) / rect.Height(); //ex.1080を1とする、y座標は左上が0なので反転
	clickY_R = clickY_R * 2 - 1; //区間[0,1]を[-1,0,1]にする
	if (rect.Height() > rect.Width()) { //縦長の時
		clickY_R = clickY_R * ((float)rect.Height() / rect.Width());
	}

	//編集ボタンが押されている場合のみ有効
	if (AC.SelectButtonFlag == true) {
		if (AC.GetRButtonFlag() == false) { //回転基点の追加
			if (AC.GetWheelButtonFlag() == true && AC.GetWheelUsedFlag() == false) { //中央ボタンで基点が追加されているかつ、まだマウスホイールを動かしていない場合
				AC.SetWheelButtonFlag(false);
				AC.SetBaseXY(BaseX, BaseY);
				AC.SetRButtonFlag(true);
			}
		}

		if (AC.DeleteVertex(clickX_R, clickY_R) != 1) { //点の削除
			if (AC.SelectLineNowJudge() == true) { //選択している線がある場合
				AC.InsertVertex(clickX, clickY); //点の挿入
			}
			else if (AC.GetWheelButtonFlag() == false && AC.GetShapeMoveNowJudge() == false && AC.GetRButtonFlag() == false) { //図形が赤色ではない場合
				AC.DeleteShape(clickX_R, clickY_R);
				if (AC.GetNoVertex() == true) {
					AC.SelectButtonFlag = false;
					AC.CopyButtonFlag = false;
					AC.DrawButtonFlag = true;
				}
			}
		}
	}

	RedrawWindow();

	CView::OnRButtonDown(nFlags, point);
}


BOOL CWinOGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	//中央ボタンで基点が追加されていたら	
	if (AC.GetWheelButtonFlag() == true) {
		AC.DrawExpansionShape(zDelta); //拡大縮小する
		AC.SetWheelUsedFlag(true);
	}
	//右ボタンで基点が追加されていたら
	else if(AC.GetRButtonFlag() == true){
		AC.DrawRotateShape(zDelta); //回転する
	}

	RedrawWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
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
	if (AC.GetShapeCloseFlag() == true) { //形状が閉じていない場合は選択できない
		if (AC.GetNoVertex() == false) {
			if (AC.SelectButtonFlag == true) {
				if (AC.GetBasePointFlag() == false) { //基点がある場合は解除できない
					AC.SelectButtonFlag = false;
					AC.DrawButtonFlag = true;
					AC.CopyButtonFlag = false;
					AC.NotSelectFlagReset();
				}
			}
			else {
				AC.SelectButtonFlag = true;
				AC.DrawButtonFlag = false;
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
			if (AC.GetBasePointFlag() == false) { //基点がある場合は解除できない
				if (AC.SelectButtonFlag == true) {
					AC.SelectButtonFlag = false;
					AC.DrawButtonFlag = true;
					AC.CopyButtonFlag = false;
					AC.NotSelectFlagReset();
				}
				if (AC.StraightButtonFlag == true) {
					AC.StraightButtonFlag = false;
				}
				AC.SquareButtonFlag = true;
			}
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
		if (AC.GetBasePointFlag() == false) { //基点がある場合は解除できない
			if (AC.SelectButtonFlag == true) {
				AC.SelectButtonFlag = false;
				AC.DrawButtonFlag = true;
				AC.CopyButtonFlag = false;
				AC.NotSelectFlagReset();
			}
			if (AC.SquareButtonFlag == true) {
				AC.SquareButtonFlag = false;
			}
			AC.StraightButtonFlag = true;
		}
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
	if (AC.GetBasePointFlag() == false) { //基点がある場合は無効
		AC.AllDelete();
		AC.SetShapeCloseFlag(false);
		if (AC.SelectButtonFlag == true) {
			AC.SelectButtonFlag = false;
			AC.DrawButtonFlag = true;
			AC.CopyButtonFlag = false;
		}
	}

	RedrawWindow();
}


void CWinOGLView::OnCopy()
{
	if (AC.SelectButtonFlag == true) {  //EDITモードがオンの時のみ使用可
		if (AC.CopyButtonFlag == false) {
			if (AC.GetBasePointFlag() == false) { //基点がある場合は解除できない
				AC.CopyButtonFlag = true;
				AC.ResetSelectVL();
			}
		}
		else {
			AC.CopyButtonFlag = false;
		}
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateCopy(CCmdUI* pCmdUI)
{
	if (AC.CopyButtonFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnDrawmode()
{

	if (AC.GetBasePointFlag() == false) { //基点がある場合は解除できない
		if (AC.SelectButtonFlag == true) {
			AC.SelectButtonFlag = false;
			AC.CopyButtonFlag = false;
			AC.NotSelectFlagReset();
			AC.DrawButtonFlag = true;
		}
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateDrawmode(CCmdUI* pCmdUI)
{
	if (AC.DrawButtonFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnCenterbase()
{
	if (AC.GetBasePointFlag() == true) { //基点がある場合のみ
		AC.CenterBase();
	}

	RedrawWindow();
}

void CWinOGLView::OnBackVertex()
{
	if (AC.DrawButtonFlag == true) { //描画モードのときのみ有効
		if (AC.GetShapeCloseFlag() == false) { //閉じていない形状がある場合
			AC.BackVertex();
		}
	}

	RedrawWindow();
}
