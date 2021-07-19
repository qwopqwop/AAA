#include "CCameraRange.h"

#include "CSceneTitle.h"

#include "CTaskManager.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592

CCameraRange *CCameraRange::mpCameraRange = 0;

#define G (9.8f / 90.0f)//重力加速度//60.0f
#define JUMPV0 (4.0f*4.0f)//ジャンプ初速//4.0f

#define MAXSPEED 20.0f //4.5f+3.0f *5//車の最高速度
#define MAXSPEED_BACK 4.0f//車の後退する最大速度
#define CAR_POWER 0.1f //*2//1フレーム辺りの車の加速していく量
#define CAR_BREAK_POWER 0.1f//前進中のブレーキの強さ
#define DECELERATE 0.1f //車の減速する量
#define FIX_ANGLE_VALUE 1.0f //角度が0度に向けて調整される量(主にX・Z用)
#define JUMPER01_POWER 3.0f;//ジャンプ台1によるジャンプの強さ

CCameraRange::CCameraRange()
:mColRange(this, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 40.0f)
{
	mpCameraRange = this;

	mTag = EPLAYER;

	mColRange.mTag = CCollider::ECAMERA_RANGE;

	mPosition = CVector(0.0f, 17.0f, -35.0f) * CPlayer::mpPlayer->mMatrixScale * CPlayer::mpPlayer->mMatrixRotate * CPlayer::mpPlayer->mMatrixTranslate;
	CCharacter::Update();
}

void CCameraRange::Update(){
	mPosition = CVector(0.0f, 17.0f, -35.0f) * CMatrix().RotateY(0)* CPlayer::mpPlayer->mMatrixScale   // * mPlayer->mMatrixScale
		* CMatrix().RotateY(CPlayer::mpPlayer->mRotation.mY)
		* CPlayer::mpPlayer->mMatrixTranslate
		+ CVector(0.0f, 0.0f, 0.0f);
	CCharacter::Update();
}

void CCameraRange::TaskCollision()
{
	mColRange.ChangePriority();
}