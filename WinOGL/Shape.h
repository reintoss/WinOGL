#pragma once
#include "Vertex.h"

class CShape{

public:
	CShape();
	CShape(CVertex* new_shape);
	~CShape();

private:

	//次の頂点リストを指すポインタ
	CVertex* vertex_head = NULL;

	//次の形状リストを指すポインタ
	CShape* next_shape = NULL;

	bool SelectShapeFlag = false;

public:

	//頂点のX・Y座標の両方を書き込む
	void SetV(CVertex* v);

	CVertex* GetV();

	//次の形状リストを指すポインタを書き込む
	CShape* SetNextS(CShape* new_nextS);

	//次の形状リストを指すポインタを読み込む
	CShape* GetNextS();

	void FreeShape();

	CVertex* AddVertex(float x, float y);
	int CountVertex();

	// 各形状のフラグをセット・取得する関数
	void SetSelectShapeFlag(bool f);
	bool GetSelectShapeFlag();

};

