#include "pch.h"
#include "AdminControl.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PI 3.14159265

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
            if (nowV->GetSelectVertexFlag() == true) {
                if (VertexMoveNowJudge == true) { //�_���ړ���
                    glColor3f(1.0, 0, 0); //��
                }
                else { //�_��I��
                    glColor3f(0, 1.0, 1.0); //�V�A��
                }
            }
            else { //�ʏ�
                glColor3f(1.0, 1.0, 1.0); //��
            }

            glPointSize(POINTSIZE);
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
            glLineWidth(LINESIZE);
            glBegin(GL_LINE_STRIP);

            glVertex2f(nowV->GetX(), nowV->GetY());

            if (nowV->GetNext() != NULL) {
                glVertex2f(nowV->GetNext()->GetX(), nowV->GetNext()->GetY());
            }
            nowV = nowV->GetNext();
        }
        glEnd();


        //SelectShapeFlag��true�̎��A�`���I������
        if (nowS->GetSelectShapeFlag() == true) {
            DrawSelectShape(nowS);
        }

        nowS = nowS->GetNextS();

    }

    //�`���h��Ԃ�
    if (ShapeFillButtonFlag == true && NoVertex == false) {
        Shape_Fill();
    }

    //���W����`�悷��
    if (AxisFlag == true) {
        DrawAxis();
    }

    //��_��`�悷��
    if (WheelButtonFlag == true || RButtonFlag == true) {
        DrawBasePoint();
    }

}


void CAdminControl::AddShape()
{
    CShape* newCShape = new CShape();
    newCShape->SetNextS(shape_head);
    shape_head = newCShape;

    if (shape_head->GetNextS() != NULL) {
        shape_head->GetNextS()->SetPreS(shape_head);
    }
}

//�_��shape_head2�ɒǉ�����֐�
void CAdminControl::AddShape2()
{

    CShape* newCShape = new CShape();
    newCShape->SetNextS(shape_head2);

    shape_head2 = newCShape;
    if (shape_head2->GetNextS() != NULL) {
        shape_head2->GetNextS()->SetPreS(shape_head2);
    }
}

//�_��shape_head3�ɒǉ�����֐�
void CAdminControl::AddShape3()
{
    CShape* newCShape = new CShape();
    newCShape->SetNextS(shape_head3);

    shape_head3 = newCShape;
    if (shape_head3->GetNextS() != NULL) {
        shape_head3->GetNextS()->SetPreS(shape_head3);
    }
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
bool CAdminControl::CrossJudge(CShape* startS, CVertex* startV, float x1, float y1)
{
    bool result = false;
    CVertex* nowV = startV;
    CVertex* s1;
    CVertex* s2;
    CVertex* g1;
    CVertex* g2 = new CVertex(x1, y1);
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

        G1 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s1, g1), VectorY(s1, g1));
        G2 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s1, g2), VectorY(s1, g2));
        G3 = Gaiseki(VectorX(g1, g2), VectorY(g1, g2), VectorX(g1, s1), VectorY(g1, s1));
        G4 = Gaiseki(VectorX(g1, g2), VectorY(g1, g2), VectorX(g1, s2), VectorY(g1, s2));

        if (G1 == 0 && G2 == 0 && G3 == 0 && G4 == 0) {
            result = false;
        }else if (G1 * G2 <= 0 && G3 * G4 <= 0) {
            result = true;
        }
    }

    //���łɊ������Ă���}�`�Ƃ̑���������
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        for (CVertex* vp = sp->GetV(); vp->GetNext() != NULL; vp = vp->GetNext()) {
            s1 = vp;
            s2 = vp->GetNext();

            G1 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s1, g1), VectorY(s1, g1));
            G2 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s1, g2), VectorY(s1, g2));
            G3 = Gaiseki(VectorX(g1, g2), VectorY(g1, g2), VectorX(g1, s1), VectorY(g1, s1));
            G4 = Gaiseki(VectorX(g1, g2), VectorY(g1, g2), VectorX(g1, s2), VectorY(g1, s2));

            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                result = true;
            }
        }
    }

    delete g2;
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

            G1 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s1, g1), VectorY(s1, g1));
            G2 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s1, g2), VectorY(s1, g2));
            G3 = Gaiseki(VectorX(g1, g2), VectorY(g1, g2), VectorX(g1, s1), VectorY(g1, s1));
            G4 = Gaiseki(VectorX(g1, g2), VectorY(g1, g2), VectorX(g1, s2), VectorY(g1, s2));

            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                result = true;
            }
        }
    }

    delete g2;
    return result;
}

//�ł����_������Ă��邩�̔���֐�(����Ă����true��Ԃ�)
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
            sum = sum + Kakudo(VectorX(s1, g1), VectorY(s1, g1), VectorX(s2, g1->GetNext()), VectorY(s2, g1->GetNext()));
        }
    }

    //����Ă����true��Ԃ�
    if (sum < 0.001 && sum > -0.001) {//����ĂȂ����
        delete s1;
        delete s2;
        return  false;
    }
    else {//����Ă��
        delete s1;
        delete s2;
        return true;
    }

}

//�쐬���悤�Ƃ��Ă���}�`�̒��ɐ}�`�����邩�̔���֐�(�O��Ă����true��Ԃ�)
bool CAdminControl::GaihouJudge(CShape* startS, float x, float y)
{
    CVertex* nowV = startS->GetV();
    CVertex* outV_N = NULL;
    CVertex* Q = NULL;
    float sum = 0;
    int c = 0;
    bool result = false;

    //�O���̐}�`�ȊO�����Ă���
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {
        c = 0;
        //�}�`�̓_�����Ă���
        for (CVertex* vp = sp->GetV(); vp != NULL; vp = vp->GetNext()) {
            sum = 0;
            Q = new CVertex(vp->GetX(), vp->GetY());

            //�O���̐}�`�̓_�����Ă���
            for (CVertex* outvp = startS->GetV(); outvp->GetNext() != NULL; outvp = outvp->GetNext()) {
                //g1 = outvp;

                outV_N = outvp->GetNext();

                //2�����̂Ȃ��p�̑��a
                sum = sum + Kakudo(VectorX(Q, outvp), VectorY(Q, outvp), VectorX(Q, outvp->GetNext()), VectorY(Q, outvp->GetNext()));

            }
            sum = sum + Kakudo(VectorX(Q, outV_N), VectorY(Q, outV_N), VectorX(Q, nowV), VectorY(Q, nowV));

            if (sum > 0.001 || sum < -0.001) {//����Ă���ꍇ
                c++;
            }
            delete Q;
        }
        if (c == sp->CountVertex() ){
            result = true;
        }
    }

    return result;
}


