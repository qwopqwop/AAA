#ifndef CSCENETITLE_H
#define CSCENETITLE_H
#include "CText.h"
#include "CScene.h"
#include "CKey.h"
#include <Windows.h>
#include "CSound.h"

class CSceneTitle : public CScene {
public:
	//�����������̃I�[�o�[���C�h
	void Init() {
		//�e�N�X�`���t�@�C����ǂݍ���
		//�t�H���g�̐ݒ�
		CText::mFont.Load("font2nd.tga");
		CText::mFont.SetRowCol(8, 256 / 16);
		CText::mFont2.Load("FontG.tga");
		CText::mFont2.SetRowCol(1, 4096 / 64);
		CText::mFont3.Load("font.tga");//�摜�̓s����
		CText::mFont3.SetRowCol(8, 256 / 16);//�\������

		//�V�[���̐ݒ�
		mScene = ETITLE;

		mVariable1 = 0;
		mPrevVariable1 = mVariable1;
		mVariable2 = 0;
		mPrevVariable2 = mVariable2;
		mStart = false;
		mStartWaitTime = 0;
		SoundMoveCarsol.Load("SE\\�J�[�\��2.wav");
		SoundDecide.Load("SE\\����Q������iSF�n�j.wav");
	}
	//�X�V�����̃I�[�o�[���C�h
	void Update() {
		//2D�`��J�n
		Start2D(0, 800, 0, 600);
		//������̕`��
		CText::DrawString("3D-RACE", -278 + 400 + 68, 430, 36, 36);
		CText::DrawString("EXTREME", -278 + 400 + 68+96-24 +6*5, 370, 18, 18, 2);
		CText::DrawString("Push Enter Key", 200, 150, 16, 16);
		
		float c[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		if (mStartWaitTime > 20 || mStart == false){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if(mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
		glColor4fv(c);
		CText::DrawString("O", 170 + mVariable2 * 250, 300 + mVariable1 * 50, 10, 10);


		
		if (mVariable1 == 0 && mVariable2 == 0){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartA", 200, 300, 16, 16);
		if (mVariable1 == 0 && mVariable2 == 1){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartB", 450, 300, 16, 16);
		if (mVariable1 == -1 && mVariable2 == 0){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartC", 200, 250, 16, 16);
		if (mVariable1 == -1 && mVariable2 == 1){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartD", 450, 250, 16, 16);
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		glColor4fv(c);
		//2D�`��I��
		End2D();
		
		if (CKey::Once(VK_RETURN)){
			mStart = true;
			SoundDecide.Play();
		}
		if (mStart){
			if (mStartWaitTime < 60){
				mStartWaitTime++;
			}
			else{
				//���̃V�[���̓Q�[��
				if (mVariable1 == 0 && mVariable2 == 0){
					mMode = 1;
				}
				else if (mVariable1 == 0 && mVariable2 == 1){
					mMode = 2;
				}
				else if (mVariable1 == -1 && mVariable2 == 0){
					mMode = 3;
				}
				else if (mVariable1 == -1 && mVariable2 == 1){
					mMode = 4;
				}
				printf("MODE:%d\n", mMode);
				mScene = ERACE1;
			}
		}

		//�܂��I�����ĂȂ���
		if (mStart == false){
			//���L�[
			if (CKey::Once(VK_UP)){
				//���̃V�[���̓Q�[��
				if (mVariable1 < 0){
					mVariable1 += 1;
				}
			}
			if (CKey::Once(VK_DOWN)){
				//���̃V�[���̓Q�[��
				if (mVariable1 > -1){
					mVariable1 -= 1;
				}
			}
			if (CKey::Once(VK_LEFT)){
				//���̃V�[���̓Q�[��
				if (mVariable2 > 0){
					mVariable2 -= 1;
				}
			}
			if (CKey::Once(VK_RIGHT)){
				//���̃V�[���̓Q�[��
				if (mVariable2 < 1){
					mVariable2 += 1;
				}
			}			
		}

		//�J�[�\���̏ꏊ��1f�O�ƕς�����u��
		if (mPrevVariable1 != mVariable1 || mPrevVariable2 != mVariable2){
			SoundMoveCarsol.Play();
			mPrevVariable1 = mVariable1;
			mPrevVariable2 = mVariable2;
		}

	}
	//�`�揈����ΌN��
	void Render();

	//���̃V�[���̎擾
	EScene GetNextScene();

	int mVariable1;
	int mPrevVariable1;
	int mVariable2;
	int mPrevVariable2;
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	bool mStart;
	int mStartWaitTime;
	static int mMode;
	
};

#endif