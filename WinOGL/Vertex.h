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
	//　次の頂点リストを指すポインタ
	CVertex* next_vertex;
public:
	//　頂点のX・Y座標の両方を書き込む
	void SetXY(float new_x, float new_y);
	//　頂点のX座標を読み込む込む
	float GetX();
	//　頂点のY座標を読み込む
	float GetY();
	void GetXY(float *d);

	//　次の頂点リストを指すポインタを書き込む
	CVertex* SetNext(CVertex* new_next);
	//　次の頂点リストを指すポインタを読み込む
	CVertex* GetNext();

};

