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
                glColor3f(1.0, 1.0, 1.0); //白
            }
            else {
                glColor3f(1.0, 0, 0); //赤
            }
            glPointSize(10);
            glBegin(GL_POINTS);
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

    //AxisFlagがtrueの時、座標軸を描画する
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

//2点間の距離を求め返却する関数
float CAdminControl::Distance(CVertex* s, float x, float y)
{
    float d;
    float X, Y;

    X = x - s->GetX();
    Y = y - s->GetY();

    d = sqrt(pow(X, 2) + pow(Y, 2));

    return d;
}


//自交差＆他交差の判定関数（交差していた場合、result = 1）
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

    //最後尾まで送り打ちたい点の1つ前をg1に保存
    while (nowV->GetNext() != NULL)
    {
        nowV = nowV->GetNext();
    }
    g1 = nowV;

    //shape_headのリスト(現在描いている図形)内の自交差判定
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

    //すでに完成している図形との他交差判定
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

//他交差（特殊）の判定関数
int CAdminControl::CrossJudge2(CShape* startS, CVertex* startV, float x, float y)
{
    int result = 0;
    CVertex* nowV = startV;
    CVertex* s1;//判定対称の線分の始点
    CVertex* s2;//判定対称の線分の終点
    CVertex* g1;//交差判定したい線分の始点
    CVertex* g2 = new CVertex(x, y);//交差判定したい線分の終点
    float G1;//↓外積１～４
    float G2;
    float G3;
    float G4;

    //最後尾の点まで送り保存(打ちたい点の1つ前をg1に保存)
    while (nowV->GetNext() != NULL)
    {
        nowV = nowV->GetNext();
    }
    g1 = nowV;

    //すでに完成している図形との交差判定
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

//打った点が内包しているかの判定関数
int CAdminControl::NaihouJudge(CShape* startS, CVertex* startV, float x, float y)
{

    CVertex* nowV = startV;
    CVertex* s1 = new CVertex(x, y);//線分1の始点
    CVertex* g1;//線分1の終点
    CVertex* s2 = new CVertex(x, y);;//線分2の始点
    CVertex* g2;//線分2の終点
    float sum = 0;

    //shape_headを見ていく
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        //vertex_headから打った点を見ていく
        for (CVertex* vp = sp->GetV(); vp->GetNext() != NULL; vp = vp->GetNext()) {
            g1 = vp;
            g2 = vp->GetNext();
            float gaiseki = Gaiseki(Vector(s1, g1), Vector(s2, g2));
            if (gaiseki < 0) {
                gaiseki *= -1;
            }
            float naiseki = Naiseki(Vector(s1, g1), Vector(s2, g2));
            sum += atan2(gaiseki, naiseki);
        }

        //内包していれば、1を返す
        if (sum - (2 * PI) < 0.01 && sum - (2 * PI) > -0.01) {
            return 1;
        }
        sum = 0;
    }

    return 0;

}

//作成しようとしている図形の中に図形があるかの判定関数
int CAdminControl::GaihouJudge(CShape* startS, CVertex* startV, float x, float y)
{
    CVertex* nowV = new CVertex(x, y);
    CVertex* s1;//線分1の始点
    CVertex* g1;//線分1の終点
    CVertex* s2;//線分2の始点
    CVertex* g2;//線分2の終点
    float sum = 0;

    //shape_headを見ていく
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {

        //vertex_headから打った点を見ていく
        for (CVertex* vp = sp->GetV(); vp->GetNext() != NULL; vp = vp->GetNext()) {
            s1 = vp;
            s2 = vp;

            //外側の図形のvertex_headを辿る
            for (CVertex* nowvp = startV; nowvp != NULL; nowvp = nowvp->GetNext()) {
                g1 = nowvp;
                if (nowvp->GetNext() != NULL) {
                    g2 = nowvp->GetNext();
                }
                else {
                    g2 = startV;
                }
                float gaiseki = Gaiseki(Vector(s1, g1), Vector(s2, g2));
                if (gaiseki < 0) {
                    gaiseki *= -1;
                }
                float naiseki = Naiseki(Vector(s1, g1), Vector(s2, g2));
                sum += atan2(gaiseki, naiseki);

            }
            //中の点が内包していれば、1を返す
            if (sum - (2 * PI) < 0.1 && sum - (2 * PI) > -0.1) {
                return 1;
            }
            sum = 0;
        }
    }

    return 0;
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

//ある直線に対し、2点が分断されているか判定する
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

void CAdminControl::FreeMemory()
{
    shape_head->FreeShape();
    //shape_head = NULL;
}

void CAdminControl::CreateShape(float x, float y)
{
    ShapeCloseFlag = false;
    //点が何もないとき(1点目)
    if (shape_head == NULL) {
        AddShape();
    }

    //図形が1つ目の時
    if (shape_head->GetNextS() == NULL) {
        //3点打つまでは追加
        if (shape_head->CountVertex() < 3) {
            shape_head->AddVertex(x, y);
        }
        //4点目以降の時閉じるか判定
        else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
            if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) != 1) {
                shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
                AddShape();
                ShapeCloseFlag = true;
            }else if (CrossJudge(shape_head, shape_head->GetV(), x, y) != 1) {
                shape_head->AddVertex(x, y);
            }
        }//交差判定
        else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == 1) {
            //なにもしない
        }//閉じてなくて交差してなければそのまま追加
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //図形が2つ目以降の時
    else if (NaihouJudge(shape_head, shape_head->GetV(), x, y) == 1) {
        //内包していれば何もしない
    }
    //1点目は
    else if (shape_head->CountVertex() < 1) {
        shape_head->AddVertex(x, y);
    }
    //2,3点目は
    else if (shape_head->CountVertex() < 3) {
        //交差判定(特殊)
        if (CrossJudge2(shape_head, shape_head->GetV(), x, y) == 1) {
            //交差してたらなにもしない
        }//閉じてなくて交差してなければそのまま追加
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //4点目以降の時閉じるか判定
    else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
        if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) == 1) {
            //自交差してなければ打つ
            if (CrossJudge(shape_head, shape_head->GetV(), x, y) != 1) {
                shape_head->AddVertex(x, y);
            }
        }//外包していれば
        else if (GaihouJudge(shape_head, shape_head->GetV(), x, y) != 1) {
            shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
            AddShape();
            ShapeCloseFlag = true;
        }
    }//交差判定
    else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == 1) {
        //なにもしない
    }//閉じてなくて交差してなければそのまま追加
    else {
        shape_head->AddVertex(x, y);
    }
}

//座標軸を描画する
void CAdminControl::DrawAxis()
{
    glBegin(GL_LINES);
    // x軸
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    // y軸
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    // z軸
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
}

//頂点・稜線・形状を選択すると色が変わる
void CAdminControl::SelectVertex(float x, float y)
{

    //各図形と各点を見ていく
    //図形は今のshape_head以外、点は始点以外を見る
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            if (Distance(nowV, x, y) <= 0.1) {
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
}

void CAdminControl::NotSelectFlagReset()
{
    //各図形と各点を見ていく
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            nowV->SetSelectVertexFlag(false);//フラグを0に
        }
    }
}

bool CAdminControl::GetShapeCloseFlag()
{
    return ShapeCloseFlag;
}
