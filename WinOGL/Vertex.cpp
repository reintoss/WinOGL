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

//�@���_��X�EY���W�̗�������������
void CVertex::SetXY(float new_x, float new_y)
{
	x = new_x;
	y = new_y;
}

//�@���_��X���W��ǂݍ���
float CVertex::GetX()
{
	return x;
}

//�@���_��Y���W��ǂݍ���
float CVertex::GetY()
{
	return y;
}

//�@���_��XY���W��ǂݍ���
void CVertex::GetXY(float *d)
{
	d[0] = x;
	d[1] = y;
}

//�@���̒��_���X�g���w���|�C���^����������
CVertex* CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
	return next_vertex;

}

//�@���̒��_���X�g���w���|�C���^��ǂݍ���
CVertex* CVertex::GetNext()
{
	return next_vertex;
}

void CVertex::FreeVertex()
{
	CVertex* nowV = this;
	while (nowV != NULL) {
		CVertex* del_cell = nowV;
		nowV = nowV->GetNext();
		delete del_cell;
	}
}