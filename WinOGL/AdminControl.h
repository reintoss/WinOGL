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
	int CrossJudge(CShape* startS, CVertex* startV, float x, float y);

	//他交差（特殊）の判定関数
	int CrossJudge2(CShape* startS, CVertex* startV, float x, float y);

	//打った点が内包しているかの判定関数
	int NaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//作成しようとしている図形の中に図形があるかの判定関数
	int GaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//2点のベクトルを計算する
	CVertex* Vector(CVertex* a, CVertex* b);

	//2点の外積を計算する
	float Gaiseki(CVertex* a, CVertex* b);

	//2つの外積から内積を計算する
	float Naiseki(CVertex* a, CVertex* b);

	//ある直線に対し、2点が分断されているか判定する
	int BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//メモリ解放
	void FreeMemory();

	//void FreeVertex();

private:
	//　頂点リストのヘッド
	//Vertex* vertex_head = NULL;
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;

};



