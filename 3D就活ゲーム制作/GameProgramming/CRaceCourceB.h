#ifndef CRACECOURCEB_H
#define CRACECOURCEB_H

#include "CModel.h"
#include "CSceneRace.h"

#include "CSceneShadowMap.h"

class CRaceCourceB : public CSceneShadowMap {
public:
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
};

#endif