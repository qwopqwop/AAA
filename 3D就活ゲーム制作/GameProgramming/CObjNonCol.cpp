#include "CObjNonCol.h"

//コンストラクタ
//model:モデルのポインタ position:位置 rotation:回転 scale:拡縮
CObjNonCol::CObjNonCol(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
:CObj(model, position, rotation, scale, false)
{
	//コライダが無いため、主にステージの装飾に利用される。
	mTag = ENONE;
}