#include "pch.h"
#include "AdminControl.h"
#include "Vertex.h"

#define PI 3.14159265
CVertex* vertex_head = NULL;

CAdminControl::CAdminControl() {

}

CAdminControl::~CAdminControl() {
    shape_head->FreeShape();
}

void CAdminControl::Draw() {

    CShape* nowS = shape_head;

    while (nowS != NULL)
    {

        CVertex* nowV = nowS->GetV();

        while (nowV != NULL)
        {
            if (nowV->GetSelectVertexFlag() == false) {
                glColor3f(1.0, 1.0, 1.0); //��
            }
            else {
                glColor3f(0, 1.0, 1.0); //�V�A��
            }
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex2f(nowV->GetX(), nowV->GetY());

            nowV = nowV->GetNext();
        }

        glEnd();

        nowV = nowS->GetV();

        while (nowV != NULL)
        {
            if (nowV->GetSelectLineFlag() == false) {
                glColor3f(1.0, 1.0, 1.0); //��
            }
            else {
                glColor3f(0, 1.0, 1.0); //�V�A��
            }
            glLineWidth(1);
            glBegin(GL_LINE_STRIP);

            glVertex2f(nowV->GetX(), nowV->GetY());

            if (nowV->GetNext() != NULL) {
                glVertex2f(nowV->GetNext()->GetX(), nowV->GetNext()->GetY());
            }
            nowV = nowV->GetNext();
        }
        glEnd();


        //SelectShapeFlag��true�̎��A�`�����h��Ԃ�
        if (nowS->GetSelectShapeFlag() == true) {
            DrawShape(nowS);
        }

        nowS = nowS->GetNextS();

    }

    //AxisFlag��true�̎��A���W����`�悷��
    if (AxisFlag == true) {
        DrawAxis();
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


//���������������̔���֐��i�������Ă����ꍇtrue�j
bool CAdminControl::CrossJudge(CShape* startS, CVertex* startV, float x, float y)
{
    bool result = false;
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

    //shape_head�̃��X�g(���ݕ`���Ă���}�`)���̎���������
    for (CVertex* vp = startV; vp->GetNext()->GetNext() != NULL; vp = vp->GetNext()) {
        s1 = vp;
        s2 = vp->GetNext();

        G1 = Gaiseki(Vector(s1, s2), Vector(s1, g1));
        G2 = Gaiseki(Vector(s1, s2), Vector(s1, g2));
        G3 = Gaiseki(Vector(g1, g2), Vector(g1, s1));
        G4 = Gaiseki(Vector(g1, g2), Vector(g1, s2));

        if (G1 * G2 <= 0 && G3 * G4 <= 0) {
            result = true;
        }
    }

    //���łɊ������Ă���}�`�Ƃ̑���������
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        for (CVertex* vp = sp->GetV(); vp->GetNext() != NULL; vp = vp->GetNext()) {
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
bool CAdminControl::CrossJudge2(CShape* startS, CVertex* startV, float x, float y)
{
    bool result = false;
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

        for (CVertex* vp = sp->GetV(); vp->GetNext() != NULL; vp = vp->GetNext()) {
            s1 = vp;
            s2 = vp->GetNext();

            G1 = Gaiseki(Vector(s1, s2), Vector(s1, g1));
            G2 = Gaiseki(Vector(s1, s2), Vector(s1, g2));
            G3 = Gaiseki(Vector(g1, g2), Vector(g1, s1));
            G4 = Gaiseki(Vector(g1, g2), Vector(g1, s2));

            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                result = true;
            }
        }
    }


    return result;
}

//�ł����_������Ă��邩�̔���֐�(����Ă����false��Ԃ�)
bool CAdminControl::NaihouJudge(CShape* startS, float x, float y)
{

    CVertex* s1 = new CVertex(x, y);//����1�̎n�_
    CVertex* g1;//����1�̏I�_
    CVertex* s2 = new CVertex(x, y);;//����2�̎n�_
    CVertex* g2;//����2�̏I�_
    float sum = 0;

    //shape_head�����Ă���
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        //vertex_head����ł����_�����Ă���
        for (CVertex* vp = sp->GetV(); vp->GetNext() != NULL; vp = vp->GetNext()) {
            g1 = vp;
            //2�����̂Ȃ��p�̑��a
            sum = sum + Kakudo(Vector(s1, g1), Vector(s2, g1->GetNext()));
        }
    }

    //����Ă����true��Ԃ�
    if (sum < 0.001 && sum > -0.001) {//����ĂȂ����
        return  false;
    }
    else {//����Ă��
        return true;
    }

}

//�쐬���悤�Ƃ��Ă���}�`�̒��ɐ}�`�����邩�̔���֐�
bool CAdminControl::GaihouJudge(CShape* startS, float x, float y)
{
    CVertex* nowV = startS->GetV();
    CVertex* outV_N = NULL;
    CVertex* a;
    CVertex* b;
    CVertex* Q;
    float sum = 0;
    int c = 0;

    //�O���̐}�`�ȊO�����Ă���
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {
        c = 0;
        //���̐}�`�̓_�����Ă���
        for (CVertex* vp = sp->GetV(); vp != NULL; vp = vp->GetNext()) {
            sum = 0;
            Q = new CVertex(vp->GetX(), vp->GetY());

            //�O���̐}�`�̓_�����Ă���
            for (CVertex* outvp = startS->GetV(); outvp->GetNext() != NULL; outvp = outvp->GetNext()) {
                //g1 = outvp;

                outV_N = outvp->GetNext();

                //2�����̂Ȃ��p�̑��a
                sum = sum + Kakudo(Vector(Q, outvp), Vector(Q, outvp->GetNext()));

            }
            sum = sum + Kakudo(Vector(Q, outV_N), Vector(Q, nowV));

            if (sum > 0.001 || sum < -0.001) {//����Ă���ꍇ
                c++;
            }
        }
        if (c == sp->CountVertex() ){
            return true;
        }
    }

    return false;
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

float CAdminControl::Kakudo(CVertex* a, CVertex* b)
{
    float result;
    float gaiseki = Gaiseki(a, b);
    float naiseki = Naiseki(a, b);

    /*if (gaiseki < 0) {
        gaiseki = gaiseki * -1;
    }*/

    result = atan2(gaiseki, naiseki);

    return result;
}

//���钼���ɑ΂��A2�_�����f����Ă��邩���肷��
bool CAdminControl::BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d)
{
    int result = false;
    float s;
    float t;

    s = (b->GetX() - a->GetX()) * (c->GetY() - a->GetY()) - (c->GetX() - a->GetX()) * (b->GetY() - a->GetY());
    t = (b->GetX() - a->GetX()) * (d->GetY() - a->GetY()) - (d->GetX() - a->GetX()) * (b->GetY() - a->GetY());

    if (s * t < 0) {
        result = true;
    }

    return result;
}

void CAdminControl::FreeMemory()
{
    shape_head->FreeShape();
    //shape_head = NULL;
}

void CAdminControl::CreateShape(float x, float y)
{
    ShapeCloseFlag = false;
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
            if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) != true) {
                shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
                AddShape();
                ShapeCloseFlag = true;
            }else if (CrossJudge(shape_head, shape_head->GetV(), x, y) != true) {
                shape_head->AddVertex(x, y);
            }
        }//��������
        else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == true) {
            //�Ȃɂ����Ȃ�
        }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //�}�`��2�ڈȍ~�̎�
    else if (NaihouJudge(shape_head, x, y) == true) {
        //����Ă���Ή������Ȃ�
        ShapeCloseFlag = true;
    }
    //1�_�ڂ�
    else if (shape_head->CountVertex() < 1) {
        shape_head->AddVertex(x, y);
    }
    //2,3�_�ڂ�
    else if (shape_head->CountVertex() < 3) {
        //��������(����)
        if (CrossJudge2(shape_head, shape_head->GetV(), x, y) == true) {
            //�������Ă���Ȃɂ����Ȃ�
        }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //4�_�ڈȍ~�̎����邩����
    else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
        if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) == true) {
            //���������ĂȂ���Αł�
            if (CrossJudge(shape_head, shape_head->GetV(), x, y) != true) {
                shape_head->AddVertex(x, y);
            }
        }//�O��ĂȂ����
        else if (GaihouJudge(shape_head, x, y) != true) {
            shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
            AddShape();
            ShapeCloseFlag = true;
        }
    }//��������
    else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == true) {
        //�Ȃɂ����Ȃ�
    }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
    else {
        shape_head->AddVertex(x, y);
    }
}

