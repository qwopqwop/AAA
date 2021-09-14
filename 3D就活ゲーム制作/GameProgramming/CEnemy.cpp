#include "CEnemy.h"
#include "CKey.h"

#include "CBullet.h"
#include "CSceneTitle.h"

//乱数を実装するインクルード群
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//三角関数関連のインクルード
#include <math.h>
#define PI 3.141592

//衝突時のエフェクト追加
#include "CEffect.h"
extern std::shared_ptr<CTexture> TextureExp;
extern std::shared_ptr<CTexture> TextureHit;
extern std::shared_ptr<CTexture> TextureBoost;
#include "CTaskManager.h"

CEnemy *CEnemy::mpEnemy = 0;

#define G (9.8f / 90.0f)//重力加速度
#define JUMPV0 (16.0f)//ジャンプ初速

#define MAXSPEED 20.0f//車の最高速度
#define MINSPEED 1.0f//車の最低速度
#define MAXSPEED_BACK 4.0f//車の後退する最大速度
#define CAR_POWER 0.1f//1フレーム辺りの車の加速していく量
#define CAR_BREAK_POWER 0.1f//前進中のブレーキの強さ

#define DECELERATE 0.1f //車の減速する量
#define FIX_ANGLE_VALUE 1.0f //角度が0度に向けて調整される量(回転値X,Z用)
#define JUMPER01_POWER 3.0f //ジャンプ台1でのジャンプ力
#define RESTART_TIME 15*60 //敵が詰まった時、一定時間経過でリスタートさせる(秒数*60fps)

