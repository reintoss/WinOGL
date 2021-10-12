#pragma once

#include <gl/GL.h>

#include "Vertex.h"

class CAdminControl{

public:
	CAdminControl();
	~CAdminControl();
	//　頂点リストのヘッド

	void Draw(); //描画処理（問8.1）

	CVertex* AddVertex(float x, float y); //Vertexの追加（問8.2）

	void FreeVertex();

private:
	//CVertex* vertex_head;

};



