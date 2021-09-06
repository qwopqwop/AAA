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
	int mVariable1;
	int mPrevVariable1;
	int mVariable2;
	int mPrevVariable2;
	bool mStart;
	int mStartWaitTime;
	int mPrevCPU_Level;
	int mSelectScene_Level;//�I����ʂ̉��i�K�ڂ�
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

	
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	CSound SoundCancel;
	
	static int mCource_Number;//�R�[�XNo.
	static int mCPU_Level;//CPU�̋���	
};

#endif