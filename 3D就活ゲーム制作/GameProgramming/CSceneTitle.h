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
public:
	//����������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Render();

	void RenderRect(int x, int y, int w, int h);

	//���̃V�[���̎擾
	EScene GetNextScene();

	int mVariable1;
	int mPrevVariable1;
	int mVariable2;
	int mPrevVariable2;
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	CSound SoundCancel;
	bool mStart;
	int mStartWaitTime;
	static int mMode;
	static int mDifficulty;
	int mPrevDifficulty;
	int mSelectScene_Level;//�I����ʂ̉��i�K�ڂ�
};

#endif