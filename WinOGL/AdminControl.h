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
	bool CrossJudge(CShape* startS, CVertex* startV, float x, float y);

	//�������i����j�̔���֐�
	bool CrossJudge2(CShape* startS, CVertex* startV, float x, float y);

	//�ł����_������Ă��邩�̔���֐�
	bool NaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//�쐬���悤�Ƃ��Ă���}�`�̒��ɐ}�`�����邩�̔���֐�
	bool GaihouJudge(CShape* startS, CVertex* startV, float x, float y);

	//���钼���ɑ΂��A2�_�����f����Ă��邩���肷��
	bool BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//2�_�̃x�N�g�����v�Z����
	CVertex* Vector(CVertex* a, CVertex* b);

	//2�_�̊O�ς��v�Z����
	float Gaiseki(CVertex* a, CVertex* b);

	//2�̊O�ς�����ς��v�Z����
	float Naiseki(CVertex* a, CVertex* b);

	//2�����̊p�x�����߂�
	float Kakudo(CVertex* a, CVertex* b);

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

	//�I�������_�̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	int SelectVertex(float x, float y);

	//�I�������`��̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	void SelectShape(float x, float y);
	//�ł����_������Ă��邩�̔���֐��i�����ɗ^����shape_head�������Ȃ��j
	bool NaihouMiniJudge(CShape* nowS, float x, float y);
	//�`�����h��Ԃ��֐�
	void DrawShape(CShape* nowS);


	//�I�������ӂ̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	int SelectLine(float x, float y);
	//�_�Ɛ��̋��������ߕԋp����֐�
	float VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp);


	//��I���̃t���O�����Z�b�g����֐�
	void NotSelectFlagReset();
	//�`�悵���}�`���������Ă��邩�̃t���O
	bool ShapeCloseFlag = false;
	//ShapeCloseFlag���擾����֐��iWinOGLView.cpp�Ŏg�����߂Ɋ֐��ɂ���j
	bool GetShapeCloseFlag();
};



