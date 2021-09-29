#include "CPlayer.h"
#include "CKey.h"

#include "CSceneTitle.h"

//衝突時のエフェクト追加
#include "CEffect.h"
extern std::shared_ptr<CTexture> TextureExp;
extern std::shared_ptr<CTexture> TextureHit;
extern std::shared_ptr<CTexture> TextureBoost;
#include "CTaskManager.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592

extern CSound SoundJump;
extern CSound SoundShot;
extern CSound SoundItemGet;
extern CSound SoundBoost;
extern CSound SoundEngine;
extern CSound SoundHorn;
extern CSound SoundCollision;
extern CSound SoundCollisionSmall;
extern CSound SoundSink;

CPlayer *CPlayer::mpPlayer = 0;

#define G (9.8f / 90.0f)//重力加速度//60.0f
#define JUMPV0 (16.0f)//ジャンプ初速//4.0f
#define MAXSPEED 20.0f //車の最高速度
#define MAXSPEED_BACK 4.0f//車の後退する最大速度
#define CAR_POWER 0.1f //*2//1フレーム辺りの車の加速していく量
#define CAR_BREAK_POWER 0.1f//前進中のブレーキの強さ
#define DECELERATE 0.1f //車の減速する量
#define FIX_ANGLE_VALUE 1.0f //角度が0度に向けて調整される量(主にX・Z用)
#define JUMPER01_POWER 3.0f;//ジャンプ台1によるジャンプの強さ

