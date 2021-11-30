#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"
#include "pch.h"

class CAdminControl {

public:
	CAdminControl();
	~CAdminControl();

	//描画処理（問8.1）
	void Draw();

	//点をshape_headに追加する関数
	void AddShape();

	//点をshape_headに追加する関数
	void AddShape2();

	//2点間の距離を求め返却する関数
	float Distance(CVertex* s, float x, float y);

	//新しい形状用のリストをcshape_headに追加する関数（新たなキャンバスを追加する）
	void CreateShape(float x, float y);

	//自交差＆他交差の判定関数
	bool CrossJudge(CShape* startS, CVertex* startV, float x, float y);

	//他交差（特殊）の判定関数
	bool CrossJudge2(CShape* startS, CVertex* startV, float x, float y);

	//打った点が内包しているかの判定関数
	bool NaihouJudge(CShape* startS, float x, float y);

	//作成しようとしている図形の中に図形があるかの判定関数
	bool GaihouJudge(CShape* startS, float x, float y);

	//ある直線に対し、2点が分断されているか判定する
	bool BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//2点のベクトルを計算する
	CVertex* Vector(CVertex* a, CVertex* b);

	//2点の外積を計算する
	float Gaiseki(CVertex* a, CVertex* b);

	//2つの外積から内積を計算する
	float Naiseki(CVertex* a, CVertex* b);

	//2線分の角度を求める
	float Kakudo(CVertex* a, CVertex* b);

	//メモリ解放
	void FreeMemory();

	//void FreeVertex();

private:
	//形状リストのヘッド
	CShape* shape_head = NULL;
	CShape* shape_head2 = NULL; //形状一時保持用

public:

	/* ボタン系 */
	//XYZボタンが押されているかどうかのフラグ
	bool AxisFlag = false;
	//XYZ軸を描画する関数
	void DrawAxis();

	//編集ボタンが押されているかどうかのフラグ
	bool SelectButtonFlag = false;

	//全削除する関数
	void AllDelete();
	//全削除ボタンが押されているかどうかのフラグ
	bool AllDeleteButtonFlag = false;

	//四角形を描画する関数
	void DrawSquare(float x1, float y1, float x2, float y2);
	//四角形ボタンが押されているかどうかのフラグ
	bool SquareButtonFlag = false;
	//点を全く打っていない状態かどうか
	bool NoVertex = true;
	//NoVertexを取得する関数
	bool GetNoVertex();
	//四角形の中に図形があるか(点を4つ与える)
	bool GaihouJudge5(CShape* nowS, CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//直線モード
	void DrawStraight(float x, float y);
	//直線ボタンが押されているかどうかのフラグ
	bool StraightButtonFlag = false;
	//直前の直線の向き(1：x正、2：x負、3：y正、4：y負)
	int StraightPreMove = 0;


public:

	//選択した点の色を変える関数（実際に色を変えるのはDraw()内）
	int SelectVertex(float x, float y);

	//選択した形状の色を変える関数（実際に色を変えるのはDraw()内）
	void SelectShape(float x, float y);
	//打った点が内包しているかの判定関数(引数に与えたShapeのみ見る)
	bool NaihouJudge2(CShape* nowS, float x, float y);
	//形状内を塗りつぶす関数
	void DrawShape(CShape* nowS);
	//三角形が図形の外かどうかを判定する
	bool ShapeInJudge(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//描画する三角形の辺が、他の辺と交差するか判定する
	bool CrossJudge3(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//与えられた2つの点が同じかどうかを判定する
	bool SameVertexJudge(CVertex* a, CVertex* b);
	//三角形の中に点が内包しているかの判定関数
	bool NaihouJudge3(CShape* nowS, CVertex* a, CVertex* b, CVertex* c);
	//3点の組み合わせ判定関数
	bool CombinationJudge(CShape* nowS, int a, int b, int c, int f);
	//nC3を求める関数
	int Combination(int n, int r);
	//Combination()をこれ以上行うかのフラグ
	bool CombinationFlag = false;

	int d[1140] = { 0 };

	//選択した辺の色を変える関数（実際に色を変えるのはDraw()内）
	int SelectLine(float x, float y);
	//点と線の距離を求め返却する関数
	float VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp);


	//被選択のフラグをリセットする関数
	void NotSelectFlagReset();
	//描画した図形たちが閉じているかのフラグ
	bool ShapeCloseFlag = false;
	//ShapeCloseFlagを取得する関数（WinOGLView.cppで使うために関数にする）
	bool GetShapeCloseFlag();
	//ShapeCloseFlagをセットする関数
	void SetShapeCloseFlag(bool f);

	//マウスがムーブした場所に点を描画する関数
	void DrawMoveVertex(float x, float y, float mx, float my);
	//今マウスが動いているかどうか
	bool VertexMoveNowJudge = false;
	//今マウスが動いているかどうかを取得する関数
	bool GetVertexMoveNowJudge();
	//MoveNowJudgeをfalseにする関数
	void ResetVertexMoveNowJudge();
	//ホールドしている点
	CVertex* HoldV = NULL;
	//ホールドしている点をリセットする関数
	void ResetHoldV();
	//ホールドしてる点があるShape
	CShape* HoldS = NULL;
	//ホールドしてる点があるShapeをリセットする関数
	void ResetHoldS();
	//点移動前の座標
	float originX = 0;
	float originY = 0;
	//点が選択済みかどうか
	bool AlreadySelectVertexFlag = false;
	//点移動時の選択判定のフラグをリセットする関数
	void ResetAlreadySelectVertexFlag();
	//移動させた点によって交差する箇所があるか
	bool VMoveCrossJudge();
	//図形の中に図形があるか(与えたShapeのみ判定対象)
	bool GaihouJudge2(CShape* nowS,CShape* HoldS);
	//与えた辺が他の辺と交差するか判定する
	bool CrossJudge4(CVertex* s1, CVertex* g1);
	//移動させた点によって交差していた場合、点を元に戻す関数
	void VMoveCancel();

	//線にダブルクリックで点を挿入する関数
	void InsertVertex(float x, float y);

	//左クリックで点を削除する関数
	void DeleteVertex(float x, float y);
	//図形の中に図形があるか(引数に削除する予定の点を与える)
	bool GaihouJudge3(CShape* HoldS, CVertex* del);

	//マウスがムーブした場所にShapeを描画する関数
	bool DrawMoveShape(float x, float y, float mx, float my);
	//Shapeが選択済みかどうか
	bool AlreadySelectShapeFlag = false;
	//Shape移動時の選択判定のフラグをリセットする関数
	void ResetAlreadySelectShapeFlag();
	//今マウスが動いているかどうか
	bool ShapeMoveNowJudge = false;
	//今マウスが動いているかどうかを取得する関数
	bool GetShapeMoveNowJudge();
	//MoveNowJudgeをfalseにする関数
	void ResetShapeMoveNowJudge();
	//移動させたShapeによって交差する箇所があるか
	bool ShapeMoveCrossJudge();
	//移動させたShapeによって交差していた場合、Shapeを元に戻す関数
	void ShapeMoveCancel();
	//Shape移動をキャンセルした場合に使用
	CShape* HoldS2 = NULL;
	//ResetHoldS2をリセットする関数
	void Reset_shape_head2();
};



