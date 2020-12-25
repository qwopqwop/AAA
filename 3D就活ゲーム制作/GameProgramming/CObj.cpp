#include "CObj.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CTaskManager.h"

#define DOOR_RANGEOFMOTION 90 //ドアの可動域
#define DOOR_OPENSPEED 2 //ドアの開く速度
#define ROLLINGSPEED 1.0f //回転する足場の回転速度

//コンストラクタ
//model:モデルのポインタ position:位置 rotation:回転 scale:拡縮
CObj::CObj(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale, int objnum)
:mpCollider(0)
{
	mpModel = model;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mObjNumber = objnum;

	if (mObjNumber == 9){
		mTag = EWATER;
	}
	if (mObjNumber >= 18 && mObjNumber <= 20){
		mTag = ECLEARWATER;
	}
	/*if (mObjNumber == 20){
		mTag = ECLEARWATER;
	}*/

	if (mObjNumber == 101){
		//チェックポイント
		mTag = ECHECKPOINT;
		//優先度変更
		mPriority = 0;
		CTaskManager::Get()->Remove(this);
		CTaskManager::Get()->Add(this);
	}
	if (mObjNumber == 102){
		//チェックポイント
		mTag = ECHECKPOINT2;
		//優先度変更
		mPriority = 0;
		CTaskManager::Get()->Remove(this);
		CTaskManager::Get()->Add(this);
	}
	if (mObjNumber == 103){
		//チェックポイント
		mTag = ECHECKPOINT3;
		//優先度変更
		mPriority = 0;
		CTaskManager::Get()->Remove(this);
		CTaskManager::Get()->Add(this);
	}
	if (mObjNumber == 111){
		//加速床
		mTag = EDASHBOARD;
		//mpCollider->mPosition.mY += 5.0f;
		//mPosition.mY += 5.0f;
	}
	if (mObjNumber == 112){
		//芝生(減速)
		mTag = EGRASS;
	}
	if (mObjNumber == 200){
		//壁
		mTag = EWALL;
	}


	if (mObjNumber == 99 || mObjNumber == 575){
		//当たり判定を生成しない
		//これにより当たり判定の数を増やすほど処理が重くなることが実証された
		//逆に当たり判定を増やさなければいいので、必要ない当たり判定はどんどんカットしていくよ。
	}
	else{
		CCharacter::Update();
		//モデルの三角形の数分、コライダの配列を作成します
		mpCollider = new CCollider[model->mTriangles.size()];
		for (int i = 0; i < model->mTriangles.size(); i++){
			//コライダを三角形コライダで設定していきます
			mpCollider[i].SetTriangle(this,
				model->mTriangles[i].mV[0],
				model->mTriangles[i].mV[1],
				model->mTriangles[i].mV[2]);
			//if (mObjNumber == 111){
			//	//加速床の当たり判定だけ上に移動させる
			//	model->mTriangles[i].mV[0] = model->mTriangles[i].mV[0] + CVector(0.0f, 5.0f, 0.0f);
			//	model->mTriangles[i].mV[1] = model->mTriangles[i].mV[1] + CVector(0.0f, 5.0f, 0.0f);
			//	model->mTriangles[i].mV[2] = model->mTriangles[i].mV[2] + CVector(0.0f, 5.0f, 0.0f);
			//}
			mpCollider[i].ChangePriority();
		}
	}


	//mEnabled = false;
}
//デストラクタ
CObj::~CObj(){
	//コライダがあれば削除
	if (mpCollider){
		//delete[] 配列を削除
		delete[] mpCollider;
	}
}

void CObj::Collision(CCollider *m, CCollider *y){
	/*if (mObjNumber != 9){
		if (CCollider::Collision(m, y)){
			if (y->mpParent->mTag == CCharacter::EPLAYER){
				mEnabled = false;
			}
		}
	}*/

	/*if (y->mpParent->mTag == CCharacter::EBULLET){
		if (CCollider::Collision(m, y)){
			if (y->mpParent->mTag == CCharacter::EBULLET){
				mEnabled = false;
				
			}
		}
	}*/
	/*if (CCollider::Collision(m, y)){
		if (y->mpParent->mTag == CCharacter::EBULLET){
			mEnabled = false;

		}
	}*/

	//if (mObjNumber == 13){//仮
	//	if (CCollider::Collision(m, y)){
	//		if (y->mpParent->mTag == CCharacter::EPLAYER){
	//			mEnabled = false;
	//		}
	//	}
	//}
	
}

void CObj::Update(){
	if (mObjNumber == 18){
		//ボートを取得しないと表示されないよ！
		if (CPlayer::mpPlayer->mHaveBoat == false){
			mScale.mY = 0.0f;
		}
		//取得すれば表示されるよ！
		else{
			mScale.mY = 35.7f;
			mRotation.mY += 3;
		}
	}
	//if (mObjNumber == 19){
	//	//ボートを取得しないと表示されないよ！
	//	if (CPlayer::mpPlayer->mHaveBoat == false){
	//		mScale.mY = 0.0f;
	//	}
	//	//取得さえすれば表示されるよ！
	//	else{
	//		mScale.mY = 35.7f;
	//		mRotation.mY -= 3;
	//	}
	//}

	if (mObjNumber == 21){//仮の番号
		//川を流れるブロック？
		mPosition.mX += 0.4f;
		if (mPosition.mX > 200.0f){
			mPosition.mX = -200.0f;
		}
	}
	if (mObjNumber == 25){//回レ回レ
		mRotation.mX += ROLLINGSPEED;
	}

	if (mObjNumber == 31){//仮の番号
		//川を流れるブロック？
		mPosition.mX += 2.5f;
		if (mPosition.mX > 3200.0f){
			mPosition.mX = 0.0f;
		}
	}

	if (mObjNumber == 334){
		mScale.mX += 0.1f;
		mScale.mY += 0.1f;
		mScale.mZ += 0.1f;
	}
	if (mObjNumber == 575){
		mPosition.mX = CPlayer::mpPlayer->mPosition.mX;
		mPosition.mZ = CPlayer::mpPlayer->mPosition.mZ;
		mRotation.mY = CPlayer::mpPlayer->mRotation.mY;
		//当たり判定は必要ない
	//	mPosition.mY = CPlayer::mpPlayer->mPosition.mY;
	}

	CCharacter::Update();

	//うまく消せない
	/*if (mObjNumber == 13){
		if (mDeleteFlag){
			mEnabled = false;
		}
	}*/

	if (mObjNumber == 10){
		if (CItem::mTargetAmount <= 0){
			mEnabled = false;
		}
	}
	if (mObjNumber == 22){
		if (CPlayer::mpPlayer->mIsGetKey){
			//ドアが開く
			//mEnabled = false;
			if (mRotation.mY < DOOR_RANGEOFMOTION){
				mRotation.mY += DOOR_OPENSPEED;
				mPosition.mX -= DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
				mPosition.mZ -= DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
			}
		}
		else{
			//ドアが閉まる
			if (mRotation.mY > 0.0f){
				mRotation.mY -= DOOR_OPENSPEED;
				mPosition.mX += DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
				mPosition.mZ += DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
			}
		}
	}
}

void CObjection::TaskCollision()
{
	/*mColBody.ChangePriority();
	mColTire.ChangePriority();
	CollisionManager.Collision(&mColBody);
	CollisionManager.Collision(&mColTire);*/
	//mpCollider.ChangePriority();
	//CollisionManager.Collision(&mpCollider);
}