CPlayer::CPlayer()
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f*3)
, mColTire(this, CVector(0.0f, -16.0f + 15.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
{
	mpPlayer = this;

	mScale = CVector(7.5f, 7.5f, 7.5f);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//バネ取得後は2.3fの予定
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//車の速度の初期化
	mTurnSpeed = 0.0f;

	mFlyingMode = false;

	CanMove = false;
	isRespawn = false;
	isSink = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EPLAYER;
	mJumpPrio = 0;
	
	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;

	SoundJump.Load("SE\\jump12.wav");
	SoundShot.Load("SE\\shot1.wav");
	SoundItemGet.Load("SE\\se_maoudamashii_system46.wav");	
	SoundBoost.Load("SE\\Shortbridge31-3.wav");
	SoundEngine.Load("SE\\SNES-Racing01-02.wav");
	SoundEngine_Turf.Load("SE\\SNES-Racing02-02.wav");
	SoundHorn.Load("SE\\car-horn1.wav");
	SoundCollision.Load("SE\\bomb1.wav");
	SoundCollisionSmall.Load("SE\\SNES-Racing01-10(Collision).wav");
	SoundSink.Load("SE\\Diving.wav");

	isSoundEngine = false;	
	isTouchGoal = false;
	mGoalTime = 0; mRank = 1;

	mSound_Engine = mSound_Engine_Prev = ENONE;
}

void CPlayer::Update(){
	//デバッグ用
#ifdef _DEBUG
	//飛行モード
	if (mFlyingMode){
		if (CKey::Push('W')){
			mVelocityJump = mMoveSpeed * 3;
		}
		else if (CKey::Push('S')){
			mVelocityJump = -mMoveSpeed * 3;
		}
		else{
			mVelocityJump = 0.0f;
		}
	}
#endif

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
	
	if (CKey::Push(VK_LEFT) && CanMove){//ハンドルを左に！
		/*バック中は逆方向に曲がる*/
		if (mCarSpeed > 0.0f){
			if (mTurnSpeed >= 0.0f&&mTurnSpeed<0.5f){
				mTurnSpeed = 0.5f;
			}
			if (mTurnSpeed < 0.0f){
				mTurnSpeed += 0.11f;
			}
			mTurnSpeed += 0.04f;
		}
		else if (mCarSpeed < 0.0f){
			if (mTurnSpeed >= 0.0f&&mTurnSpeed<0.5f){
				mTurnSpeed = -0.5f;
			}
			if (mTurnSpeed < 0.0f){
				mTurnSpeed += -0.11f;
			}
			mTurnSpeed += -0.04f;
		}
	}
	else if (CKey::Push(VK_RIGHT) && CanMove){//ハンドルを右に！		
		/*バック中は逆方向に曲がる*/
		if (mCarSpeed > 0.0f){
			if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
				mTurnSpeed = -0.5f;
			}
			if (mTurnSpeed > 0.0f){
				mTurnSpeed -= 0.11f;
			}
			mTurnSpeed -= 0.04f;
		}
		else if(mCarSpeed < 0.0f){
			if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
				mTurnSpeed = 0.5f;
			}
			if (mTurnSpeed > 0.0f){
				mTurnSpeed -= -0.11f;
			}
			mTurnSpeed -= -0.04f;
		}
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
	float turnspd = mTurnSpeed;
	if (mCarSpeed > -4.0f && mCarSpeed < 4.0f){
		if (mCarSpeed >= 0.0f){
			turnspd = mTurnSpeed * mCarSpeed / 4.0f;
		}
		else{
			turnspd = mTurnSpeed * -mCarSpeed / 4.0f;
		}		
	}
	else{
		turnspd = mTurnSpeed;
	}
	mRotation.mY += turnspd;
	
	if (mRotation.mZ > 180){
		mRotation.mZ = -180;
	}
	else if (mRotation.mZ < -180){
		mRotation.mZ = 180;
	}

	mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();
	//Y方向(重力)は分ける
	//mPosition = CVector(0.0f, mVelocityJump, 0.0f) * mMatrix;//できてない
	mPosition = CVector(0.0f, mVelocityJump*2.0f, 0.0f) * //mMatrixScale * 
		CMatrix().RotateZ(0) *
		CMatrix().RotateX(0) *
		CMatrix().RotateY(0)
		*mMatrixTranslate;

	//転落してしまった時(Rキーで即リスタート)
	if (mPosition.mY < -700.0f || CKey::Once('R')){
		//落下の勢いを0にする
		mVelocityJump = 0.0f;
		//車の速度を0に
		mCarSpeed = 0.0f;
		//座標、回転値を変更する
		mPosition = mVCheckPositions[mChecks];
		mRotation = mVCheckRotations[mChecks];
		isSink = false;
		isRespawn = true;		
	}
	CCharacter::Update();
	
	//重力の影響を反映する
	mVelocityJump -= G;
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
			//自分のコライダが本体の時
			if (mc->mTag == CCollider::EBODY){				
				//芝生通過中の処理
				if (yc->mpParent->mTag == CCharacter::EGRASS){
					CVector tmpVec;//仮ベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
						//ブースト効果の方が優先される
						if (isBoost == false){
							//一定速度までスピード低下
							if (mCarSpeed > 3.2f + 1.8f){
								if (mCarSpeed > 4.0f + 1.8f){
									mCarSpeed -= 0.8f;
								}
								else{
									mCarSpeed = 3.2f +1.8f;
								}
							}
						}
					}
				}
				//中間地点通過時の処理
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//中間地点1
					if (mChecks == 0){
						//各中間地点を通過しないと1周したとみなされない
						CVector tmpVec;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							mChecks = 1;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//中間地点2
					if (mChecks == 1){
						//各中間地点を通過しないと1周したとみなされない
						CVector tmpVec;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							mChecks = 2;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//中間地点3
					if (mChecks == 2){
						//各中間地点を通過しないと1周したとみなされない
						CVector tmpVec;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							mChecks = 3;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::EGOALPOINT){//中間地点3
					if (mChecks == 3){
						//各中間地点を通過しないと1周したとみなされない
						CVector tmpVec;//数合わせのためだけのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							isTouchGoal = true;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EGOALPOINT
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD
					|| yc->mpParent->mTag == CCharacter::EWATER){
					//これらのタグが付く物体は通過可能
					if (yc->mpParent->mTag == CCharacter::EWATER){
						CVector tmpVec;//値を返すためのベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							if (isSink == false){
								SoundSink.Play();
								isSink = true;
							}
						}
					}
				}
				else{
					if (mJumpPrio < 1){
						CVector adjust;//調整用ベクトル
						//三角形と球の衝突判定
						if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
							//位置の更新
							mPosition = mPosition - adjust * -1;
							//行列の更新
							CCharacter::Update();

							if (yc->mpParent->mTag == CCharacter::EGRASS){
								mSound_Engine = EONGRASS;
							}
							else{
								mSound_Engine = ENOTONGRASS;
							}							
							
							//エンジン音を鳴らす
							if (mCarSpeed > 0.0f){
								if (isSoundEngine == false){
									mSound_Engine_Prev = mSound_Engine;
									if (mSound_Engine == EONGRASS){
										SoundEngine_Turf.Repeat();										
									}
									else if (mSound_Engine == ENOTONGRASS){
										SoundEngine.Repeat();
									}									
									isSoundEngine = true;
								}
								else{
									//芝生、非芝生の切り替わり
									if (mSound_Engine_Prev != mSound_Engine){
										mSound_Engine_Prev = mSound_Engine;
										if (mSound_Engine_Prev == EONGRASS){
											SoundEngine_Turf.Repeat();
											SoundEngine.Stop();
										}
										else if (mSound_Engine_Prev == ENOTONGRASS){
											SoundEngine.Repeat();
											SoundEngine_Turf.Stop();
										}
									}
								}
							}
							//車が停止している時
							else if (mCarSpeed == 0.0f){
								SoundEngine.Stop();
								SoundEngine_Turf.Stop();
								isSoundEngine = false;
							}
							//車がバックしている時
							else if (mCarSpeed < 0.0f){
								if (isSoundEngine == false){
									mSound_Engine_Prev = mSound_Engine;
									if (mSound_Engine == EONGRASS){
										SoundEngine_Turf.Repeat();
									}
									else if (mSound_Engine == ENOTONGRASS){
										SoundEngine.Repeat();
									}
									isSoundEngine = true;
								}
								else{
									//芝生、非芝生の切り替わり
									if (mSound_Engine_Prev != mSound_Engine){
										mSound_Engine_Prev = mSound_Engine;
										if (mSound_Engine_Prev == EONGRASS){
											SoundEngine_Turf.Repeat();
											SoundEngine.Stop();
										}
										else if (mSound_Engine_Prev == ENOTONGRASS){
											SoundEngine.Repeat();
											SoundEngine_Turf.Stop();
										}
									}
								}
							}
							if (yc->mpParent->mTag == CCharacter::EWALL){								
								//壁衝突処理の修正中
								if (mCarSpeed > 2.0f){
									mCarSpeed *= 0.95f;
									if (mCarSpeed < 2.0f){
										mCarSpeed = 2.0f;
									}
								}
							}
							else if(yc->mpParent->mTag == CCharacter::EJUMPER){//ジャンプ台に接触した時
								mVelocityJump = JUMPER01_POWER;
								SoundJump.Play();
							}
							else{
								mVelocityJump = 0;
								mRotation = CCollider::CalculateEulerAngle(mc, yc, mMatrixRotate, PI);
							}
						}
					}

				}
			}
			//自分のコライダが判定の時
			if (mc->mTag == CCollider::ESEARCH){
				//加速床に乗った時の処理
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector tmpVec;//値を返すためのベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
						//mCarSpeed += 10.0f;
						//mCarSpeed = MAXSPEED+10.0f;
						if (isBoost == false){
							SoundBoost.Play();
							new CEffect(mPosition + CVector(0.0f, 15.5f, 0.0f), 60.0f, 60.0f, TextureBoost, 3, 5, 1, 1);
						}
						isBoost = true;
						mBoostTime = 45;
					}
				}
			}
		}
		if (yc->mType == CCollider::ESPHERE){
			if (CCollider::Collision(mc, yc)){
				if (mc->mTag == CCollider::EBODY){
					if (yc->mpParent->mTag == CCharacter::EENEMY){
						if (yc->mTag == CCollider::EBODY){
							CVector adjust;//調整用ベクトル
							//球同士の衝突判定
							if (CCollider::Collision(mc, yc, &adjust)){
								//位置の更新
								mPosition = mPosition - adjust * -1;
								//行列の更新
								CCharacter::Update();
							}
						}
					}
				}
				
			}
		}
		break;
	}
}

//各チェックポイント通過時のリスポーン地点を決める
void CPlayer::SetRespawnPoint(int checknumber, CVector position, CVector rotation){
	mVCheckPositions[checknumber] = position;
	mVCheckRotations[checknumber] = rotation;
}

//スタート地点に着く(主にレース開始時に呼び出し)
void CPlayer::GetReady(){
	mPosition = mVCheckPositions[0];
	mRotation = mVCheckRotations[0];
	CCharacter::Update();
}

void CPlayer::TaskCollision()
{
	mColBody.ChangePriority();
	mColTire.ChangePriority();
	CollisionManager.Collision(&mColBody);
	CollisionManager.Collision(&mColTire);
}