#include "CEnemy.h"
#include "CKey.h"

#include "CItem.h"
#include "CBullet.h"
#include "CSceneTitle.h"

//乱数を実装するインクルード群
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//衝突時のエフェクト追加
#include "CEffect.h"
extern std::shared_ptr<CTexture> TextureExp;
extern std::shared_ptr<CTexture> TextureHit;
#include "CTaskManager.h"

extern CSound SoundBoost;
extern CSound SoundEngine;
extern CSound SoundCollision;
extern CSound SoundCollisionSmall;

int CEnemy::RenderType;

CEnemy *CEnemy::mpEnemy = 0;

#define G (9.8f / 120.0f)//重力加速度//60.0f
#define JUMPV0 (4.0f*4.0f)//ジャンプ初速//4.0f

#define MAXSPEED 4.5f+3.0f //-0.5f//車の最高速度 //一応プレイヤーが追いつける程度に最高速は少し低め
#define MAXSPEED_BACK 1.0f*2 //車の後退する最大速度
#define CAR_POWER 0.05f*2//1フレーム辺りの車の加速していく量
#define CAR_BREAK_POWER 0.025f*2 //前進中のブレーキの強さ

#define DECELERATE 0.05f*2 //車の減速する量
#define FIX_ANGLE_VALUE 0.5f*2 //角度が0度に向けて調整される量(主にX・Z用)

//#define MAXSPEED 7.0f //車の最高速度
//#define MAXSPEED_BACK 2.0f //車の後退する最大速度
//#define CAR_POWER 0.1f //1フレーム辺りの車の加速していく量
//#define CAR_BREAK_POWER 0.05f //前進中のブレーキの強さ
//
//#define DECELERATE 0.1f //車の減速する量
//#define FIX_ANGLE_VALUE 0.5f //角度が0度に向けて調整される量(主にX・Z用)