CVertex* CAdminControl::Vector(CVertex* a, CVertex* b)
{
    //CVertex* result = new CVertex(b->GetX() - a->GetX(), b->GetY() - a->GetY());

    CVertex* result = NULL;

    float X = b->GetX() - a->GetX();
    float Y = b->GetY() - a->GetY();

    result = new CVertex(X, Y);

    return result;
}

float CAdminControl::VectorX(CVertex* a, CVertex* b)
{
    float X = b->GetX() - a->GetX();

    return X;
}

float CAdminControl::VectorY(CVertex* a, CVertex* b)
{
    float Y = b->GetY() - a->GetY();

    return Y;
}

float CAdminControl::Gaiseki(CVertex* a, CVertex* b)
{
    float result;

    result = (a->GetX() * b->GetY()) - (a->GetY() * b->GetX());

    return result;
}

float CAdminControl::Gaiseki(float ax, float ay, float bx, float by)
{
    float result;

    result = (ax * by) - (ay * bx);

    return result;
}

float CAdminControl::Naiseki(CVertex* a, CVertex* b)
{
    float result;

    result = (a->GetX() * b->GetX()) + (a->GetY() * b->GetY());

    return result;
}

float CAdminControl::Naiseki(float ax, float ay, float bx, float by)
{
    float result;

    result = (ax * bx) + (ay * by);

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

float CAdminControl::Kakudo(float ax, float ay, float bx, float by)
{
    float result;
    float gaiseki = Gaiseki(ax, ay, bx, by);
    float naiseki = Naiseki(ax, ay, bx, by);

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


    //�`�󂪉����Ȃ��Ƃ�
    if (shape_head == NULL) {
        AddShape();
        NoVertex = false;
    }

    //�}�`��1�ڂ̎�
    if (shape_head->GetNextS() == NULL) {
        //3�_�ł܂ł͒ǉ�
        if (shape_head->CountVertex() < 3) {
            shape_head->AddVertex(x, y);
            ShapeCloseFlag = false;
        }
        //4�_�ڈȍ~�̎����邩����
        else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
            //�I�_���n�_�ɂȂ��邱�Ƃł܂������ǂ���(������)
            if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) == true) {
                //�����Ă�ꍇ���������ĂȂ���Ε����ɑł�
                if (CrossJudge(shape_head, shape_head->GetV(), x, y) == false) {
                    shape_head->AddVertex(x, y);
                }
            }//������(�n�_�Ȃ����Ƃ���)���ĂȂ���Ε���
            else {
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
    //�}�`��2�ڈȍ~�̎�
    else if (NaihouJudge(shape_head, x, y) == true) {
        //����Ă���Ή������Ȃ�
    }
    //1�_�ڂ�
    else if (shape_head->CountVertex() < 1) {
        shape_head->AddVertex(x, y);
        ShapeCloseFlag = false;
    }
    //2,3�_�ڂ�
    else if (shape_head->CountVertex() < 3) {
        //��������(����)
        if (CrossJudge2(shape_head, shape_head->GetV(), x, y) == true) {
            //�������Ă���Ȃɂ����Ȃ�
        }//���ĂȂ��Č������ĂȂ���΂��̂܂ܒǉ�
        else {
            shape_head->AddVertex(x, y);
            ShapeCloseFlag = false;
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

//�S�폜����֐�
void CAdminControl::AllDelete()
{
    if (shape_head != NULL) {
        shape_head->FreeShape();
        shape_head = NULL;
    }
    if (shape_head2 != NULL) {
        shape_head2->FreeShape();
        shape_head2 = NULL;
    }
    if (shape_head3 != NULL) {
        shape_head3->FreeShape();
        shape_head3 = NULL;
    }
    NoVertex = true;
}

//�l�p�`��`�悷��֐�
void CAdminControl::DrawSquare(float x1, float y1, float x2, float y2)
{
    CVertex* a = NULL;
    CVertex* b = NULL;
    CVertex* c = NULL;
    CVertex* d = NULL;
    float diff_x = x1 - x2;
    float diff_y = y1 - y2;
    bool f = true;
    int count = 0;

    if (diff_x>= 0.05 || diff_x <= -0.05) {
        if (diff_y >= 0.05 || diff_y <= -0.05) {

            if (shape_head == NULL) { //�}�`��1�ڂ̂Ƃ�
                AddShape();
                NoVertex = false;
                f = true;
            }
            else { //�}�`��2�ڈȍ~�̂Ƃ�
                a = new CVertex(x1, y1);
                b = new CVertex(x2, y1);
                c = new CVertex(x2, y2);
                d = new CVertex(x1, y2);

                //�����
                for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
                    if (NaihouJudge2(nowS, x1, y1) == true) {
                        f = false;
                    }
                }

                //�O���
                if (f == true) {
                    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
                        if (GaihouJudge5(nowS, a, b, c, d) == true) {
                            count++;
                            f = false;
                            break;
                        }
                    }

                    //��������
                    if (f == true) {
                        if (CrossJudge4(a, b) == false && CrossJudge4(b, c) == false && CrossJudge4(c, d) == false && CrossJudge4(d, a) == false) {
                            f = true;
                        }
                        else {
                            f = false;
                        }
                    }
                }
            }

            if (f == true) {
                shape_head->AddVertex(x1, y1);
                shape_head->AddVertex(x2, y1);
                shape_head->AddVertex(x2, y2);
                shape_head->AddVertex(x1, y2);
                shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
                AddShape();
                ShapeCloseFlag = true;
            }
        }
    }

    delete a;
    delete b;
    delete c;
    delete d;

}

//NoVertex���擾����֐�
bool CAdminControl::GetNoVertex()
{
    return NoVertex;
}

//�l�p�`�̒��ɐ}�`�����邩(�_��4�^����)
bool CAdminControl::GaihouJudge5(CShape* nowS, CVertex* a, CVertex* b, CVertex* c, CVertex* d)
{

    float sum = 0;

    CVertex* V = nowS->GetV(); //�}�`��1�_

    float kakudo1 = Kakudo(VectorX(V, a), VectorY(V, a), VectorX(V, b), VectorY(V, b));
    float kakudo2 = Kakudo(VectorX(V, b), VectorY(V, b), VectorX(V, c), VectorY(V, c));
    float kakudo3 = Kakudo(VectorX(V, c), VectorY(V, c), VectorX(V, d), VectorY(V, d));
    float kakudo4 = Kakudo(VectorX(V, d), VectorY(V, d), VectorX(V, a), VectorY(V, a));

    //2�����̂Ȃ��p�̑��a
    sum = kakudo1 + kakudo2 + kakudo3 + kakudo4;

    if (sum > 0.001 || sum < -0.001) {//����Ă���ꍇ
        return true;
    }
    else {
        return false;
    }

}

//�������[�h
void CAdminControl::DrawStraight(float x, float y)
{

    float kakudo;
    CVertex* a = new CVertex(x, y);
    CVertex* b = NULL;
    CVertex* preV = NULL;
    CVertex* prepreV = NULL;


    //�}�`�������Ȃ��Ƃ�
    if (shape_head == NULL) {
        CreateShape(x, y);
    }
    else if (shape_head->GetV() == NULL) { //1�_�ڂ̏ꍇ
        CreateShape(x, y);
    }
    else { //2�_�ڈȍ~�̏ꍇ
        for (CVertex* nowV = shape_head->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            prepreV = preV;
            preV = nowV;
        }
        b = new CVertex(preV->GetX() + 0.5, preV->GetY());
        kakudo = Kakudo(VectorX(preV, b), VectorY(preV, b), VectorX(preV, a), VectorY(preV, a));
        if (kakudo >= -2.356 && kakudo < -0.785) { //-45���`-135��(y������)
            if (prepreV == NULL) {
                CreateShape(preV->GetX(), y);
                StraightPreMove = 4;
            }
            else if (StraightPreMove != 3) {
                CreateShape(preV->GetX(), y);
                StraightPreMove = 4;
            }
        }
        else if (kakudo > 0.785 && kakudo <= 2.356) { //45���`135��(y������)
            if (prepreV == NULL) {
                CreateShape(preV->GetX(), y);
                StraightPreMove = 3;
            }else if(StraightPreMove != 4){
                CreateShape(preV->GetX(), y);
                StraightPreMove = 3;
            }  
        }
        else if (kakudo > -0.785 && kakudo <= 0.785) { //-45���`45��(x������)
            if (prepreV == NULL) {
                CreateShape(x, preV->GetY());
                StraightPreMove = 1;
            }
            else if (StraightPreMove != 2) {
                CreateShape(x, preV->GetY());
                StraightPreMove = 1;
            }
        }
        else { //135���`-135��(x������)
            if (prepreV == NULL) {
                CreateShape(x, preV->GetY());
                StraightPreMove = 2;
            }
            else if (StraightPreMove != 1) {
                CreateShape(x, preV->GetY());
                StraightPreMove = 2;
            }
        }
    }
    delete a;
    delete b;

}

//�I�𒆂̓_�܂��͕ӂ����Z�b�g����֐�
void CAdminControl::ResetSelectVL()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            if (nowV->GetSelectVertexFlag() == true) {
                nowV->SetSelectVertexFlag(false);
            }
            if (nowV->GetSelectLineFlag() == true) {
                nowV->SetSelectLineFlag(false);
            }
        }
    }
}


