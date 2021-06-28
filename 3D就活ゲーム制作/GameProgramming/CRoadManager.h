#ifndef CROADMANAGER_H
#define CROADMANAGER_H

#include "CObjFloor.h"

class CRoadManager : public CObjFloor
{
public:
	//CRoadManager(モデルデータのポインタ,位置,回転,拡大縮小,スタート位置,進行方向,各ポイントの半径)
	CRoadManager(CModel*, const CVector& pos, const CVector& rot, const CVector& scale, const CVector& startPos, const CVector& foward, const float radius);
	//Init(モデルデータのポインタ,位置,回転,拡大縮小,スタート位置,進行方向)
	void Init(CModel*, const CVector& pos, const CVector& rot, const CVector& scale, const CVector& startPos, const CVector& foward, const float radius);
};

#endif
