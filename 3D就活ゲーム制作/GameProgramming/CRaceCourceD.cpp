#include "CRaceCourceD.h"
#include "CObj.h"


void CRaceCourceD::Init(){
	//�V�[���̐ݒ�
	mScene = ERACE4;

	CSceneRace::Init();

	//�؂蕨
	new CObj(&mCource04, CVector(-360.0f, 5.0f - 33.0f, -230.0f), CVector(), CVector(5110.0f, 5110.0f, 5110.0f), 1);
}
void CRaceCourceD::Update(){
	CSceneRace::Update();

}