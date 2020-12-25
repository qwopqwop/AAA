#ifndef COBJ_H
#define COBJ_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CObj :public CCharacter{
public:
	//コライダの追加
	CCollider *mpCollider;
	//コンストラクタで初期設定
	CObj(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale, int objnum);
	~CObj();

	int mObjNumber;
	
	//衝突処理
	void Collision(CCollider *m, CCollider *y);

	void Update();

	//bool mDeleteFlag;

	////コライダの更新
	//void TaskCollision();
};

class CObjection :public CObj{
public:
	//コライダの更新
	void TaskCollision();
};
#endif