//�N���b�N�����ꏊ�Ɍ`����R�s�[����֐�
int CAdminControl::DrawCopyShape(float x, float y)
{

    float CenterX = 0;
    float CenterY = 0;
    float DiffX = 0;
    float DiffY = 0;
    int c = 0;

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        //�`�󂪑I������Ă���ꍇ�̂ݗL��
        if (nowS->GetSelectShapeFlag() == true) {
            for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
                CenterX = CenterX + nowV->GetX();
                CenterY = CenterY + nowV->GetY();
                c++;
            }
            CenterX = CenterX / c; //�R�s�[���`��̏d�Sx���W
            CenterY = CenterY / c; //�R�s�[���`��̏d�Sy���W
            for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
                DiffX = nowV->GetX() - CenterX;
                DiffY = nowV->GetY() - CenterY;
                shape_head->AddVertex(x + DiffX, y + DiffY);
            }
            HoldS = shape_head;
            AddShape();
            return 1;
            break;
        }
    }
    return 0;
}

//�������Ă����ꍇ�A�R�s�[�����`����폜����֐�
void CAdminControl::DeleteCopyShape()
{
    CShape* preS = shape_head;

    preS->SetNextS(HoldS->GetNextS()); //�폜����`��̑O�ƌ���q����
    HoldS->OnlyFreeShape();

}

//�`�撆�̓_�����ɖ߂�
void CAdminControl::BackVertex()
{
    CShape* nowS = shape_head;
    CVertex* preV = nowS->GetV();
    CShape* preS = shape_head;

    for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        if (nowV->GetNext() == NULL) {
            if (nowV != nowS->GetV()) { //�n�_�ł͂Ȃ��ꍇ
                preV->SetNext(NULL);
                delete nowV;
                break;
            }
        }
        preV = nowV;
    }
}

//���̑�����ς���(+�̏ꍇ��true,-�̏ꍇ��false���󂯎��)
void CAdminControl::LineSizeChange(bool f)
{
    if (f == true) { //���𑾂�����
        if (LINESIZE <= 10) {
            POINTSIZE += 0.7;
            LINESIZE += 0.5;
        }
    }
    else { //�����ׂ�����
        if (LINESIZE != 2.0) {
            POINTSIZE -= 0.7;
            LINESIZE -= 0.5;
        }
    }
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
                if (nowV->GetSelectVertexFlag() == false) {
                    NotSelectFlagReset();
                    nowV->SetSelectVertexFlag(true);
                    c = 1;
                }
                else {
                    nowV->SetSelectVertexFlag(false);
                }
            }
        }
    }
        return c;
}

//��I���̃t���O�����Z�b�g����֐�
void CAdminControl::NotSelectFlagReset()
{

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
                    if (WheelButtonFlag == false || RButtonFlag == false) { //�}�`���ԐF�ł͂Ȃ��ꍇ
                        nowS->SetSelectShapeFlag(false);
                    }
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
            sum = sum + Kakudo(VectorX(s1, g1), VectorY(s1, g1), VectorX(s2, g1->GetNext()), VectorY(s2, g1->GetNext()));
        }

        //����Ă���΁Atrue��Ԃ�
        if (sum < 0.001 && sum > -0.001) {//����ĂȂ����
            delete s1;
            delete s2;
            return  false;
        }
        else {//����Ă��
            delete s1;
            delete s2;
            return true;
        }

}

//�`���I������֐�
void CAdminControl::DrawSelectShape(CShape* nowS)
{

    //�_�Ɛ���S�ĐF�ς�
    if(ShapeMoveNowJudge == true){ //�`�󂪈ړ����̂Ƃ�
        glColor3f(1.0, 0, 0); //��
    }
    else if (WheelButtonFlag == true) { //�`�󂪊g��k�����̂Ƃ�
        glColor3f(0, 1.0, 0); //��
    }
    else if (RButtonFlag == true) { //�`�󂪉�]���̂Ƃ�
        glColor3f(1.0, 1.0, 0); //��
    }
    else {
        glColor3f(0, 1.0, 1.0); //�V�A��
    }

    glPointSize(POINTSIZE);
    glBegin(GL_POINTS); //�_
    for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd();

    if (ShapeMoveNowJudge == true) { //�`�󂪈ړ����܂��͊g��E�k�����̂Ƃ�
        glColor3f(1.0, 0, 0); //��
    }
    else if (WheelButtonFlag == true) { //�`�󂪊g��k�����̂Ƃ�
        glColor3f(0, 1.0, 0); //��
    }
    else if (RButtonFlag == true) { //�`�󂪉�]���̂Ƃ�
        glColor3f(1.0, 1.0, 0); //��
    }
    else {
        glColor3f(0, 1.0, 1.0); //�V�A��
    }
    glLineWidth(LINESIZE);
    glBegin(GL_LINE_STRIP); //��
    for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd();
}

