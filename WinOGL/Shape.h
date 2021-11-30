#pragma once
#include <Windows.h>
#include "Vertex.h"

class CShape{

public:
	CShape();
	CShape(CVertex* new_shape);
	~CShape();

private:

	//���̒��_���X�g���w���|�C���^
	CVertex* vertex_head = NULL;

	//���̌`�󃊃X�g���w���|�C���^
	CShape* next_shape = NULL;

	//�O�̌`�󃊃X�g���w���|�C���^
	CShape* pre_shape = NULL;

	bool SelectShapeFlag = false;

public:

	//���_��X�EY���W�̗�������������
	void SetV(CVertex* v);

	CVertex* GetV();

	//vertex_head��NULL����������
	void SetV_NULL();

	//���̌`�󃊃X�g���w���|�C���^����������
	CShape* SetNextS(CShape* new_nextS);

	//���̌`�󃊃X�g���w���|�C���^��ǂݍ���
	CShape* GetNextS();

	//�O�̒��_���X�g���w���|�C���^����������
	CShape* SetPreS(CShape* new_preS);

	//�O�̒��_���X�g���w���|�C���^��ǂݍ���
	CShape* GetPreS();

	//�`������ׂč폜
	void FreeShape();
	//�^�����`��̂ݍ폜
	void OnlyFreeShape();

	CVertex* AddVertex(float x, float y);
	int CountVertex();

	// �e�`��̃t���O���Z�b�g�E�擾����֐�
	void SetSelectShapeFlag(bool f);
	bool GetSelectShapeFlag();

};