CEnemy::CEnemy()
//車体のY座標は0.0fにしたいんだけど・・・
//0.0fにしたら車体が浮いてるように見えてしまう
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
, mColTire(this, CVector(0.0f, -16.0f + 15.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
, mSearch(this, CVector(0.0f, 15.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
{
	srand(time(NULL));

	mpEnemy = this;

	mScale = CVector(2.5f, 2.5f, 2.5f);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//バネ取得後は2.3fの予定
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//車の速度の初期化
	mTurnSpeed = 0.0f;

	//mRotation.mY = -90;

	mCanJump = false;
	CanMove = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EENEMY;

	//スタート地点の座標を設定;
	mStartPoint[0] = 300.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -100.0f;

	mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	RenderType = 0;//描画処理 0:ゲーム画面  1:ミニマップ

	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;

	SoundBoost.Load("SE\\Shortbridge31-3.wav");
	SoundEngine.Load("SE\\SNES-Racing01-02.wav");
	SoundCollision.Load("SE\\bomb1.wav");
	SoundCollisionSmall.Load("SE\\SNES-Racing01-10(Collision).wav");


	isSoundEngine = false;
	//SoundEngine.Repeat();


	mSearch.mTag = CCollider::ESEARCH;
	mPointCnt = 0;//最初のポイントを設定
	//mpPoint = &mPoint[mPointCnt];//目指すポイントのポインタを設定

	////mPointと同じ座標とかその他もろもろをmPointRandに渡す
	//mPointRand = mPoint;
	//mPointRand2 = mPoint2;
	//mPointRand3 = mPoint3;
	//mPointRand4 = mPoint4;
	//mPointRand5 = mPoint5;
	//mPointRand6 = mPoint6;
	//mPointRand7 = mPoint7;
	//mPointRand8 = mPoint8;
	//mPointRand9 = mPoint9;
	//mPointRand10 = mPoint10;
	//mPointRand11 = mPoint11;
	//mPointRand12 = mPoint12;
	////mPointを中心にX座標,Z座標にずれを生じさせる
	//mPointRand->mPosition.mX += rand() % 101 - 50;
	//mPointRand->mPosition.mZ += rand() % 101 - 50;[

	//Point1 = *mPoint;
	//Point2 = *mPoint2;
	//Point3 = *mPoint3;
	//Point4 = *mPoint4;
	//Point5 = *mPoint5;
	//Point6 = *mPoint6;
	//Point1.mTag = EPOINT;
	//Point2.mTag = EPOINT;
	//Point3.mTag = EPOINT;
	//Point4.mTag = EPOINT;
	//Point5.mTag = EPOINT;
	//Point6.mTag = EPOINT;
	////敵AIがHARD以上ならポイントの数を拡張する
	//if (CSceneTitle::mDifficulty == 3){
	//	Point7 = *mPoint7;
	//	Point8 = *mPoint8;
	//	Point9 = *mPoint9;
	//	Point10 = *mPoint10;
	//	Point11 = *mPoint11;
	//	Point12 = *mPoint12;
	//	Point7.mTag = EPOINT;
	//	Point8.mTag = EPOINT;
	//	Point9.mTag = EPOINT;
	//	Point10.mTag = EPOINT;
	//	Point11.mTag = EPOINT;
	//	Point12.mTag = EPOINT;
	//}
	////Point1.mPosition = Point1.mPosition + CVector(1.0f *(rand() % 100 - 50), 1.0f *(rand() % 100 - 50), 1.0f *(rand() % 100 - 50));
	//mpPoint = &Point1;

	mpPoint = mPoint;

	mVPoint = mpPoint->mPosition;

}

void CEnemy::Update(){
	/*if (CKey::Push('A')){
	mRotation.mY++;
	}
	if (CKey::Push('D')){
	mRotation.mY--;
	}
	if (CKey::Push('W')){
	mPosition = CVector(0.0f, mVelocityJump, 0.5f) * mMatrix;
	}
	else if (CKey::Push('S')){
	mPosition = CVector(0.0f, mVelocityJump, -0.5f) * mMatrix;
	}
	else{
	mPosition = CVector(0.0f, mVelocityJump, 0.0f) * mMatrix;
	}*/

	//if (CKey::Push('Q')){//でば
	//	mRotation.mY++;
	//}
	//if (CKey::Push('E')){//つぐ
	//	mRotation.mY--;
	//}

	if (CKey::Once('B')){//超急ブレーキ
		//mCarSpeed = 0.0f;
		printf("mPoint…X:%.1f Y:%.1f Z:%.1f\n", mPoint->mPosition.mX, mPoint->mPosition.mY, mPoint->mPosition.mZ);
		printf("mpPoint…X:%.1f Y:%.1f Z:%.1f\n", mpPoint->mPosition.mX, mpPoint->mPosition.mY, mpPoint->mPosition.mZ);
	}


	if (CKey::Once('T')){
		printf("%d\n", rand(), rand());
	}

	////Aキー、Dキーが同時に入力されているか
	//if (CKey::Push('A') && CKey::Push('D')){
	//	mADMoveX = 0.0f;
	//}
	//else if (CKey::Push('A')){//Aキーだけ
	//	mADMoveX = mMoveSpeed;
	//}
	//else if (CKey::Push('D')){//Dキーだけ
	//	mADMoveX = -mMoveSpeed;
	//}
	//else{//どちらも入力なし
	//	mADMoveX = 0.0f;
	//}
	////Wキー、Sキーが同時に入力されているか
	//if (CKey::Push('W') && CKey::Push('S')){
	//	mWSMoveZ = 0.0f;
	//}
	//else if (CKey::Push('W')){//Wキーだけ
	//	mWSMoveZ = mMoveSpeed;
	//}
	//else if (CKey::Push('S')){//Sキーだけ
	//	mWSMoveZ = -mMoveSpeed;
	//}
	//else{//どちらも入力なし
	//	mWSMoveZ = 0.0f;
	//}

	//ブースト残り時間
	if (mBoostTime > 0){
		mBoostTime--;
	}
	else{
		isBoost = false;
	}
	//ブースト有効時
	if (isBoost){
		mBoostMaxSpeed = 10.0f;

		if (mCarSpeed < MAXSPEED + mBoostMaxSpeed){
			//ブースト時のアクセル効果は実質3倍
			mCarSpeed += CAR_POWER;
			mCarSpeed += CAR_POWER;
		}
	}
	//ブーストが切れている時
	else{
		//最高速度が通常まで減速
		if (mBoostMaxSpeed > 0.0f){
			if (mBoostMaxSpeed > 0.2f){
				mBoostMaxSpeed -= 0.2f;
			}
			else{
				mBoostMaxSpeed = 0.0f;
			}
		}
		//ブーストが終了して現在の速度が最高速度を超過していた場合、最高速度に合わせて減速されていく
		if (mCarSpeed > MAXSPEED + mBoostMaxSpeed){
			mCarSpeed = MAXSPEED + mBoostMaxSpeed;
		}
	}


	if (CKey::Push(VK_UP) && CanMove || mChecks >= 0 && CanMove){
		if (mCarSpeed < MAXSPEED + mBoostMaxSpeed){
			mCarSpeed += CAR_POWER;
		}
	}
	else if (CKey::Push(VK_DOWN) && CanMove){
		if (mCarSpeed > -MAXSPEED_BACK){
			mCarSpeed -= CAR_POWER;
			//前進中ならブレーキがかかり、早めに止まる
			if (mCarSpeed >= CAR_BREAK_POWER){
				mCarSpeed -= CAR_BREAK_POWER;
			}
		}
	}
	else{
		//前進中
		if (mCarSpeed > 0.0f){
			//減速値でマイナスにならないように調整
			if (mCarSpeed < DECELERATE){
				mCarSpeed = 0.0f;
			}
			else{
				mCarSpeed -= DECELERATE;
			}
		}
		//後退中
		else if (mCarSpeed < 0.0f){
			if (mCarSpeed > -DECELERATE){
				mCarSpeed = 0.0f;
			}
			else{
				mCarSpeed += DECELERATE;
			}
		}
	}

	//プレイヤーのX角度が0に戻されていく
	if (mRotation.mX > 0.0f){
		if (mRotation.mX > FIX_ANGLE_VALUE){
			mRotation.mX -= FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mX = 0.0f;
		}
	}
	else if (mRotation.mX < 0.0f){
		if (mRotation.mX < -FIX_ANGLE_VALUE){
			mRotation.mX += FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mX = 0.0f;
		}
	}
	//プレイヤーのZ角度が0に戻されていく
	if (mRotation.mZ > 0.0f){
		if (mRotation.mZ > FIX_ANGLE_VALUE){
			mRotation.mZ -= FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mZ = 0.0f;
		}
	}
	else if (mRotation.mZ < 0.0f){
		if (mRotation.mZ < -FIX_ANGLE_VALUE){
			mRotation.mZ += FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mZ = 0.0f;
		}
	}

	//ポイントへのベクトルを求める
	//CVector dir = mpPoint->mPosition - mPosition;
	CVector dir = mVPoint - mPosition;
	//左方向へのベクトルを求める
	CVector left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	////左右の回転処理(Y軸)
	//if (left.Dot(dir) > 0.0f){
	//	mRotation.mY += 0.3f * 100;
	//}
	//else if (left.Dot(dir) < 0.0f){
	//	mRotation.mY -= 0.3f * 100;
	//}

	//目的地が左側にあり、操作可能な時
	if (left.Dot(dir) > 0.0f && CanMove){ //ハンドルを左に！
		//mRotation.mY++;
		if (mTurnSpeed >= 0.0f&&mTurnSpeed<0.5f){
			mTurnSpeed = 0.5f;
		}
		if (mTurnSpeed < 0.0f){
			mTurnSpeed += 0.11f;
		}
		mTurnSpeed += 0.04f;
	}
	//あるいは目的地が右方面で、操作可能な時
	else if (left.Dot(dir) < 0.0f && CanMove){//ハンドルを右に！
		//mRotation.mY--;
		if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
			mTurnSpeed = -0.5f;
		}
		if (mTurnSpeed > 0.0f){
			mTurnSpeed -= 0.11f;
		}
		mTurnSpeed -= 0.04f;
	}
	else{
		if (mTurnSpeed > 0.0f){
			mTurnSpeed -= 0.05f;
		}
		else if (mTurnSpeed < 0.0f){
			mTurnSpeed += 0.05f;
		}
		if (mTurnSpeed<0.04f && mTurnSpeed>-0.04f){
			mTurnSpeed = 0.0f;
		}
	}
	if (mTurnSpeed > 1.0f){
		mTurnSpeed = 1.0f;
	}
	else if (mTurnSpeed < -1.0f){
		mTurnSpeed = -1.0f;
	}
	mRotation.mY += mTurnSpeed;


	if (CKey::Push('Z')){
		mRotation.mX++;
	}
	if (CKey::Push('X')){
		mRotation.mX--;
	}
	if (CKey::Push('C')){
		mRotation.mZ += 2;
	}
	if (CKey::Push('V')){
		mRotation.mZ -= 2;
	}
	if (mRotation.mZ > 180){
		mRotation.mZ = -180;
	}
	else if (mRotation.mZ < -180){
		mRotation.mZ = 180;
	}

	//前に車が進んでいる時
	if (mCarSpeed > 0.0f){
		if (isSoundEngine == false){
			//SoundEngine.Repeat();
			isSoundEngine = true;
		}
	}
	//車が停止している時
	else if (mCarSpeed == 0.0f){
		//SoundEngine.Stop();
		isSoundEngine = false;
	}
	//車がバックしている時
	else if (mCarSpeed < 0.0f){
		if (isSoundEngine == false){
			//バックは違う音を鳴らす予定
			//SoundEngine.Repeat();
			isSoundEngine = true;
		}
	}

	//mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ)  * CMatrix().RotateY(mRotation.mY) * mMatrix;

	/*mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ) * CMatrix().RotateY(90) * mMatrix;*/
	//mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ+mCarSpeed) * CMatrix().RotateY(90) * mMatrix;

	/*常に地面に垂直に落下するようにする(プレイヤーの真下ではない)*/
	//mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ + mCarSpeed) * mMatrix;

	//mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrix;
	mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();
	//Y方向(重力)は分ける
	//mPosition = CVector(0.0f, mVelocityJump, 0.0f) * mMatrix;//できてない
	mPosition = CVector(0.0f, mVelocityJump*2.0f, 0.0f) * //mMatrixScale * 
		CMatrix().RotateZ(0) *
		CMatrix().RotateX(0) *
		CMatrix().RotateY(0)
		*mMatrixTranslate;//できてる？
	//mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;


	
	/*if (left.Dot(dir) < 0.0f){
		mRotation.mY -= 11;
	}
	else if (left.Dot(dir) > 0.0f){
		mRotation.mY += 11;
	}*/

	//コースアウトした時
	if (mPosition.mY < -150.0f){
		//落下の勢いを0にする
		mVelocityJump = 0.0f;
		//車の速度を0に
		mCarSpeed = 0.0f;

		mRotation = CVector(0.0f, 0.0f, 0.0f);
		if (mChecks == 0){
			//スタートした時の位置、方向に戻される
			mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
			mRotation.mY = 0.0f;
		}
		else if (mChecks == 1){
			mPosition = CVector(-80.0f, mStartPoint[1], 2175.0f);
			mRotation.mY = -55.0f;
		}
		else if (mChecks == 2){
			mPosition = CVector(-1620.0f, mStartPoint[1], 450.0f);
			mRotation.mY = -175.0f;
		}
		else if (mChecks == 3){
			mPosition = CVector(-1212.0f, mStartPoint[1], -1616.0f);
			mRotation.mY = 120.0f;
		}
	}
	//mPosition.mZ += 1.0f;
	CCharacter::Update();

	//重力の影響を反映する
	mVelocityJump -= G;

	
	if (CKey::Push('I')){
		mpPoint->mRotation.mY++;
	}

	/*mpPoint->mPosition.mX = sposX;
	mpPoint->mPosition.mZ = sposZ;*/


	
}

void CEnemy::Collision(CCollider *mc, CCollider *yc){
	//自身のコライダタイプの判定
	switch (mc->mType){
	case CCollider::ESPHERE://球コライダ
		//相手のコライダが三角コライダの時
		if (yc->mType == CCollider::ETRIANGLE){
			////水泳可能な時は水場(水の上)を通過できる
			//if (yc->mTag == EWATER && mCanSwim){
			//}
			//else{
			//}

			////チェックポイント関連の処理
			//if (CCollider::Collision(mc, yc)){
			//	if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//中間地点1
			//		if (mChecks == 0){
			//			//各中間地点を通過しないと1周したとみなされない
			//			mChecks = 1;
			//		}
			//	}
			//	if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//中間地点2
			//		if (mChecks == 1){
			//			//各中間地点を通過しないと1周したとみなされない
			//			mChecks = 2;
			//		}
			//	}
			//	if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//中間地点3
			//		if (mChecks == 2){
			//			//各中間地点を通過しないと1周したとみなされない
			//			mChecks = 3;
			//		}
			//	}
			//}

			//自分のコライダが本体の時
			if (mc->mTag == CCollider::EBODY){
				//芝生通過中の処理
				if (yc->mpParent->mTag == CCharacter::EGRASS){
					CVector aiueo;//仮のベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//ブースト効果の方が優先される
						if (isBoost == false){
							//printf("speed down…\n");
							//一定速度までスピード低下
							if (mCarSpeed > 3.0f){
								if (mCarSpeed > 3.3f){
									mCarSpeed -= 0.3f;
								}
								else{
									mCarSpeed = 3.0f;
								}
							}
						}
						
						/*if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
							mTurnSpeed = -0.5f;
						}
						if (mTurnSpeed > 0.0f){
							mTurnSpeed -= 0.11f;
						}
						mTurnSpeed -= 0.04f;*/

					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//中間地点1
					if (mChecks == 0){
						//各中間地点を通過しないと1周したとみなされない
						CVector aiu;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 1;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//中間地点2
					if (mChecks == 1){
						//各中間地点を通過しないと1周したとみなされない
						CVector aiu;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 2;
							//mChecks = 0;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//中間地点3
					if (mChecks == 2){
						//各中間地点を通過しないと1周したとみなされない
						CVector aiu;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 3;
						}
					}
				}
				////加速床に乗った時の処理
				//if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
				//	CVector aiueo;//とりまベクトル
				//	if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
				//		//mCarSpeed += 10.0f;
				//		isBoost = true;
				//		mBoostTime = 45;
				//		printf("SPEED UP!\n");
				//	}
				//}



				if (yc->mpParent->mTag == CCharacter::EWATER){
					//通過可能、ステージ1の水
				}
				else if (yc->mpParent->mTag == CCharacter::ECLEARWATER){
					//通過可能、ステージ2の水
				}
				else if (yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD){
					//処理は行われるが、これらのパネルは通過可能
				}
				else{
					

					CVector adjust;//調整用ベクトル
					//		//三角形と球の衝突判定
					//		CCollider::CollisionTriangleSphere(yc, mc, &adjust);
					if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
						//位置の更新
						mPosition = mPosition - adjust * -1;
						//行列の更新
						CCharacter::Update();

						if (yc->mpParent->mTag == CCharacter::EWALL){
							//衝突したのが壁だった場合は壁には引っかからず落下
							//壁にぶつかると衝突音がし、車が減速する
							//速い時に衝突で減速、遅い時の衝突は特に変化なし
							if (mCarSpeed > 4.5f){
								mCarSpeed = 2.0f;
								//mCarSpeed /= 2.0f;
							//	SoundCollision.Play();
								//激突時、エフェクト発生
								new CEffect(mPosition + CVector(0.0f, 35.0f, 0.0f), 50.0f, 50.0f, TextureExp, 4, 4, 1, 0);
								//new CEffect(mPosition + CVector(0.0f, 390.0f/2, 0.0f), 390.0f, 390.0f, TextureExp, 4, 4, 111);

								printf("ｺﾞﾝｯ");
							}
							else if (mCarSpeed > 3.0f){
								mCarSpeed = 2.0f;
							//	SoundCollisionSmall.Play();
								//軽くぶつけた時もエフェクト発生
								new CEffect(mPosition + CVector(0.0f, 15.5f, 0.0f), 17.0f, 17.0f, TextureHit, 3, 8, 1, 1);
								printf("ｺﾞｽｯ");
							}
							else{
								//壁にこすりながらの移動時、速度が遅くなる
								if (mCarSpeed > 2.0f){
									mCarSpeed = 2.0f;
									/*new CEffect(mPosition + CVector(0.0f, 20.0f, 0.0f), 68.0f + 12.0f, 17.0f + 3.0f, TextureHit, 1, 1, 1, 1);
									new CEffect(mPosition + CVector(0.0f, 60.0f, 0.0f), 68.0f + 12.0f, 17.0f + 3.0f, TextureHit, 1, 1, 1, 0);*/
								}
							}
							//mCarSpeed = -mCarSpeed * 1.0;
							//mVelocityJump = 2.0f;

						}
						else{
							mVelocityJump = 0;
							mCanJump = true;

							/*if (mRotation.mX < yc->mpParent->mRotation.mX){
							mRotation.mX++;
							}
							else if (mRotation.mX > yc->mpParent->mRotation.mX){
							mRotation.mX--;
							}*/

							int rotateofycmx = yc->mpParent->mRotation.mX;
							rotateofycmx %= 360; //-360度から360度までの数値に変換
							//-235=125 300=-60 -180度未満か、180度以上の角度は
							if (rotateofycmx < -180){
								rotateofycmx += 360;
							}
							else if (rotateofycmx >= 180){
								rotateofycmx -= 360;
							}
							mRotation.mX = rotateofycmx;
							//if (mRotation.mX < yc->mpParent->mRotation.mX){
							//	mRotation.mX = yc->mpParent->mRotation.mX;
							//}
							//else if (mRotation.mX > yc->mpParent->mRotation.mX){
							//	mRotation.mX = yc->mpParent->mRotation.mX;
							//}
							//mRotation = yc->mpParent->mRotation;
						}
					}
					

				}
			}
			//自分のコライダが判定の時
			if (mc->mTag == CCollider::ESEARCH){
				//加速床に乗った時の処理
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector aiueo;//とりまベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//mCarSpeed += 10.0f;

						if (isBoost == false){
							printf("SPEED UP!\n");
							//SoundBoost.Play();
						}
						isBoost = true;
						mBoostTime = 45;
					}
				}
			}




			/*if(yc->mpParent->mTag == CCharacter::ESPRING){
			mJumpV0 = 2.3f;
			}*/
			/*if (CItem::mpItem->mItemNumber == 2){
			mJumpV0 = 2.3f;
			}*/
		}
		if (yc->mType == CCollider::ESPHERE){
			if (CCollider::Collision(mc, yc)){
				//
				if (mc->mTag == CCollider::EBODY){
					if (yc->mpParent->mTag == CCharacter::EPLAYER){
						CVector adjust;//調整用ベクトル
						////		//球同士の衝突判定
						if (CCollider::Collision(mc, yc, &adjust)){
							//位置の更新
							mPosition = mPosition - adjust * -1;
							//行列の更新
							CCharacter::Update();
							//printf("X:%f Y:%f Z:%f",mPosition.mX,mPosition.mY,mPosition.mZ);
							//printf("敵の衝突処理");
						}
					}
					
					if (yc->mpParent->mTag == CCharacter::EENEMY
						&& yc->mTag == CCollider::EBODY){
						if (mc == yc){
							return;
						}
						CVector adjust;//調整用ベクトル
						////		//球同士の衝突判定
						if (CCollider::Collision(mc, yc, &adjust)){
							//位置の更新
							mPosition = mPosition - adjust * -1;
							//行列の更新
							CCharacter::Update();
							//printf("X:%f Y:%f Z:%f",mPosition.mX,mPosition.mY,mPosition.mZ);
						}
					}					
				}

				if (mc->mTag == CCollider::ESEARCH){
					//ポインタからポインタに向けて移動する
					if (yc->mpParent->mTag == CCharacter::EPOINT){
						CVector adjust;//調整用ベクトル
						//		//球同士の衝突判定
						if (CCollider::Collision(mc, yc, &adjust)){
							//衝突したポインタと目指しているポインタが同じ時
							if (yc->mpParent == mpPoint){
								//mPointCnt++;//次のポイントにする
								////最後だったら最初にする
								//mPointCnt %= mPointSize;
								//mpPoint = &mPoint[mPointCnt];
								
								

								int r = (mc->mRadius + yc->mRadius) * 0.8f;
								int gap = (rand() % (r * 2) - r);
								//敵AIのLvにより分散値も変更される予定
								if (CSceneTitle::mDifficulty == 1){
									r = (mc->mRadius + yc->mRadius) * 0.8f;
									gap = (rand() % (r * 2) - r);
								}
								else if (CSceneTitle::mDifficulty == 2){
									r = (mc->mRadius + yc->mRadius) * 0.4f;
									gap = (rand() % (r * 2) - r);
								}
								else if (CSceneTitle::mDifficulty == 3){
									r = (mc->mRadius + yc->mRadius) * 0.2f;
									gap = (rand() % (r * 2) - r);
								}

								//次のポイントのポインタを設定
								if (mpPoint == mPoint){
									mVPoint = mPoint2->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint2;
																		
									//printf("次の目的地：X:%f,Y:%f,Z%f:\n", mpPoint->mPosition.mX, mpPoint->mPosition.mY, mpPoint->mPosition.mZ);
								}
								else if(mpPoint==mPoint2){
									mVPoint = mPoint3->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint3;
								}
								else if (mpPoint == mPoint3){
									mVPoint = mPoint4->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint4;
								}
								else if (mpPoint == mPoint4){
									mVPoint = mPoint5->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint5;
								}
								else if (mpPoint == mPoint5){
									mVPoint = mPoint6->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint6;
								}
								else if (mpPoint == mPoint6){
									//難易度HARD以上ではさらに細かくポイントが設定されている
									if (CSceneTitle::mDifficulty == 2 || CSceneTitle::mDifficulty == 3){
										mVPoint = mPoint7->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
										mpPoint = mPoint7;
									}
									else{
										mVPoint = mPoint->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
										mpPoint = mPoint;
									}
								}
								//敵AIがHARD以上での挙動
								else if (mpPoint == mPoint7){
									mVPoint = mPoint8->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint8;
								}
								else if (mpPoint == mPoint8){
									mVPoint = mPoint9->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint9;
								}
								else if (mpPoint == mPoint9){
									mVPoint = mPoint10->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint10;
								}
								else if (mpPoint == mPoint10){
									mVPoint = mPoint11->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint11;
								}
								else if (mpPoint == mPoint11){
									mVPoint = mPoint12->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint12;
								}
								else if (mpPoint == mPoint12){
									mVPoint = mPoint->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap;
									mpPoint = mPoint;
								}
								

								//printf("次の目的地…X:%.1f Y:%.1f Z:%.1f\n", mpPoint->mPosition.mX, mpPoint->mPosition.mY, mpPoint->mPosition.mZ);
							}
						}
					}
					//switch (yc->mpParent->mTag){
					//case EPOINT://ポイントの時
					//	//衝突したポインタと目指しているポインタが同じ時
					//	if (yc->mpParent == mpPoint){
					//		mPointCnt++;//次のポイントにする
					//		//最後だったら最初にする
					//		mPointCnt %= mPointSize;
					//		//次のポイントのポインタを設定
					//		mpPoint = &mPoint[mPointCnt];
					//		printf("a");
					//	}
					//	break;
					//default:
					//	;
					//}
				}
			}
		}


		break;
	}

	////球コラと三角形の時
	//if (mc->mType == CCollider::ESPHERE && yc->mType == CCollider::ETRIANGLE){
	//	CVector adjust;//調整用ベクトル
	//	//コライダのmとyが衝突しているか判定
	//	//三角形と球の衝突判定
	//	if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
	//		//位置の更新
	//		mPosition = mPosition - adjust * -1;
	//		//行列の更新
	//		CCharacter::Update();
	//	}
	//}

}

void CEnemy::TaskCollision()
{
	mColBody.ChangePriority();
	mColTire.ChangePriority();
	mSearch.ChangePriority();
	CollisionManager.Collision(&mColBody);
	CollisionManager.Collision(&mColTire);
	CollisionManager.Collision(&mSearch);
}

//誘導ポイント
CPoint *CEnemy::mPoint;
CPoint *CEnemy::mPoint2;
CPoint *CEnemy::mPoint3;
CPoint *CEnemy::mPoint4;
CPoint *CEnemy::mPoint5;
CPoint *CEnemy::mPoint6;
CPoint *CEnemy::mPoint7;
CPoint *CEnemy::mPoint8;
CPoint *CEnemy::mPoint9;
CPoint *CEnemy::mPoint10;
CPoint *CEnemy::mPoint11;
CPoint *CEnemy::mPoint12;

int CEnemy::mPointSize = 0;