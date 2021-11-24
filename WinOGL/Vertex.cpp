#include "pch.h"
#include "Vertex.h"

CVertex::CVertex()
{
	x = 0.0;
	y = 0.0;
	next_vertex = NULL;
}

CVertex::CVertex(float new_x, float new_y)
{
	SetXY(new_x, new_y);
	next_vertex = NULL;
}

CVertex::~CVertex()
{

}

//　頂点のX・Y座標の両方を書き込む
void CVertex::SetXY(float new_x, float new_y)
{
	x = new_x;
	y = new_y;
}

//　頂点のX座標を読み込む
float CVertex::GetX()
{
	return x;
}

//　頂点のY座標を読み込む
float CVertex::GetY()
{
	return y;
}

//　頂点のXY座標を読み込む
void CVertex::GetXY(float *d)
{
	d[0] = x;
	d[1] = y;
}

//　次の頂点リストを指すポインタを書き込む
CVertex* CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
	return next_vertex;

}

//　次の頂点リストを指すポインタを読み込む
CVertex* CVertex::GetNext()
{
	return next_vertex;
}

CVertex* CVertex::SetPre(CVertex* new_pre)
{
	return pre_vertex = new_pre;
}

CVertex* CVertex::GetPre()
{
	return pre_vertex;
}

// 各点のフラグをセットする関数
void CVertex::SetSelectVertexFlag(bool f)
{
	if (f == true) {
		SelectVertexFlag = true;
	}
	else {
		SelectVertexFlag = false;
	}
}

// 各点のフラグを取得する関数
bool CVertex::GetSelectVertexFlag()
{
	return SelectVertexFlag;
}

// 各線のフラグをセットする関数
void CVertex::SetSelectLineFlag(bool f)
{
	if (f == true) {
		SelectLineFlag = true;
	}
	else {
		SelectLineFlag = false;
	}
}

// 各線のフラグを取得する関数
bool CVertex::GetSelectLineFlag()
{
	return SelectLineFlag;
}

/*
// 各点のナンバーをセットする関数
void CVertex::SetVNumber(int n)
{
	VNumber = n;
}

// 各点のナンバーを取得する関数
int CVertex::GetVNumber()
{
	return VNumber;
}
*/

void CVertex::FreeVertex()
{
	CVertex* nowV = this;

	while (nowV != NULL) {
		CVertex* del_cell = nowV;
		if (nowV->GetNext() != NULL) {
			nowV = nowV->GetNext();
			delete del_cell;
		}
	}
}