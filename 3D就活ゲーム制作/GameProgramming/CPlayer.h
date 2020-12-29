#ifndef CPLAYER_H
#define CPLAYER_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CSound.h"

class CPlayer :public CCharacter{
public:
	static CPlayer*mpPlayer;
	//デフォルトコンストラクタ
	CPlayer();

	//更新
	void Update();

	//プレイヤーの体の当たり判定
	CCollider mColBody;
	//プレイヤーのタイヤの当たり判定
	CCollider mColTire;

	//衝突処理
	void Collision(CCollider *mc, CCollider *yc);

	//ジャンプ速度の変数
	float mVelocityJump;
	//ジャンプ力(ジャンプ初速)の変数
	float mJumpV0;

	//ジャンプを当たり判定よりも早く処理させるもの
	int mJumpPrio;

	//移動速度
	float mMoveSpeed;
	float mADMoveX;
	float mWSMoveZ;
	float mCarSpeed;

	//回転速度
	float mTurnSpeed;

	bool mCanJump;//ジャンプ可能か(≒地面に足が着いてるか)
	int mCanSwim;//水泳可能か
	int mCanFire;//発砲可能か
	//弾の再発射間隔
	int FireCount;

	CSound SoundJump;
	CSound SoundShot;
	CSound SoundItemGet;
	CSound SoundBoost;
	CSound SoundEngine;//一番の難題
	bool isSoundEngine;
	CSound SoundHorn;
	CSound ShutUp;
	int mBuzzerCount;
	CSound SoundCollision;
	CSound SoundCollisionSmall;

	float mStartPoint[3];//{ X,Y,Z }

	bool mIsGetKey;
	bool mHaveBoat;

	bool mFlyingMode;

	int mChecks;

	bool isBoost;//ブースト中か(加速床で一定時間有効)
	float mBoostMaxSpeed;//ブーストで底上げされる最高速度の数値
	int mBoostTime;//ブースト状態が継続される時間

	bool CanMove;//プレイヤーが操作可能か否か(カウントダウン前・ゴール後などは否)

	static int RenderType;//ゲーム画面の描画とミニマップの描画を分ける変数


	//コライダの更新
	void TaskCollision();
};


#endif