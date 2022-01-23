#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"
#include "pch.h"

class CAdminControl {

public:
	CAdminControl();
	~CAdminControl();

	//�`�揈���i��8.1�j
	void Draw();

	//�_��shape_head�ɒǉ�����֐�
	void AddShape();

	//�_��shape_head2�ɒǉ�����֐�
	void AddShape2();

	//�_��shape_head3�ɒǉ�����֐�
	void AddShape3();

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
	float VectorX(CVertex* a, CVertex* b);
	float VectorY(CVertex* a, CVertex* b);

	//2�_�̊O�ς��v�Z����
	float Gaiseki(CVertex* a, CVertex* b);
	float Gaiseki(float ax, float ay, float bx, float by);

	//2�̊O�ς�����ς��v�Z����
	float Naiseki(CVertex* a, CVertex* b);
	float Naiseki(float ax, float ay, float bx, float by);

	//2�����̊p�x�����߂�
	float Kakudo(CVertex* a, CVertex* b);
	float Kakudo(float ax, float ay, float bx, float by);

	//���������
	void FreeMemory();

private:
	//�`�󃊃X�g�̃w�b�h
	CShape* shape_head = NULL;
	CShape* shape_head2 = NULL; //�`��ꎞ�ێ��p
	CShape* shape_head3 = NULL; //�`��h��Ԃ��p


public:

	/* �{�^���n */

	//�`�惂�[�h
	bool DrawButtonFlag = true;


	//�ҏW���[�h
	bool SelectButtonFlag = false;

	//XYZ�{�^����������Ă��邩�ǂ����̃t���O
	bool AxisFlag = false;
	//XYZ����`�悷��֐�
	void DrawAxis();

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
	//���O�̒����̌���(1�Fx���A2�Fx���A3�Fy���A4�Fy��)
	int StraightPreMove = 0;

	//�I�𒆂̓_�܂��͕ӂ����Z�b�g����֐�
	void ResetSelectVL();
	//�`��R�s�[�{�^����������Ă��邩�ǂ����̃t���O
	bool CopyButtonFlag = false;
	//�N���b�N�����ꏊ�Ɍ`����R�s�[����֐�
	int DrawCopyShape(float x, float y);
	//�������Ă����ꍇ�A�R�s�[�����`����폜����֐�
	void DeleteCopyShape();

	//�`�撆�̓_�����ɖ߂�
	void BackVertex();

	//���̑�����ς���(+�̏ꍇ��true,-�̏ꍇ��false���󂯎��)
	void LineSizeChange(bool f);
	//�_�̃T�C�Y
	float POINTSIZE = 11.4;
	//���̃T�C�Y
	float LINESIZE = 2.0;

	//�`��̓h��Ԃ��{�^����������Ă��邩�ǂ����̃t���O
	bool ShapeFillButtonFlag = false;
	//�`���h��Ԃ��֐�
	void Shape_Fill();

