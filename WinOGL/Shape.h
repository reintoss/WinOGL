#pragma once
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

public:

	//���_��X�EY���W�̗�������������
	void SetV(CVertex* v);

	CVertex* GetV();

	//���̌`�󃊃X�g���w���|�C���^����������
	CShape* SetNextS(CShape* new_nextS);

	//���̌`�󃊃X�g���w���|�C���^��ǂݍ���
	CShape* GetNextS();

	void FreeShape();

	CVertex* AddVertex(float x, float y);
	int CountVertex();

};

