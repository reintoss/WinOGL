#pragma once

class CVertex{

public:
	CVertex();
	CVertex(float new_x, float new_y);
	~CVertex();
private:
	//　頂点のX座標
	float x;
	//　頂点のY座標
	float y;

	float xy[2];


	bool SelectVertexFlag = false;
	bool SelectLineFlag = false;

	int VNumber = 1;

	//　次の頂点リストを指すポインタ
	CVertex* next_vertex = NULL;
	//　前の頂点リストを指すポインタ
	CVertex* pre_vertex = NULL;

public:
	//　頂点のX・Y座標の両方を書き込む
	void SetXY(float new_x, float new_y);
	//　頂点のX座標を読み込む
	float GetX();
	//　頂点のY座標を読み込む
	float GetY();
	//　頂点のXY座標を読み込む
	void GetXY(float *d);

	//　次の頂点リストを指すポインタを書き込む
	CVertex* SetNext(CVertex* new_next);
	//　次の頂点リストを指すポインタを読み込む
	CVertex* GetNext();

	//　前の頂点リストを指すポインタを書き込む
	CVertex* SetPre(CVertex* new_pre);
	//　前の頂点リストを指すポインタを読み込む
	CVertex* GetPre();

	// 各点のフラグをセット・取得する関数
	void SetSelectVertexFlag(bool f);
	bool GetSelectVertexFlag();

	// 各線のフラグをセット・取得する関数
	void SetSelectLineFlag(bool f);
	bool GetSelectLineFlag();

	// 各点のナンバーをセット・取得する関数
	void SetVNumber(int n);
	int GetVNumber();

	void FreeVertex();
};

