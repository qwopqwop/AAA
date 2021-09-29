#ifndef CSCENETITLE_H
#define CSCENETITLE_H
#include "CText.h"
#include "CScene.h"
#include "CKey.h"
#include <Windows.h>
#include "CSound.h"
//�E�E�E
#include "CSceneRace.h"

class CSceneTitle : public CScene {
private:
	int mCarsol_Pos;
	bool mStart;
	int mStartWaitTime;
	int mSelect_Step;//�I����ʂ̉��i�K�ڂ�
	int mLevel;//�G�̃��x��
	int mCourceNum;
public:
	//����������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Render();

	//���̃V�[���̎擾
	EScene GetNextScene();
	
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	CSound SoundCancel;
	
	//CPU�̋���
	enum ECPU_Level{
		EEASY = 1,//�ȒP
		ENORMAL,//����
		EHARD,//���
	};
	static ECPU_Level mCPU_Level;

	//CPU�̋���
	enum ECource{
		ECOURCEEDITOR = 0,//�G�f�B�^�[
		ECOURCE1,//�R�[�XA
		ECOURCE2,//�R�[�XB...
		ECOURCE3,
		ECOURCE4,
		ECOURCE5,		
	};
	static ECource mCource;
};

#endif