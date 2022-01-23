#pragma once

#include <gl/GL.h>
#include "Vertex.h"
#include "Shape.h"
#include "pch.h"

class CAdminControl {

public:
	CAdminControl();
	~CAdminControl();

	//描画処理（問8.1）
	void Draw();

	//点をshape_headに追加する関数
	void AddShape();

	//点をshape_head2に追加する関数
	void AddShape2();

	//点をshape_head3に追加する関数
	void AddShape3();

	//2点間の距離を求め返却する関数
	float Distance(CVertex* s, float x, float y);

	//新しい形状用のリストをcshape_headに追加する関数（新たなキャンバスを追加する）
	void CreateShape(float x, float y);

	//自交差＆他交差の判定関数
	bool CrossJudge(CShape* startS, CVertex* startV, float x, float y);

	//他交差（特殊）の判定関数
	bool CrossJudge2(CShape* startS, CVertex* startV, float x, float y);

	//打った点が内包しているかの判定関数
	bool NaihouJudge(CShape* startS, float x, float y);

	//作成しようとしている図形の中に図形があるかの判定関数
	bool GaihouJudge(CShape* startS, float x, float y);

	//ある直線に対し、2点が分断されているか判定する
	bool BundanJudge(CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//2点のベクトルを計算する
	CVertex* Vector(CVertex* a, CVertex* b);
	float VectorX(CVertex* a, CVertex* b);
	float VectorY(CVertex* a, CVertex* b);

	//2点の外積を計算する
	float Gaiseki(CVertex* a, CVertex* b);
	float Gaiseki(float ax, float ay, float bx, float by);

	//2つの外積から内積を計算する
	float Naiseki(CVertex* a, CVertex* b);
	float Naiseki(float ax, float ay, float bx, float by);

	//2線分の角度を求める
	float Kakudo(CVertex* a, CVertex* b);
	float Kakudo(float ax, float ay, float bx, float by);

	//メモリ解放
	void FreeMemory();

private:
	//形状リストのヘッド
	CShape* shape_head = NULL;
	CShape* shape_head2 = NULL; //形状一時保持用
	CShape* shape_head3 = NULL; //形状塗りつぶし用


public:

	/* ボタン系 */

	//描画モード
	bool DrawButtonFlag = true;


	//編集モード
	bool SelectButtonFlag = false;

	//XYZボタンが押されているかどうかのフラグ
	bool AxisFlag = false;
	//XYZ軸を描画する関数
	void DrawAxis();

	//全削除する関数
	void AllDelete();
	//全削除ボタンが押されているかどうかのフラグ
	bool AllDeleteButtonFlag = false;

	//四角形を描画する関数
	void DrawSquare(float x1, float y1, float x2, float y2);
	//四角形ボタンが押されているかどうかのフラグ
	bool SquareButtonFlag = false;
	//点を全く打っていない状態かどうか
	bool NoVertex = true;
	//NoVertexを取得する関数
	bool GetNoVertex();
	//四角形の中に図形があるか(点を4つ与える)
	bool GaihouJudge5(CShape* nowS, CVertex* a, CVertex* b, CVertex* c, CVertex* d);

	//直線モード
	void DrawStraight(float x, float y);
	//直線ボタンが押されているかどうかのフラグ
	bool StraightButtonFlag = false;
	//直前の直線の向き(1：x正、2：x負、3：y正、4：y負)
	int StraightPreMove = 0;

	//選択中の点または辺をリセットする関数
	void ResetSelectVL();
	//形状コピーボタンが押されているかどうかのフラグ
	bool CopyButtonFlag = false;
	//クリックした場所に形状をコピーする関数
	int DrawCopyShape(float x, float y);
	//交差していた場合、コピーした形状を削除する関数
	void DeleteCopyShape();

	//描画中の点を元に戻す
	void BackVertex();

	//線の太さを変える(+の場合はtrue,-の場合はfalseを受け取る)
	void LineSizeChange(bool f);
	//点のサイズ
	float POINTSIZE = 11.4;
	//線のサイズ
	float LINESIZE = 2.0;

	//形状の塗りつぶしボタンが押されているかどうかのフラグ
	bool ShapeFillButtonFlag = false;
	//形状を塗りつぶす関数
	void Shape_Fill();

	//三角形の中に点が内包しているかの判定関数
	bool NaihouJudge3(CShape* nowS, CVertex* a, CVertex* b, CVertex* c);
	//描画する三角形の辺が、他の辺と交差するか判定する
	bool CrossJudge3(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//三角形が図形の外かどうかを判定する
	bool ShapeInJudge(CShape* startS, CVertex* a, CVertex* b, CVertex* c);
	//与えられた2つの点が同じかどうかを判定する
	bool SameVertexJudge(CVertex* a, CVertex* b);
	//shape_head3をリセットする関数
	void Reset_shape_head3();
	//星型かどうか判定する関数
	bool StarJudge(CShape* nowS);

	//視点変更ボタンが押されているかどうかのフラグ
	bool ViewModeButtonFlag = false;
	//視点の倍率を更新する関数
	void ScaleUpdate(short zDelta);
	//視点の倍率(初期値)
	float Scale = 1.0;
	//視点の平行移動量を更新する関数
	void TranslateUpdate(float Lx, float Ly, float mx, float my);
	//視点の平行移動量
	float TransX = 0.0;
	float TransY = 0.0;
	//1フレーム前のx,y座標
	float PreX = 0.0;
	float PreY = 0.0;
	//視点の回転量を更新する関数
	void RotateUpdate(float Rx, float Ry, float mx, float my);
	//視点の回転量
	float RotateX = 0.0;
	float RotateY = 0.0;
	//1フレーム前のマウスのx,y座標
	float PreMouseX = 0.0;
	float PreMouseY = 0.0;
	//右クリックのフラグ
	bool RButtonDownFlag = false;
	//各数値を初期化する関数
	void InitViewValue();

	//立体ボタンが押されているかどうかのフラグ
	bool SolidButtonFlag = false;
	//底面を塗りつぶす関数
	void Shape_Fill2();
	//形状を立体にする関数
	void SolidMake();
	//奥行の値を更新する関数
	void DepthUpdate(short zDelta);
	//上面の形状を塗りつぶす関数
	void Shape_Fill_Depth();
	//形状が時計回りで描画されたか判定する関数
	bool Clockwise(CShape* nowS);
	//3点が時計回りで描画されたか判定する関数
	bool Clockwise(CVertex* v1, CVertex* v2,CVertex* v3);

	//ワイヤーモデルボタンが押されているかどうかのフラグ
	bool WireButtonFlag = false;
	//形状をワイヤーフレームモデルにする関数
	void WireMake();

	//立体選択ボタンが押されているかどうかのフラグ
	bool SolidSelectButtonFlag = false;
	//SelectShapeFlagをリセットする関数
	void ResetSelectShapeFlag();
	//立体物を選択する関数
	void SelectSolid();
	//選択中のソリッドモデルの辺を描画する関数
	void DrawSelectSolidLine();

public:

	/* 点の選択 */

	//選択した点の色を変える関数（実際に色を変えるのはDraw()内）
	int SelectVertex(float x, float y);


	/* 形状の選択 */

	//選択した形状の色を変える関数（実際に色を変えるのはDraw()内）
	void SelectShape(float x, float y);
	//打った点が内包しているかの判定関数(引数に与えたShapeのみ見る)
	bool NaihouJudge2(CShape* nowS, float x, float y);
	//形状を選択する関数
	void DrawSelectShape(CShape* nowS);


	/* 線の選択 */

	//選択した辺の色を変える関数（実際に色を変えるのはDraw()内）
	int SelectLine(float x, float y);
	//点と線の距離を求め返却する関数
	float VtoL_Distance(CVertex* vp1, CVertex* vp2, CVertex* vp);


	/* 形状が閉じているかどうか(描画中ではないか) */

	//被選択のフラグをリセットする関数
	void NotSelectFlagReset();
	//描画した図形たちが閉じているかのフラグ
	bool ShapeCloseFlag = true;
	//ShapeCloseFlagを取得する関数（WinOGLView.cppで使うために関数にする）
	bool GetShapeCloseFlag();
	//ShapeCloseFlagをセットする関数
	void SetShapeCloseFlag(bool f);


	/* 点の移動 */

	//マウスがムーブした場所に点を描画する関数
	void DrawMoveVertex(float x, float y, float mx, float my);
	//今マウスが動いているかどうか
	bool VertexMoveNowJudge = false;
	//今マウスが動いているかどうかを取得する関数
	bool GetVertexMoveNowJudge();
	//MoveNowJudgeをfalseにする関数
	void ResetVertexMoveNowJudge();
	//終点をホールドした場合はtrue
	bool EndVertexFlag = false;
	//ホールドしている点
	CVertex* HoldV = NULL;
	//ホールドしている点をリセットする関数
	void ResetHoldV();
	//ホールドしてる点があるShape
	CShape* HoldS = NULL;
	//ホールドしてる点があるShapeをリセットする関数
	void ResetHoldS();
	//点移動前の座標
	float originX = 0;
	float originY = 0;
	//点が選択済みかどうか
	bool AlreadySelectVertexFlag = false;
	//点移動時の選択判定のフラグをリセットする関数
	void ResetAlreadySelectVertexFlag();
	//AnyVertexMoveNowFlagをリセットする関数
	void ResetAnyVertexMoveNowFlag();
	//移動させた点によって交差する箇所があるか
	bool VMoveCrossJudge();
	//図形の中に図形があるか(与えたShapeのみ判定対象)
	bool GaihouJudge2(CShape* nowS,CShape* HoldS);
	//与えた辺が他の辺と交差するか判定する
	bool CrossJudge4(CVertex* s1, CVertex* g1);
	//移動させた点によって交差していた場合、点を元に戻す関数
	void VMoveCancel();


	/* 点の挿入 */

	//線にダブルクリックで点を挿入する関数
	void InsertVertex(float x, float y);
	//選択されている線があるか判定する関数
	bool SelectLineNowJudge();


	/* 点の削除 */

	//左クリックで点を削除する関数
	int DeleteVertex(float x, float y);
	//図形の中に図形があるか(引数に削除する予定の点を与える)
	bool GaihouJudge3(CShape* HoldS, CVertex* del);
	//左クリックで形状を削除する関数
	void DeleteShape(float x, float y);


	/* 形状の移動 */

	//マウスがムーブした場所にShapeを描画する関数
	bool DrawMoveShape(float x, float y, float mx, float my);
	//形状の移動量
	float ShapeMoveX = 0.0;
	float ShapeMoveY = 0.0;
	//形状の1フレーム前のx,y座標
	float ShapePreX = 0.0;
	float ShapePreY = 0.0;
	//Shapeが選択済みかどうか
	bool AlreadySelectShapeFlag = false;
	//Shape移動時の選択判定のフラグをリセットする関数
	void ResetAlreadySelectShapeFlag();
	//今マウスが動いているかどうか
	bool ShapeMoveNowJudge = false;
	//今マウスが動いているかどうかを取得する関数
	bool GetShapeMoveNowJudge();
	//MoveNowJudgeをfalseにする関数
	void ResetShapeMoveNowJudge();
	//移動させたShapeによって交差する箇所があるか
	bool ShapeMoveCrossJudge();
	//移動させたShapeによって交差していた場合、Shapeを元に戻す関数
	void ShapeMoveCancel();
	//shape_head2をリセットする関数
	void Reset_shape_head2();


	/* 形状の拡大・縮小 */

	//マウスの中央ボタンが押下されたか
	bool WheelButtonFlag = false;
	//WheelButtonFlagをセット・取得する関数
	void SetWheelButtonFlag(bool f);
	bool GetWheelButtonFlag();
	//基点を描画する関数
	void DrawBasePoint();
	//基点を形状の重心にする関数
	void CenterBase();
	//マウス中央ボタンが押下されたときのx,y座標
	float BaseX = 0;
	float BaseY = 0;
	//clickX_C,clickY_Cをセットする関数
	void SetBaseXY(float x, float y);
	//形状を拡大・縮小する関数
	void DrawExpansionShape(short zDelta);
	//拡大・縮小によって形状が交差した場合、元に戻す関数
	void ShapeExepansionCancel();
	//拡大・縮小しすぎを防ぐ
	bool ExpansionJudge(CRect rect);


	/* 形状の回転 */

	//shape_head2がNULLかどうか判定する関数
	bool shape_head2_NULLJudge();
	//右クリックによって基点が追加されたかどうか
	bool RButtonFlag = false;
	//RButtonFlagをセット・取得する関数
	void SetRButtonFlag(bool f);
	bool GetRButtonFlag();
	//マウスホイールを使用したかどうか(基点追加時に初期化)
	bool WheelUsedFlag = false;
	//WheelUsedFlagをセット・取得する関数
	void SetWheelUsedFlag(bool f);
	bool GetWheelUsedFlag();
	//基点があるかどうかを取得する関数(あればtrue)
	bool GetBasePointFlag();
	//形状を回転する関数
	void DrawRotateShape(short zDelta);
};



