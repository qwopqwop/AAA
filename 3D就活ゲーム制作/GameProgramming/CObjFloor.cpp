#include "CObjFloor.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CTaskManager.h"

//コンストラクタ
//model:モデルのポインタ position:位置 rotation:回転 scale:拡縮
CObjFloor::CObjFloor(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
:mpCollider(0)
{
	mpModel = model;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	//mTag = ENONE;//初期値と一緒

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
CObjFloor::~CObjFloor(){
	//コライダがあれば削除
	if (mpCollider){
		//delete[] 配列を削除
		delete[] mpCollider;
	}
}
//更新処理
void CObjFloor::Update(){
	CCharacter::Update();
}