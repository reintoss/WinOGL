#include "pch.h"
#include "AdminControl.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PI 3.14159265
//CVertex* vertex_head = NULL;

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
                if (VertexMoveNowJudge == true) { //点が移動中
                    glColor3f(1.0, 0, 0); //赤
                }
                else { //点を選択
                    glColor3f(0, 1.0, 1.0); //シアン
                }
            }
            else { //通常
                glColor3f(1.0, 1.0, 1.0); //白
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
                glColor3f(1.0, 1.0, 1.0); //白
            }
            else {
                glColor3f(0, 1.0, 1.0); //シアン
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


        //SelectShapeFlagがtrueの時、形状内を塗りつぶす
        if (nowS->GetSelectShapeFlag() == true) {
            DrawShape(nowS);
        }

        nowS = nowS->GetNextS();

    }

    //AxisFlagがtrueの時、座標軸を描画する
    if (AxisFlag == true) {
        DrawAxis();
    }

    if (WheelButtonFlag == true) {
        DrawBasePoint(0.5,0.5);
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

void CAdminControl::AddShape2()
{
    //CShape* newCShape = new CShape();
    //newCShape->SetNextS(shape_head);
    //shape_head = newCShape;

    CShape* newCShape = new CShape();
    newCShape->SetNextS(shape_head2);

    shape_head2 = newCShape;
    if (shape_head2->GetNextS() != NULL) {
        shape_head2->GetNextS()->SetPreS(shape_head2);
    }
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


//自交差＆他交差の判定関数（交差していた場合true）
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

    //すでに完成している図形との他交差判定
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

//他交差（特殊）の判定関数
bool CAdminControl::CrossJudge2(CShape* startS, CVertex* startV, float x, float y)
{
    bool result = false;
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

//打った点が内包しているかの判定関数(内包していればtrueを返す)
bool CAdminControl::NaihouJudge(CShape* startS, float x, float y)
{

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
            //2線分のなす角の総和
            sum = sum + Kakudo(VectorX(s1, g1), VectorY(s1, g1), VectorX(s2, g1->GetNext()), VectorY(s2, g1->GetNext()));
        }
    }

    //内包していればtrueを返す
    if (sum < 0.001 && sum > -0.001) {//内包してなければ
        delete s1;
        delete s2;
        return  false;
    }
    else {//内包してれば
        delete s1;
        delete s2;
        return true;
    }

}

//作成しようとしている図形の中に図形があるかの判定関数(外包していればtrueを返す)
bool CAdminControl::GaihouJudge(CShape* startS, float x, float y)
{
    CVertex* nowV = startS->GetV();
    CVertex* outV_N = NULL;
    CVertex* a;
    CVertex* b;
    CVertex* Q = NULL;
    float sum = 0;
    int c = 0;
    bool result = false;

    //外側の図形以外を見ていく
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {
        c = 0;
        //図形の点を見ていく
        for (CVertex* vp = sp->GetV(); vp != NULL; vp = vp->GetNext()) {
            sum = 0;
            Q = new CVertex(vp->GetX(), vp->GetY());

            //外側の図形の点を見ていく
            for (CVertex* outvp = startS->GetV(); outvp->GetNext() != NULL; outvp = outvp->GetNext()) {
                //g1 = outvp;

                outV_N = outvp->GetNext();

                //2線分のなす角の総和
                sum = sum + Kakudo(VectorX(Q, outvp), VectorY(Q, outvp), VectorX(Q, outvp->GetNext()), VectorY(Q, outvp->GetNext()));

            }
            sum = sum + Kakudo(VectorX(Q, outV_N), VectorY(Q, outV_N), VectorX(Q, nowV), VectorY(Q, nowV));

            if (sum > 0.001 || sum < -0.001) {//内包している場合
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

//ある直線に対し、2点が分断されているか判定する
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


    //形状が何もないとき
    if (shape_head == NULL) {
        AddShape();
        NoVertex = false;
    }

    //図形が1つ目の時
    if (shape_head->GetNextS() == NULL) {
        //3点打つまでは追加
        if (shape_head->CountVertex() < 3) {
            shape_head->AddVertex(x, y);
            ShapeCloseFlag = false;
        }
        //4点目以降の時閉じるか判定
        else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
            //終点を始点につなげることでまたぐかどうか(自交差)
            if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) == true) {
                //↑してる場合自交差してなければ閉じずに打つ
                if (CrossJudge(shape_head, shape_head->GetV(), x, y) == false) {
                    shape_head->AddVertex(x, y);
                }
            }//自交差(始点つなげたときの)してなければ閉じる
            else {
                shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
                AddShape();
                ShapeCloseFlag = true;
            }
        }//交差判定
        else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == true) {
            //なにもしない
        }//閉じてなくて交差してなければそのまま追加
        else {
            shape_head->AddVertex(x, y);
        }
    }
    //図形が2つ目以降の時
    else if (NaihouJudge(shape_head, x, y) == true) {
        //内包していれば何もしない
    }
    //1点目は
    else if (shape_head->CountVertex() < 1) {
        shape_head->AddVertex(x, y);
    }
    //2,3点目は
    else if (shape_head->CountVertex() < 3) {
        //交差判定(特殊)
        if (CrossJudge2(shape_head, shape_head->GetV(), x, y) == true) {
            //交差してたらなにもしない
        }//閉じてなくて交差してなければそのまま追加
        else {
            shape_head->AddVertex(x, y);
            ShapeCloseFlag = false;
        }
    }
    //4点目以降の時閉じるか判定
    else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
        if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) == true) {
            //自交差してなければ打つ
            if (CrossJudge(shape_head, shape_head->GetV(), x, y) != true) {
                shape_head->AddVertex(x, y);
            }
        }//外包してなければ
        else if (GaihouJudge(shape_head, x, y) != true) {
            shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
            AddShape();
            ShapeCloseFlag = true;
        }
    }//交差判定
    else if (CrossJudge(shape_head, shape_head->GetV(), x, y) == true) {
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

//全削除する関数
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
    NoVertex = true;
}

