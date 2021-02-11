#include "CRaceCourceC.h"
#include "CObj.h"


void CRaceCourceC::Init(){
	//ÉVÅ[ÉìÇÃê›íË
	mScene = ERACE3;

	CSceneRace::Init();

	
	new CObj(&mCource03, CVector(0.0f, 5.0f - 373.0f, -350.0f), CVector(), CVector(40.0f, 20.0f, 40.0f), 1);
	
}
void CRaceCourceC::Update(){
	CSceneRace::Update();

}