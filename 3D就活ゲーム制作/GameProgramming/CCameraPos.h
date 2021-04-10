#ifndef CCAMERAPOS_H
#define CCAMERAPOS_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CCameraPos :public CCharacter{
public:
	static CCameraPos*mpCamera;
	//デフォルトコンストラクタ
	CCameraPos();

	//更新
	void Update();

	//カメラの当たり判定
	CCollider mColCam;
	
	//衝突処理
	void Collision(CCollider *mc, CCollider *yc);

	////移動速度
	//float mMoveSpeed;
	//float mADMoveX;
	//float mWSMoveZ;
	//float mCarSpeed;
	////回転速度
	//float mTurnSpeed;
	
	bool CanMove;//プレイヤーが操作可能か否か(カウントダウン前・ゴール後などは否)
	
	//コライダの更新
	void TaskCollision();
};


#endif