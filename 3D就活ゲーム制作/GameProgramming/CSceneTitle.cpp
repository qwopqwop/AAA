#include "CSceneTitle.h"
#include "CRectangle.h"

#define WAITTIME_STARTLOAD 150 //�I�������A���̃V�[���ڍs�܂ł̎���
#define WAITTIME_NOWLOADING  110 //�uNOW LOADING�v���\�������܂ł̎���
#define WAITTIME_STARTFADEOUT  WAITTIME_STARTLOAD - 90 //�uNOW LOADING�v���t�F�[�h�A�E�g���n�܂�܂ł̎���

//int CSceneTitle::mCource_Number = 1; //�R�[�X��Lap���̏����ݒ�
CSceneTitle::ECPU_Level CSceneTitle::mCPU_Level = CSceneTitle::ENORMAL;//�GAI�̋����̏����ݒ�

//int CSceneTitle::mCource_Number;//�R�[�XNo.
CSceneTitle::ECource CSceneTitle::mCource = CSceneTitle::ECOURCE1;

//CSceneTitle::EMode CSceneTitle::mMode = CSceneTitle::EMODE_GRANDPRIX;

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
	
	//�V�[���̐ݒ�
	mScene = ETITLE;

	mLevel = mCPU_Level;
	mCourceNum = mCource;
	mSelect_Step = 1;
	mCarsol_Pos = 0;
	mStart = false;
	mStartWaitTime = 0;
	SoundMoveCarsol.Load("SE\\Carsol2.wav");
	SoundDecide.Load("SE\\Decision_Small(SF).wav");
	SoundCancel.Load("SE\\Cancel2.wav");
}
//�X�V�����̃I�[�o�[���C�h
void CSceneTitle::Update() {
	if (CKey::Once(VK_RETURN)){
		//�J�n�t���O����������A�ł��悤�����͖�Ȃ�
		if (mStart == false){
			SoundDecide.Play();
		}
		if (mSelect_Step == 1){
			//���ɑI�ԍ��ڂ�
			mSelect_Step++;

			
		}
		else if (mSelect_Step == 2){
			//�I�яI������Q�[���J�n
			mStart = true;
		}
	}

	if (mStart){
		if (mStartWaitTime < WAITTIME_STARTLOAD){
			mStartWaitTime++;
		}
		else{
			switch (mLevel)
			{
			case 1:
				mCPU_Level = EEASY;
				break;
			case 2:
				mCPU_Level = ENORMAL;
				break;
			case 3:
				mCPU_Level = EHARD;
				break;
			default:
				printf("�s���ȓ�Փx���I������܂����@��Փx��normal�ɐݒ肵�܂�\n");
				mCPU_Level = ENORMAL;//
				break;
			}			
			//���̃V�[���̓��[�X���
			printf("�I�������R�[�X�ԍ�:No.%d  ", mCource);
			printf("�I������CPU�̃��x��:%d\n", mCPU_Level);
			
			/*if (mMode == EMODE_GRANDPRIX)
				printf("�O�����v��\n");
			if (mMode == EMODE_TIMEATTACK)
				printf("�^�C���A�^�b�N\n");
			if (mMode == EMODE_EDITOR)
				printf("�G�f�B�b�g\n");*/

			//�I�������R�[�X�ɑΉ�����V�[���ֈڍs			
			if (mCource == 1){
				//���̃V�[���̓R�[�X1
				mScene = ERACE1;
			}
			else if (mCource == 2){
				//���̃V�[���̓R�[�X2
				mScene = ERACE2;
			}
			else if (mCource == 3){
				//���̃V�[���̓R�[�X3
				mScene = ERACE3;
			}
			else if (mCource == 4){
				//���̃V�[���̓R�[�X3
				mScene = ERACE4;
			}
			else if (mCource == 5){
				//���̃V�[���̓R�[�X3
				mScene = ERACE5;
			}
			else if (mCource == 0){
				//�R�[�X�G�f�B�^�Ɉڍs
				mScene = EEDIT;
			}
		}
	}
	//�܂��I�����ĂȂ���
	if (mStart == false){
		//���L�[�őI��

		//�I�����1:�O�����v�����[�h(GP)or�^�C���A�^�b�N���[�h(TA)�̑I��

		//�I�����2:GP���[�h����Փx�I��    TA���[�h���R�[�X�I��


		//�I�����1:�R�[�X�̑I��
		if (mSelect_Step == 1){
			//��,���L�[�ŃR�[�X�I��
			if (CKey::Once(VK_LEFT)){
				//���̃V�[���̓Q�[��
				if (mCarsol_Pos > 0){
					mCarsol_Pos -= 1;
					SoundMoveCarsol.Play();
				}

				
			}
			if (CKey::Once(VK_RIGHT)){
				//���̃V�[���̓Q�[��
				if (mCarsol_Pos < 2){
					mCarsol_Pos += 1;
					SoundMoveCarsol.Play();					
				}
				
			}


			
		}
		//�I�����2�F�GAI�̋����̐ݒ�
		else if (mSelect_Step == 2){
			if (CKey::Once(VK_LEFT)){
				if (mLevel > 1){
					mLevel--;
				}
				else{
					mLevel = 3;
				}
				SoundMoveCarsol.Play();
			}
			if (CKey::Once(VK_RIGHT)){
				if (mLevel < 3){
					mLevel++;
				}
				else{
					mLevel = 1;
				}
				SoundMoveCarsol.Play();
			}
			//Esc�L�[���ABackSpace�L�[�ŁA�O�̑I����ʂɖ߂�
			if (CKey::Once(VK_BACK) || CKey::Once(VK_ESCAPE)){
				mSelect_Step--;
				SoundCancel.Play();
			}
		}
	}
	if (mCarsol_Pos == 0){
		mCource = ECOURCE1;
	}
	else if (mCarsol_Pos == 1){
		mCource = ECOURCE2;
	}
	else if (mCarsol_Pos == 2){
		mCource = ECOURCE5;
	}

	/*if (mCarsol_Pos == 0){
		mMode = EMODE_GRANDPRIX;
	}
	else if (mCarsol_Pos == 1){
		mMode = EMODE_TIMEATTACK;
	}*/

	Render();//�e�L�X�g���̕`��
}

