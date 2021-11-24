#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"

class CAdminControl {

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
	bool NaihouJudge(CShape* startS, float x, float y);

	//�쐬���悤�Ƃ��Ă���}�`�̒��ɐ}�`�����邩�̔���֐�
	bool GaihouJudge(CShape* startS, float x, float y);

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

	/* �{�^���n */
	//XYZ�{�^����������Ă��邩�ǂ����̃t���O
	bool AxisFlag = false;
	//XYZ����`�悷��֐�
	void DrawAxis();

	//�ҏW�{�^����������Ă��邩�ǂ����̃t���O
	bool SelectButtonFlag = false;

	//�S�폜����֐�
	void AllDelete();
	//�S�폜�{�^����������Ă��邩�ǂ����̃t���O
	bool AllDeleteButtonFlag = false;

	//�l�p�`��`�悷��֐�
	void DrawSquare(float x1, float y1, float x2, float y2);
	//�l�p�`�{�^����������Ă��邩�ǂ����̃t���O
	bool SquareButtonFlag = false;
	//�_��S���ł��Ă��Ȃ���Ԃ��ǂ���
	bool NoVertex = true;
	//NoVertex���擾����֐�
	bool GetNoVertex();
	//�l�p�`�̒��ɐ}�`�����邩(�_��4�^����)
	bool GaihouJudge5(CShape* nowS, CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//�������[�h
	void DrawStraight(float x, float y);
	//�����{�^����������Ă��邩�ǂ����̃t���O
	bool StraightButtonFlag = false;



public:

	//�I�������_�̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	int SelectVertex(float x, float y);

	//�I�������`��̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	void SelectShape(float x, float y);
	//�ł����_������Ă��邩�̔���֐�(�����ɗ^����Shape�̂݌���)
	bool NaihouJudge2(CShape* nowS, float x, float y);
	//�`�����h��Ԃ��֐�
	void DrawShape(CShape* nowS);
	//�O�p�`���}�`�̊O���ǂ����𔻒肷��
	bool ShapeInJudge(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//�`�悷��O�p�`�̕ӂ��A���̕ӂƌ������邩���肷��
	bool CrossJudge3(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//�^����ꂽ2�̓_���������ǂ����𔻒肷��
	bool SameVertexJudge(CVertex* a, CVertex* b);
	//�O�p�`�̒��ɓ_������Ă��邩�̔���֐�
	bool NaihouJudge3(CShape* nowS, CVertex* a, CVertex* b, CVertex* c);
	//3�_�̑g�ݍ��킹����֐�
	bool CombinationJudge(CShape* nowS, int a, int b, int c, int f);
	//nC3�����߂�֐�
	int Combination(int n, int r);
	//Combination()������ȏ�s�����̃t���O
	bool CombinationFlag = false;

	int d[1140] = { 0 };

	//�I�������ӂ̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	int SelectLine(float x, float y);
	//�_�Ɛ��̋��������ߕԋp����֐�
	float VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp);


	//��I���̃t���O�����Z�b�g����֐�
	void NotSelectFlagReset();
	//�`�悵���}�`���������Ă��邩�̃t���O
	bool ShapeCloseFlag = true;
	//ShapeCloseFlag���擾����֐��iWinOGLView.cpp�Ŏg�����߂Ɋ֐��ɂ���j
	bool GetShapeCloseFlag();

	//�}�E�X�����[�u�����ꏊ�ɓ_��`�悷��֐�
	void DrawMoveVertex(float x, float y, float mx, float my);
	//���}�E�X�������Ă��邩�ǂ���
	bool MoveNowJudge = false;
	//���}�E�X�������Ă��邩�ǂ������擾����֐�
	bool GetMoveNowJudge();
	//MoveNowJudge��false�ɂ���֐�
	void ResetMoveNowJudge();
	//�z�[���h���Ă���_
	CVertex* HoldV = NULL;
	//�z�[���h���Ă���_�����Z�b�g����֐�
	void ResetHoldV();
	//�z�[���h���Ă�_������Shape
	CShape* HoldS = NULL;
	//�z�[���h���Ă�_������Shape�����Z�b�g����֐�
	void ResetHoldS();
	//�_�ړ��O�̍��W
	float originX = 0;
	float originY = 0;
	//�_���I���ς݂��ǂ���
	bool AlreadySelectVertexFlag = false;
	//�_�ړ����̑I�𔻒�̃t���O�����Z�b�g����֐�
	void ResetAlreadySelectVertexFlag();
	//�ړ��������_�ɂ���Č�������ӏ������邩
	bool VMoveCrossJudge();
	//�}�`�̒��ɐ}�`�����邩(�^����Shape�̂ݔ���Ώ�)
	bool GaihouJudge2(CShape* nowS,CShape* HoldS);
	//�^�����ӂ����̕ӂƌ������邩���肷��
	bool CrossJudge4(CVertex* s1, CVertex* g1);
	//�ړ��������_�ɂ���Č������Ă����ꍇ�A�_�����ɖ߂��֐�
	void VMoveCancel();

	//���Ƀ_�u���N���b�N�œ_��}������֐�
	void InsertVertex(float x, float y);

	//���N���b�N�œ_���폜����֐�
	void DeleteVertex(float x, float y);
	//�}�`�̒��ɐ}�`�����邩(�����ɍ폜����\��̓_��^����)
	bool GaihouJudge3(CShape* HoldS, CVertex* del);

};



