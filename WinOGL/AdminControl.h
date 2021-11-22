#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"

class CAdminControl {

public:
	CAdminControl();
	~CAdminControl();

	//描画処理（問8.1）
	void Draw();

	//点をshape_headに追加する関数
	void AddShape();

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
	//　頂点リストのヘッド
	//Vertex* vertex_head = NULL;
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;

public:
	//XYZボタンが押されているかどうかのフラグ
	bool AxisFlag = false;
	//XYZ軸を描画する関数
	void DrawAxis();


	//編集ボタンが押されているかどうかのフラグ
	bool SelectButtonFlag = false;

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

	//マウスがムーブした場所に点を描画する関数
	void DrawMoveVertex(float x, float y, float mx, float my);
	//今マウスが動いているかどうか
	bool MoveNowJudge = false;
	//今マウスが動いているかどうかを取得する関数
	bool GetMoveNowJudge();
	//MoveNowJudgeをfalseにする関数
	void ResetMoveNowJudge();
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
	bool CrossJudge4(CVertex* vp);
	//移動させた点によって交差していた場合、点を元に戻す関数
	void VMoveCancel();
};



