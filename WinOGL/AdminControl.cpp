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

//2�_�Ԃ̋��������ߕԋp����֐�
float CAdminControl::Distance(CVertex* s, float x, float y)
{
    float d;
    float X, Y;

    X = x - s->GetX();
    Y = y - s->GetY();

    d = sqrt(pow(X, 2) + pow(Y, 2));

    return d;
}


//���������������̔���֐��i�������Ă����ꍇ�Aresult = 1�j
int CAdminControl::CrossJudge(CShape* startS, CVertex* startV, float x, float y)
{
    int result = 0;
    CVertex* nowV = startV;
    CVertex* s1;
    CVertex* s2;
    CVertex* g1;
    CVertex* g2 = new CVertex(x, y);
    float G1;
    float G2;
    float G3;
    float G4;

    //�Ō���܂ő���ł������_��1�O��g1�ɕۑ�
    while (nowV->GetNext() != NULL)
    {
        nowV = nowV->GetNext();
    }
    g1 = nowV;

    //cshape_head�̃��X�g(���ݕ`���Ă���}�`)���̎���������
    for (CVertex* vp = startV; vp->GetNext()->GetNext() != NULL; vp = vp->GetNext()) {
        s1 = vp;
        s2 = vp->GetNext();

        G1 = Gaiseki(Vector(s1, s2), Vector(s1, g1));
        G2 = Gaiseki(Vector(s1, s2), Vector(s1, g2));
        G3 = Gaiseki(Vector(g1, g2), Vector(g1, s1));
        G4 = Gaiseki(Vector(g1, g2), Vector(g1, s2));

        if (G1 * G2 <= 0 && G3 * G4 <= 0) {
            result = 1;
        }
    }

    //���łɊ������Ă���}�`�Ƃ̑���������
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        for (CVertex* vp = sp->GetV(); vp->GetNext()->GetNext() != NULL; vp = vp->GetNext()) {
            s1 = vp;
            s2 = vp->GetNext();

            G1 = Gaiseki(Vector(s1, s2), Vector(s1, g1));
            G2 = Gaiseki(Vector(s1, s2), Vector(s1, g2));
            G3 = Gaiseki(Vector(g1, g2), Vector(g1, s1));
            G4 = Gaiseki(Vector(g1, g2), Vector(g1, s2));

            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                result = 1;
            }
        }
    }


    return result;
}

//�������i����j�̔���֐�
int CAdminControl::CrossJudge2(CShape* startS, CVertex* startV, float x, float y)
{
    int result = 0;
    CVertex* nowV = startV;
    CVertex* s1;//����Ώ̂̐����̎n�_
    CVertex* s2;//����Ώ̂̐����̏I�_
    CVertex* g1;//�������肵���������̎n�_
    CVertex* g2 = new CVertex(x, y);//�������肵���������̏I�_
    float G1;//���O�ςP�`�S
    float G2;
    float G3;
    float G4;

    //�Ō���̓_�܂ő���ۑ�(�ł������_��1�O��g1�ɕۑ�)
    while (nowV->GetNext() != NULL)
    {
        nowV = nowV->GetNext();
    }
    g1 = nowV;

    //���łɊ������Ă���}�`�Ƃ̌�������
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        for (CVertex* vp = sp->GetV(); vp->GetNext()->GetNext() != NULL; vp = vp->GetNext()) {
            s1 = vp;
            s2 = vp->GetNext();

            G1 = Gaiseki(Vector(s1, s2), Vector(s1, g1));
            G2 = Gaiseki(Vector(s1, s2), Vector(s1, g2));
            G3 = Gaiseki(Vector(g1, g2), Vector(g1, s1));
            G4 = Gaiseki(Vector(g1, g2), Vector(g1, s2));

            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                result = 1;
            }
        }
    }


    return result;
}


CVertex* CAdminControl::Vector(CVertex* a, CVertex* b)
{
    CVertex* result = new CVertex(b->GetX() - a->GetX(), b->GetY() - a->GetY());

    return result;
}

float CAdminControl::Gaiseki(CVertex* a, CVertex* b)
{
    float result;

    result = (a->GetX() * b->GetY()) - (a->GetY() * b->GetX());

    return result;
}

float CAdminControl::Naiseki(CVertex* a, CVertex* b)
{
    float result;

    result = (a->GetX() * b->GetX()) + (a->GetY() * b->GetY());

    return result;
}

//���钼���ɑ΂��A2�_�����f����Ă��邩���肷��
int CAdminControl::BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d)
{
    int result = 0;
    float s;
    float t;

    s = (b->GetX() - a->GetX()) * (c->GetY() - a->GetY()) - (c->GetX() - a->GetX()) * (b->GetY() - a->GetY());
    t = (b->GetX() - a->GetX()) * (d->GetY() - a->GetY()) - (d->GetX() - a->GetX()) * (b->GetY() - a->GetY());

    if (s * t < 0) {
        result = 1;
    }

    return result;
}

void CAdminControl::CreateShape(float x, float y)
{
    //�_�������Ȃ��Ƃ�(1�_��)
    if (shape_head == NULL) {
        AddShape();
    }

    //�}�`��1�ڂ̎�
    if (shape_head->GetNextS() == NULL) {
        //3�_�ł܂ł͒ǉ�
        if (shape_head->CountVertex() < 3) {
            shape_head->AddVertex(x, y);
        }
        //4�_�ڈȍ~�̎����邩����
        else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
            if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) != 1) {
                shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
                AddShape();
            }
        }//��������
        else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == 1) {
            //�Ȃɂ����Ȃ�
        }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //�}�`��2�ڈȍ~�̎�
    else if (shape_head->CountVertex() < 1) {
        shape_head->AddVertex(x, y);
    }
    //3�_�ڂ܂ł�
    else if (shape_head->CountVertex() < 3) {
        //��������(����)
        if (CrossJudge2(shape_head, shape_head->GetV(), x, y) == 1) {
            //�������Ă���Ȃɂ����Ȃ�
        }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //4�_�ڈȍ~�̎����邩����
    else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
        if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) != 1) {
            shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
            AddShape();
        }
    }//��������
    else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == 1) {
        //�Ȃɂ����Ȃ�
    }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
    else {
        shape_head->AddVertex(x, y);
    }
}



