#pragma once

#include <gl/GL.h>

#include "Vertex.h"

class CAdminControl{

public:
	CAdminControl();
	~CAdminControl();
	//�@���_���X�g�̃w�b�h

	void Draw(); //�`�揈���i��8.1�j

	CVertex* AddVertex(float x, float y); //Vertex�̒ǉ��i��8.2�j

	void FreeVertex();

private:
	//CVertex* vertex_head;

};



