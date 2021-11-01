#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"

class CAdminControl{

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
	bool NaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//作成しようとしている図形の中に図形があるかの判定関数
	bool GaihouJudge(CShape* startS, CVertex* startV, float x, float y);

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
	//打った点が内包しているかの判定関数（引数に与えたshape_headしか見ない）
	bool NaihouMiniJudge(CShape* nowS, float x, float y);
	//形状内を塗りつぶす関数
	void DrawShape(CShape* nowS);


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
};



