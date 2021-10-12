#include "pch.h"
#include "AdminControl.h"

CVertex* vertex_head = NULL;

CAdminControl::CAdminControl() {

}

CAdminControl::~CAdminControl() {

}

void CAdminControl::Draw(){

	CVertex* vp = vertex_head;

    glColor3f(1.0, 1.0, 1.0);
    glPointSize(5);

	glBegin(GL_POINTS);

	float d[2];
	for (vp = vertex_head; vp != NULL; vp = vp->GetNext()) {
		vp->GetXY(d);
		glVertex2f(d[0],d[1]);
	}

	glEnd();
}

// ’¸“_’Ç‰ÁŠÖ”
CVertex* CAdminControl::AddVertex(float new_x, float new_y) {

	CVertex* New_Vertex = new CVertex(new_x, new_y);

	if (vertex_head == NULL) {
		vertex_head = New_Vertex;
	}else {
		CVertex* vp = vertex_head;
		while (vp->GetNext() != NULL) {
			vp = vp->GetNext();
		}
		vp->SetNext(New_Vertex);
	}

	return New_Vertex;
}

void CAdminControl::FreeVertex(){
	CVertex* temp = NULL;
	CVertex* vp = vertex_head;
	while (vertex_head != NULL)
	{
		temp = vertex_head;
		vertex_head = vertex_head->GetNext();
		delete temp;
	}
}