CEnemy::CEnemy()
//車体のY座標は0.0fにしたいんだけど・・・
//0.0fにしたら車体が浮いてるように見えてしまう
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
, mColTire(this, CVector(0.0f, -16.0f + 15.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
, mSearch(this, CVector(0.0f, 20.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 15)
{
	srand(time(NULL));

	mpEnemy = this;

	mScale = CVector(2.5f * 3, 2.5f * 3, 2.5f * 3);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//バネ取得後は2.3fの予定
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//車の速度の初期化
	mTurnSpeed = 0.0f;

	mCanJump = false;
	CanMove = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EENEMY;
	mEnemyAI = EPRO;

	//スタート地点の座標を設定;
	mStartPoint[0] = 350.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -100.0f;

	mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;
	mSearch.mTag = CCollider::ESEARCH;
	
	mPointTime = 0;//現ポイントに移ってからの経過時間

	mpPoint = mPoint;
	mVPoint = mpPoint->mPosition;//一番最初は分散無し
	mVPoint_prev = mVPoint;

	mMaxSpeed_PtoP = 20.0f;

	mEnemyLap = 1;//敵のラップ数を１周目に設定する
	isTouchGoal = false;
	isEnemyGoaled = false;//まだゴールしてない状態にする
	mGoalTime = 0; mRank = 1;
}

void CEnemy::Update(){

	//速度調整
	if (CSceneTitle::mCource_Number == 1){
		//次のポイントから次の次のポイントへのベクトル
		CVector vNext = mpPoint->GetNextPoint()->mPosition - mPosition;
		//現在の向き
		CVector vLeft = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//内積から曲がり具合を求める(0:90°　1.0：真っすぐ）
		float corve = abs(vLeft.Dot(vNext.Normalize()));
		if (corve > 0.9f){
			corve = 0.7f;
		}
		else if (corve < 0.7f){
			corve = 1.0f;
		}
		//速度上限の計算
		mMaxSpeed_PtoP = MAXSPEED * corve;
	}
	else if(CSceneTitle::mCource_Number == 2){
		//次のポイントから次の次のポイントへのベクトル
		CVector vNext = mpPoint->GetNextPoint()->mPosition - mPosition;
		//現在の向き
		CVector vLeft = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//内積から曲がり具合を求める(0:90°　1.0：真っすぐ）
		float corve = abs(vLeft.Dot(vNext.Normalize()));
		/*if (corve > 0.9f){
			corve = 0.05f;
		}*/
		if (corve < 0.5f){
			corve = 1.0f;
		}
		//速度上限の計算
		mMaxSpeed_PtoP = MAXSPEED * corve;
	}
	else if (CSceneTitle::mCource_Number == 5){
		//次のポイントから次の次のポイントへのベクトル
		CVector vNext = mpPoint->GetNextPoint()->mPosition - mPosition;
		//現在の向き
		CVector vLeft = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//内積から曲がり具合を求める(0:90°　1.0：真っすぐ）
		float corve = abs(vLeft.Dot(vNext.Normalize()));
		if (corve > 0.6f){
			corve = 0.05f;
		}
		else if (corve < 0.5f){
			corve = 1.0f;
		}
		//速度上限の計算
		mMaxSpeed_PtoP = MAXSPEED * corve;
	}
	else{		
		mMaxSpeed_PtoP = 20.0f;
	}
	//スピードは最低速度を下回らない
	if (mMaxSpeed_PtoP < MINSPEED){
		mMaxSpeed_PtoP = MINSPEED;
	}

	//ポイントへのベクトルを求める
	CVector dir = mVPoint - mPosition;
	//左方向へのベクトルを求める
	CVector left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);

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


	if (CKey::Push(VK_UP) && CanMove && mCarSpeed < mMaxSpeed_PtoP || mChecks >= 0 && CanMove && mCarSpeed < mMaxSpeed_PtoP){
		if (mCarSpeed < MAXSPEED + mBoostMaxSpeed){
			/*if (left.Dot(dir) > -5.0f && left.Dot(dir) < 5.0f){
				mCarSpeed += CAR_POWER;
			}*/
			mCarSpeed += CAR_POWER;
		}
	}
	/*else if (CKey::Push(VK_DOWN) && CanMove || left.Dot(dir) < -5.0f && CanMove || left.Dot(dir) > 5.0f && CanMove)*/
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

	if (mMaxSpeed_PtoP<2.0f){
		mTurnSpeed *= 2.0f;//低速時は回転性能アップ※停止すると回転不可
	}

	mRotation.mY += mTurnSpeed;

	if (mRotation.mZ > 180){
		mRotation.mZ = -180;
	}
	else if (mRotation.mZ < -180){
		mRotation.mZ = 180;
	}
		
	//X,Z方向の移動とY軸方向(重力)の移動は別々に行う
	mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();
	//常に地面に対して垂直に落下
	mPosition = CVector(0.0f, mVelocityJump*2.0f, 0.0f) *
		CMatrix().RotateZ(0) *
		CMatrix().RotateX(0) *
		CMatrix().RotateY(0)
		*mMatrixTranslate;

	//コースアウトした時、もしくは壁に詰まる等して目標地点が一定時間更新されなかった時
	if (mPosition.mY < -700.0f || mPointTime > RESTART_TIME){
		mPointTime = 0;
		//落下の勢いを0にする
		mVelocityJump = 0.0f;
		//車の速度を0に
		mCarSpeed = 0.0f;
		//1つ前の目標地点に戻される
		mPosition = mVPoint_prev;
	}
	CCharacter::Update();

	//重力の影響を反映する
	mVelocityJump -= G;
	
	if (CKey::Push('I')){
		mpPoint->mRotation.mY++;
	}	
}

void CEnemy::Collision(CCollider *mc, CCollider *yc){
	//自身のコライダタイプの判定
	switch (mc->mType){
	case CCollider::ESPHERE://球コライダ
		//相手のコライダが三角コライダの時
		if (yc->mType == CCollider::ETRIANGLE){
			//自分のコライダが本体の時
			if (mc->mTag == CCollider::EBODY){
				//芝生通過中の処理
				if (yc->mpParent->mTag == CCharacter::EGRASS){
					CVector aiueo;//仮のベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//ブースト効果の方が優先される
						if (isBoost == false){
							//一定速度までスピード低下
							if (mCarSpeed > 3.2f + 1.8f){
								if (mCarSpeed > 4.0f + 1.8f){
									mCarSpeed -= 0.8f;
								}
								else{
									mCarSpeed = 3.2f + 1.8f;
								}
							}
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//中間地点1
					if (mChecks == 0){
						//各中間地点を通過しないと1周したとみなされない
						CVector aiu;//値返し用ベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 1;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//中間地点2
					if (mChecks == 1){
						//各中間地点を通過しないと1周したとみなされない
						CVector aiu;//値返し用ベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 2;
							//mChecks = 0;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//中間地点3
					if (mChecks == 2){
						//各中間地点を通過しないと1周したとみなされない
						CVector aiu;//値返し用ベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 3;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::EGOALPOINT){//ゴール地点
					if (mChecks == 3){
						//各中間地点を通過していなければ1周判定がなされない
						CVector aiu;//値返し用ベクトル
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							isTouchGoal = true;
						}
					}
				}
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
					//これらのタグが付いたオブジェクトは通過可能
				}
				else{
					CVector adjust;//調整用ベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
						//位置の更新
						mPosition = mPosition - adjust * -1;
						//行列の更新
						CCharacter::Update();
						if (yc->mpParent->mTag == CCharacter::EWALL){
							//速い時に衝突で減速、遅い時の衝突は特に変化なし
							if (mCarSpeed > 6.5f){
								mCarSpeed = 2.0f;
								//激突時、エフェクト発生
								new CEffect(mPosition + CVector(0.0f, 35.0f, 0.0f), 100.0f, 100.0f, TextureExp, 4, 4, 1, 0);
							}
							else if (mCarSpeed > 4.0f){
								mCarSpeed = 2.0f;
								//軽くぶつけた時もエフェクト発生
								new CEffect(mPosition + CVector(0.0f, 15.5f, 0.0f), 60.0f, 60.0f, TextureHit, 3, 8, 1, 1);
							}
							else{
								//壁にこすりながらの移動時、速度が遅くなる
								if (mCarSpeed > 2.0f){
									mCarSpeed = 2.0f;
								}
							}
						}
						else if (yc->mpParent->mTag == CCharacter::EJUMPER){//ジャンプ台に接触した時
							mVelocityJump = JUMPER01_POWER;
							mCanJump = true;
						}
						else{
							mVelocityJump = 0;
							mCanJump = true;
							mRotation = CCollider::CalculateEulerAngle(mc, yc, mMatrixRotate, PI);
						}
					}
					

				}
			}
			//自分のコライダが索敵コライダの時
			if (mc->mTag == CCollider::ESEARCH){
				//加速床に乗った時の処理
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector aiueo;//とりまベクトル
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						if (isBoost == false){
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
				//
				if (mc->mTag == CCollider::EBODY){
					if (yc->mpParent->mTag == CCharacter::EPLAYER
						&& yc->mTag == CCollider::EBODY){
						CVector adjust;//調整用ベクトル
						////		//球同士の衝突判定
						if (CCollider::Collision(mc, yc, &adjust)){
							//位置の更新
							mPosition = mPosition - adjust * -1;
							//行列の更新
							CCharacter::Update();
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
						}
					}					
				}
				//
				if (mc->mTag == CCollider::ESEARCH){
					//ポインタからポインタに向けて移動する
					if (yc->mpParent->mTag == CCharacter::EPOINT){
						CVector adjust;//調整用ベクトル
						//		//球同士の衝突判定
						if (CCollider::Collision(mc, yc, &adjust)){
							//衝突したポインタと目指しているポインタが同じ時
							if (yc->mpParent == mpPoint){
								//ポイント経過時間のリセット
								mPointTime = 0;
								//とりあえず先にint付けで生成しておく
								int r = (mc->mRadius + yc->mRadius) * 0.8f;
								int gap = (rand() % (r * 2) - r);
								//敵AIのLvにより分散値も変化
								if (CSceneTitle::mCPU_Level == 1){
									r = (mc->mRadius + yc->mRadius) * 0.5f;
									gap = (rand() % (r * 2) - r);
								}
								else if (CSceneTitle::mCPU_Level == 2){
									r = (mc->mRadius + yc->mRadius) * 0.4f;
									gap = (rand() % (r * 2) - r);
								}
								else if (CSceneTitle::mCPU_Level == 3){
									r = (mc->mRadius + yc->mRadius) * 0.2f;
									gap = (rand() % (r * 2) - r);
								}
								//次のポイントを設定する
								SetNextPoint(mpPoint, gap, false);								
							}
						}
					}
				}
			}
		}
		break;
	}
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

//SetNextPoint(現在のポイント,誤差の有無,誤差の量,現在のポイントが最終ポイントか)
void CEnemy::SetNextPoint(CPoint *current_point, int gap_amount, bool iscurrentpointlast){
	bool gap = true;
	if (CSceneTitle::mCource_Number == 5){
		if (mChecks < 1){//中間ポイント1まで誤差無し
			gap = false;
		}
	}

	//「一つ前の目標地点」の更新
	mVPoint_prev = mpPoint->mPosition;
	//現ポイントは最終ポイントか
	if (iscurrentpointlast){
		//誤差の有無
		if (gap){
			mVPoint = mPoint->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap_amount;
		}
		else{
			mVPoint = mPoint->mPosition;
		}
		//最初の目標地点に戻る
		mpPoint = mPoint;
	}
	else{
		CPoint *nexp = GetNextPoint(current_point, iscurrentpointlast);
		//誤差の有無
		if (gap){//ぶれ有り
			mVPoint = nexp->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap_amount;
		}
		else{//ぶれ無し
			mVPoint = nexp->mPosition;
		}
		//次の目標地点に移る
		mpPoint = nexp;
	}	
}

//次のポイントの取得
CPoint* CEnemy::GetNextPoint(CPoint *current_point, bool iscurrentpointlast){
	return current_point->GetNextPoint();
}

//誘導ポイント
CPoint *CEnemy::mPoint;