//�`���h��Ԃ��֐�
void CAdminControl::Shape_Fill()
{
    CVertex* v1 = NULL;
    CVertex* v2 = NULL;
    CVertex* v3 = NULL;
    CVertex* del = NULL;

    int f = 0, v1f = 0, v2f = 0, v3f = 0;

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        if (nowS->GetAnyVertexMoveNowFlag() == false) {

            //�O�p�`�̏ꍇ�͕��ʂɓh��Ԃ�
            if (nowS->CountVertex() == 4) {
                glBegin(GL_TRIANGLES);
                glColor3f(0.8, 0.8, 0.8); //�W���O���[
                glVertex2f(nowS->GetV()->GetX(), nowS->GetV()->GetY());
                glVertex2f(nowS->GetV()->GetNext()->GetX(), nowS->GetV()->GetNext()->GetY());
                glVertex2f(nowS->GetV()->GetNext()->GetNext()->GetX(), nowS->GetV()->GetNext()->GetNext()->GetY());
                glEnd();
            }
            else if (nowS->CountVertex() >= 5) {

                //shape_head3�Ɍ`����R�s�[
                AddShape3();
                for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
                    shape_head3->AddVertex(nowV->GetX(), nowV->GetY());
                }

                v1 = shape_head3->GetV();
                v2 = v1->GetNext();
                v3 = v2->GetNext();

                while (1) {
                    f = 0;

                    if (shape_head3->CountVertex() == 4) {
                        glBegin(GL_TRIANGLES);
                        glColor3f(0.8, 0.8, 0.8); //�W���O���[
                        glVertex2f(v1->GetX(), v1->GetY());
                        glVertex2f(v2->GetX(), v2->GetY());
                        glVertex2f(v3->GetX(), v3->GetY());
                        glEnd();
                        break;
                    }

                    if (NaihouJudge3(shape_head3, v1, v2, v3) == false) { //�O�p�`�̒��ɐ}�`�̓_���Ȃ�
                        //if (CrossJudge3(shape_head3, v1, v2, v3) == false) { //�O�p�`���}�`�̕ӂƌ������Ȃ�
                        if (ShapeInJudge(shape_head3, v1, v2, v3) == true) { //�O�p�`�̏d�S���}�`�̒��ɂ���
                            f = 1;
                            glBegin(GL_TRIANGLES);
                            glColor3f(0.8, 0.8, 0.8); //�W���O���[
                            glVertex2f(v1->GetX(), v1->GetY());
                            glVertex2f(v2->GetX(), v2->GetY());
                            glVertex2f(v3->GetX(), v3->GetY());
                            glEnd();
                        }
                        //}
                    }

                    if (f == 1) {
                        //v2������
                        if (v2 != shape_head->GetV() && v2->GetNext() != NULL) { //v2���n�_���I�_�ł͂Ȃ��ꍇ
                            v1->SetNext(v3);
                            delete v2;
                            v1 = shape_head3->GetV();
                            v2 = v1->GetNext();
                            v3 = v2->GetNext();
                        }
                        else {
                            del = shape_head3->GetV();
                            shape_head3->SetV(nowS->GetV()->GetNext());
                            v1->SetNext(v3);
                            delete del;
                            delete v2;
                            v1 = shape_head3->GetV();
                            v2 = v1->GetNext();
                            v3 = v2->GetNext();
                        }
                    }
                    else {
                        //v1,v2,v3�����炷
                        if (v3->GetNext() != NULL) { //v3���I�_�ł͂Ȃ��ꍇ
                            v1 = v1->GetNext();
                            v2 = v1->GetNext();
                            v3 = v2->GetNext();
                        }
                        else {
                            v1 = v1->GetNext();
                            v2 = shape_head3->GetV();
                            v3 = v2->GetNext();
                        }
                    }

                }
                Reset_shape_head3();
            }
        }
    }
}

