#include "CCameraPos.h"
//#include "CKey.h"

#include "CItem.h"
#include "CSceneTitle.h"

#include "CTaskManager.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592

CCameraPos *CCameraPos::mpCamera = 0;

#define G (9.8f / 90.0f)//重力加速度//60.0f
#define JUMPV0 (4.0f*4.0f)//ジャンプ初速//4.0f

#define MAXSPEED 20.0f //4.5f+3.0f *5//車の最高速度
#define MAXSPEED_BACK 1.0f*2 *2//車の後退する最大速度
#define CAR_POWER 0.05f*2 //*2//1フレーム辺りの車の加速していく量
#define CAR_BREAK_POWER 0.025f*2 *2//前進中のブレーキの強さ
//#define MAXSPEED_LIMITUP 10.0f //ブースト中の最高速度の上限突破量

#define DECELERATE 0.05f*2 //車の減速する量
#define FIX_ANGLE_VALUE 0.5f*2 //角度が0度に向けて調整される量(主にX・Z用)

#define JUMPER01_POWER 3.0f;//ジャンプ台1によるジャンプの強さ

//#define MAXSPEED 7.0f //車の最高速度
//#define MAXSPEED_BACK 2.0f //車の後退する最大速度
//#define CAR_POWER 0.1f //1フレーム辺りの車の加速していく量
//#define CAR_BREAK_POWER 0.05f //前進中のブレーキの強さ
//
//#define DECELERATE 0.1f //車の減速する量
//#define FIX_ANGLE_VALUE 0.5f //角度が0度に向けて調整される量(主にX・Z用)

