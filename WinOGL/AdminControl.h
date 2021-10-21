#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"

class CAdminControl{

public:
	CAdminControl();
	~CAdminControl();

	//�`�揈���i��8.1�j
	void Draw();

	//�_��shape_head�ɒǉ�����֐�
	void AddShape();

	//2�_�Ԃ̋��������ߕԋp����֐�
	float Distance(CVertex* s, float x, float y);

	//�V�����`��p�̃��X�g��cshape_head�ɒǉ�����֐��i�V���ȃL�����o�X��ǉ�����j
	void CreateShape(float x, float y);

	//���������������̔���֐�
	int CrossJudge(CShape* startS, CVertex* startV, float x, float y);

	//�������i����j�̔���֐�
	int CrossJudge2(CShape* startS, CVertex* startV, float x, float y);

	//�ł����_������Ă��邩�̔���֐�
	int NaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//�쐬���悤�Ƃ��Ă���}�`�̒��ɐ}�`�����邩�̔���֐�
	int GaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//2�_�̃x�N�g�����v�Z����
	CVertex* Vector(CVertex* a, CVertex* b);

	//2�_�̊O�ς��v�Z����
	float Gaiseki(CVertex* a, CVertex* b);

	//2�̊O�ς�����ς��v�Z����
	float Naiseki(CVertex* a, CVertex* b);

	//���钼���ɑ΂��A2�_�����f����Ă��邩���肷��
	int BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//���������
	void FreeMemory();

	//void FreeVertex();

private:
	//�@���_���X�g�̃w�b�h
	//Vertex* vertex_head = NULL;
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;

};



