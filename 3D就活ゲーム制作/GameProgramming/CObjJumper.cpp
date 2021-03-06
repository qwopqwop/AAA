#include "CObjJumper.h"

//コンストラクタ
//model:モデルのポインタ position:位置 rotation:回転 scale:拡縮
CObjJumper::CObjJumper(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
:CObj(model, position, rotation, scale)
{
	//ジャンプ台、種類によってぶっとばすパワーを変化させる予定。
	mTag = EJUMPER;
}