CCameraPos::CCameraPos()
//車体のY座標は0.0fにしたいんだけど・・・
//0.0fにしたら車体が浮いてるように見えてしまう
:mColCam(this, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
{
	mpCamera = this;	

	CanMove = false;	
	mTag = EPLAYER;

	//if (CSceneTitle::mMode == 3){
	//	//スタート地点の座標を設定;
	//	mStartPoint[0] = 0.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = 80.0f;
	//	mStartRotation = 90.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else if (CSceneTitle::mMode == 2){
	//	//スタート地点の座標を設定;
	//	mStartPoint[0] = 2222.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -2510.0f;
	//	mStartRotation = 0.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else if (CSceneTitle::mMode == 4){
	//	//スタート地点の座標を設定;
	//	mStartPoint[0] = 0.0f;  mStartPoint[1] = 0.0f;  mStartPoint[2] = 0.0f;
	//	//mStartPoint[0] = 450.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -50.0f;
	//	mStartRotation = 180.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else if (CSceneTitle::mMode == 5){
	//	//スタート地点の座標を設定;
	//	mStartPoint[0] = -3755.5f;  mStartPoint[1] = 13.5f;  mStartPoint[2] = 16060.5f;
	//	mStartRotation = -145.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else{
	//	//スタート地点の座標を設定;
	//	mStartPoint[0] = 350.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -100.0f;
	//	mStartRotation = 0.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	mColCam.mTag = CCollider::ECAMERA;

	/*mPosition = CVector(0.0f, 170.0f, 400.0f) * CPlayer::mpPlayer->mMatrixRotate * CPlayer::mpPlayer->mMatrixTranslate;
	mPosition = CCameraRange::mpCameraRange->mPosition;
	printf("X:%f\nY:%f\nZ:%f\n", mPosition.mX, mPosition.mY, mPosition.mZ);*/
	mPosition = CPlayer::mpPlayer->mPosition;
	mPosition = CCameraRange::mpCameraRange->mPosition;
	//printf("X:%f\nY:%f\nZ:%f\n", mPosition.mX, mPosition.mY, mPosition.mZ);
	CCharacter::Update();
	mVCamY = 0;

	//mVPoint = mpPoint->mPosition;//一番最初は分散無し
	//CPlayer::mpPlayer->mColTire.mPosition;
	//printf("X:%f\nY:%f\nZ:%f\n", CCameraRange::mpCameraRange->mPosition.mX, CCameraRange::mpCameraRange->mPosition.mY, CCameraRange::mpCameraRange->mPosition.mZ);
	//CCameraRange::mpCameraRange->mPosition.mX;
}

void CCameraPos::Update(){	
	


	////mPosition = CVector(0.0f, 0.0f, 0.0f) * mMatrixRotate * mMatrixTranslate;
	////CCharacter::Update();
	////mPosition = CVector(0.0f, 0.0f, 0.0f) * //mMatrixScale * 
	////	CMatrix().RotateZ(0) *
	////	CMatrix().RotateX(0) *
	////	CMatrix().RotateY(0)
	////	*mMatrixTranslate;//できてる？
	////mPosition = CVector(0.0f,170.0f,400.0f) * CPlayer::mpPlayer->mMatrixRotate * CPlayer::mpPlayer->mMatrixTranslate;
	//////mPosition.mY += 120.0f;
	//////mPosition.mZ += 100.0f;	
	////mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;	
	////printf(".");//こっちはプレイヤーから距離がかなり離れてても処理をし続けてくれる。



	//mVPoint = CCameraRange::mpCameraRange->mPosition;

	//mPosition.mY = CCameraRange::mpCameraRange->mPosition.mY;
	//CCharacter::Update();

	////ポイントへのベクトルを求める
	////CVector dir = mpPoint->mPosition - mPosition;
	//dir = mVPoint - mPosition;
	////左方向へのベクトルを求める
	//left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);

	///*if (left.Dot(dir) > 0.0f){
	//	mRotation.mY+=10;		
	//}
	//else if (left.Dot(dir) < 0.0f){
	//	mRotation.mY-=10;
	//}*/
	//while (left.Dot(dir) > 0.0f){
	//	mRotation.mY++;
	//	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	//}
	//while (left.Dot(dir) < 0.0f){
	//	mRotation.mY--;
	//	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	//}

	///*while (left.Dot(dir) > 0.0f){
	//	mVCamY++;
	//	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mVCamY);
	//}
	//while (left.Dot(dir) < 0.0f){
	//	mVCamY--;
	//	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mVCamY);
	//}*/

	////mRotation = CCameraRange::mpCameraRange->mRotation;
	//
	////printf("%f  %f\n", mVCamY, mRotation.mY - mVCamY);

	//CCharacter::Update();

	////プレイヤーの車のスピードを絶対値に変化
	//mCameraSpeed = CPlayer::mpPlayer->mCarSpeed;
	//if (mCameraSpeed < 0.0f){
	//	mCameraSpeed *= -1;
	//}

	//mPosition = CVector(0.0f, 0.0f, mCameraSpeed) * mMatrixRotate * mMatrixTranslate;
	//CCharacter::Update();






	//mVPoint = CCameraRange::mpCameraRange->mPosition;
	////mPosition.mY = CCameraRange::mpCameraRange->mPosition.mY;
	//mRotation = CPlayer::mpPlayer->mRotation;
	//CCharacter::Update();
	////ポイントへのベクトルを求める
	////CVector dir = mpPoint->mPosition - mPosition;
	//dir = mVPoint - mPosition;
	////左方向へのベクトルを求める
	//left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mVCamY);
	//while (left.Dot(dir) > 0.0f){
	//	mVCamY++;
	//	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mVCamY);
	//}
	//while (left.Dot(dir) < 0.0f){
	//	mVCamY--;
	//	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mVCamY);
	//}

	mVPoint = CCameraRange::mpCameraRange->mPosition;
	mPosition.mY = CCameraRange::mpCameraRange->mPosition.mY;
	mRotation = CPlayer::mpPlayer->mRotation;
	CCharacter::Update();
	//ポイントへのベクトルを求める
	//CVector dir = mpPoint->mPosition - mPosition;
	dir = mVPoint - mPosition;
	//左方向へのベクトルを求める
	left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	while (left.Dot(dir) > 0.0f){
		mRotation.mY++;
		left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	}
	while (left.Dot(dir) < 0.0f){
		mRotation.mY--;
		left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	}	
	CCharacter::Update();
	//プレイヤーの車のスピードを絶対値に変化
	mCameraSpeed = CPlayer::mpPlayer->mCarSpeed;
	if (mCameraSpeed < 0.0f){
		mCameraSpeed *= -1;
	}
	mPosition = CVector(0.0f, 0.0f, mCameraSpeed) * mMatrixRotate * mMatrixTranslate;
	//mPosition = CVector(0.0f, 0.0f, mCameraSpeed) * CCameraRange::mpCameraRange->mMatrixRotate * CCameraRange::mpCameraRange->mMatrixTranslate;  // * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();

	//if (CPlayer::mpPlayer->mTurnSpeed > 0.0f){
	//	mPosition = CVector(0.0f, 0.0f, 5.22f*CPlayer::mpPlayer->mTurnSpeed) * mMatrixRotate * mMatrixTranslate;
	//}
	//else if (CPlayer::mpPlayer->mTurnSpeed < 0.0f){
	//	mPosition = CVector(0.0f, 0.0f, -5.22f*CPlayer::mpPlayer->mTurnSpeed) * mMatrixRotate * mMatrixTranslate;
	//}
	////mPosition = CVector(CPlayer::mpPlayer->mADMoveX, 0.0f, 0.0f) * mMatrixRotate * mMatrixTranslate;
	//CCharacter::Update();

}

void CCameraPos::Collision(CCollider *mc, CCollider *yc){
	//自身のコライダタイプの判定
	switch (mc->mType){
	case CCollider::ESPHERE://球コライダ
		//相手のコライダが三角コライダの時
		if (yc->mType == CCollider::ETRIANGLE){
			//自分のコライダが本体の時
			if (mc->mTag == CCollider::ECAMERA){				
				//else{ 以外は通過可能
				if (yc->mpParent->mTag == CCharacter::EWATER){
					//通過可能、ステージ1の水
				}
				else if (yc->mpParent->mTag == CCharacter::ECLEARWATER){
					//通過可能、ステージ2の水
				}
				else if (yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EGOALPOINT
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD){
					//処理は行われるが、これらのパネルは通過可能
				}
				else{
					CVector adjust;//調整用ベクトル
					//		//三角形と球の衝突判定
					if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
						//位置の更新
						mPosition = mPosition - adjust * -1;
						//行列の更新
						CCharacter::Update();
						//printf("カメラ激突\n");
					}
				}
			}
		}

		//if (yc->mType == CCollider::ESPHERE){
		//	//カメラは車と衝突しない
		//	if (mc->mTag == CCollider::ECAMERA){
		//		//printf("1");
		//		if (yc->mpParent->mTag == CCharacter::EPLAYER){
		//			//printf("2");
		//			if (yc->mTag == CCollider::ECAMERA_RANGE){
		//				//printf("3");
		//				CVector adjust;//調整用ベクトル
		//				////		//球同士の衝突判定
		//				if (CCollider::Collision(mc, yc, &adjust)==false){
		//					//printf("範囲外\n");
		//					//printf("%f  %f  %f\n", yc->mpParent->mPosition.mX, yc->mpParent->mPosition.mY, yc->mpParent->mPosition.mZ);							
		//					/*mPosition = yc->mpParent->mPosition;
		//					mPosition.mY += 100.0f;*/
		//					////位置の更新
		//					//mPosition = mPosition - adjust * -1;
		//					mPosition = yc->mpParent->mPosition;
		//					mPosition = mPosition - adjust*-1;
		//					//行列の更新
		//					CCharacter::Update();
		//					CVector aio = CVector(0.0f, 0.0f, (mc->mRadius+yc->mRadius) *-1)*CMatrix().RotateY(yc->mpParent->mRotation.mY);
		//					//CVector aio = CVector(0.0f, 0.0f, (mc->mRadius + yc->mRadius) *-1)*CMatrix().RotateY(mVCamY);
		//					//
		//					mPosition = mPosition + aio;
		//					CCharacter::Update();
		//					//CVector adj2;
		//					//if (CCollider::Collision(mc, yc, &adj2)){
		//					//	//adj2.mY = 0;
		//					//	mPosition = mPosition - adj2 *-1 * CMatrix().RotateY(yc->mpParent->mRotation.mY);
		//					//	//行列の更新
		//					//	CCharacter::Update();
		//					//}
		//					//CVector adj2;
		//					//if (CCollider::Collision(mc, yc, &adj2)){
		//					//	//printf("?");
		//					//	mPosition = mPosition - adj2 *-1;
		//					//	//行列の更新
		//					//	CCharacter::Update();
		//					//	mPosition.mY = yc->mpParent->mPosition.mY;
		//					//	CCharacter::Update();
		//					//	printf("1: %f %f %f\n", adjust.mX, adjust.mY, adjust.mZ);
		//					//	printf("2: %f %f %f\n", adj2.mX, adj2.mY, adj2.mZ);
		//					//	/*mPosition.mY = yc->mpParent->mPosition.mY;
		//					//	CCharacter::Update();*/
		//					//}
		//					//mPosition = mPosition - adjust * -1;							
		//				}						
		//			}
		//		}
		//	}
		//}

		//if (yc->mType == CCollider::ESPHERE){
		//	if (CCollider::Collision(mc, yc)){				
		//		//カメラは車と衝突しない
		//		if (mc->mTag == CCollider::ECAMERA){
		//			if (yc->mpParent->mTag == CCharacter::EPLAYER){
		//				if (yc->mTag == CCollider::ECAMERA_RANGE){
		//					CVector adjust;//調整用ベクトル
		//					////		//球同士の衝突判定
		//					if (CCollider::Collision(mc, yc, &adjust)){
		//						////位置の更新
		//						//mPosition = mPosition - adjust * -1;
		//						////行列の更新
		//						//CCharacter::Update();

		//						printf("範囲外\n");
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
		break;
	}
}


void CCameraPos::TaskCollision()
{
	mColCam.ChangePriority();
	CollisionManager.Collision(&mColCam);
}