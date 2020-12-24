#ifndef CENEMY_H
#define CENEMY_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CSound.h"

class CEnemy :public CCharacter{
public:
	static CEnemy*mpEnemy;
	//デフォルトコンストラクタ
	CEnemy();

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

	//移動速度
	float mMoveSpeed;
	float mADMoveX;
	float mWSMoveZ;
	float mCarSpeed;

	//回転速度
	float mTurnSpeed;

	bool mCanJump;//ジャンプ可能か(≒地面に足が着いてるか)

	CSound SoundBoost;
	CSound SoundEngine;//一番の難題
	bool isSoundEngine;
	CSound SoundCollision;
	CSound SoundCollisionSmall;

	float mStartPoint[3];//{ X,Y,Z }

	int mChecks;

	bool isBoost;//ブースト中か(加速床で一定時間有効)
	float mBoostMaxSpeed;//ブーストで底上げされる最高速度の数値
	int mBoostTime;//ブースト状態が継続される時間

	bool CanMove;//プレイヤーが操作可能か否か(カウントダウン前・ゴール後などは否)

	static int RenderType;//ゲーム画面の描画とミニマップの描画を分ける変数
};


#endif