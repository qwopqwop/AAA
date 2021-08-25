#include "CSceneTitle.h"

#include "CRectangle.h"

#define WAITTIME_STARTLOAD 60 +60+30 //�I�������߂Ă��玟�̃V�[���Ɉڍs����܂ł̎���
#define WAITTIME_NOWLOADING  110 //WAITTIME_STARTLOAD - 10 //�uNOW LOADING�v���\�������܂ł̎���
#define WAITTIME_STARTFADEOUT  WAITTIME_STARTLOAD - 90 //WAITTIME_STARTLOAD - 10 //�uNOW LOADING�v���\�������܂ł̎���

int CSceneTitle::mMode = 1; //�R�[�X��Lap���̏����ݒ�
int CSceneTitle::mDifficulty = 2; //�GAI�̋����̏����ݒ�@�@1:EASY�@2:NORMAL�@3:HARD

//���̃V�[���̎擾
CScene::EScene CSceneTitle::GetNextScene(){
	return mScene;
}


//�����������̃I�[�o�[���C�h
void CSceneTitle::Init() {
	//�e�N�X�`���t�@�C����ǂݍ���
	//�t�H���g�̐ݒ�
	CText::mFont.Load("font\\font2nd.tga");
	CText::mFont.SetRowCol(8, 256 / 16);
	CText::mFont2.Load("font\\FontG.tga");
	CText::mFont2.SetRowCol(1, 4096 / 64);
	CText::mFont3.Load("font\\FontDIY.tga");//����t�H���g
	CText::mFont3.SetRowCol(8, 176 / 11);
	//CText::mFont3.Load("FontDIYx5.tga");//����t�H���g
	//CText::mFont3.SetRowCol(8, 880 / 55);//�T�C�Y5�{(�v)
	
	//�V�[���̐ݒ�
	mScene = ETITLE;

	mSelectScene_Level = 1;
	mPrevDifficulty = mDifficulty;
	mVariable1 = 0;
	mPrevVariable1 = mVariable1;
	mVariable2 = 0;
	mPrevVariable2 = mVariable2;
	mStart = false;
	mStartWaitTime = 0;
	SoundMoveCarsol.Load("SE\\Carsol2.wav");
	SoundDecide.Load("SE\\Decision_Small(SF).wav");
	SoundCancel.Load("SE\\Cancel2.wav");
}
//�X�V�����̃I�[�o�[���C�h
void CSceneTitle::Update() {
	//2D�`��J�n
	Start2D(0, 800, 0, 600);
	//R,G,B,��
	float c[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//������̕`��
	CText::DrawString("3D-RACE", -278 + 400 + 68, 430, 36, 36);
	c[0] = c[1] = c[2] = 0.0f;
	glColor4fv(c);
	CText::DrawString("Demo Ver0.0", -278 + 400 + 68 + 131, 345 + 32, 8, 8, 3);
	c[0] = c[1] = c[2] = 1.0f;
	glColor4fv(c);
	CText::DrawString("Push Enter Key", 200, 177, 16, 16);
	//
	if (mSelectScene_Level == 1){
		CText::DrawString("[", 66 + mVariable2 * 250, 284 + mVariable1 * 50, 20, 30, 2);
		CText::DrawString("]", 262 + mVariable2 * 250, 284 + mVariable1 * 50, 20, 30, 2);
	}
	if (mSelectScene_Level == 2 && mStart == false){
		if (mDifficulty == 1){
			CText::DrawString("[", 190, 102, 16, 24, 2);
			CText::DrawString("]", 278, 102, 16, 24, 2);
		}
		if (mDifficulty == 2){
			CText::DrawString("[", 338, 102, 16, 24, 2);
			CText::DrawString("]", 474, 102, 16, 24, 2);
		}
		if (mDifficulty == 3){
			CText::DrawString("[", 534, 102, 16, 24, 2);
			CText::DrawString("]", 622, 102, 16, 24, 2);
		}
	}

	//�������O�͑I�����ɓ_�ł��Ȃ�
	if (mStartWaitTime > 20 || mStart == false){
		c[0] = c[1] = c[2] = 1.0f;
	}
	else if (mStartWaitTime % 8 < 4){
		c[0] = c[1] = c[2] = 0.5f;
	}
	glColor4fv(c);
	//���������͑I�����ɓ_�ł����邱�Ƃ��\

	////���[�h�I���Ɠ�Փx�I���ŃJ�[�\���̊�_���Ⴄ
	//if (mSelectScene_Level == 1){
	//	//CText::DrawString("O", 170 + mVariable2 * 250, 300 + mVariable1 * 50, 10, 10);
	//}
	////else if (mSelectScene_Level == 2){
	////	//if (mDifficulty == 1){
	////	//	//CText::DrawString("O", 175, 100, 10, 10);
	////	//	CText::DrawString("[", 190, 102, 16, 24, 2);
	////	//	CText::DrawString("]", 278, 102, 16, 24, 2);
	////	//}
	////	//if (mDifficulty == 2){
	////	//	//CText::DrawString("O", 323, 100, 10, 10);
	////	//	CText::DrawString("[", 338, 102, 16, 24, 2);
	////	//	CText::DrawString("]", 474, 102, 16, 24, 2);
	////	//}
	////	//if (mDifficulty == 3){
	////	//	//CText::DrawString("O", 519, 100, 10, 10);
	////	//	CText::DrawString("[", 534, 102, 16, 24, 2);
	////	//	CText::DrawString("]", 622, 102, 16, 24, 2);
	////	//}
	////	//CText::DrawString("O", -80 + mDifficulty * 250, 100, 10, 10);
	////}


	/*��Փx�I��(�GAI�̋���)*/
	if (mDifficulty == 1 && mSelectScene_Level == 2){//��Փx:EASY
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	}
	else{
		c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
	}
	//�I�����ɓ_�ł���
	if (mStart && mDifficulty == 1){
		if (mStartWaitTime > 20){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if (mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
	}
	glColor4fv(c);
	CText::DrawString("EASY", 200, 100, 12, 12);

	if (mDifficulty == 2 && mSelectScene_Level == 2){//��Փx:NORMAL
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	}
	else{
		c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
	}
	//�I�����ɓ_�ł���
	if (mStart && mDifficulty == 2){
		if (mStartWaitTime > 20){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if (mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
	}
	glColor4fv(c);
	CText::DrawString("NORMAL", 347, 100, 12, 12);

	if (mDifficulty == 3 && mSelectScene_Level == 2){//��Փx:HARD
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	}
	else{
		c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
	}
	//�I�����ɓ_�ł���
	if (mStart && mDifficulty == 3){
		if (mStartWaitTime > 20){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if (mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
	}
	glColor4fv(c);
	CText::DrawString("HARD", 543, 100, 12, 12);

	/*���[�h�I��(�X�e�[�W�I��)�̍���*/
	if (mVariable1 == 0 && mVariable2 == 0){
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	}
	else{
		c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
	}
	//�I�����ɓ_�ł���
	if (mStart && mVariable1 == 0 && mVariable2 == 0){
		if (mStartWaitTime > 20){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if (mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
	}
	glColor4fv(c);
	CText::DrawString("COURCE-1", 80, 280, 12, 14);

	if (mVariable1 == 0 && mVariable2 == 1){
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	}
	else{
		c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
	}
	//�I�����ɓ_�ł���
	if (mStart && mVariable1 == 0 && mVariable2 == 1){
		if (mStartWaitTime > 20){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if (mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
	}
	glColor4fv(c);
	CText::DrawString("COURCE-2", 330, 280, 12, 14);

	if (mVariable1 == 0 && mVariable2 == 2){
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	}
	else{
		c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
	}
	//�I�����ɓ_�ł���
	if (mStart && mVariable1 == 0 && mVariable2 == 2){
		if (mStartWaitTime > 20){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if (mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
	}
	glColor4fv(c);
	CText::DrawString("COURCE-3", 580, 280, 12, 14);

	//�Ō�ɒl��1.0f�ɖ߂�
	c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
	glColor4fv(c);

	char mrecord[70];// :���܂߂��ő啶�����̐ݒ�
	if (mMode == 1){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_A / 10000 % 100, CSceneRace::mRecord_A / 100 % 100, CSceneRace::mRecord_A % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mMode == 2){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_B / 10000 % 100, CSceneRace::mRecord_B / 100 % 100, CSceneRace::mRecord_B % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mMode == 3){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_C / 10000 % 100, CSceneRace::mRecord_C / 100 % 100, CSceneRace::mRecord_C % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mMode == 4){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_D / 10000 % 100, CSceneRace::mRecord_D / 100 % 100, CSceneRace::mRecord_D % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mMode == 5){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_E / 10000 % 100, CSceneRace::mRecord_E / 100 % 100, CSceneRace::mRecord_E % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mMode == 6){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_F / 10000 % 100, CSceneRace::mRecord_F / 100 % 100, CSceneRace::mRecord_F % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mMode == 127){
		sprintf(mrecord, "EDIT");
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}


	c[0] = c[1] = c[2] = 0.5f;
	glColor4fv(c);
	//DrawBox(0, 0, 640, 480, White, TRUE);
	c[0] = c[1] = c[2] = 1.0f;
	/*c[0] = rand() % 11 * 0.1f;
	c[1] = rand() % 11 * 0.1f;
	c[2] = rand() % 11 * 0.1f;*/
	glColor4fv(c);	


	/*glBegin(GL_TRIANGLES);	
	glVertex2d(0 + 200, 0 + 200);
	glVertex2d(400 +200 , 300 + 200);
	glVertex2d(0 + 200, 300 + 200);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(0 + 200, 0 + 200);
	glVertex2d(400 + 200, 0 + 200);
	glVertex2d(400 + 200, 300 + 200);
	glEnd();*/


	
	/*int x_zahyo = 400, y_zahyo = 300;
	int width = 100, height = 100;
	glBegin(GL_TRIANGLES);	
	glVertex2d(x_zahyo - width, y_zahyo - height);
	glVertex2d(x_zahyo + width, y_zahyo + height);
	glVertex2d(x_zahyo - width, y_zahyo + height);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(x_zahyo - width, y_zahyo - height);
	glVertex2d(x_zahyo + width, y_zahyo - height);
	glVertex2d(x_zahyo + width, y_zahyo + height);
	glEnd();*/
	
	/*CRectangle::DrawRectangle(10, 10, 10, 10, 10, 10, 10, 10);
	CRectangle::Render();*/
	//RenderRect(400, 300, 100, 100);
	
	float colo[] = { 1.0f, 1.0f, 1.0f, 0.5f };
	colo[3] = 0.6f;
	glColor4fv(colo);

	if (mStartWaitTime >= WAITTIME_STARTFADEOUT){
		c[0] = c[1] = c[2] = 0.0f;
		c[3] = 0.025f * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		glColor4fv(c);
		CRectangle::Render(400, 300, 400, 300);
	}	
	c[0] = c[1] = c[2] = c[3] = 1.0f;
	glColor4fv(c);



	//�V�[�����ڍs���钼�O�ŕ\��
	if (mStartWaitTime > WAITTIME_NOWLOADING){
		CText::DrawString("Please Wait...", 555, 14, 9, 11);
	}
	//2D�`��I��
	End2D();

	if (CKey::Once(VK_RETURN)){
		//�J�n�t���O����������A�ł��悤�����͖�Ȃ�
		if (mStart == false){
			SoundDecide.Play();
		}
		if (mSelectScene_Level < 2){
			//���ɑI�ԍ��ڂ�
			mSelectScene_Level++;
		}
		else{
			//�I�яI������Q�[���J�n
			mStart = true;
		}
	}
	if (mStart){
		if (mStartWaitTime < WAITTIME_STARTLOAD){
			mStartWaitTime++;
		}
		else{
			//���̃V�[���̓��[�X���
			printf("�I�������R�[�X�ԍ�:No.%d  ", mMode);
			printf("�I������CPU�̃��x��:%d\n", mDifficulty);
			//�I�������R�[�X�ɑΉ�����V�[���ֈڍs			
			if (mMode == 1){
				//���̃V�[���̓R�[�X1
				mScene = ERACE1;
			}
			else if (mMode == 2){
				//���̃V�[���̓R�[�X2
				mScene = ERACE2;
			}
			else if (mMode == 3){
				//���̃V�[���̓R�[�X3
				mScene = ERACE3;
			}
			else if (mMode == 4){
				//���̃V�[���̓R�[�X3
				mScene = ERACE4;
			}
			else if (mMode == 5){
				//���̃V�[���̓R�[�X3
				mScene = ERACE5;
			}
			else if (mMode == 6){
				//���̃V�[���̓R�[�X3
				mScene = ERACE6;
			}
			else if (mMode == 127){
				//�R�[�X�G�f�B�^�Ɉڍs
				mScene = EEDIT;
			}
		}
	}

	//�܂��I�����ĂȂ���
	if (mStart == false){
		//���L�[�őI��
		//�I�����1:���[�h�̑I��
		if (mSelectScene_Level == 1){
			//if (CKey::Once(VK_UP)){
			//	//���̃V�[���̓Q�[��
			//	if (mVariable1 < 0){
			//		mVariable1 += 1;
			//	}
			//}
			//if (CKey::Once(VK_DOWN)){
			//	//���̃V�[���̓Q�[��
			//	if (mVariable1 > -1){
			//		mVariable1 -= 1;
			//	}
			//}
			if (CKey::Once(VK_LEFT)){
				//���̃V�[���̓Q�[��
				if (mVariable2 > 0){
					mVariable2 -= 1;
				}
			}
			if (CKey::Once(VK_RIGHT)){
				//���̃V�[���̓Q�[��
				if (mVariable2 < 2){
					mVariable2 += 1;
				}
			}
		}
		//�I�����2�F�GAI�̋����̐ݒ�
		else if (mSelectScene_Level == 2){
			if (CKey::Once(VK_LEFT)){
				if (mDifficulty > 1){
					mDifficulty--;
				}
				else{
					mDifficulty = 3;
				}
			}
			if (CKey::Once(VK_RIGHT)){
				if (mDifficulty < 3){
					mDifficulty++;
				}
				else{
					mDifficulty = 1;
				}
			}
			//Esc�L�[���ABackSpace�L�[�ŁA�O�̑I����ʂɖ߂�
			if (CKey::Once(VK_BACK) || CKey::Once(VK_ESCAPE)){
				mSelectScene_Level--;
				SoundCancel.Play();
			}
		}
	}
	if (mVariable1 == 0 && mVariable2 == 0){
		mMode = 1;
	}
	else if (mVariable1 == 0 && mVariable2 == 1){
		mMode = 2;
	}
	else if (mVariable1 == 0 && mVariable2 == 2){
		mMode = 5;
	}
	else if (mVariable1 == -1 && mVariable2 == 0){
		mMode = 127;//EDITOR��No.
	}
	else if (mVariable1 == -1 && mVariable2 == 1){
		mMode = 3;
	}
	else if (mVariable1 == -1 && mVariable2 == 2){
		mMode = 6;
	}

	//�J�[�\���̏ꏊ��1f�O�ƕς�����u��
	if (mPrevVariable1 != mVariable1 || mPrevVariable2 != mVariable2 || mPrevDifficulty != mDifficulty){
		SoundMoveCarsol.Play();
		mPrevVariable1 = mVariable1;
		mPrevVariable2 = mVariable2;
		mPrevDifficulty = mDifficulty;
	}

}

void CSceneTitle::Render(){

}

void CSceneTitle::RenderRect(int x, int y, int w, int h){
	glBegin(GL_TRIANGLES);
	glVertex2d(x - w, y - h);
	glVertex2d(x + w, y + h);
	glVertex2d(x - w, y + h);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(x - w, y - h);
	glVertex2d(x + w, y - h);
	glVertex2d(x + w, y + h);
	glEnd();
}