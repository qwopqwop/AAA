#ifndef CRACECOURCEB_H
#define CRACECOURCEB_H

#include "CModel.h"
#include "CSceneRace.h"

#include "CSceneShadowMap.h"

class CRaceCourceB : public CSceneShadowMap {
public:
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
};

#endif