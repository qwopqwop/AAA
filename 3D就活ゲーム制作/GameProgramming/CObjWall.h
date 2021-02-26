//#ifndef COBJWALL_H
//#define COBJWALL_H
//
//#include "CObj.h"
////#include"CModel.h"
//
//class CObjWall :public CObj{
//public:
//	//CObjWall(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
//
//	////更新処理のオーバーライド
//	//void Update();
//
//	//コライダの追加
//	CCollider *mpCollider;
//	//コンストラクタで初期設定
//	CObjWall(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
//	~CObjWall();
//
//	int mObjNumber;
//
//	////衝突処理
//	//void Collision(CCollider *m, CCollider *y);
//
//	void Update();
//
//	/*CObjWall();*/
//};
//#endif


#ifndef COBJWALL_H
#define COBJWALL_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CObjWall :public CCharacter{
public:
	//コライダの追加
	CCollider *mpCollider;
	//コンストラクタで初期設定
	CObjWall(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
	~CObjWall ();

	int mObjNumber;

	void Update();//virtual…再定義可能になる?

};
#endif


