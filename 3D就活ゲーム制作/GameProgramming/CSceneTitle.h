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
	
	static int mCource_Number;//�R�[�XNo.
	static int mCPU_Level;//CPU�̋���
};

#endif