#ifndef CRIGIDOBJ_H
#define CRIGIDOBJ_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CSound.h"

class CRigidObj :public CCharacter{
private:
	//プレイヤーの体の当たり判定
	CCollider mColBody;
	CCollider mColBody2nd;
	//ジャンプ速度の変数
	float mVelocity;
	int mJumpPrio;
public:
	static CRigidObj*mpRigObj;
	
	//コライダの追加
	CCollider *mpCollider;
	
	//コンストラクタで初期設定
	CRigidObj(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
	~CRigidObj();

	//更新
	void Update();	
	//衝突処理
	void Collision(CCollider *mc, CCollider *yc);
	//コライダの更新
	void TaskCollision();
};


#endif