//���W����`�悷��
void CAdminControl::DrawAxis()
{
    glBegin(GL_LINES);
    // x��
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    // y��
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    // z��
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
}

//�I�������_�̐F��ς���i���ۂɐF��ς���̂�Draw()���j
int CAdminControl::SelectVertex(float x, float y)
{
    int c = 0;

    //�e�}�`�Ɗe�_�����Ă���
    //�}�`�͍���shape_head�ȊO�A�_�͎n�_�ȊO������
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
            if (Distance(nowV, x, y) <= 0.05) {
                c++;
                if (nowV->GetSelectVertexFlag() == false) {
                    NotSelectFlagReset();
                    nowV->SetSelectVertexFlag(true);
                }
                else {
                    nowV->SetSelectVertexFlag(false);
                }
            }
        }
    }

    if (c > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

//��I���̃t���O�����Z�b�g����֐�
void CAdminControl::NotSelectFlagReset()
{

    CVertex* g1;
    CVertex* g2;

    //�e�}�`�Ɗe�_�����Ă���
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        nowS->SetSelectShapeFlag(false);
        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            nowV->SetSelectVertexFlag(false);
            nowV->SetSelectLineFlag(false);
        }
    }
}

//�I�������`��̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
void CAdminControl::SelectShape(float x, float y)
{
   //����shape_head�ȊO�̊e�}�`�����Ă���
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (NaihouJudge2(nowS, x, y) == true) {
                if (nowS->GetSelectShapeFlag() == false) {
                    NotSelectFlagReset();
                    nowS->SetSelectShapeFlag(true);
                }
                else {
                    nowS->SetSelectShapeFlag(false);
                }
            }
    }
}

