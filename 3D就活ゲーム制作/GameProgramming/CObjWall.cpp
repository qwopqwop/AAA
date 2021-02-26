//#include "CObjWall.h"
//
//////コンストラクタ
//////model:モデルのポインタ position:位置 rotation:回転 scale:拡縮
////CObjWall::CObjWall(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
////:mpCollider(0)
////{
////	mpModel = model;
////	mPosition = position;
////	mRotation = rotation;
////	mScale = scale;
////		
////	mTag = EWALL;
////
////	CCharacter::Update();
////	//モデルの三角形の数分、コライダの配列を作成します
////	mpCollider = new CCollider[model->mTriangles.size()];
////	for (int i = 0; i < model->mTriangles.size(); i++){
////		//コライダを三角形コライダで設定していきます
////		mpCollider[i].SetTriangle(this,
////			model->mTriangles[i].mV[0],
////			model->mTriangles[i].mV[1],
////			model->mTriangles[i].mV[2]);
////		mpCollider[i].ChangePriority();
////	}
////}
//
////デストラクタ
//CObjWall::~CObjWall(){
//	//コライダがあれば削除
//	if (mpCollider){
//		//delete[] 配列を削除
//		delete[] mpCollider;
//	}
//}
////void CObjWall::Collision(CCollider *m, CCollider *y){
////	//
////}
//void CObjWall::Update(){
//	CCharacter::Update();
//}
////CObjWall::CObjWall(){
////
////}



#include "CObjWall.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CTaskManager.h"

#define DOOR_RANGEOFMOTION 90 //ドアの可動域
#define DOOR_OPENSPEED 2 //ドアの開く速度
#define ROLLINGSPEED 1.0f //回転する足場の回転速度

#define G (9.8f / 120.0f)//重力加速度//60.0f

//コンストラクタ
//model:モデルのポインタ position:位置 rotation:回転 scale:拡縮
CObjWall::CObjWall(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
:mpCollider(0)
{
	mpModel = model;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	mTag = EWALL;
	
	CCharacter::Update();
	//モデルの三角形の数分、コライダの配列を作成します
	mpCollider = new CCollider[model->mTriangles.size()];
	for (int i = 0; i < model->mTriangles.size(); i++){
		//コライダを三角形コライダで設定していきます
		mpCollider[i].SetTriangle(this,
			model->mTriangles[i].mV[0],
			model->mTriangles[i].mV[1],
			model->mTriangles[i].mV[2]);
		mpCollider[i].ChangePriority();
	}
}
//デストラクタ
CObjWall::~CObjWall(){
	//コライダがあれば削除
	if (mpCollider){
		//delete[] 配列を削除
		delete[] mpCollider;
	}
}

void CObjWall::Update(){
	CCharacter::Update();
}