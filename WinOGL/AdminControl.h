#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"

class CAdminControl{

public:
	CAdminControl();
	~CAdminControl();

	//描画処理（問8.1）
	void Draw();

	void AddShape();
	float Distance(CVertex* s, float x, float y);
	void CreateShape(float x, float y);

	//void FreeVertex();

private:
	//　頂点リストのヘッド
	//Vertex* vertex_head = NULL;
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;

};



