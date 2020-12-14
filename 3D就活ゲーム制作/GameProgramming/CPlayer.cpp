#include "CPlayer.h"
#include "CKey.h"

#include "CItem.h"
#include "CBullet.h"

extern CSound SoundJump;
extern CSound SoundShot;
extern CSound SoundItemGet;
extern CSound SoundBoost;
extern CSound SoundEngine;
extern CSound SoundCollision;
extern CSound SoundCollisionSmall;

int CPlayer::RenderType;

CPlayer *CPlayer::mpPlayer = 0;

#define G (9.8f / 120.0f)//重力加速度//60.0f
#define JUMPV0 (4.0f*4.0f)//ジャンプ初速//4.0f

#define MAXSPEED 4.5f+3.0f //車の最高速度
#define MAXSPEED_BACK 1.0f*2 //車の後退する最大速度
#define CAR_POWER 0.05f*2 //1フレーム辺りの車の加速していく量
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

CPlayer::CPlayer()
//:mColHead(this, CVector(0.0f, 19.2f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColBody(this, CVector(0.0f, 14.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 3.0f)
//, mColLeg(this, CVector(0.0f, 9.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColKnee(this, CVector(0.0f, 5.5f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColFoot(this, CVector(0.0f, 2.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//: mColHead(this, CVector(0.0f, 6.2f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColBody(this, CVector(0.0f, 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 3.0f)
//, mColLeg(this, CVector(0.0f, -4.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColKnee(this, CVector(0.0f, -7.5f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColFoot(this, CVector(0.0f, -11.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)

