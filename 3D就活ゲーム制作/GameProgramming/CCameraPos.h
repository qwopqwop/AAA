#ifndef CCAMERAPOS_H
#define CCAMERAPOS_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CCameraPos :public CCharacter{
private:
	float mZoom_distance;

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

	float mCameraSpeed;
	bool CanMove;//プレイヤーが操作可能か(カウントダウン前が不可)
	
	//コライダの更新
	void TaskCollision();

	CVector mVPoint;
	float mVCamY;//カメラの方向

};


#endif