	//�O�p�`�̒��ɓ_������Ă��邩�̔���֐�
	bool NaihouJudge3(CShape* nowS, CVertex* a, CVertex* b, CVertex* c);
	//�`�悷��O�p�`�̕ӂ��A���̕ӂƌ������邩���肷��
	bool CrossJudge3(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//�O�p�`���}�`�̊O���ǂ����𔻒肷��
	bool ShapeInJudge(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//�^����ꂽ2�̓_���������ǂ����𔻒肷��
	bool SameVertexJudge(CVertex* a, CVertex* b);
	//shape_head3�����Z�b�g����֐�
	void Reset_shape_head3();
	//���^���ǂ������肷��֐�
	bool StarJudge(CShape* nowS);

	//���_�ύX�{�^����������Ă��邩�ǂ����̃t���O
	bool ViewModeButtonFlag = false;
	//���_�̔{�����X�V����֐�
	void ScaleUpdate(short zDelta);
	//���_�̔{��(�����l)
	float Scale = 1.0;
	//���_�̕��s�ړ��ʂ��X�V����֐�
	void TranslateUpdate(float Lx, float Ly, float mx, float my);
	//���_�̕��s�ړ���
	float TransX = 0.0;
	float TransY = 0.0;
	//1�t���[���O��x,y���W
	float PreX = 0.0;
	float PreY = 0.0;
	//���_�̉�]�ʂ��X�V����֐�
	void RotateUpdate(float Rx, float Ry, float mx, float my);
	//���_�̉�]��
	float RotateX = 0.0;
	float RotateY = 0.0;
	//1�t���[���O�̃}�E�X��x,y���W
	float PreMouseX = 0.0;
	float PreMouseY = 0.0;
	//�E�N���b�N�̃t���O
	bool RButtonDownFlag = false;
	//�e���l������������֐�
	void InitViewValue();

	//���̃{�^����������Ă��邩�ǂ����̃t���O
	bool SolidButtonFlag = false;
	//��ʂ�h��Ԃ��֐�
	void Shape_Fill2();
	//�`��𗧑̂ɂ���֐�
	void SolidMake();
	//���s�̒l���X�V����֐�
	void DepthUpdate(short zDelta);
	//��ʂ̌`���h��Ԃ��֐�
	void Shape_Fill_Depth();
	//�`�󂪎��v���ŕ`�悳�ꂽ�����肷��֐�
	bool Clockwise(CShape* nowS);
	//3�_�����v���ŕ`�悳�ꂽ�����肷��֐�
	bool Clockwise(CVertex* v1, CVertex* v2,CVertex* v3);

	//���C���[���f���{�^����������Ă��邩�ǂ����̃t���O
	bool WireButtonFlag = false;
	//�`������C���[�t���[�����f���ɂ���֐�
	void WireMake();

	//���̑I���{�^����������Ă��邩�ǂ����̃t���O
	bool SolidSelectButtonFlag = false;
	//SelectShapeFlag�����Z�b�g����֐�
	void ResetSelectShapeFlag();
	//���̕���I������֐�
	void SelectSolid();
	//�I�𒆂̃\���b�h���f���̕ӂ�`�悷��֐�
	void DrawSelectSolidLine();

public:

	/* �_�̑I�� */

	//�I�������_�̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	int SelectVertex(float x, float y);


	/* �`��̑I�� */

	//�I�������`��̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	void SelectShape(float x, float y);
	//�ł����_������Ă��邩�̔���֐�(�����ɗ^����Shape�̂݌���)
	bool NaihouJudge2(CShape* nowS, float x, float y);
	//�`���I������֐�
	void DrawSelectShape(CShape* nowS);


	/* ���̑I�� */

	//�I�������ӂ̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
	int SelectLine(float x, float y);
	//�_�Ɛ��̋��������ߕԋp����֐�
	float VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp);


	/* �`�󂪕��Ă��邩�ǂ���(�`�撆�ł͂Ȃ���) */

	//��I���̃t���O�����Z�b�g����֐�
	void NotSelectFlagReset();
	//�`�悵���}�`���������Ă��邩�̃t���O
	bool ShapeCloseFlag = true;
	//ShapeCloseFlag���擾����֐��iWinOGLView.cpp�Ŏg�����߂Ɋ֐��ɂ���j
	bool GetShapeCloseFlag();
	//ShapeCloseFlag���Z�b�g����֐�
	void SetShapeCloseFlag(bool f);


	/* �_�̈ړ� */

	//�}�E�X�����[�u�����ꏊ�ɓ_��`�悷��֐�
	void DrawMoveVertex(float x, float y, float mx, float my);
	//���}�E�X�������Ă��邩�ǂ���
	bool VertexMoveNowJudge = false;
	//���}�E�X�������Ă��邩�ǂ������擾����֐�
	bool GetVertexMoveNowJudge();
	//MoveNowJudge��false�ɂ���֐�
	void ResetVertexMoveNowJudge();
	//�I�_���z�[���h�����ꍇ��true
	bool EndVertexFlag = false;
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
	//AnyVertexMoveNowFlag�����Z�b�g����֐�
	void ResetAnyVertexMoveNowFlag();
	//�ړ��������_�ɂ���Č�������ӏ������邩
	bool VMoveCrossJudge();
	//�}�`�̒��ɐ}�`�����邩(�^����Shape�̂ݔ���Ώ�)
	bool GaihouJudge2(CShape* nowS,CShape* HoldS);
	//�^�����ӂ����̕ӂƌ������邩���肷��
	bool CrossJudge4(CVertex* s1, CVertex* g1);
	//�ړ��������_�ɂ���Č������Ă����ꍇ�A�_�����ɖ߂��֐�
	void VMoveCancel();


	/* �_�̑}�� */

	//���Ƀ_�u���N���b�N�œ_��}������֐�
	void InsertVertex(float x, float y);
	//�I������Ă���������邩���肷��֐�
	bool SelectLineNowJudge();


	/* �_�̍폜 */

	//���N���b�N�œ_���폜����֐�
	int DeleteVertex(float x, float y);
	//�}�`�̒��ɐ}�`�����邩(�����ɍ폜����\��̓_��^����)
	bool GaihouJudge3(CShape* HoldS, CVertex* del);
	//���N���b�N�Ō`����폜����֐�
	void DeleteShape(float x, float y);


	/* �`��̈ړ� */

	//�}�E�X�����[�u�����ꏊ��Shape��`�悷��֐�
	bool DrawMoveShape(float x, float y, float mx, float my);
	//�`��̈ړ���
	float ShapeMoveX = 0.0;
	float ShapeMoveY = 0.0;
	//�`���1�t���[���O��x,y���W
	float ShapePreX = 0.0;
	float ShapePreY = 0.0;
	//Shape���I���ς݂��ǂ���
	bool AlreadySelectShapeFlag = false;
	//Shape�ړ����̑I�𔻒�̃t���O�����Z�b�g����֐�
	void ResetAlreadySelectShapeFlag();
	//���}�E�X�������Ă��邩�ǂ���
	bool ShapeMoveNowJudge = false;
	//���}�E�X�������Ă��邩�ǂ������擾����֐�
	bool GetShapeMoveNowJudge();
	//MoveNowJudge��false�ɂ���֐�
	void ResetShapeMoveNowJudge();
	//�ړ�������Shape�ɂ���Č�������ӏ������邩
	bool ShapeMoveCrossJudge();
	//�ړ�������Shape�ɂ���Č������Ă����ꍇ�AShape�����ɖ߂��֐�
	void ShapeMoveCancel();
	//shape_head2�����Z�b�g����֐�
	void Reset_shape_head2();


	/* �`��̊g��E�k�� */

	//�}�E�X�̒����{�^�����������ꂽ��
	bool WheelButtonFlag = false;
	//WheelButtonFlag���Z�b�g�E�擾����֐�
	void SetWheelButtonFlag(bool f);
	bool GetWheelButtonFlag();
	//��_��`�悷��֐�
	void DrawBasePoint();
	//��_���`��̏d�S�ɂ���֐�
	void CenterBase();
	//�}�E�X�����{�^�����������ꂽ�Ƃ���x,y���W
	float BaseX = 0;
	float BaseY = 0;
	//clickX_C,clickY_C���Z�b�g����֐�
	void SetBaseXY(float x, float y);
	//�`����g��E�k������֐�
	void DrawExpansionShape(short zDelta);
	//�g��E�k���ɂ���Č`�󂪌��������ꍇ�A���ɖ߂��֐�
	void ShapeExepansionCancel();
	//�g��E�k����������h��
	bool ExpansionJudge(CRect rect);


	/* �`��̉�] */

	//shape_head2��NULL���ǂ������肷��֐�
	bool shape_head2_NULLJudge();
	//�E�N���b�N�ɂ���Ċ�_���ǉ����ꂽ���ǂ���
	bool RButtonFlag = false;
	//RButtonFlag���Z�b�g�E�擾����֐�
	void SetRButtonFlag(bool f);
	bool GetRButtonFlag();
	//�}�E�X�z�C�[�����g�p�������ǂ���(��_�ǉ����ɏ�����)
	bool WheelUsedFlag = false;
	//WheelUsedFlag���Z�b�g�E�擾����֐�
	void SetWheelUsedFlag(bool f);
	bool GetWheelUsedFlag();
	//��_�����邩�ǂ������擾����֐�(�����true)
	bool GetBasePointFlag();
	//�`�����]����֐�
	void DrawRotateShape(short zDelta);
};



