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

public:
	//XYZ�{�^����������Ă��邩�ǂ����̃t���O
	bool AxisFlag = false;
	//XYZ����`�悷��֐�
	void DrawAxis();


	//�ҏW�{�^����������Ă��邩�ǂ����̃t���O
	bool SelectButtonFlag = false;
	//�I�������_�̐F��ς���֐��i���ۂɐF��ς���̂�Draw()�j
	void SelectVertex(float x, float y);
	//��I���̃t���O�����Z�b�g����֐�
	void NotSelectFlagReset();
	//���`�悵�Ă���}�`���������Ă��邩�̃t���O
	bool ShapeCloseFlag;
	//ShapeCloseFlag���擾����֐��iWinOGLView.cpp�Ŏg�����߂Ɋ֐��ɂ���j
	bool GetShapeCloseFlag();
};



