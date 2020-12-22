#ifndef CSCENETITLE_H
#define CSCENETITLE_H
#include "CText.h"
#include "CScene.h"
#include "CKey.h"
#include <Windows.h>

class CSceneTitle : public CScene {
public:
	//�����������̃I�[�o�[���C�h
	void Init() {
		//�e�N�X�`���t�@�C����ǂݍ���
		//�t�H���g�̐ݒ�
		/*CText::mFont.Load("FontG.tga");
		CText::mFont.SetRowCol(1, 4096 / 64);*/

		CText::mFont.Load("font2nd.tga");
		CText::mFont.SetRowCol(8, 256 / 16);

		//�V�[���̐ݒ�
		mScene = ETITLE;
	}
	//�X�V�����̃I�[�o�[���C�h
	void Update() {
		//2D�`��J�n
		Start2D(0, 800, 0, 600);
		//������̕`��
		CText::DrawString("RACE GAME", -278 + 400, 10 + 400, 36, 36);
		CText::DrawString("Push Enter Key", 200, 150, 16, 16);
		//2D�`��I��
		End2D();
		
		if (CKey::Once(VK_RETURN)){
			//���̃V�[���̓Q�[��
			mScene = ERACE1;
		}
	}
	//�`�揈����ΌN��
	void Render();

	//���̃V�[���̎擾
	EScene GetNextScene();
};

#endif