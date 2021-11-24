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

CVertex* CVertex::SetPre(CVertex* new_pre)
{
	return pre_vertex = new_pre;
}

CVertex* CVertex::GetPre()
{
	return pre_vertex;
}

// �e�_�̃t���O���Z�b�g����֐�
void CVertex::SetSelectVertexFlag(bool f)
{
	if (f == true) {
		SelectVertexFlag = true;
	}
	else {
		SelectVertexFlag = false;
	}
}

// �e�_�̃t���O���擾����֐�
bool CVertex::GetSelectVertexFlag()
{
	return SelectVertexFlag;
}

// �e���̃t���O���Z�b�g����֐�
void CVertex::SetSelectLineFlag(bool f)
{
	if (f == true) {
		SelectLineFlag = true;
	}
	else {
		SelectLineFlag = false;
	}
}

// �e���̃t���O���擾����֐�
bool CVertex::GetSelectLineFlag()
{
	return SelectLineFlag;
}

/*
// �e�_�̃i���o�[���Z�b�g����֐�
void CVertex::SetVNumber(int n)
{
	VNumber = n;
}

// �e�_�̃i���o�[���擾����֐�
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