//車体のY座標は0.0fにしたいんだけど・・・
//0.0fにしたら車体が浮いてるように見えてしまう
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
, mColTire(this, CVector(0.0f, -16.0f+15.0f+1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
{
	mpPlayer = this;

	mScale = CVector(2.0f, 2.0f, 2.0f);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//バネ取得後は2.3fの予定
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//車の速度の初期化
	mTurnSpeed = 0.0f;

	//mRotation.mY = -90;

	mCanJump = false;
	mCanSwim = false;
	mCanFire = false;
	mIsGetKey = false;
	mHaveBoat = false;
	mFlyingMode = false;

	CanMove = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EPLAYER;
	mJumpPrio = 0;

	//スタート地点の座標を設定;
	mStartPoint[0] = 300.0f;  mStartPoint[1] = 63.0f;  mStartPoint[2] = -100.0f;
	
	mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	RenderType = 0;//描画処理 0:ゲーム画面  1:ミニマップ

	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;

	SoundJump.Load("SE\\jump12.wav");
	SoundShot.Load("SE\\shot1.wav");
	SoundItemGet.Load("SE\\se_maoudamashii_system46.wav");	
	SoundBoost.Load("SE\\Shortbridge31-3.wav");
	SoundEngine.Load("SE\\SNES-Racing01-02.wav");
	SoundCollision.Load("SE\\bomb1.wav");
	SoundCollisionSmall.Load("SE\\SNES-Racing01-10(Collision).wav");
	


	isSoundEngine = false;
	//SoundEngine.Repeat();
}

void CPlayer::Update(){
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

	if (CKey::Push('Q')){//でば
		mRotation.mY++;
	}
	if (CKey::Push('E')){//つぐ
		mRotation.mY--;
	}

	if (CKey::Push('B')){//超急ブレーキ
		mCarSpeed = 0.0f;
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

	

	if (FireCount > 0){
		FireCount--;
	}

	//弾を発射可能な時
	if (mCanFire){
		//スペースキー入力で弾発射
		if (CKey::Push(VK_SPACE)){
			if (FireCount <= 0){
				CBullet *bullet = new CBullet();
				bullet->Set(2.0f, 0.1f);
				//bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
				bullet->mPosition = CVector(0.0f, 1.0f, 10.0f) * CMatrix().RotateY(90) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->mScale = CVector(3.0f, 3.0f, 3.0f);
				//	TaskManager.Add(bullet);
				FireCount = 12;
				//Bullet.Set(0.1f, 1.5f);
				//Bullet.mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
				//Bullet.mRotation = mRotation;

				//レーザー発射音の再生
				SoundShot.Play();
			}
		}
	}

	//飛行モード有効時(デバッグ用)
	if (mFlyingMode){
		if (CKey::Push('W')){
			mVelocityJump = mMoveSpeed*3;
		}
		else if (CKey::Push('S')){
			mVelocityJump = -mMoveSpeed*3;
		}
		else{
			mVelocityJump = 0.0f;
		}
	}

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


	if (CKey::Push(VK_UP) && CanMove){
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

	if (CKey::Push(VK_LEFT) && CanMove){ //ハンドルを左に！
		//mRotation.mY++;
		if (mTurnSpeed>=0.0f&&mTurnSpeed<0.5f){
			mTurnSpeed = 0.5f;
		}
		if (mTurnSpeed < 0.0f){
			mTurnSpeed += 0.11f;
		}
		mTurnSpeed += 0.04f;
	}
	else if (CKey::Push(VK_RIGHT) && CanMove){//ハンドルを右に！
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
		mRotation.mZ+=2;
	}
	if (CKey::Push('V')){
		mRotation.mZ-=2;
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
			SoundEngine.Repeat();
			isSoundEngine = true;
		}		
	}
	//車が停止している時
	else if (mCarSpeed == 0.0f){
		SoundEngine.Stop();
		isSoundEngine = false;
	}
	//車がバックしている時
	else if (mCarSpeed < 0.0f){
		if (isSoundEngine == false){
			//バックは違う音を鳴らす予定
			SoundEngine.Repeat();
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

	//転落してしまった時(Rキーで即リスタート)
	if (mPosition.mY < -150.0f || CKey::Once('R')){
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

	CCharacter::Update();
	
	//重力の影響を反映する
	mVelocityJump -= G;
	if (CKey::Once('K')){
		if (mCanJump){
			//ボート乗船時はジャンプ不可
			if (mHaveBoat){
			}
			else{
				mCanJump = false;
				mVelocityJump = mJumpV0;
				mJumpPrio = 2;

				////ジャンプ音再生
				SoundJump.Play();
			}
		}
	}
	if (mJumpPrio > 0){
		mJumpPrio--;
	}
	
}

void CPlayer::Collision(CCollider *mc, CCollider *yc){
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
							printf("speed down…\n");
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



				if (mCanSwim && yc->mpParent->mTag == CCharacter::EWATER){
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
					if (mJumpPrio < 1){

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
									mCarSpeed = -0.4f;
									//mCarSpeed /= 2.0f;
									SoundCollision.Play();
									printf("ｺﾞﾝｯ");
								}
								else if (mCarSpeed > 3.0f){
									mCarSpeed = -0.4f;
									//mCarSpeed /= 2.0f;
									SoundCollisionSmall.Play();
									printf("ｺﾞｽｯ");
								}
								else{
									//壁にこすりながらの移動時、速度が遅くなる
									if (mCarSpeed > 2.0f){
										mCarSpeed = 2.0f;
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
			}
			//自分のコライダが判定の時
			if (mc->mTag == CCollider::ESEARCH){
				//加速床に乗った時の処理
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector aiueo;//とりまベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//mCarSpeed += 10.0f;
						isBoost = true;
						mBoostTime = 45;
						printf("SPEED UP!\n");
						SoundBoost.Play();
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
				if (yc->mpParent->mTag == CCharacter::ESPRING){
					mJumpV0 = 2.2f;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::ESCREW){
					mCanSwim = true;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::EGUN){
					mCanFire = true;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::EKEY){
					mIsGetKey = true;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::EBOAT){//ボート乗船時
					mHaveBoat = true;
					mMoveSpeed = 0.85f;
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