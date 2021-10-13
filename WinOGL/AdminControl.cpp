#include "pch.h"
#include "AdminControl.h"

CVertex* vertex_head = NULL;

CAdminControl::CAdminControl() {

}

CAdminControl::~CAdminControl() {
	shape_head->FreeShape();
}

void CAdminControl::Draw(){

    CShape* nowS = shape_head;

    while (nowS != NULL)
    {
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(10);
        glBegin(GL_POINTS);

        CVertex* nowV = nowS->GetV();

        while (nowV != NULL)
        {
            glVertex2f(nowV->GetX(), nowV->GetY());

            nowV = nowV->GetNext();
        }

        glEnd();
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(10);
        glBegin(GL_LINE_STRIP);

        nowV = nowS->GetV();

        while (nowV != NULL)
        {
            glVertex2f(nowV->GetX(), nowV->GetY());

            nowV = nowV->GetNext();
        }
        glEnd();
        nowS = nowS->GetNextS();

    }
}


void CAdminControl::AddShape()
{
    CShape* newCShape = new CShape();
    newCShape->SetNextS(shape_head);
    shape_head = newCShape;
}

//2“_ŠÔ‚Ì‹——£‚ð‹‚ß•Ô‹p‚·‚éŠÖ”
float CAdminControl::Distance(CVertex* s, float x, float y)
{
    float d;
    float X, Y;

    X = x - s->GetX();
    Y = y - s->GetY();

    d = sqrt(pow(X, 2) + pow(Y, 2));

    return d;
}

void CAdminControl::CreateShape(float x, float y)
{
    if (shape_head == NULL) {
        AddShape();
    }
    if (shape_head->CountVertex() < 3) {
        shape_head->AddVertex(x, y);
    }
    else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
        shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
        AddShape();
    }
    else {
        shape_head->AddVertex(x, y);
    }
}


