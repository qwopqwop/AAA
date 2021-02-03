#ifndef CENEMY_H
#define CENEMY_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CSound.h"
#include "CPoint.h"

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
	//敵AIの強さによるスピード差
	float mCPULevelSpeed;

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

	//コライダの更新
	void TaskCollision();




	//誘導ポイント//中心、全エネミー共通のポイント
	static CPoint *mPoint;
	static CPoint *mPoint2;
	static CPoint *mPoint3;
	static CPoint *mPoint4;
	static CPoint *mPoint5;
	static CPoint *mPoint6;
	static CPoint *mPoint7;
	static CPoint *mPoint8;
	static CPoint *mPoint9;
	static CPoint *mPoint10;
	static CPoint *mPoint11;
	static CPoint *mPoint12;
	////乱数でのブレを含む誘導ポイント
	//CPoint *mPointRand;
	//CPoint *mPointRand2;
	//CPoint *mPointRand3;
	//CPoint *mPointRand4;
	//CPoint *mPointRand5;
	//CPoint *mPointRand6;
	//CPoint *mPointRand7;
	//CPoint *mPointRand8;
	//CPoint *mPointRand9;
	//CPoint *mPointRand10;
	//CPoint *mPointRand11;
	//CPoint *mPointRand12;

	//// *は使わない方がいい？
	//CPoint Point1;
	//CPoint Point2;
	//CPoint Point3;
	//CPoint Point4;
	//CPoint Point5;
	//CPoint Point6;
	//CPoint Point7;
	//CPoint Point8;
	//CPoint Point9;
	//CPoint Point10;
	//CPoint Point11;
	//CPoint Point12;

	static int mPointSize;//ポイントの数
	CPoint *mpPoint;//目指すポイント
	int mPointCnt;//ポイントのカウンタ

	//コライダ
	CCollider mSearch;

	CPoint *mpPoint2;
	CVector mVPoint;

	int mEnemyLap;
	bool isEnemyGoaled;
};


#endif