//�O�p�`�̒��ɓ_������Ă��邩�̔���֐�(����Ă����true��Ԃ�)
bool CAdminControl::NaihouJudge3(CShape* nowS2, CVertex* v1, CVertex* v2, CVertex* v3)
{

    float sum = 0;

    //vertex_head����ł����_�����Ă���
    for (CVertex* nowV = nowS2->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        if (SameVertexJudge(nowV, v1) == false && SameVertexJudge(nowV, v2) == false && SameVertexJudge(nowV, v3) == false) {
            float kakudo1 = Kakudo(VectorX(nowV, v1), VectorY(nowV, v1), VectorX(nowV, v2), VectorY(nowV, v2));
            float kakudo2 = Kakudo(VectorX(nowV, v2), VectorY(nowV, v2), VectorX(nowV, v3), VectorY(nowV, v3));
            float kakudo3 = Kakudo(VectorX(nowV, v3), VectorY(nowV, v3), VectorX(nowV, v1), VectorY(nowV, v1));
            sum = sum + kakudo1 + kakudo2 + kakudo3;
        }
    }

    //����Ă���΁Atrue��Ԃ�
    if (sum < 0.001 && sum > -0.001) {//����ĂȂ����
        return  false;
    }
    else {//����Ă��
        return true;
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


            G1 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s2, g2), VectorY(s2, g2));
            G2 = Gaiseki(VectorX(s1, s2), VectorY(s1, s2), VectorX(s2, g1), VectorY(s2, g1));
            G3 = Gaiseki(VectorX(g2, g1), VectorY(g2, g1), VectorX(g2, s2), VectorY(g2, s2));
            G4 = Gaiseki(VectorX(g2, g1), VectorY(g2, g1), VectorX(g2, s1), VectorY(g2, s1));

            if (G1 == 0 && G2 == 0 && G3 == 0 && G4 == 0) {
                result = false;
            }
            else if (G1 * G2 <= 0 && G3 * G4 <= 0) {
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

//shape_head3�����Z�b�g����֐�
void CAdminControl::Reset_shape_head3()
{
    shape_head3->FreeShape();
    shape_head3 = NULL;
}

//���_�̔{�����X�V����֐�
void CAdminControl::ScaleUpdate(short zDelta)
{
    if (zDelta > 0) {
        Scale += 0.1;
    }
    else {
        if (Scale >= 0.1) {
            Scale -= 0.1;
        }
    }

}

//���_�̕��s�ړ��ʂ��X�V����֐�
void CAdminControl::TranslateUpdate(float Lx, float Ly, float mx, float my)
{
    
    //1�t���[���O���Ȃ��ꍇ�APre���W�͍��N���b�N�������W
    if (PreX == 0.0 && PreY == 0.0) {
        PreX = Lx;
        PreY = Ly;
    }

    TransX += mx - PreX;
    TransY += my - PreY;

    PreX = mx;
    PreY = my;
    
}

//���_�̉�]�ʂ��X�V����֐�
void CAdminControl::RotateUpdate(float Rx, float Ry, float mx, float my)
{

    //1�t���[���O���Ȃ��ꍇ�APre���W�͉E�N���b�N�������W
    if (PreMouseX == 0.0 && PreMouseY == 0.0) {
        PreMouseX = Rx;
        PreMouseY = Ry;
    }
    
    RotateX += (my - PreMouseY) * 20.0 * (-1.0);
    RotateY += (mx - PreMouseX) * 20.0;

    PreMouseX = mx;
    PreMouseY = my;
}

//�e���l������������֐�
void CAdminControl::InitViewValue()
{
    Scale = 1.0;
    TransX = 0.0;
    TransY = 0.0;
    RotateX = 0.0;
    RotateY = 0.0;

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
            d1 = Kakudo(VectorX(vp1, vp2), VectorY(vp1, vp2), VectorX(vp1, vp), VectorY(vp1, vp));
            d2 = Kakudo(VectorX(vp2, vp1), VectorY(vp2, vp1), VectorX(vp2, vp), VectorY(vp2, vp));
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
        delete vp;
        return 1;
    }
    else {
        delete vp;
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

    gaiseki = Gaiseki(VectorX(vp1, vp2), VectorY(vp1, vp2), VectorX(vp1, vp), VectorY(vp1, vp));
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

//ShapeCloseFlag���Z�b�g����֐�
void CAdminControl::SetShapeCloseFlag(bool f)
{
    ShapeCloseFlag = f;
}

//�}�E�X�����[�u�����ꏊ�ɓ_��`��
void CAdminControl::DrawMoveVertex(float x, float y, float mx, float my)
{

    if (VertexMoveNowJudge == false) {
        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
                if (Distance(nowV, x, y) <= 0.05) {
                    if (nowV->GetSelectVertexFlag() == true) {
                        HoldV = nowV;
                        if (nowV->GetNext() == NULL) {
                            EndVertexFlag = true;
                        }
                        HoldS = nowS;
                        originX = nowV->GetX();
                        originY = nowV->GetY();
                        AlreadySelectVertexFlag = true;
                        nowS->SetAnyVertexMoveNowFlag(true);
                        break;
                    }
                }
            }
        }
    }

    //�����I���ς݂̓_���N���b�N����A�z�[���h����Ă���
    if (AlreadySelectVertexFlag == true) {
        VertexMoveNowJudge = true;
        //�n�_�������͏I�_��I�񂾏ꍇ
        if (EndVertexFlag == true) {
            HoldS->GetV()->SetXY(mx, my);
            HoldV->SetXY(mx, my);
        }//�n�_�I�_�ȊO��I�񂾏ꍇ
        else {
            HoldV->SetXY(mx, my);
        }
    }

}

//���}�E�X�������Ă��邩�ǂ������擾����֐�
bool CAdminControl::GetVertexMoveNowJudge()
{
    return VertexMoveNowJudge;

}

//MoveNowJudge��false�ɂ���֐�
void CAdminControl::ResetVertexMoveNowJudge()
{
    VertexMoveNowJudge = false;
}

//�z�[���h���Ă�_�����Z�b�g����֐�
void CAdminControl::ResetHoldV()
{
    HoldV = NULL;
}

//�z�[���h���Ă�_������Shape�����Z�b�g����֐�
void CAdminControl::ResetHoldS()
{
    HoldS = NULL;
}

//�_�ړ����̑I�𔻒�̃t���O�����Z�b�g����֐�
void CAdminControl::ResetAlreadySelectVertexFlag()
{
    AlreadySelectVertexFlag = false;
}

//AnyVertexMoveNowFlag�����Z�b�g����֐�
void CAdminControl::ResetAnyVertexMoveNowFlag()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        nowS->SetAnyVertexMoveNowFlag(false);
    }
}

//�ړ��������_�ɂ���Č�������ӏ������邩
bool CAdminControl::VMoveCrossJudge()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
            if (HoldS != nowS) {
                //�ړ��������_������Shape�̒��ɂȂ�������
                if (NaihouJudge2(nowS, HoldV->GetX(), HoldV->GetY()) == true) {
                    return true;
                }
                //�_���ړ����������Ƃɂ���āAShape�̒��ɑ���Shape���Ȃ�������
                if (GaihouJudge2(nowS, HoldS) == true) {
                    return true;
                }
            }
        }
    }

    //��������
    CVertex* preV = HoldS->GetV();
    for (CVertex* vp = HoldS->GetV()->GetNext(); vp != NULL; vp = vp->GetNext()) {
        if (vp == HoldV) {
            if (CrossJudge4(HoldV, preV) == true) {
                return true;
            }
            if (HoldV->GetNext() != NULL) {
                if (CrossJudge4(HoldV, HoldV->GetNext()) == true) {
                    return true;
                }
            }
            else {
                if (CrossJudge4(HoldV,HoldS->GetV()->GetNext()) == true) {
                    return true;
                }
            }
        }
        preV = vp;
    }

    return false;
}

//�}�`�̒��ɐ}�`�����邩(�O��Ă����true��Ԃ�)
bool CAdminControl::GaihouJudge2(CShape* nowS, CShape* HoldS)
{
    CVertex* nowV = HoldS->GetV();
    CVertex* outV_N = NULL;
    CVertex* Q = NULL;
    float sum = 0;
    int c = 0;
    bool result = false;

    //�O���ł͂Ȃ��}�`�̓_�����Ă���
    for (CVertex* vp = nowS->GetV(); vp != NULL; vp = vp->GetNext()) {
        sum = 0;
        Q = new CVertex(vp->GetX(), vp->GetY());

        //�O���̐}�`�̓_�����Ă���
        for (CVertex* outvp = HoldS->GetV(); outvp->GetNext() != NULL; outvp = outvp->GetNext()) {
            //g1 = outvp;

            outV_N = outvp->GetNext();

            //2�����̂Ȃ��p�̑��a
            sum = sum + Kakudo(VectorX(Q, outvp), VectorY(Q, outvp), VectorX(Q, outvp->GetNext()), VectorY(Q, outvp->GetNext()));

        }
        sum = sum + Kakudo(VectorX(Q, outV_N), VectorY(Q, outV_N), VectorX(Q, nowV), VectorY(Q, nowV));

        if (sum > 0.001 || sum < -0.001) {//����Ă���ꍇ
            c++;
        }
        delete Q;
    }
    if (c == nowS->CountVertex()) {
        result = true;
    }

    return result;
}

