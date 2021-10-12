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

	float xy[2];
	//�@���̒��_���X�g���w���|�C���^
	CVertex* next_vertex;
public:
	//�@���_��X�EY���W�̗�������������
	void SetXY(float new_x, float new_y);
	//�@���_��X���W��ǂݍ��ލ���
	float GetX();
	//�@���_��Y���W��ǂݍ���
	float GetY();
	void GetXY(float *d);

	//�@���̒��_���X�g���w���|�C���^����������
	CVertex* SetNext(CVertex* new_next);
	//�@���̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetNext();

};

