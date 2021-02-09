#include "CRaceCourceC.h"
#include "CObj.h"


void CRaceCourceC::Init(){
	//シーンの設定
	mScene = ERACE3;

	CSceneRace::Init();

	//借り物
	new CObj(&mCource03, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(5110.0f, 5110.0f, 5110.0f), 1);
}
void CRaceCourceC::Update(){
	CSceneRace::Update();

}