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


	bool SelectVertexFlag = false;
	bool SelectLineFlag = false;

	int VNumber = 1;

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

	// �e�_�̃t���O���Z�b�g�E�擾����֐�
	void SetSelectVertexFlag(bool f);
	bool GetSelectVertexFlag();

	// �e���̃t���O���Z�b�g�E�擾����֐�
	void SetSelectLineFlag(bool f);
	bool GetSelectLineFlag();

	// �e�_�̃i���o�[���Z�b�g�E�擾����֐�
	void SetVNumber(int n);
	int GetVNumber();

	void FreeVertex();
};