//�ł����_������Ă��邩�̔���֐�(�����ɗ^����Shape�̂݌���)
bool CAdminControl::NaihouJudge2(CShape* nowS, float x, float y)
{
    CVertex* s1 = new CVertex(x, y);//����1�̎n�_
    CVertex* g1;//����1�̏I�_
    CVertex* s2 = new CVertex(x, y);;//����2�̎n�_
    CVertex* g2;//����2�̏I�_
    float sum = 0;

        //vertex_head����ł����_�����Ă���
        for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
            g1 = nowV;
            //2�����̂Ȃ��p�̑��a
            sum = sum + Kakudo(Vector(s1, g1), Vector(s2, g1->GetNext()));
        }

        //����Ă���΁Atrue��Ԃ�
        if (sum < 0.001 && sum > -0.001) {//����ĂȂ����
            return  false;
        }
        else {//����Ă��
            return true;
        }

    return false;
}

//�`�����h��Ԃ��֐�
void CAdminControl::DrawShape(CShape* nowS)
{

    //������h��Ԃ��i�������j
    glBegin(GL_TRIANGLES);
    glColor3f(0.8, 0.8, 0.8); //�W���O���[

    for (CVertex* v1 = nowS->GetV(); v1->GetNext() != NULL; v1 = v1->GetNext()) {
        for (CVertex* v2 = nowS->GetV(); v2->GetNext() != NULL; v2 = v2->GetNext()) {
            if (SameVertexJudge(v1, v2) == false) {
                for (CVertex* v3 = nowS->GetV(); v3->GetNext() != NULL; v3 = v3->GetNext()) {
                    if (SameVertexJudge(v1, v3) == false && SameVertexJudge(v2, v3) == false) {
                        if (NaihouJudge3(nowS, v1, v2, v3) == false) { //�O�p�`�̒��ɐ}�`�̓_���Ȃ�
                            if (CrossJudge3(nowS, v1, v2, v3) == false) { //�O�p�`���}�`�̕ӂƌ������Ȃ�
                                if (ShapeInJudge(nowS, v1, v2, v3) == true) { //�O�p�`�̏d�S���}�`�̒��ɂ���
                                    glVertex2f(v1->GetX(), v1->GetY());
                                    glVertex2f(v2->GetX(), v2->GetY());
                                    glVertex2f(v3->GetX(), v3->GetY());
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    glEnd();


    //�_�Ɛ���S�ĐF�ς�
    /*glColor3f(0, 1.0, 1.0); //�V�A��
    glPointSize(10);
    glBegin(GL_POINTS); //�_
    for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd();

    glColor3f(0, 1.0, 1.0); //�V�A��
    glLineWidth(1);
    glBegin(GL_LINE_STRIP); //��
    for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd(); */
}

//�O�p�`���}�`�̊O���ǂ����𔻒肷��(�O�Ȃ�false)
bool CAdminControl::ShapeInJudge(CShape* nowS, CVertex* a, CVertex* b, CVertex* c)
{
    float centerX, centerY;

    centerX = (a->GetX() + b->GetX() + c->GetX()) / 3;
    centerY = (a->GetY() + b->GetY() + c->GetY()) / 3;

    if (NaihouJudge2(nowS, centerX, centerY) == true) { //�O�p�`�̏d�S�������Ȃ��
        return true;
    }
    else {
        return  false;
    }

}

//�`�悷��O�p�`�̕ӂ��A���̕ӂƌ������邩���肷��(�������Ă���true)
bool CAdminControl::CrossJudge3(CShape* startS, CVertex* a, CVertex* b, CVertex* c)
{
    CVertex* nowV = startS->GetV();
    CVertex* s1;
    CVertex* s2;
    CVertex* g1;
    CVertex* g2;
    float G1;
    float G2;
    float G3;
    float G4;
    bool result = false;


    g1 = a;
    g2 = b;
    for (int i = 0; i < 3; i++) {
        if (i == 1) {
            g1 = a;
            g2 = c;
        }
        else if (i == 2) {
            g1 = b;
            g2 = c;
        }

        //shape_head�̃��X�g(���ݕ`���Ă���}�`)���̎���������
        for (CVertex* vp = startS->GetV(); vp != NULL; vp = vp->GetNext()) {
            s1 = vp;
            if (vp->GetNext() != NULL) {
                s2 = s1->GetNext();
            }
            else {
                s2 = startS->GetV();
            }


            G1 = Gaiseki(Vector(s1, s2), Vector(s2, g2));
            G2 = Gaiseki(Vector(s1, s2), Vector(s2, g1));
            G3 = Gaiseki(Vector(g2, g1), Vector(g2, s2));
            G4 = Gaiseki(Vector(g2, g1), Vector(g2, s1));

            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                if (SameVertexJudge(s1, g1) == false && SameVertexJudge(s1, g2) == false) {
                    if (SameVertexJudge(s2, g1) == false && SameVertexJudge(s2, g2) == false) {
                        result = true;
                    }
                }
            }
        }
    }

    return result;
}

//�^����ꂽ2�̓_���������ǂ����𔻒肷��(�����Ȃ�true)
bool CAdminControl::SameVertexJudge(CVertex* a, CVertex* b)
{
    if (a->GetX() - b->GetX() == 0 && a->GetY() - b->GetY() == 0) {
        return true;
    }
    else {
        return false;
    }
}

//�O�p�`�̒��ɓ_������Ă��邩�̔���֐�(����Ă����true��Ԃ�)
bool CAdminControl::NaihouJudge3(CShape* nowS, CVertex* a, CVertex* b, CVertex* c)
{

    float sum = 0;

    //vertex_head����ł����_�����Ă���
    for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        if (SameVertexJudge(nowV, a) == false && SameVertexJudge(nowV, b) == false && SameVertexJudge(nowV, c) == false) {
            sum = sum + Kakudo(Vector(nowV, a), Vector(nowV, b)) + Kakudo(Vector(nowV, b), Vector(nowV, c)) + Kakudo(Vector(nowV, c), Vector(nowV, a));
        }
    }

    //����Ă���΁Atrue��Ԃ�
    if (sum < 0.001 && sum > -0.001) {//����ĂȂ����
        return  false;
    }
    else {//����Ă��
        return true;
    }

    return false;
}

//�I�������ӂ̐F��ς���֐��i���ۂɐF��ς���̂�Draw()���j
int CAdminControl::SelectLine(float x, float y)
{
    int c = 0;
    CVertex* vp = new CVertex(x, y); //�ł����_
    CVertex* vp1;
    CVertex* vp2;
    float d1, d2;

    //�e�}�`�Ɗe�_�����Ă���
    //�}�`�͍���shape_head�ȊO�A�_�͎n�_����I�_������
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {

        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            vp1 = nowV;
            if (nowV->GetNext() != NULL) {
                vp2 = nowV->GetNext();
            }
            else {
                vp2 = nowS->GetV();
            }

            //�ł����_�ƁA�e�ӂ̗��[�̊p�x�̑��a�����߂�
            d1 = Kakudo(Vector(vp1, vp2), Vector(vp1, vp));
            d2 = Kakudo(Vector(vp2, vp1), Vector(vp2, vp));
            if (d1 < 0) {
                d1 = d1 * (-1);
            }
            if (d2 < 0) {
                d2 = d2 * (-1);
            }

            if (d1 <= (PI / 2) && d2 <= (PI / 2)) {
                if (VtoL_Distance(vp1, vp2, vp) <= 0.03) {
                    c++;
                    if (nowV->GetSelectLineFlag() == false) {
                        NotSelectFlagReset();
                        nowV->SetSelectLineFlag(true);
                    }
                    else {
                        nowV->SetSelectLineFlag(false);
                    }
                }
            }
        }

    }

    if (c > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

//�_�Ɛ��̋��������ߕԋp����֐�
float CAdminControl::VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp)
{   
    float X, Y;
    float A, B; //�x�N�g���̑傫��
    float gaiseki, d;

    X = vp2->GetX() - vp1->GetX();
    Y = vp2->GetY() - vp1->GetY();
    A = sqrt(pow(X, 2) + pow(Y, 2));

    X = vp->GetX() - vp1->GetX();
    Y = vp->GetY() - vp1->GetY();
    B = sqrt(pow(X, 2) + pow(Y, 2));

    gaiseki = Gaiseki(Vector(vp1,vp2), Vector(vp1, vp));
    if (gaiseki < 0) {
        gaiseki = gaiseki * (-1);
    }

    d = B * (gaiseki / (A * B));

    return d;

}

//�`�悵���}�`���������Ă��邩�̃t���O���擾����֐�
bool CAdminControl::GetShapeCloseFlag()
{
    return ShapeCloseFlag;
}
