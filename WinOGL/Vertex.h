#pragma once

class CVertex{

public:
	CVertex();
	CVertex(float new_x, float new_y);
	~CVertex();
private:
	//�@���_��X���W
	float x;
	//�@���_��Y���W
	float y;

	bool SelectVertexFlag = false;

	float xy[2];

	//�@���̒��_���X�g���w���|�C���^
	CVertex* next_vertex = NULL;
	//�@�O�̒��_���X�g���w���|�C���^
	CVertex* pre_vertex = NULL;

public:
	//�@���_��X�EY���W�̗�������������
	void SetXY(float new_x, float new_y);
	//�@���_��X���W��ǂݍ���
	float GetX();
	//�@���_��Y���W��ǂݍ���
	float GetY();
	//�@���_��XY���W��ǂݍ���
	void GetXY(float *d);

	//�@���̒��_���X�g���w���|�C���^����������
	CVertex* SetNext(CVertex* new_next);
	//�@���̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetNext();

	//�@�O�̒��_���X�g���w���|�C���^����������
	CVertex* SetPre(CVertex* new_pre);
	//�@�O�̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetPre();


	//�@���_��X�EY���W�̗�������������
	void SetSelectVertexFlag(bool f);
	bool GetSelectVertexFlag();

	void FreeVertex();
};

