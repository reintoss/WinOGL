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
                glColor3f(0, 1.0, 1.0); //シアン
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


//自交差＆他交差の判定関数（交差していた場合true）
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
            result = true;
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

//打った点が内包しているかの判定関数(内包していればfalseを返す)
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
            sum = sum + Kakudo(Vector(s1, g1), Vector(s2, g1->GetNext()));
        }
    }

    //内包していればtrueを返す
    if (sum < 0.001 && sum > -0.001) {//内包してなければ
        return  false;
    }
    else {//内包してれば
        return true;
    }

}

//作成しようとしている図形の中に図形があるかの判定関数
bool CAdminControl::GaihouJudge(CShape* startS, float x, float y)
{
    CVertex* nowV = startS->GetV();
    CVertex* outV_N = NULL;
    CVertex* a;
    CVertex* b;
    CVertex* Q;
    float sum = 0;
    int c = 0;

    //外側の図形以外を見ていく
    for (CShape* sp = startS->GetNextS(); sp != NULL; sp = sp->GetNextS()) {
        c = 0;
        //中の図形の点を見ていく
        for (CVertex* vp = sp->GetV(); vp != NULL; vp = vp->GetNext()) {
            sum = 0;
            Q = new CVertex(vp->GetX(), vp->GetY());

            //外側の図形の点を見ていく
            for (CVertex* outvp = startS->GetV(); outvp->GetNext() != NULL; outvp = outvp->GetNext()) {
                //g1 = outvp;

                outV_N = outvp->GetNext();

                //2線分のなす角の総和
                sum = sum + Kakudo(Vector(Q, outvp), Vector(Q, outvp->GetNext()));

            }
            sum = sum + Kakudo(Vector(Q, outV_N), Vector(Q, nowV));

            if (sum > 0.001 || sum < -0.001) {//内包している場合
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
            if (CrossJudge(shape_head, shape_head->GetV()->GetNext(), shape_head->GetV()->GetX(), shape_head->GetV()->GetY()) != true) {
                shape_head->AddVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
                AddShape();
                ShapeCloseFlag = true;
            }else if (CrossJudge(shape_head, shape_head->GetV(), x, y) != true) {
                shape_head->AddVertex(x, y);
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
        ShapeCloseFlag = true;
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

//選択した点の色を変える（実際に色を変えるのはDraw()内）
int CAdminControl::SelectVertex(float x, float y)
{
    int c = 0;

    //各図形と各点を見ていく
    //図形は今のshape_head以外、点は始点以外を見る
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
                }
                else {
                    nowS->SetSelectShapeFlag(false);
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
            sum = sum + Kakudo(Vector(s1, g1), Vector(s2, g1->GetNext()));
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

//形状内を塗りつぶす関数
void CAdminControl::DrawShape(CShape* nowS)
{

    //内部を塗りつぶす（実装中）
    glBegin(GL_TRIANGLES);
    glColor3f(0.8, 0.8, 0.8); //淡いグレー

    for (CVertex* v1 = nowS->GetV(); v1->GetNext() != NULL; v1 = v1->GetNext()) {
        for (CVertex* v2 = nowS->GetV(); v2->GetNext() != NULL; v2 = v2->GetNext()) {
            if (SameVertexJudge(v1, v2) == false) {
                for (CVertex* v3 = nowS->GetV(); v3->GetNext() != NULL; v3 = v3->GetNext()) {
                    if (SameVertexJudge(v1, v3) == false && SameVertexJudge(v2, v3) == false) {
                        if (NaihouJudge3(nowS, v1, v2, v3) == false) { //三角形の中に図形の点がない
                            if (CrossJudge3(nowS, v1, v2, v3) == false) { //三角形が図形の辺と交差しない
                                if (ShapeInJudge(nowS, v1, v2, v3) == true) { //三角形の重心が図形の中にある
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


    //点と線を全て色変え
    /*glColor3f(0, 1.0, 1.0); //シアン
    glPointSize(10);
    glBegin(GL_POINTS); //点
    for (CVertex* nowV = nowS->GetV(); nowV->GetNext() != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd();

    glColor3f(0, 1.0, 1.0); //シアン
    glLineWidth(1);
    glBegin(GL_LINE_STRIP); //線
    for (CVertex* nowV = nowS->GetV(); nowV != NULL; nowV = nowV->GetNext()) {
        glVertex2f(nowV->GetX(), nowV->GetY());
    }
    glEnd(); */
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
            sum = sum + Kakudo(Vector(nowV, a), Vector(nowV, b)) + Kakudo(Vector(nowV, b), Vector(nowV, c)) + Kakudo(Vector(nowV, c), Vector(nowV, a));
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

    gaiseki = Gaiseki(Vector(vp1,vp2), Vector(vp1, vp));
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