//四角形を描画する関数
void CAdminControl::DrawSquare(float x1, float y1, float x2, float y2)
{
    CVertex* a = NULL;
    CVertex* b = NULL;
    CVertex* c = NULL;
    CVertex* d = NULL;
    float diff_x = x1 - x2;
    float diff_y = y1 - y2;
    bool f = false;
    int count = 0;

    if (diff_x>= 0.05 || diff_x <= -0.05) {
        if (diff_y >= 0.05 || diff_y <= -0.05) {

            if (shape_head == NULL) { //図形が1つ目のとき
                AddShape();
                NoVertex = false;
                f = true;
            }
            else { //図形が2つ目以降のとき
                a = new CVertex(x1, y1);
                b = new CVertex(x2, y1);
                c = new CVertex(x2, y2);
                d = new CVertex(x1, y2);

                //外包判定
                for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
                    if (GaihouJudge5(nowS, a, b, c, d) == true) {
                        count++;
                    }
                }

                //交差判定
                if (count == 0) {
                    if (CrossJudge4(a, b) == false && CrossJudge4(b, c) == false && CrossJudge4(c, d) == false && CrossJudge4(d, a) == false) {
                        f = true;
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

//NoVertexを取得する関数
bool CAdminControl::GetNoVertex()
{
    return NoVertex;
}

//四角形の中に図形があるか(点を4つ与える)
bool CAdminControl::GaihouJudge5(CShape* nowS, CVertex* a, CVertex* b, CVertex* c, CVertex* d)
{

    float sum = 0;

    CVertex* V = nowS->GetV(); //図形の1点

    float kakudo1 = Kakudo(VectorX(V, a), VectorY(V, a), VectorX(V, b), VectorY(V, b));
    float kakudo2 = Kakudo(VectorX(V, b), VectorY(V, b), VectorX(V, c), VectorY(V, c));
    float kakudo3 = Kakudo(VectorX(V, c), VectorY(V, c), VectorX(V, d), VectorY(V, d));
    float kakudo4 = Kakudo(VectorX(V, d), VectorY(V, d), VectorX(V, a), VectorY(V, a));

    //2線分のなす角の総和
    sum = kakudo1 + kakudo2 + kakudo3 + kakudo4;

    if (sum > 0.001 || sum < -0.001) {//内包している場合
        return true;
    }
    else {
        return false;
    }

}

//直線モード
void CAdminControl::DrawStraight(float x, float y)
{

    float kakudo;
    CVertex* a = new CVertex(x, y);
    CVertex* b = NULL;
    CVertex* preV = NULL;
    CVertex* prepreV = NULL;


    //図形が何もないとき
    if (shape_head == NULL) {
        CreateShape(x, y);
    }
    else if (shape_head->GetV() == NULL) { //1点目の場合
        CreateShape(x, y);
    }
    else { //2点目以降の場合
        for (CVertex* nowV = shape_head->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            prepreV = preV;
            preV = nowV;
        }
        b = new CVertex(preV->GetX() + 0.5, preV->GetY());
        kakudo = Kakudo(VectorX(preV, b), VectorY(preV, b), VectorX(preV, a), VectorY(preV, a));
        if (kakudo >= -2.356 && kakudo < -0.785) { //-45°～-135°(y負方向)
            if (prepreV == NULL) {
                CreateShape(preV->GetX(), y);
                StraightPreMove = 4;
            }
            else if (StraightPreMove != 3) {
                CreateShape(preV->GetX(), y);
                StraightPreMove = 4;
            }
        }
        else if (kakudo > 0.785 && kakudo <= 2.356) { //45°～135°(y正方向)
            if (prepreV == NULL) {
                CreateShape(preV->GetX(), y);
                StraightPreMove = 3;
            }else if(StraightPreMove != 4){
                CreateShape(preV->GetX(), y);
                StraightPreMove = 3;
            }  
        }
        else if (kakudo > -0.785 && kakudo <= 0.785) { //-45°～45°(x正方向)
            if (prepreV == NULL) {
                CreateShape(x, preV->GetY());
                StraightPreMove = 1;
            }
            else if (StraightPreMove != 2) {
                CreateShape(x, preV->GetY());
                StraightPreMove = 1;
            }
        }
        else { //135°～-135°(x負方向)
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

//選択した点の色を変える（実際に色を変えるのはDraw()内）
int CAdminControl::SelectVertex(float x, float y)
{
    int c = 0;

    //各図形と各点を見ていく
    //図形は今のshape_head以外、点は始点以外を見る
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

//被選択のフラグをリセットする関数
void CAdminControl::NotSelectFlagReset()
{

    CVertex* g1;
    CVertex* g2;

    //各図形と各点を見ていく
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        nowS->SetSelectShapeFlag(false);
        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            nowV->SetSelectVertexFlag(false);
            nowV->SetSelectLineFlag(false);
        }
    }
}

//選択した形状の色を変える関数（実際に色を変えるのはDraw()内）
void CAdminControl::SelectShape(float x, float y)
{
   //今のshape_head以外の各図形を見ていく
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (NaihouJudge2(nowS, x, y) == true) {
                if (nowS->GetSelectShapeFlag() == false) {
                    NotSelectFlagReset();
                    nowS->SetSelectShapeFlag(true);
                    WheelButtonFlag = false;
                }
                else {
                    if (GetWheelButtonFlag() == false) { //図形が赤色ではない場合
                        nowS->SetSelectShapeFlag(false);
                    }
                }
            }
    }
}

//打った点が内包しているかの判定関数(引数に与えたShapeのみ見る)
bool CAdminControl::NaihouJudge2(CShape* nowS, float x, float y)
{
    CVertex* s1 = new CVertex(x, y);//線分1の始点
    CVertex* g1;//線分1の終点
    CVertex* s2 = new CVertex(x, y);;//線分2の始点
    CVertex* g2;//線分2の終点
    float sum = 0;

        //vertex_headから打った点を見ていく
        for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
            g1 = nowV;
            //2線分のなす角の総和
            sum = sum + Kakudo(VectorX(s1, g1), VectorY(s1, g1), VectorX(s2, g1->GetNext()), VectorY(s2, g1->GetNext()));
        }

        //内包していれば、trueを返す
        if (sum < 0.001 && sum > -0.001) {//内包してなければ
            delete s1;
            delete s2;
            return  false;
        }
        else {//内包してれば
            delete s1;
            delete s2;
            return true;
        }

}

//形状内を塗りつぶす関数
void CAdminControl::DrawShape(CShape* nowS)
{

    /*
    //内部を塗りつぶす
    glBegin(GL_TRIANGLES);
    glColor3f(0.8, 0.8, 0.8); //淡いグレー

    int v1c = 0;
    int v2c = 0;
    int v3c = 0;
    int f = 0;
    CombinationFlag = false;

    for (CVertex* v1 = nowS->GetV(); v1->GetNext() != NULL; v1 = v1->GetNext()) {
        v1c++;
        v2c = 0;
        v3c = 0;
        for (CVertex* v2 = nowS->GetV(); v2->GetNext() != NULL; v2 = v2->GetNext()) {
            v2c++;
            v3c = 0;
            if (SameVertexJudge(v1, v2) == false) {
                for (CVertex* v3 = nowS->GetV(); v3->GetNext() != NULL; v3 = v3->GetNext()) {
                    v3c++;
                    if (SameVertexJudge(v1, v3) == false && SameVertexJudge(v2, v3) == false) {
                        //if (CombinationJudge(nowS, v1c, v2c, v3c, f) == true) { //3点の組み合わせが既出でない
                            f = 1;
                            if (NaihouJudge3(nowS, v1, v2, v3) == false) { //三角形の中に図形の点がない
                                if (CrossJudge3(nowS, v1, v2, v3) == false) { //三角形が図形の辺と交差しない
                                    if (ShapeInJudge(nowS, v1, v2, v3) == true) { //三角形の重心が図形の中にある
                                        glVertex2f(v1->GetX(), v1->GetY());
                                        glVertex2f(v2->GetX(), v2->GetY());
                                        glVertex2f(v3->GetX(), v3->GetY());
                                        if (CombinationFlag == true) {
                                            break;
                                        }
                                    }
                                }
                            }
                        //}
                    }
                }
                if (CombinationFlag == true) {
                    break;
                }
            }
        }
        if (CombinationFlag == true) {
            break;
        }
    }

    glEnd();
    */

    //点と線を全て色変え
    if(ShapeMoveNowJudge == true || WheelButtonFlag == true){ //形状が移動中または拡大・縮小中のとき
        glColor3f(1.0, 0, 0); //赤
    }
    else {
        glColor3f(0, 1.0, 1.0); //シアン
    }

    glPointSize(10);
    glBegin(GL_POINTS); //点
    for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd();

    if (ShapeMoveNowJudge == true || WheelButtonFlag == true) { //形状が移動中または拡大・縮小中のとき
        glColor3f(1.0, 0, 0); //赤
    }
    else {
        glColor3f(0, 1.0, 1.0); //シアン
    }
    glLineWidth(1);
    glBegin(GL_LINE_STRIP); //線
    for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd();
}

//三角形が図形の外かどうかを判定する(外ならfalse)
bool CAdminControl::ShapeInJudge(CShape* nowS, CVertex* a, CVertex* b, CVertex* c)
{
    float centerX, centerY;

    centerX = (a->GetX() + b->GetX() + c->GetX()) / 3;
    centerY = (a->GetY() + b->GetY() + c->GetY()) / 3;

    if (NaihouJudge2(nowS, centerX, centerY) == true) { //三角形の重心が内包するならば
        return true;
    }
    else {
        return  false;
    }

}

//描画する三角形の辺が、他の辺と交差するか判定する(交差してたらtrue)
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

        //shape_headのリスト(現在描いている図形)内の自交差判定
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

//与えられた2つの点が同じかどうかを判定する(同じならtrue)
bool CAdminControl::SameVertexJudge(CVertex* a, CVertex* b)
{
    if (a->GetX() - b->GetX() == 0 && a->GetY() - b->GetY() == 0) {
        return true;
    }
    else {
        return false;
    }
}

//三角形の中に点が内包しているかの判定関数(内包していればtrueを返す)
bool CAdminControl::NaihouJudge3(CShape* nowS, CVertex* a, CVertex* b, CVertex* c)
{

    float sum = 0;

    //vertex_headから打った点を見ていく
    for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        if (SameVertexJudge(nowV, a) == false && SameVertexJudge(nowV, b) == false && SameVertexJudge(nowV, c) == false) {
            float kakudo1 = Kakudo(VectorX(nowV, a), VectorX(nowV, a), VectorX(nowV, b), VectorY(nowV, b));
            float kakudo2 = Kakudo(VectorX(nowV, b), VectorX(nowV, b), VectorX(nowV, c), VectorY(nowV, c));
            float kakudo3 = Kakudo(VectorX(nowV, c), VectorX(nowV, c), VectorX(nowV, a), VectorY(nowV, a));
            sum = sum + kakudo1 + kakudo2 + kakudo3;
        }
    }

    //内包していれば、trueを返す
    if (sum < 0.001 && sum > -0.001) {//内包してなければ
        return  false;
    }
    else {//内包してれば
        return true;
    }

    return false;
}

//3点の組み合わせ判定関数(既出でなければtrue)
bool CAdminControl::CombinationJudge(CShape* nowS, int a, int b, int c, int f)
{

    int tmp = 0;
    int ans;

    //初回のみ配列初期化
    if (f == 0) {
        for (int i = 0; i < 1140; i++) {
            d[i] = 0;
        }
    }

    //昇順にする
    if (a > b) {
        tmp = a;
        a = b;
        b = tmp;
    }
    if (b > c) {
        tmp = b;
        b = c;
        c = tmp;
    }
    if (a > b) {
        tmp = a;
        a = b;
        b = tmp;
    }

    //素数の一意性により、3つの整数に対応した数字が得られる
    ans = pow(5, a) + pow(3, b) + pow(2, c);

    //配列内探索
    for (int i = 0; i < 1140; i++) {
        if (d[i] == 0) {
            tmp = i;
            break;
        }
        if (d[i] == ans) {
            return false;
        }
    }

    d[tmp] = ans;

    int v = Combination(nowS->CountVertex() - 1, 3);
    //配列の中がnC3個の時、今後CombinationJudge()は行わないようにする
    if (v == tmp + 1) {
        CombinationFlag = true;
    }

    return true;
}

//nC3を求める関数
int CAdminControl::Combination(int n, int r)
{

    int ans;
    int i;

    ans = 1;

    for (i = 0; i < r; i++)
    {
        ans *= n;
        n -= 1;
    }

    for (i = 1; i <= r; i++)
    {
        ans /= i;
    }

    return ans;

}



//選択した辺の色を変える関数（実際に色を変えるのはDraw()内）
int CAdminControl::SelectLine(float x, float y)
{
    int c = 0;
    CVertex* vp = new CVertex(x, y); //打った点
    CVertex* vp1;
    CVertex* vp2;
    float d1, d2;

    //各図形と各点を見ていく
    //図形は今のshape_head以外、点は始点から終点を見る
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {

        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            vp1 = nowV;
            if (nowV->GetNext() != NULL) {
                vp2 = nowV->GetNext();
            }
            else {
                vp2 = nowS->GetV();
            }

            //打った点と、各辺の両端の角度の総和を求める
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

//点と線の距離を求め返却する関数
float CAdminControl::VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp)
{   
    float X, Y;
    float A, B; //ベクトルの大きさ
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

//描画した図形たちが閉じているかのフラグを取得する関数
bool CAdminControl::GetShapeCloseFlag()
{
    return ShapeCloseFlag;
}

//ShapeCloseFlagをセットする関数
void CAdminControl::SetShapeCloseFlag(bool f)
{
    ShapeCloseFlag = f;
}

//マウスがムーブした場所に点を描画
void CAdminControl::DrawMoveVertex(float x, float y, float mx, float my)
{

    if (VertexMoveNowJudge == false) {
        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
                if (Distance(nowV, x, y) <= 0.05) {
                    if (nowV->GetSelectVertexFlag() == true) {
                        HoldV = nowV;
                        HoldS = nowS;
                        originX = nowV->GetX();
                        originY = nowV->GetY();
                        AlreadySelectVertexFlag = true;
                        break;
                    }
                }
            }
        }
    }

    //もし選択済みの点がクリックされ、ホールドされてたら
    if (AlreadySelectVertexFlag == true) {
        VertexMoveNowJudge = true;
        //始点もしくは終点を選んだ場合
        if (SameVertexJudge(HoldS->GetV(), HoldV) == true) {
            HoldS->GetV()->SetXY(mx, my);
            HoldV->SetXY(mx, my);
        }//始点終点以外を選んだ場合
        else {
            HoldV->SetXY(mx, my);
        }
    }

}

//今マウスが動いているかどうかを取得する関数
bool CAdminControl::GetVertexMoveNowJudge()
{
    return VertexMoveNowJudge;

}

//MoveNowJudgeをfalseにする関数
void CAdminControl::ResetVertexMoveNowJudge()
{
    VertexMoveNowJudge = false;
}

//ホールドしてる点をリセットする関数
void CAdminControl::ResetHoldV()
{
    HoldV = NULL;
}

//ホールドしてる点があるShapeをリセットする関数
void CAdminControl::ResetHoldS()
{
    HoldS = NULL;
}

//点移動時の選択判定のフラグをリセットする関数
void CAdminControl::ResetAlreadySelectVertexFlag()
{
    AlreadySelectVertexFlag = false;
}

//移動させた点によって交差する箇所があるか
bool CAdminControl::VMoveCrossJudge()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
            if (HoldS != nowS) {
                //移動させた点が他のShapeの中にないか判定
                if (NaihouJudge2(nowS, HoldV->GetX(), HoldV->GetY()) == true) {
                    return true;
                }
                //点が移動させたことによって、Shapeの中に他のShapeがないか判定
                if (GaihouJudge2(nowS, HoldS) == true) {
                    return true;
                }
            }
        }
    }

    //交差判定
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

//図形の中に図形があるか(外包していればtrueを返す)
bool CAdminControl::GaihouJudge2(CShape* nowS, CShape* HoldS)
{
    CVertex* nowV = HoldS->GetV();
    CVertex* outV_N = NULL;
    CVertex* Q = NULL;
    float sum = 0;
    int c = 0;
    bool result = false;

    //外側ではない図形の点を見ていく
    for (CVertex* vp = nowS->GetV(); vp != NULL; vp = vp->GetNext()) {
        sum = 0;
        Q = new CVertex(vp->GetX(), vp->GetY());

        //外側の図形の点を見ていく
        for (CVertex* outvp = HoldS->GetV(); outvp->GetNext() != NULL; outvp = outvp->GetNext()) {
            //g1 = outvp;

            outV_N = outvp->GetNext();

            //2線分のなす角の総和
            sum = sum + Kakudo(VectorX(Q, outvp), VectorY(Q, outvp), VectorX(Q, outvp->GetNext()), VectorY(Q, outvp->GetNext()));

        }
        sum = sum + Kakudo(VectorX(Q, outV_N), VectorY(Q, outV_N), VectorX(Q, nowV), VectorY(Q, nowV));

        if (sum > 0.001 || sum < -0.001) {//内包している場合
            c++;
        }
        delete Q;
    }
    if (c == nowS->CountVertex()) {
        result = true;
    }

    return result;
}

//与えた辺が他の辺と交差するか判定する(交差していればtrueを返す)
bool CAdminControl::CrossJudge4(CVertex* s1, CVertex* g1)
{
    CVertex* s2;
    CVertex* g2;
    float G1;
    float G2;
    float G3;
    float G4;

    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        //他交差
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

                if (G1 * G2 <= 0 && G3 * G4 <= 0) {
                    return true;
                }
            }
        }
        else { //自交差
            for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {

                s2 = nowV;
                g2 = nowV->GetNext();

                G1 = Gaiseki(VectorX(s1, g1), VectorY(s1, g1), VectorX(s1, s2), VectorY(s1, s2));
                G2 = Gaiseki(VectorX(s1, g1), VectorY(s1, g1), VectorX(s1, g2), VectorY(s1, g2));
                G3 = Gaiseki(VectorX(s2, g2), VectorY(s2, g2), VectorX(s2, s1), VectorY(s2, s1));
                G4 = Gaiseki(VectorX(s2, g2), VectorY(s2, g2), VectorX(s2, g1), VectorY(s2, g1));

                if (SameVertexJudge(s2, g1) == false && SameVertexJudge(g2, s1) == false) { //比較線が同じ線かどうか
                    if (SameVertexJudge(s1, s2) == false) { //比較線が次の線かどうか
                        if (SameVertexJudge(g1, g2) == false) { //比較線が前の線かどうか
                            if (G1 * G2 <= 0 && G3 * G4 <= 0) {
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

//移動させた点によって交差していた場合、点を元に戻す関数
void CAdminControl::VMoveCancel()
{
    //始点もしくは終点を選んだ場合
    if (SameVertexJudge(HoldS->GetV(), HoldV) == true) {
        HoldS->GetV()->SetXY(originX, originY);
        HoldV->SetXY(originX, originY);
    }//始点終点以外を選んだ場合
    else {
        HoldV->SetXY(originX, originY);
    }
}

//線にダブルクリックで点を挿入する関数
void CAdminControl::InsertVertex(float x, float y)
{
    CVertex* vp = new CVertex(x, y); //打った点
    CVertex* vp1;
    CVertex* vp2;
    float d1, d2;
    int c = 0;

    //各図形と各点を見ていく
    //図形は今のshape_head以外、点は始点から終点を見る
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {

        for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
            //線が選択されていたら点を挿入(ダブルクリックなのでfalseのとき)
            if (nowV->GetSelectLineFlag() == false) {
                vp1 = nowV;
                if (nowV->GetNext() != NULL) {
                    vp2 = nowV->GetNext();
                }
                else {
                    vp2 = nowS->GetV();
                }

                //打った点と、各辺の両端の角度の総和を求める
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
                    }
                }
            }
        }
    }
}

//左クリックで点を削除する関数
int CAdminControl::DeleteVertex(float x, float y)
{

    //各図形と各点を見ていく
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        CVertex* pre_vp = nowS->GetV();
        CVertex* startV = nowS->GetV();
        for (CVertex* nowV = nowS->GetV()->GetNext(); nowV != NULL; nowV = nowV->GetNext()) {
            CVertex* del = nowV;
            //点が選択されている場合のみ有効
            if (nowV->GetSelectVertexFlag() == true) {
                HoldS = nowS;
                if (nowS->CountVertex() > 4) {
                    if (Distance(nowV, x, y) <= 0.05) {
                        if (del->GetNext() == NULL) { //終点(=始点)を選んだ場合
                            if (CrossJudge4(pre_vp, nowS->GetV()->GetNext()) == false) {
                                if (GaihouJudge3(HoldS, del) == false) {
                                    nowS->SetV(nowS->GetV()->GetNext());
                                    nowV->SetXY(nowS->GetV()->GetX(), nowS->GetV()->GetY());
                                    delete startV;
                                }
                            }
                        }
                        else { //終点(始点)以外を選んだ場合
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

//図形の中に図形があるか(引数に削除する予定の点を与える)
bool CAdminControl::GaihouJudge3(CShape* HoldS, CVertex* del)
{
    CVertex* nowV = shape_head->GetV();
    CVertex* outvpNext = NULL;
    float sum = 0;

    //外側の図形以外を見ていく
    for (CShape* sp = shape_head->GetNextS(); sp != NULL; sp = sp->GetNextS()) {
        sum = 0;
        if (sp != HoldS) {
            CVertex* V = sp->GetV(); //外側の図形以外の1点

            //外側の図形の点を見ていく
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
                        else { //削除する予定の点が終点(=始点)の場合
                            sum = sum + Kakudo(VectorX(V, outvp), VectorY(V, outvp), VectorX(V, HoldS->GetV()->GetNext()), VectorY(V, HoldS->GetV()->GetNext()));
                        }
                    }
                }
            }
            if (sum > 0.001 || sum < -0.001) {//内包している場合
                return true;
            }

        }
    }

    return false;
}

//左クリックで形状を削除する関数
void CAdminControl::DeleteShape(float x, float y)
{
    CShape* preS = shape_head;

    //各図形を見ていく
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        //形状が選択されている場合のみ有効
        if (nowS->GetSelectShapeFlag() == true) {
            if (NaihouJudge2(nowS, x, y) == true) {
                if (nowS->GetNextS() != NULL) {
                    preS->SetNextS(nowS->GetNextS()); //削除する形状の前と後を繋げる
                    nowS->OnlyFreeShape();
                }
                else {
                    preS->SetNextS(NULL); //削除する形状の前をNULLに繋げる
                    nowS->OnlyFreeShape();
                }
            }
            break;
        }
        preS = nowS;
    }
}

//マウスがムーブした場所にShapeを描画する関数
bool CAdminControl::DrawMoveShape(float x, float y, float mx, float my)
{

    bool result = false;
    float Center_X = 0;
    float Center_Y = 0;
    float Diff_X = 0;
    float Diff_Y = 0;
    int c = 0;

    //選択したShapeをホールドする
    if (ShapeMoveNowJudge == false) {
        for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (NaihouJudge2(nowS, x, y) == true) {
                if (nowS->GetSelectShapeFlag() == true) {
                    HoldS = nowS; //元の形状を保持(平行移動させる用)
                    Reset_shape_head2();
                    AddShape2();
                    for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) { //元の形状を保持(交差していた場合に元に戻す用)
                        shape_head2->AddVertex(nowV->GetX(), nowV->GetY());
                    }
                    AlreadySelectShapeFlag = true;
                    result = true;
                    break;
                }
            }
        }
    }

    //もし選択済みのシェイプがクリックされ、ホールドされてたら
    if (AlreadySelectShapeFlag == true) {
        ShapeMoveNowJudge = true;

        for (CVertex* nowV = HoldS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
            Center_X = Center_X + nowV->GetX();
            Center_Y = Center_Y + nowV->GetY();
            c++;
        }

        for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {

            Diff_X = mx - (Center_X / c);
            Diff_Y = my - (Center_Y / c);

            nowV->SetXY(nowV->GetX() + Diff_X, nowV->GetY() + Diff_Y);
        }
    }

    return result;
}

//Shape移動時の選択判定のフラグをリセットする関数
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

//移動させたShapeによって交差する箇所があるか
bool CAdminControl::ShapeMoveCrossJudge()
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
            if (HoldS != nowS) {
                //点が移動させたことによって、Shapeの中に他のShapeがないか判定
                if (GaihouJudge2(nowS, HoldS) == true) {
                    return true;
                }
                if (NaihouJudge2(nowS, HoldS->GetV()->GetX(), HoldS->GetV()->GetY()) == true) {
                    return true;
                }
            }
    }

    //交差判定
    for (CVertex* nowHV = HoldS->GetV(); nowHV->GetNext() != NULL; nowHV = nowHV->GetNext()) {
        if (CrossJudge4(nowHV, nowHV->GetNext()) == true) {
            return true;
        }
    }

    return false;
}

//移動させたShapeによって交差していた場合、Shapeを元に戻す関数
void CAdminControl::ShapeMoveCancel()
{
    CVertex* nowV2 = shape_head2->GetV();

    for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        nowV->SetXY(nowV2->GetX(), nowV2->GetY());
        nowV2 = nowV2->GetNext();
    }
    Reset_shape_head2();
}

//shape_head2をリセットする関数
void CAdminControl::Reset_shape_head2()
{
    shape_head2->FreeShape();
    shape_head2 = NULL;
}

//WheelButtonFlagをセットする関数
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

//WheelButtonFlagを取得する関数
bool CAdminControl::GetWheelButtonFlag()
{
    return WheelButtonFlag;
}

//図形を拡大する基点を描画する関数
void CAdminControl::DrawBasePoint(float x, float y)
{
    for (CShape* nowS = shape_head->GetNextS(); nowS != NULL; nowS = nowS->GetNextS()) {
        if (nowS->GetSelectShapeFlag() == true) {
            if (WheelButtonFlag == true) { //中央ボタン(拡大縮小)
                glColor3f(0, 1.0, 0); //緑
            }
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex2f(BaseX, BaseY);
            glEnd();
        }
    }
}

//BaseX,BaseYをセットする関数
void CAdminControl::SetBaseXY(float x, float y)
{
    BaseX = x;
    BaseY = y;
}

//形状を拡大・縮小する関数
void CAdminControl::DrawExpansionShape(short zDelta)
{
    float k; //倍数
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

//拡大・縮小によって形状が交差した場合、元に戻す関数
void CAdminControl::ShapeExepansionCancel()
{
    float k = 0.9; //倍数
    float a, b;

    for (CVertex* nowV = HoldS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        a = k * (nowV->GetX() - BaseX) + BaseX;
        b = k * (nowV->GetY() - BaseY) + BaseY;
        nowV->SetXY(a, b);
    }
}

//縮小しすぎを防ぐ(問題なしならfalse)
bool CAdminControl::ExpansionJudge()
{

    for (CVertex* nowV = HoldS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        if (nowV != HoldS->GetV()) {
            if (Distance(nowV, HoldS->GetV()->GetX(), HoldS->GetV()->GetY()) <= 0.05) {
                return true;
            }
        }
    }
    return false;
}

//shape_head2がNULLかどうか判定する関数(NULLならtrueを返す)
bool CAdminControl::shape_head2_NULLJudge()
{
    if (shape_head2 == NULL) {
        return true;
    }
    else {
        return false;
    }
}