//�^�����ӂ����̕ӂƌ������邩���肷��(�������Ă����true��Ԃ�)
bool CAdminControl::CrossJudge4(CVertex* s1, CVertex* g1)
{
    CVertex* s2;
    CVertex* g2;
    float G1;
    float G2;
    float G3;
    float G4;

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        //������
        if (nowS != HoldS) {
            for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
                if (nowV->GetNext() != NULL) {
                    s2 = nowV;
                    g2 = nowV->GetNext();
                }
                else {
                    s2 = nowV;
                    g2 = nowS->GetV();
                }
                G1 = Gaiseki(VectorX(s1, g1), VectorY(s1, g1), VectorX(s1, s2), VectorY(s1, s2));
                G2 = Gaiseki(VectorX(s1, g1), VectorY(s1, g1), VectorX(s1, g2), VectorY(s1, g2));
                G3 = Gaiseki(VectorX(s2, g2), VectorY(s2, g2), VectorX(s2, s1), VectorY(s2, s1));
                G4 = Gaiseki(VectorX(s2, g2), VectorY(s2, g2), VectorX(s2, g1), VectorY(s2, g1));

                if (G1 == 0 && G2 == 0 && G3 == 0 && G4 == 0) {

                }
                else if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                    return true;
                }
            }
        }
        else { //������
            for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {

                s2 = nowV;
                g2 = nowV->GetNext();

                G1 = Gaiseki(VectorX(s1, g1), VectorY(s1, g1), VectorX(s1, s2), VectorY(s1, s2));
                G2 = Gaiseki(VectorX(s1, g1), VectorY(s1, g1), VectorX(s1, g2), VectorY(s1, g2));
                G3 = Gaiseki(VectorX(s2, g2), VectorY(s2, g2), VectorX(s2, s1), VectorY(s2, s1));
                G4 = Gaiseki(VectorX(s2, g2), VectorY(s2, g2), VectorX(s2, g1), VectorY(s2, g1));

                if (SameVertexJudge(s2, g1) == false && SameVertexJudge(g2, s1) == false) { //��r�������������ǂ���
                    if (SameVertexJudge(s1, s2) == false) { //��r�������̐����ǂ���
                        if (SameVertexJudge(g1, g2) == false) { //��r�����O�̐����ǂ���
                            if (G1 == 0 && G2 == 0 && G3 == 0 && G4 == 0) {

                            }else if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                                return true;
                            }
                        }
                    }
                }
                else {

                }

            }
        }
    }

    return false;
}

//�ړ��������_�ɂ���Č������Ă����ꍇ�A�_�����ɖ߂��֐�
void CAdminControl::VMoveCancel()
{
    //�n�_�������͏I�_��I�񂾏ꍇ
    if (SameVertexJudge(HoldS->GetV(), HoldV) == true) {
        HoldS->GetV()->SetXY(originX, originY);
        HoldV->SetXY(originX, originY);
    }//�n�_�I�_�ȊO��I�񂾏ꍇ
    else {
        HoldV->SetXY(originX, originY);
    }
}

//���Ƀ_�u���N���b�N�œ_��}������֐�
void CAdminControl::InsertVertex(float x, float y)
{
    CVertex* vp = new CVertex(x, y); //�ł����_
    CVertex* vp1;
    CVertex* vp2;
    float d1, d2;
    int c = 0;

    //�e�}�`�Ɗe�_�����Ă���
    //�}�`�͍���shape_head�ȊO�A�_�͎n�_����I�_������
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {

        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            //�����I������Ă�����_��}��
            if (nowV->GetSelectLineFlag() == true) {
                vp1 = nowV;
                if (nowV->GetNext() != NULL) {
                    vp2 = nowV->GetNext();
                }
                else {
                    vp2 = nowS->GetV();
                }

                //�ł����_�ƁA�e�ӂ̗��[�̊p�x�̑��a�����߂�
                d1 = Kakudo(VectorX(vp1, vp2), VectorY(vp1, vp2), VectorX(vp1, vp), VectorY(vp1, vp));
                d2 = Kakudo(VectorX(vp2, vp1), VectorY(vp2, vp1), VectorX(vp2, vp), VectorY(vp2, vp));
                if (d1 < 0) {
                    d1 = d1 * (-1);
                }
                if (d2 < 0) {
                    d2 = d2 * (-1);
                }

                if (d1 <= (PI / 2) && d2 <= (PI / 2)) {
                    if (VtoL_Distance(vp1, vp2, vp) <= 0.03) {
                        vp->SetNext(nowV->GetNext());
                        nowV->SetNext(vp);
                        NotSelectFlagReset();
                        vp->SetSelectVertexFlag(true);
                    }
                }
            }
        }
    }
}

//�I������Ă���������邩���肷��֐�
bool CAdminControl::SelectLineNowJudge()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            if (nowV->GetSelectLineFlag() == true) {
                return true;
            }
        }
    }
    return false;
}

//���N���b�N�œ_���폜����֐�
int CAdminControl::DeleteVertex(float x, float y)
{

    //�e�}�`�Ɗe�_�����Ă���
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        CVertex* pre_vp = nowS->GetV();
        CVertex* startV = nowS->GetV();
        for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
            CVertex* del = nowV;
            //�_���I������Ă���ꍇ�̂ݗL��
            if (nowV->GetSelectVertexFlag() == true) {
                HoldS = nowS;
                if (nowS->CountVertex() > 4) {
                    if (Distance(nowV, x, y) <= 0.05) {
                        if (del->GetNext() == NULL) { //�I�_(=�n�_)��I�񂾏ꍇ
                            if (CrossJudge4(pre_vp, nowS->GetV()->GetNext()) == false) {
                                if (GaihouJudge3(HoldS, del) == false) {
                                    nowS->SetV(nowS->GetV()->GetNext());
                                    nowV->SetXY(nowS->GetV()->GetX(), nowS->GetV()->GetY());
                                    delete startV;
                                }
                            }
                        }
                        else { //�I�_(�n�_)�ȊO��I�񂾏ꍇ
                            if (CrossJudge4(pre_vp, del->GetNext()) == false) {
                                if (GaihouJudge3(HoldS, del) == false) {
                                    pre_vp->SetNext(del->GetNext());
                                    delete del;
                                }
                            }
                        }
                        return 1;
                    }
                }
            }
            pre_vp = nowV;
        }
    }
    return 0;
}

