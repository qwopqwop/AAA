#include "CCameraPos.h"
//#include "CKey.h"

#include "CItem.h"
#include "CSceneTitle.h"

#include "CTaskManager.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592
#define CAMERA_DISTANCE (302.0f)

CCameraPos *CCameraPos::mpCamera = 0;

#define G (9.8f / 90.0f)//重力加速度//60.0f
#define JUMPV0 (4.0f*4.0f)//ジャンプ初速//4.0f

#define MAXSPEED 20.0f //4.5f+3.0f *5//車の最高速度
#define MAXSPEED_BACK 4.0f//車の後退する最大速度
#define CAR_POWER 0.1f //*2//1フレーム辺りの車の加速していく量
#define CAR_BREAK_POWER 0.1f//前進中のブレーキの強さ
#define DECELERATE 0.1f //車の減速する量
#define FIX_ANGLE_VALUE 1.0f //角度が0度に向けて調整される量(主にX・Z用)
#define JUMPER01_POWER 3.0f;//ジャンプ台1によるジャンプの強さ


CCameraPos::CCameraPos()
:mColCam(this, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
{
	mpCamera = this;	

	CanMove = false;	
	mTag = EPLAYER;

	CCharacter::Update();

	mColCam.mTag = CCollider::ECAMERA;

	mPosition = CPlayer::mpPlayer->mPosition;
	mPosition = CCameraRange::mpCameraRange->mPosition;
	//printf("X:%f\nY:%f\nZ:%f\n", mPosition.mX, mPosition.mY, mPosition.mZ);
	mRotation = CPlayer::mpPlayer->mRotation;
	CCharacter::Update();
	mVCamY = 0;
}

void CCameraPos::Update(){	
	mVPoint = CCameraRange::mpCameraRange->mPosition;
	CCharacter::Update();
	//ポイントへのベクトルを求める
	CVector dir = mVPoint - mPosition;
	//左方向へのベクトルを求める
	CVector left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	CVector up = CVector(0.0f, 1.0f, 0.0f) * CMatrix().RotateX(mRotation.mX) * CMatrix().RotateY(mRotation.mY);
	//左方向
	while (left.Dot(dir) > 0.0f){
		mRotation.mY++;
		left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	}
	while (left.Dot(dir) < 0.0f){
		mRotation.mY--;
		left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	}
	//上方向
	while (up.Dot(dir) > 0.0f){
		mRotation.mX--;
		up = CVector(0.0f, 1.0f, 0.0f) * CMatrix().RotateX(mRotation.mX) * CMatrix().RotateY(mRotation.mY);
	}
	while (up.Dot(dir) < 0.0f){
		mRotation.mX++;
		up = CVector(0.0f, 1.0f, 0.0f) * CMatrix().RotateX(mRotation.mX) * CMatrix().RotateY(mRotation.mY);
	}
	CCharacter::Update();
	//プレイヤーの車のスピードを絶対値に変化
	mCameraSpeed = CPlayer::mpPlayer->mCarSpeed;
	if (mCameraSpeed < 0.0f){
		mCameraSpeed *= -1;
	}
	mPosition = CVector(0.0f, 0.0f, mCameraSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();

	//カメラとプレイヤーの距離を一定にする
	dir = (mPosition - CPlayer::mpPlayer->mPosition).Normalize() * CAMERA_DISTANCE;
	mPosition = CPlayer::mpPlayer->mPosition + dir;
	CCharacter::Update();

	//プレイヤーでリスポーンフラグが建った時の処理
	if (CPlayer::mpPlayer->isRespawn){
		mPosition = CVector(0.0f, 17.0f, -35.0f) * CPlayer::mpPlayer->mMatrixScale * CPlayer::mpPlayer->mMatrixRotate * CPlayer::mpPlayer->mMatrixTranslate;
		CCharacter::Update();
		CPlayer::mpPlayer->isRespawn = false;//フラグ消去
	}
}

void CCameraPos::Collision(CCollider *mc, CCollider *yc){
	//自身のコライダタイプの判定
	switch (mc->mType){
	case CCollider::ESPHERE://球コライダ
		//相手のコライダが三角コライダの時
		if (yc->mType == CCollider::ETRIANGLE){
			//自分のコライダが本体の時
			if (mc->mTag == CCollider::ECAMERA){
				if (yc->mpParent->mTag == CCharacter::EWATER
					|| yc->mpParent->mTag == CCharacter::ECLEARWATER
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EGOALPOINT					
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD){
					//これらのパネルは通行可能
				}
				else{
					CVector adjust;//調整用ベクトル
					//		//三角形と球の衝突判定
					if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
						//位置の更新
						mPosition = mPosition - adjust * -1;
						//行列の更新
						CCharacter::Update();
					}
				}
			}
		}
		break;
	}
}


void CCameraPos::TaskCollision()
{
	mColCam.ChangePriority();
	CollisionManager.Collision(&mColCam);
}