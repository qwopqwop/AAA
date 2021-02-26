#ifndef COBJBOOST_H
#define COBJBOOST_H

#include "CObj.h"

class CObjBoost :public CObj{
public:
	//コンストラクタで初期設定
	CObjBoost(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);

	////CObj側で衝突処理…は以前、処理が爆重になったのでやらない、やれない、やりたくない
	//void Collision(CCollider *m, CCollider *y);
	////更新処理のオーバーライド
	//void Update();
};
#endif