//�}�`�̒��ɐ}�`�����邩(�����ɍ폜����\��̓_��^����)
bool CAdminControl::GaihouJudge3(CShape* HoldS, CVertex* del)
{
    CVertex* nowV = shape_head->GetV();
    CVertex* outvpNext = NULL;
    float sum = 0;

    //�O���̐}�`�ȊO�����Ă���
    for (CShape* sp = shape_head->GetNextS(); sp != NULL; sp = sp->GetNextS()) {
        sum = 0;
        if (sp != HoldS) {
            CVertex* V = sp->GetV(); //�O���̐}�`�ȊO��1�_

            //�O���̐}�`�̓_�����Ă���
            for (CVertex* outvp = HoldS->GetV()->GetNext(); outvp != NULL; outvp = outvp->GetNext()) {
                if (outvp->GetNext() != NULL) {
                    outvpNext = outvp->GetNext();
                }
                else {
                    outvpNext = HoldS->GetV()->GetNext();
                }

                if (outvp != del) {
                    if (outvpNext != del) {
                        sum = sum + Kakudo(VectorX(V, outvp), VectorY(V, outvp), VectorX(V, outvpNext), VectorY(V, outvpNext));
                    }
                    else {
                        if (outvpNext->GetNext() != NULL) {
                            sum = sum + Kakudo(VectorX(V, outvp), VectorY(V, outvp), VectorX(V, outvpNext->GetNext()), VectorY(V, outvpNext->GetNext()));
                        }
                        else { //�폜����\��̓_���I�_(=�n�_)�̏ꍇ
                            sum = sum + Kakudo(VectorX(V, outvp), VectorY(V, outvp), VectorX(V, HoldS->GetV()->GetNext()), VectorY(V, HoldS->GetV()->GetNext()));
                        }
                    }
                }
            }
            if (sum > 0.001 || sum < -0.001) {//����Ă���ꍇ
                return true;
            }

        }
    }

    return false;
}

//���N���b�N�Ō`����폜����֐�
void CAdminControl::DeleteShape(float x, float y)
{
    CShape* preS = shape_head;

    //�e�}�`�����Ă���
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        //�`�󂪑I������Ă���ꍇ�̂ݗL��
        if (nowS->GetSelectShapeFlag() == true) {
            if (NaihouJudge2(nowS, x, y) == true) {
                if (nowS->GetNextS() != NULL) {
                    preS->SetNextS(nowS->GetNextS()); //�폜����`��̑O�ƌ���q����
                    nowS->OnlyFreeShape();
                }
                else {
                    preS->SetNextS(NULL); //�폜����`��̑O��NULL�Ɍq����
                    nowS->OnlyFreeShape();
                }
            }
            break;
        }
        preS = nowS;
    }

    if (shape_head->GetNextS() == NULL) {
        shape_head->FreeShape();
        shape_head = NULL;
        NoVertex = true;
    }
}

//�}�E�X�����[�u�����ꏊ��Shape��`�悷��֐�
bool CAdminControl::DrawMoveShape(float x, float y, float mx, float my)
{

    bool result = false;
    float Center_X = 0;
    float Center_Y = 0;
    float Diff_X = 0;
    float Diff_Y = 0;
    int c = 0;

    //�I������Shape���z�[���h����
    if (ShapeMoveNowJudge == false) {
        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (NaihouJudge2(nowS, x, y) == true) {
                if (nowS->GetSelectShapeFlag() == true) {
                    HoldS = nowS; //���̌`���ێ�(���s�ړ�������p)
                    Reset_shape_head2();
                    AddShape2();
                    for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) { //���̌`���ێ�(�������Ă����ꍇ�Ɍ��ɖ߂��p)
                        shape_head2->AddVertex(nowV->GetX(), nowV->GetY());
                    }
                    AlreadySelectShapeFlag = true;
                    result = true;
                    break;
                }
            }
        }
    }

    //�����I���ς݂̃V�F�C�v���N���b�N����A�z�[���h����Ă���
    if (AlreadySelectShapeFlag == true) {
        ShapeMoveNowJudge = true;

        //1�t���[���O���Ȃ��ꍇ�APre���W�͍��N���b�N�������W
        if (ShapePreX == 0.0 && ShapePreY == 0.0) {
            ShapePreX = x;
            ShapePreY = y;
        }

        ShapeMoveX = mx - ShapePreX;
        ShapeMoveY = my - ShapePreY;

        ShapePreX = mx;
        ShapePreY = my;

        for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            nowV->SetXY(nowV->GetX() + ShapeMoveX, nowV->GetY() + ShapeMoveY);
        }
    }

    return result;
}

//Shape�ړ����̑I�𔻒�̃t���O�����Z�b�g����֐�
void CAdminControl::ResetAlreadySelectShapeFlag()
{
    AlreadySelectShapeFlag = false;
}

bool CAdminControl::GetShapeMoveNowJudge()
{
    return ShapeMoveNowJudge;
}

void CAdminControl::ResetShapeMoveNowJudge()
{
    ShapeMoveNowJudge = false;
}

//�ړ�������Shape�ɂ���Č�������ӏ������邩
bool CAdminControl::ShapeMoveCrossJudge()
{

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (HoldS != nowS) {
                //�_���ړ����������Ƃɂ���āAShape�̒��ɑ���Shape���Ȃ�������
                if (GaihouJudge2(nowS, HoldS) == true) {
                    return true;
                }
                if (NaihouJudge2(nowS, HoldS->GetV()->GetX(), HoldS->GetV()->GetY()) == true) {
                    return true;
                }
            }
    }

    //��������
    for (CVertex* nowHV = HoldS->GetV(); nowHV->GetNext() != NULL; nowHV = nowHV->GetNext()) {
        if (CrossJudge4(nowHV, nowHV->GetNext()) == true) {
            return true;
        }
    }

    return false;
}

//�ړ�������Shape�ɂ���Č������Ă����ꍇ�AShape�����ɖ߂��֐�
void CAdminControl::ShapeMoveCancel()
{
    CVertex* nowV2 = shape_head2->GetV();

    for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        nowV->SetXY(nowV2->GetX(), nowV2->GetY());
        nowV2 = nowV2->GetNext();
    }
    Reset_shape_head2();
}

//shape_head2�����Z�b�g����֐�
void CAdminControl::Reset_shape_head2()
{
    shape_head2->FreeShape();
    shape_head2 = NULL;
}