void CSceneTitle::Render(){
	//2D�`��J�n
	Start2D(0, 800, 0, 600);	
	float c[] = { 1.0f, 1.0f, 1.0f, 1.0f };//{ R,G,B,�� }
	

	//������̕`��
	CText::DrawString("3D-RACE", 190, 430, 36, 36);
	CText::DrawString("Push Enter Key", 200, 177, 16, 16);
	if (mSelect_Step == 1){//�R�[�X�I���̃J�[�\��
		CText::DrawString("[", 66 + mCarsol_Pos * 250, 284, 20, 30, 2);
		CText::DrawString("]", 262 + mCarsol_Pos * 250, 284, 20, 30, 2);
	}
	if (mSelect_Step == 2 && mStart == false){
		if (mLevel == 1){//CPU�̋����I���̃J�[�\��
			CText::DrawString("[", 190, 102, 16, 24, 2);
			CText::DrawString("]", 278, 102, 16, 24, 2);
		}
		if (mLevel == 2){
			CText::DrawString("[", 338, 102, 16, 24, 2);
			CText::DrawString("]", 474, 102, 16, 24, 2);
		}
		if (mLevel == 3){
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
	//��������悪�I���������ɓ_�ł���

	//�R�[�X�I���n�̃e�L�X�g
	for (int i = 0; i <= 2; i++){
		if (mCarsol_Pos == i){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		//�I�����ɓ_�ł���
		if (mStart && mCarsol_Pos == i){
			if (mStartWaitTime > 20){
				c[0] = c[1] = c[2] = 1.0f;
			}
			else if (mStartWaitTime % 8 < 4){
				c[0] = c[1] = c[2] = 0.5f;
			}
		}
		glColor4fv(c);
		if (i == 0){
			CText::DrawString("COURCE-1", 80, 280, 12, 14);
		}
		else if (i == 1){
			CText::DrawString("COURCE-2", 330, 280, 12, 14);
		}
		else if (i == 2){
			CText::DrawString("COURCE-3", 580, 280, 12, 14);
		}

		/*if (i == 0){
			CText::DrawString("GRAND-PRIX MODE", 200, 310, 14, 17);
		}
		else if (i == 1){
			CText::DrawString("TIMEATTACK MODE", 200, 260, 14, 17);
		}*/
	}
	//CPU���x���̃e�L�X�g
	for (int i = 1; i <= 3; i++){		
		if (mLevel == i && mSelect_Step == 2){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		//�I�����ɓ_�ł���
		if (mStart && mLevel == i){
			if (mStartWaitTime > 20){
				c[0] = c[1] = c[2] = 1.0f;
			}
			else if (mStartWaitTime % 8 < 4){
				c[0] = c[1] = c[2] = 0.5f;
			}
		}
		glColor4fv(c);
		if (i == 1){
			CText::DrawString("EASY", 200, 100, 12, 12);
		}
		else if (i == 2){
			CText::DrawString("NORMAL", 347, 100, 12, 12);
		}
		else if (i == 3){
			CText::DrawString("HARD", 543, 100, 12, 12);
		}		
	}		
	//�S�Ă̒l��1.0f�ɖ߂��Ă���
	c[0] = c[1] = c[2] = c[3] = 1.0f;
	glColor4fv(c);

	char record[16];
	if (mCource == 0){
		sprintf(record, "EDIT");
		CText::DrawString(record, 20, 580, 10, 12);
	}
	else{
		for (int i = 1; i <= COURCE_TOTAL; i++){
			if (mCource == i){
				sprintf(record, "BEST:%02d:%02d:%02d", CSceneRace::mRecords[i] / 10000 % 100, CSceneRace::mRecords[i] / 100 % 100, CSceneRace::mRecords[i] % 100);
				CText::DrawString(record, 20, 580, 10, 12);
			}			
		}
	}	

	c[0] = c[1] = c[2] = 1.0f;
	glColor4fv(c);
	
	//�t�F�[�h�A�E�g�̕\��
	if (mStartWaitTime >= WAITTIME_STARTFADEOUT){
		c[0] = c[1] = c[2] = 0.0f;
		c[3] = 0.025f * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		glColor4fv(c);
		CRectangle::Render(400, 300, 400, 300);
	}
	//�S�Ă̒l��1.0f�ɖ߂��Ă���
	c[0] = c[1] = c[2] = c[3] = 1.0f;
	glColor4fv(c);
	
	//�V�[�����ڍs���钼�O�ŕ\��
	if (mStartWaitTime > WAITTIME_NOWLOADING){
		CText::DrawString("Please Wait...", 555, 14, 9, 11);
	}
	//2D�`��I��
	End2D();
}