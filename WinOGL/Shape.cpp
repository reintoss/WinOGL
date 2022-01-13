#include "pch.h"
#include "Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CShape::CShape(){


}

CShape::CShape(CVertex* new_v){

	vertex_head = new_v;

}

CShape::~CShape(){

	vertex_head->FreeVertex();

}

void CShape::SetV(CVertex* v){

	vertex_head = v;

}

CVertex* CShape::GetV(){

	return vertex_head;

}

//vertex_headにNULLを書き込む
void CShape::SetV_NULL()
{
    vertex_head = NULL;
}


CShape* CShape::SetNextS(CShape* new_nextS){

	next_shape = new_nextS;
	return next_shape;

}

CShape* CShape::GetNextS(){

	return next_shape;

}

CShape* CShape::SetPreS(CShape* new_preS)
{
    return pre_shape = new_preS;
}

CShape* CShape::GetPreS()
{
    return pre_shape;
}

void CShape::FreeShape(){

	CShape* nowS = this;
    while (nowS != NULL) {
        CShape* del_cellS = nowS;
        nowS = nowS->GetNextS();
        delete del_cellS;
    }
}

void CShape::OnlyFreeShape()
{
    CShape* del_cellS = this;
    delete del_cellS;
}

CVertex* CShape::AddVertex(float x, float y)
{
    //ヘッドを読み込む
    CVertex* nowV = vertex_head;
    //打たれた点を保存する
    CVertex* newVertex = new CVertex(x, y);

    //ヘッドが入っていれば
    if (nowV != NULL)
    {
        //最後尾まで送る
        while (nowV->GetNext() != NULL)
        {
            nowV = nowV->GetNext();
        }
        //最後尾の次に追加する
        nowV->SetNext(newVertex);
        nowV->GetNext()->SetPre(nowV);
        //nowV->GetNext()->SetVNumber(nowV->GetVNumber() + 1);
    }
    else
    {
        //打たれた点をヘッドにする
        vertex_head = newVertex;
    }

    return newVertex;
}

//点が3つ以下だと、図形にならない→カウント
int CShape::CountVertex()
{
    int c = 0;

    for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext()) {
        c++;
    }

    return c;
}

void CShape::SetSelectShapeFlag(bool f)
{
    if (f == true) {
        SelectShapeFlag = true;
    }
    else {
        SelectShapeFlag = false;
    }
}

bool CShape::GetSelectShapeFlag()
{
    return SelectShapeFlag;
}

void CShape::SetAnyVertexMoveNowFlag(bool f)
{
    if (f == true) {
        AnyVertexMoveNowFlag = true;
    }
    else {
        AnyVertexMoveNowFlag = false;
    }
}

bool CShape::GetAnyVertexMoveNowFlag()
{
    return AnyVertexMoveNowFlag;
}

