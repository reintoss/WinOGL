#pragma once
#include <Windows.h>
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

	//前の形状リストを指すポインタ
	CShape* pre_shape = NULL;

	//形状が選択されているかどうかのフラグ
	bool SelectShapeFlag = false;

	//形状のある点が移動中かどうかのフラグ
	bool AnyVertexMoveNowFlag = false;

	//奥行の値
	float Depth = 0.1;

public:

	//頂点のX・Y座標の両方を書き込む
	void SetV(CVertex* v);

	CVertex* GetV();

	//vertex_headにNULLを書き込む
	void SetV_NULL();

	//次の形状リストを指すポインタを書き込む
	CShape* SetNextS(CShape* new_nextS);

	//次の形状リストを指すポインタを読み込む
	CShape* GetNextS();

	//前の頂点リストを指すポインタを書き込む
	CShape* SetPreS(CShape* new_preS);

	//前の頂点リストを指すポインタを読み込む
	CShape* GetPreS();

	//形状をすべて削除
	void FreeShape();
	//与えた形状のみ削除
	void OnlyFreeShape();

	CVertex* AddVertex(float x, float y);
	int CountVertex();

	//SelectShapeFlagをフラグをセット・取得する関数
	void SetSelectShapeFlag(bool f);
	bool GetSelectShapeFlag();

	//AnyVertexMoveNowFlagをセット・取得する関数
	void SetAnyVertexMoveNowFlag(bool f);
	bool GetAnyVertexMoveNowFlag();

	//Depthをセット・取得する関数
	void SetDepth(float d);
	float GetDepth();
};