//WheelButtonFlag���Z�b�g����֐�
void CAdminControl::SetWheelButtonFlag(bool f)
{
    if (WheelButtonFlag == false) {
        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (nowS->GetSelectShapeFlag() == true) {
                WheelButtonFlag = true;
            }
        }
    }
    else {
        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (nowS->GetSelectShapeFlag() == true) {
                WheelButtonFlag = false;
            }
        }
    }
}

//WheelButtonFlag���擾����֐�
bool CAdminControl::GetWheelButtonFlag()
{
    return WheelButtonFlag;
}

//��_��`�悷��֐�
void CAdminControl::DrawBasePoint()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        if (nowS->GetSelectShapeFlag() == true) {
            if (WheelButtonFlag == true) { //�����{�^��(�g��k��)
                glColor3f(0, 1.0, 0); //��
            }else{ //�E�N���b�N(��])
                glColor3f(1.0, 1.0, 0); //��
            }
            glPointSize(20);
            glBegin(GL_POINTS);
            glVertex2f(BaseX, BaseY);
            glEnd();

            glColor3f(0, 0, 0); //��
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex2f(BaseX, BaseY);
            glEnd();
        }
    }
}

//��_���`��̏d�S�ɂ���֐�
void CAdminControl::CenterBase()
{
    float CenterX = 0;
    float CenterY = 0;
    int c = 0;

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        if (nowS->GetSelectShapeFlag() == true) {
            if (WheelButtonFlag == true) { //�����{�^��(�g��k��)
                glColor3f(0, 1.0, 0); //��
            }
            else { //�E�N���b�N(��])
                glColor3f(1.0, 1.0, 0); //��
            }

            //�d�S�����߂�
            for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
                CenterX = CenterX + nowV->GetX();
                CenterY = CenterY + nowV->GetY();
                c++;
            }
            BaseX = CenterX / c;
            BaseY = CenterY / c;

            glPointSize(20);
            glBegin(GL_POINTS);
            glVertex2f(BaseX, BaseY);
            glEnd();

            glColor3f(0, 0, 0); //��
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex2f(BaseX, BaseY);
            glEnd();
        }
    }
}

//BaseX,BaseY���Z�b�g����֐�
void CAdminControl::SetBaseXY(float x, float y)
{
    BaseX = x;
    BaseY = y;
}

//�`����g��E�k������֐�
void CAdminControl::DrawExpansionShape(short zDelta)
{
    float k; //�{��
    float a, b;
    int f = 0;

    if (zDelta > 0) {
        k = 1.1;
    }
    else {
        k = 0.9;
    }

        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (nowS->GetSelectShapeFlag() == true) {
                if (shape_head2 == NULL) {
                    AddShape2();
                    f = 1;
                }
                for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
                    if (f == 1) {
                        shape_head2->AddVertex(nowV->GetX(), nowV->GetY());
                    }
                    a = k * (nowV->GetX() - BaseX) + BaseX;
                    b = k * (nowV->GetY() - BaseY) + BaseY;
                    nowV->SetXY(a, b);
                }
                HoldS = nowS;
                break;
            }
        }
}

//�g��E�k���ɂ���Č`�󂪌��������ꍇ�A���ɖ߂��֐�
void CAdminControl::ShapeExepansionCancel()
{
    float k = 0.9; //�{��
    float a, b;

    for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        a = k * (nowV->GetX() - BaseX) + BaseX;
        b = k * (nowV->GetY() - BaseY) + BaseY;
        nowV->SetXY(a, b);
    }
}

//�g��E�k����������h��(���Ȃ��Ȃ�false)
bool CAdminControl::ExpansionJudge(CRect rect)
{
    int countV = 0;
    int count = 0;
    float MaxX = 0;
    float MaxY = 0;

    //�E�B���h�E�T�C�Y�̎擾
    if (rect.Width() > rect.Height()) { //�E�B���h�E�������̂Ƃ�
        MaxY = 1.0;
        MaxX = ((float)rect.Width() / rect.Height());
    }
    else { //�E�B���h�E���c���̂Ƃ�
        MaxX = 1.0;
        MaxY = ((float)rect.Height() / rect.Width());
    }

    //�g��E�k�����������ǂ���
    for (CVertex* nowV = HoldS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        countV++;
        if (nowV != HoldS->GetV()) {
            if (Distance(nowV, HoldS->GetV()->GetX(), HoldS->GetV()->GetY()) <= 0.05) {
                return true;
            }
        }
        if (nowV->GetX() >= MaxX || nowV->GetX() <= MaxX * (-1) || nowV->GetY() >= MaxY || nowV->GetY() <= MaxY * (-1)) {
            count++;
        }
    }
    if (countV == count) { //�_���ׂĂ��E�B���h�E�O�̎�
        return true;
    }

    return false;
}

//shape_head2��NULL���ǂ������肷��֐�(NULL�Ȃ�true��Ԃ�)
bool CAdminControl::shape_head2_NULLJudge()
{
    if (shape_head2 == NULL) {
        return true;
    }
    else {
        return false;
    }
}
//RButtonFlag���Z�b�g����֐�
void CAdminControl::SetRButtonFlag(bool f)
{
    RButtonFlag = f;
}

//RButtonFlag���擾����֐�
bool CAdminControl::GetRButtonFlag()
{
    return RButtonFlag;
}

//WheelUsedFlag���Z�b�g�E�擾����֐�
void CAdminControl::SetWheelUsedFlag(bool f)
{
    WheelUsedFlag = f;
}

//WheelUsedFlag���Z�b�g�E�擾����֐�
bool CAdminControl::GetWheelUsedFlag()
{
    return WheelUsedFlag;
}

//��_�����邩�ǂ������擾����֐�(�����true)
bool CAdminControl::GetBasePointFlag()
{
    if (WheelButtonFlag == false && RButtonFlag == false) {
        return false;
    }
    else {
        return true;
    }
}

//�`�����]����֐�
void CAdminControl::DrawRotateShape(short zDelta)
{
    float k; //��]��(���W�A��)
    float a, b;
    int f = 0;

    //1�z�C�[���Ŗ�0.1���W�A��(=��5.7��)��]
    if (zDelta > 0) {
        k = -0.1;
    }
    else {
        k = 0.1;
    }

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        if (nowS->GetSelectShapeFlag() == true) {
            if (shape_head2 == NULL) {
                AddShape2();
                f = 1;
            }
            for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
                if (f == 1) {
                    shape_head2->AddVertex(nowV->GetX(), nowV->GetY());
                }
                a = (nowV->GetX() - BaseX) * cos(k) - (nowV->GetY() - BaseY) * sin(k) + BaseX;
                b = (nowV->GetX() - BaseX) * sin(k) + (nowV->GetY() - BaseY) * cos(k) + BaseY;
                nowV->SetXY(a, b);
            }
            HoldS = nowS;
            break;
        }
    }
}
