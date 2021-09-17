#include "glew.h"
#include "CSceneRace.h"
#include "CSceneTitle.h"
#include "CCamera.h"
#include "CText.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CKey.h"
#include "CRenderTexture.h"
#include "CRectangle.h"

//�X�}�[�g�|�C���^�̐���
std::shared_ptr<CTexture> TextureExp(new CTexture());
std::shared_ptr<CTexture> TextureHit(new CTexture());
std::shared_ptr<CTexture> TextureBoost(new CTexture());

extern CSound BGM;
extern CSound JingleOpening;
extern CSound SoundCountDown;
extern CSound SoundStart;
extern CSound SoundGoal;
extern CSound SoundMoveCarsol;
extern CSound SoundDecide;
extern CSound SoundPauseOn;
extern CSound SoundPauseOff;


//������mBestTime�̒l�͊֌W�Ȃ�(mRecord_ �̒l�����邽��)
int CSceneRace::mBestTime = 0;
int CSceneRace::mRecord_A = 10000;
int CSceneRace::mRecord_B = 13000;
int CSceneRace::mRecord_C = 22000;
int CSceneRace::mRecord_D = 460000;
int CSceneRace::mRecord_E = 40000;

//�I�v�V������ʂ���ύX���ł���ϐ�
bool CSceneRace::isEnableShadow = true;//�e
bool CSceneRace::isEnableMiniMap = true;//�~�j�}�b�v
bool CSceneRace::isEnableBackMirror = true;//�o�b�N�~���[
bool CSceneRace::isEnableSpeedometer = false;//���x�v

//��ʃT�C�Y��800x600��z��
#define SCREENSIZE_X 800
#define SCREENSIZE_Y 600
//�`��G���A�̎w��(���[��X���W,���[��Y���W,��,����)
#define MINIMAP_AREA 590,10,200,150
#define BACKMIRROR_FRAME_AREA 286,491,229,154
#define BACKMIRROR_VIEW_AREA 288,493,225,150
#define BACKMIRROR_EXTRAFRAME_AREA 286,598,228,5

#define TEXWIDTH (800)
#define TEXHEIGHT (600)

#define OPENINGTIME 5*60

CRenderTexture mRenderTexture;

CSceneRace::~CSceneRace() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneRace::Init() {
	
	for (int i = 0; i < GROUND_AMOUNT; i++){
		mpGrounds[i] = NULL;
	}
	for (int i = 0; i < ENEMYS_AMOUNT + 1; i++){//+1�̓v���C���[��
		mCarShadow[i] = NULL;
	}

	//�����e�N�X�`���̓ǂݍ���
	TextureExp->Load("effect\\exp.tga");
	//�Փ˃e�N�X�`���̓ǂݍ���
	TextureHit->Load("effect\\Hit01_panop.tga");
	//�����e�N�X�`���̓ǂݍ���
	TextureBoost->Load("effect\\boost01.tga");

	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	CText::mFont.Load("font\\FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font\\font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	CText::mFont3.Load("font\\FontDIY.tga");
	CText::mFont3.SetRowCol(8, 176 / 11);
	
	/*�S�R�[�X���ʂ̃}�e���A��*/
	//�Ԃ̓ǂݍ���
	mRover.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\white.mtl");//�v���C���[
	mCarRed.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\red.mtl");//�ȉ��G�̎�
	mCarBlue.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\blue.mtl");
	mCarGreen.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\green.mtl");
	mCarYellow.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\yellow.mtl");
	mCarPink.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\pink.mtl");
	mCarCyan.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\cyan.mtl");
	mCarWhite.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\white.mtl");
	mCarBlack.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\black.mtl");
	mCarGray.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\gray.mtl");
	mCheckPoint.Load("material\\common_mat\\cube.obj", "material\\single_color\\invisible.mtl");//���Ԓn�_(�����A�|���S��1��)
	//���^�C���̓ǂݍ���
	mTileBlack.Load("material\\common_mat\\cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("material\\common_mat\\cube.obj", "material\\racing_mat\\tile_white.mtl");	
	//�n�ʂ̓ǂݍ���
	mPlane.Load("material\\common_mat\\plane.obj", "material\\common_mat\\plane.mtl");
	mSign_Left.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnLeft.mtl");//�W��:����
	mSign_Right.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnRight.mtl");//�W��:�E��
	//�~�j�}�b�v��ł̃v���C���[�E�G�̃J�[�\���A�S�[���n�_�̓ǂݍ���
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//�v���C���[
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//�G
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");//�S�[���A�C�R��
	mPole.Load("material\\common_mat\\cube.obj", "material\\common_mat\\soil.mtl");//�|�[��(�ؐ�)
	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");//������
	mJumper01.Load("material\\common_mat\\cube.obj", "material\\common_mat\\on.mtl");//�W�����v��
	/*�R�[�X�ʂɓǂݍ��ރ}�e���A��*/
	if (CSceneTitle::mCource_Number == 1){
		mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");//�H��
		mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");//�Ő�
		mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");//��(���)
		mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");//��(��)
		mCurb01.Load("material\\racing_mat\\Curb01.obj", "material\\racing_mat\\Curb01.mtl");//�g���^�C��
		mGoalTile01.Load("material\\racing_mat\\cource01_goaltile.obj", "material\\racing_mat\\cource01_goaltile.mtl");//�������^�C��
	}
	else if (CSceneTitle::mCource_Number == 2){
		mCource02Road.Load("material\\racing_mat\\cource2nd\\cource02road.obj", "material\\racing_mat\\cource2nd\\cource02road.mtl");
		mCource02Wall.Load("material\\racing_mat\\cource2nd\\cource02wall.obj", "material\\racing_mat\\cource2nd\\cource02wall.mtl");
		mCource02Jump.Load("material\\racing_mat\\cource2nd\\cource02jumper.obj", "material\\racing_mat\\cource2nd\\cource02jumper.mtl");
	}
	else if (CSceneTitle::mCource_Number == 3){
		mCource03Road.Load("material\\racing_mat\\stage3\\cource03road.obj", "material\\racing_mat\\stage3\\cource03road.mtl");
		mCource03Wall.Load("material\\racing_mat\\stage3\\cource03wall.obj", "material\\racing_mat\\stage3\\cource03wall.mtl");
		mCource03Fence.Load("material\\racing_mat\\stage3\\cource03fence.obj", "material\\racing_mat\\stage3\\cource03fence.mtl");
	}
	else if (CSceneTitle::mCource_Number == 4){
		//�R�[�X�G�f�B�^�[�̃^�C���̓ǂݍ���
		mTile_Curve01_Floor.Load("material\\racing_mat\\stage_edit\\Curve01_floor.obj", "material\\racing_mat\\stage_edit\\Curve01_floor.mtl");
		mTile_Curve01_Wall.Load("material\\racing_mat\\stage_edit\\Curve01_wall.obj", "material\\racing_mat\\stage_edit\\Curve01_wall.mtl");
		mTile_Curve02_Floor.Load("material\\racing_mat\\stage_edit\\Curve02_floor.obj", "material\\racing_mat\\stage_edit\\Curve02_floor.mtl");
		mTile_Curve02_Wall.Load("material\\racing_mat\\stage_edit\\Curve02_wall.obj", "material\\racing_mat\\stage_edit\\Curve02_wall.mtl");
		mTile_Curve03_Floor.Load("material\\racing_mat\\stage_edit\\Curve03_floor.obj", "material\\racing_mat\\stage_edit\\Curve03_floor.mtl");
		mTile_Curve03_Wall.Load("material\\racing_mat\\stage_edit\\Curve03_wall.obj", "material\\racing_mat\\stage_edit\\Curve03_wall.mtl");
		mTile_Curve04_Floor.Load("material\\racing_mat\\stage_edit\\Curve04_floor.obj", "material\\racing_mat\\stage_edit\\Curve04_floor.mtl");
		mTile_Curve04_Wall.Load("material\\racing_mat\\stage_edit\\Curve04_wall.obj", "material\\racing_mat\\stage_edit\\Curve04_wall.mtl");
		mTile_Straight01_Floor.Load("material\\racing_mat\\stage_edit\\Straight01_floor.obj", "material\\racing_mat\\stage_edit\\Straight01_floor.mtl");
		mTile_Straight01_Wall.Load("material\\racing_mat\\stage_edit\\Straight01_wall.obj", "material\\racing_mat\\stage_edit\\Straight01_wall.mtl");
		mTile_Straight02_Floor.Load("material\\racing_mat\\stage_edit\\Straight02_floor.obj", "material\\racing_mat\\stage_edit\\Straight02_floor.mtl");
		mTile_Straight02_Wall.Load("material\\racing_mat\\stage_edit\\Straight02_wall.obj", "material\\racing_mat\\stage_edit\\Straight02_wall.mtl");
		mTile_Slope01_Floor.Load("material\\racing_mat\\stage_edit\\Slope01_floor.obj", "material\\racing_mat\\stage_edit\\Slope01_floor.mtl");
		mTile_Slope01_Wall.Load("material\\racing_mat\\stage_edit\\Slope01_wall.obj", "material\\racing_mat\\stage_edit\\Slope01_wall.mtl");
		mTile_Slope02_Floor.Load("material\\racing_mat\\stage_edit\\Slope02_floor.obj", "material\\racing_mat\\stage_edit\\Slope02_floor.mtl");
		mTile_Slope02_Wall.Load("material\\racing_mat\\stage_edit\\Slope02_wall.obj", "material\\racing_mat\\stage_edit\\Slope02_wall.mtl");
		mTile_Slope03_Floor.Load("material\\racing_mat\\stage_edit\\Slope03_floor.obj", "material\\racing_mat\\stage_edit\\Slope03_floor.mtl");
		mTile_Slope03_Wall.Load("material\\racing_mat\\stage_edit\\Slope03_wall.obj", "material\\racing_mat\\stage_edit\\Slope03_wall.mtl");
		mTile_Slope04_Floor.Load("material\\racing_mat\\stage_edit\\Slope04_floor.obj", "material\\racing_mat\\stage_edit\\Slope04_floor.mtl");
		mTile_Slope04_Wall.Load("material\\racing_mat\\stage_edit\\Slope04_wall.obj", "material\\racing_mat\\stage_edit\\Slope04_wall.mtl");
		mTile_Wide_Floor.Load("material\\racing_mat\\stage_edit\\Tile_WideF.obj", "material\\racing_mat\\stage_edit\\Tile_WideF.mtl");
		mTile_Wide_Wall.Load("material\\racing_mat\\stage_edit\\Tile_WideW.obj", "material\\racing_mat\\stage_edit\\Tile_WideW.mtl");
		mBlock_Floor.Load("material\\racing_mat\\stage_edit\\BlockF.obj", "material\\racing_mat\\stage_edit\\BlockF.mtl");
		mBlock_Wall.Load("material\\racing_mat\\stage_edit\\BlockW.obj", "material\\racing_mat\\stage_edit\\BlockW.mtl");
	}
	else if (CSceneTitle::mCource_Number == 5){
		//�R�[�X05�̓ǂݍ���
		mCource05Wall.Load("material\\racing_mat\\stage5\\cource05wall.obj", "material\\racing_mat\\stage5\\cource05wall.mtl");
		mCource05Mountain.Load("material\\racing_mat\\stage5\\cource05mountain.obj", "material\\racing_mat\\stage5\\cource05mountain.mtl");//�S�Ă̎R����
		mCource05Road.Load("material\\racing_mat\\stage5\\cource05road2.obj", "material\\racing_mat\\stage5\\cource05road2.mtl");
		mCource05Lake.Load("material\\racing_mat\\stage5\\cource05_lake.obj", "material\\racing_mat\\stage5\\cource05_lake.mtl");
		mCource05Grass_Floor.Load("material\\racing_mat\\stage5\\cource05grassF03.obj", "material\\racing_mat\\stage5\\cource05grassF03.mtl");
		mCource05Grass_Wall.Load("material\\racing_mat\\stage5\\cource05grass_wall.obj", "material\\racing_mat\\stage5\\cource05grass_wall.mtl");
		mCource05GoalTile.Load("material\\racing_mat\\stage5\\Checker_Tile.obj", "material\\racing_mat\\stage5\\Checker_Tile.mtl");
	}

	//�X�e�[�W1BGM�̓ǂݍ���
	if (CSceneTitle::mCource_Number == 1){
		BGM.Load("BGM\\revolumed_PopsGuitar_No.01.wav");
		mBestTime = mRecord_A;
	}
	else if (CSceneTitle::mCource_Number == 2){
		BGM.Load("BGM\\revolumed_game_maoudamashii_1_battle34.wav");
		mBestTime = mRecord_B;
	}
	else if (CSceneTitle::mCource_Number == 3){
		BGM.Load("BGM\\revolumed_bgm_maoudamashii_neorock33.wav");
		mBestTime = mRecord_C;
	}
	else if (CSceneTitle::mCource_Number == 4){
		BGM.Load("BGM\\revolumed_Spring_Breeze.wav");
		mBestTime = mRecord_D;
	}
	else if (CSceneTitle::mCource_Number == 5){
		BGM.Load("BGM\\Go_on_the_mountain_road.wav");
		mBestTime = mRecord_E;
	}
	JingleOpening.Load("SE\\jingle19.wav");
	//���ʉ��̓ǂݍ���//��Ƀ��[�X��
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");
	//��Ƀ|�[�Y��
	SoundMoveCarsol.Load("SE\\Carsol2.wav");
	SoundDecide.Load("SE\\Decision_Small(SF).wav");
	SoundPauseOn.Load("SE\\button79.wav");
	SoundPauseOff.Load("SE\\button80.wav");
	

	//�J�������_��Y���W
	mCamY = 0.0f;
	//�Փ˔���̕`��(�f�o�b�O�p)
	mPutCol = false;

	//�ꕔ�e�L�X�g���_�ł��鎞��
	mTextBlinkTime = 0;

	isOpening = true;
	mTime_Opening = 0;
	//���[�X�̃J�E���g�_�E���֘A
	mFrame = 0;
	mCountDown = 4;//countdown��3�b����n�܂�
	//�J�E���g�_�E����Ƀ��[�X�J�n
	isStartRace = isGoal = false;
	//���Ԃ̃��Z�b�g
	mTime = 0;
	mTime_Output = 0;
		
	//���b�v���̏�����
	mLap = 1;
	//n���ŃS�[��(��{��3��)
	mMaxLap = 3;
	if (CSceneTitle::mCource_Number == 5){
		mMaxLap = 2;//�R�[�X5��2��(1����������)
	}
	//�L�^�X�V���ĂȂ����
	isNewRecord = false;
	
	//���ŏI���ʂ̏�����
	mRanking = 1;
	//�v���C���[���S�[����̌o�ߎ���
	mAfterGoalTime = 0;
	
	//������Ԃł̓|�[�Y��Ԗ���
	isPause = false;
	mPause_SelectCarsol = 1;
	mPause_OptionCarsol = 1;
	mPauseScreen = EPAUSE;

	isEnableShadow_Cource = true;//�Q�[�����ł̐؂�ւ��s��
	isEnableShadow_Car = false;//�����_�ł͔�\������

	isFadeIn = true;
	isFadeOut = false;
	isBlackOutTime = 0;
	
	mRenderTexture.Init();
	

	/* �e�N�X�`�����j�b�g�P��Depth�e�N�X�`���Ŏg�p */
	glActiveTexture(GL_TEXTURE1);
	//�e�N�X�`���̐���
	glGenTextures(1, &mDepthTextureID);
	//�g�p����e�N�X�`���̃o�C���h
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);

	/* Depth�e�N�X�`���̊��蓖�� */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXWIDTH, TEXHEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	/* �e�N�X�`�����g��E�k��������@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* �e�N�X�`���̌J��Ԃ����@�̎w�� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* �������ރ|���S���̃e�N�X�`�����W�l�̂q�ƃe�N�X�`���Ƃ̔�r���s���悤�ɂ��� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	/* �����q�̒l���e�N�X�`���̒l�ȉ��Ȃ�^�i�܂�����j */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	/* ��r�̌��ʂ��P�x�l�Ƃ��ē��� */
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	/* �e�N�X�`�����W�Ɏ��_���W�n�ɂ����镨�̂̍��W�l��p���� */
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	/* ���������e�N�X�`�����W�����̂܂� (S, T, R, Q) �Ɏg�� */
	static const GLdouble genfunc[][4] = {
		{ 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 0.0, 1.0 },
	};

	glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
	glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
	glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
	glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

	//�e�N�X�`���̉���
	glBindTexture(GL_TEXTURE_2D, 0);
	//�e�N�X�`�����j�b�g��0�ɖ߂�
	glActiveTexture(GL_TEXTURE0);	
}


void CSceneRace::Update() {
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����

	e = CCameraPos::mpCamera->mPosition;
	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 35.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	u = CVector(0.0f, 1.0f, 0.0f);//*mPlayer->mMatrixRotate;
	//�J�����̐ݒ�
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	//�^�X�N�}�l�[�W���̍X�V�E�`��
	if (isPause == false){
		CTaskManager::Get()->Update();
	}

	RenderShadow();

	CTaskManager::Get()->Render();	
	//�Փˏ���
	CTaskManager::Get()->TaskCollision();
	//�폜����
	CTaskManager::Get()->Delete();

	//�f�o�b�O�p
#ifdef _DEBUG
	if (CKey::Once('2')){//Player�̍��W���o��
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Push('5')){//Player�̉�]�l���o��
		printf("%f:%f:%f\n", CPlayer::mpPlayer->mRotation.mX, CPlayer::mpPlayer->mRotation.mY, CPlayer::mpPlayer->mRotation.mZ);
	}
	if (CKey::Once('6')){//Player�̃`�F�b�N�|�C���g�̏�Ԃ��o��
		printf("%d\n", CPlayer::mpPlayer->mChecks);
	}
	if (CKey::Once('7')){//Player��s���[�h�؂�ւ�
		if (CPlayer::mpPlayer->mFlyingMode){
			CPlayer::mpPlayer->mFlyingMode = false;
		}
		else{
			CPlayer::mpPlayer->mFlyingMode = true;
		}
	}//�����蔻��`��̐؂�ւ�
	if (CKey::Once('9')){
		if (mPutCol){
			mPutCol = false;
		}
		else{
			mPutCol = true;
		}
	}
	if (mPutCol){
		//�Փ˔���̕`��
		CollisionManager.Render();
	}
	//Player�𑦎��g�b�v�X�s�[�h��
	if (CKey::Once('G')){
		mPlayer->mCarSpeed = 20.0f;
	}
	//�Ō�̃`�F�b�N�|�C���g��ʉ߂��������ɂ���
	if (CKey::Once('V')){
		mPlayer->mChecks = 3;
	}
#endif	

	//�|�[�Y��ʂɓ����Ă��Ȃ���
	if (isPause == false){
		if (isStartRace || isGoal){
			//mTime:�S�[����������I�Ƀ^�C�}�[�͍쓮��������(�Q�[����ʂɂ͕\������Ȃ�)
			//59:59:59���ő厞��
			if (mTime < 595959){
				if (mTime % 10000 == 5959){
					mTime += 1 + 40 + 4000;
				}
				else if (mTime % 100 == 59){
					mTime += 1 + 40;
				}
				else{
					mTime += 1;
				}
			}
			for (int i = 0; i < ENEMYS_AMOUNT; i++){
				if (mEnemys[i]->isEnemyGoaled == false){
					mEnemys[i]->mPointTime++;
				}
			}
		}
		if (isStartRace){
			//mTime_Output:�Q�[����ʂɕ\�������^�C��(�S�[����Ƀ^�C�}�[��~)
			if (mTime_Output < 595959){
				if (mTime_Output % 10000 == 5959){
					mTime_Output += 1 + 40 + 4000;
				}
				else if (mTime_Output % 100 == 59){
					mTime_Output += 1 + 40;
				}
				else{
					mTime_Output += 1;
				}
			}
		}
	}

	//���[�X�O�̃I�[�v�j���O
	if (isOpening){
		//OP�W���O���Đ�
		if (mTime_Opening == 0)
			JingleOpening.Play();
		mTime_Opening++;
		if (mTime_Opening > OPENINGTIME)
			isOpening = false;
	}
	//�J�E���g�_�E���J�n�A�uGO!�v�ő���̎�t�J�n
	else if (mFrame < 60){
		mFrame++;
	}
	else{
		if (mCountDown >= 0){
			mCountDown--;
			if (mCountDown == 0){
				isStartRace = true;
				SoundStart.Play();
			}
			else if (mCountDown > 0){
				SoundCountDown.Play();
			}
			else if (mCountDown < 0){
				BGM.Repeat();
			}
		}
		mFrame = 0;
	}
	//�J�E���g0�őS�������s�\��
	if (mCountDown == 0){
		CPlayer::mpPlayer->CanMove = true;
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			mEnemys[i]->CanMove = true;
		}
	}

	//�o�b�N�~���[�̕`��
	if (isEnableBackMirror){
		RenderBackMirror();		
	}
	//�~�j�}�b�v�̕`��
	if (isEnableMiniMap){
		RenderMiniMap();
	}
	//�e�L�X�g�A�t�F�[�h�C�����̕`��
	Render();	
	
	//�v���C���[���S�[���n�_��ʉ߂������̏���
	if ((CPlayer::mpPlayer->isTouchGoal)
		&& (CPlayer::mpPlayer->mChecks == 3)
		&& (isStartRace)){
		if (mLap == mMaxLap){
			//�x�X�g�^�C���X�V��
			if (mTime < mBestTime){
				mBestTime = mTime;
				isNewRecord = true;
				//�R�[�X�ɂ���ĐV�����L�^����
				if (CSceneTitle::mCource_Number == 1){
					mRecord_A = mBestTime;
				}
				else if (CSceneTitle::mCource_Number == 2){
					mRecord_B = mBestTime;
				}
				else if (CSceneTitle::mCource_Number == 3){
					mRecord_C = mBestTime;
				}
				else if (CSceneTitle::mCource_Number == 4){
					mRecord_D = mBestTime;
				}
				else if (CSceneTitle::mCource_Number == 5){
					mRecord_E = mBestTime;
				}
			}
			mPlayer->mRank = mRanking;
			mRanking++;
			isStartRace = false;
			isGoal = true;
			BGM.Stop();
			SoundGoal.Play();
			//CPlayer::mpPlayer->CanMove = false;//�����X�g�b�v
			CPlayer::mpPlayer->mChecks = 0;
			CPlayer::mpPlayer->mGoalTime = mTime;
			CPlayer::mpPlayer->isTouchGoal = false;
		}
		else{
			mLap++;
			CPlayer::mpPlayer->mChecks = 0;
			CPlayer::mpPlayer->isTouchGoal = false;
		}
	}
	//CPU�̎Ԃ��S�[���n�_��ʉ߂������̏���
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		if (mEnemys[i]->isTouchGoal
			&& (mEnemys[i]->mChecks == 3)
			&& (mEnemys[i]->isEnemyGoaled == false)){
			//���̓G���ŏI���b�v�������ꍇ
			if (mEnemys[i]->mEnemyLap == mMaxLap){
				mEnemys[i]->mRank = mRanking;
				mRanking++;
				mEnemys[i]->isTouchGoal = false;
				mEnemys[i]->isEnemyGoaled = true;
				mEnemys[i]->mGoalTime = mTime;
			}
			//�܂��ŏI���b�v�łȂ��ꍇ
			else{
				//�`�F�b�N�|�C���g�����Z�b�g���A���̎��X�^�[�g
				mEnemys[i]->mChecks = 0;
				mEnemys[i]->isTouchGoal = false;
				mEnemys[i]->mEnemyLap++;
			}
		}
	}

	if (CKey::Once('P')){
		//�J�E���g�_�E���I����A�|�[�Y�̐؂�ւ����\�ɂȂ�B
		if (mCountDown < 0){
			//�S�[����͐؂�ւ��s��
			if (isGoal)return;

			if (isPause&&mPauseScreen == EOPTION)return;

			//�u�^�C�g���ɖ߂�v�I����̓J�[�\���𓮂����Ȃ�
			if (isFadeOut)return;

			//�|�[�Y��ON�EOFF�؂�ւ�
			isPause = !isPause;
			mPause_SelectCarsol = 1;
			if (isPause){
				SoundPauseOn.Play();
			}
			else{
				SoundPauseOff.Play();
			}
		}		
	}

	/*�V�[���؂�ւ��n�̏���*/
	//�S�[����Enter�L�[�������^�C�g����ʈڍs
	if (isGoal){
		if (CKey::Once(VK_RETURN)){
			//���̃V�[���̓Q�[��
			mScene = ETITLE;
		}
	}
	//�|�[�Y��Esc�L�[�������^�C�g����ʈڍs
	if (isPause){
		//�u�^�C�g���ɖ߂�v�I�����ĈÓ]���n�܂�ƃJ�[�\���𓮂����Ȃ�
		if (isFadeOut)return;
			
		//�|�[�Y�̒��ŁA�ݒ��ʂ��J���Ă��鎞
		if (mPauseScreen == EOPTION){
			//���L�[
			if (CKey::Once(VK_UP)){
				if (mPause_OptionCarsol > 1){
					mPause_OptionCarsol--;
					SoundMoveCarsol.Play();
				}
			}
			//���L�[
			if (CKey::Once(VK_DOWN)){
				if (mPause_OptionCarsol < 5){
					mPause_OptionCarsol++;
					SoundMoveCarsol.Play();
				}
			}
			//Enter�L�[
			if (CKey::Once(VK_RETURN)){
				//�e�̕`���ON�EOFF
				if (mPause_OptionCarsol == 1){
					isEnableShadow = !isEnableShadow;
				}
				//�o�b�N�~���[�\����ON�EOFF
				if (mPause_OptionCarsol == 2){
					isEnableBackMirror = !isEnableBackMirror;
				}
				//�~�j�}�b�v�\����ON�EOFF
				if (mPause_OptionCarsol == 3){
					isEnableMiniMap = !isEnableMiniMap;
				}
				////�X�s�[�h���[�^�[�\����ON�EOFF
				if (mPause_OptionCarsol == 4){
					isEnableSpeedometer = !isEnableSpeedometer;
				}
				//�ݒ��ʂ����
				if (mPause_OptionCarsol == 5){
					mPauseScreen = EPAUSE;
				}
				SoundDecide.Play();
			}
		}
		else{
			//���L�[
			if (CKey::Once(VK_UP)){
				if (mPause_SelectCarsol > 1){
					mPause_SelectCarsol--;
					SoundMoveCarsol.Play();
				}
			}
			//���L�[
			if (CKey::Once(VK_DOWN)){
				if (mPause_SelectCarsol < 3){
					mPause_SelectCarsol++;
					SoundMoveCarsol.Play();
				}
			}
			//Enter�L�[
			if (CKey::Once(VK_RETURN)){
				if (mPause_SelectCarsol == 1){
					//�|�[�Y����
					isPause = !isPause;
					mPause_SelectCarsol = 1;
					SoundPauseOff.Play();
				}
				else if (mPause_SelectCarsol == 2){
					//�I�v�V������ʕ\��
					mPauseScreen = EOPTION;
					SoundDecide.Play();
				}
				else if (mPause_SelectCarsol == 3){
					SoundDecide.Play();
					isFadeOut = true;
					isBlackOutTime = 0;
				}
			}
		}
	}

	return;
}

void CSceneRace::Render(){
	//2D�`��J�n
	Start2D(0, 800, 0, 600);

	//���ʂ̕`��
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//���[�X�J�n�O��OP���ɃR�[�X���������ɕ\��
	if (isOpening){
		if (CSceneTitle::mCource_Number == 1){
			CText::DrawString("COURCE 1", 20, 20, 10, 12);
		}
		else if (CSceneTitle::mCource_Number == 2){
			CText::DrawString("COURCE 2", 20, 20, 10, 12);
		}
		else if (CSceneTitle::mCource_Number == 3){
			CText::DrawString("COURCE 3", 20, 20, 10, 12);
		}
		else if (CSceneTitle::mCource_Number == 4){
			CText::DrawString("COURCE 4", 20, 20, 10, 12);
		}
		else if (CSceneTitle::mCource_Number == 5){
			CText::DrawString("COURCE 5", 20, 20, 10, 12);
		}
	}

	//���Ԃ̕\��
	char time[20];// :���܂߂��ő啶�����̐ݒ�
	sprintf(time, "TIME:%02d:%02d:%02d", mTime_Output / 10000 % 100, mTime_Output / 100 % 100, mTime_Output % 100);
	CText::DrawString(time, 20, 530, 10, 12);
	//�x�X�g�^�C���̕\��
	char besttime[20];// :���܂߂��ő啶�����̐ݒ�
	sprintf(besttime, "BEST:%02d:%02d:%02d", mBestTime / 10000 % 100, mBestTime / 100 % 100, mBestTime % 100);
	CText::DrawString(besttime, 20, 580, 10, 12);

	//�J�E���g�_�E���̕\��
	char mcountd[7];
	//�c��3�b�܂ł̊ԁA�\��
	sprintf(mcountd, "%d", mCountDown);
	if (mCountDown > 0 && mCountDown <= 3){
		CText::DrawString(mcountd, 400, 300, 25, 30);
	}
	else if (mCountDown == 0){
		CText::DrawString("GO!", 400 - 40, 300, 25, 30);
	}
	char lap[19];
	sprintf(lap, "LAP%d/%d", mLap, mMaxLap);
	CText::DrawString(lap, 20, 500, 10, 12, 2);
	//�v���C���[�̎Ԃ̑��x�\��
	char carspeed[33];
	sprintf(carspeed, "SPEED:%4.1f", CPlayer::mpPlayer->mCarSpeed);
	if (isEnableSpeedometer){
		CText::DrawString(carspeed, 20 + 580, 20, 10, 12);
	}
	
	//�S�[����A�p�����Ď��s���鏈��
	if (mLap == mMaxLap && isStartRace == false && isGoal){
		//60f�ňꏄ
		if (mTextBlinkTime < 60){
			mTextBlinkTime++;
		}
		else{
			mTextBlinkTime = 0;
		}
		//Enter�L�[���͂Ń^�C�g����ʂɖ߂�邱�Ƃ�`����e�L�X�g
		if (mTextBlinkTime < 30){
			CText::DrawString("Press Enter to Title", 222, 77, 10, 12, 2);
		}
		//�V�L�^���������o������
		if (isNewRecord){
			//CText::DrawString("FINISH!", 400 - 20 * 6, 300, 20, 24);
			if (mTextBlinkTime < 15 || mTextBlinkTime >= 30 && mTextBlinkTime < 45){
				CText::DrawString("NEW RECORD!", 55, 551, 8, 9, 2);
			}
		}
	}

	//�S�[����ɕ\������镶��
	if (isGoal){
		if (mAfterGoalTime < 999){
			mAfterGoalTime++;
		}
		if (mAfterGoalTime > 85){
			color[3] = 1.0f - 0.1f * (mAfterGoalTime - 85);
			glColor4fv(color);
		}
		CText::DrawString("FINISH!", 400 - 25 * 6, 300, 25, 30);

		color[3] = 0.05f * (mAfterGoalTime - 100);
		glColor4fv(color);
		char rank[8];
		if (mPlayer->mRank == 1){      //1st
			sprintf(rank, "%dst", mPlayer->mRank);
		}
		else if (mPlayer->mRank == 2){ //2nd
			sprintf(rank, "%dnd", mPlayer->mRank);
		}
		else if (mPlayer->mRank == 3){ //3rd
			sprintf(rank, "%drd", mPlayer->mRank);
		}
		else{ //4th,5th,...
			sprintf(rank, "%dth", mPlayer->mRank);
		}
		CText::DrawString(rank, 357, 310, 10 * 2, 12 * 2, 2);

		//�S�[�������Ԃ̋L�^��\��
		char goaltime[30];
		char name[8];
		sprintf(goaltime, "%d YOU  %02d:%02d:%02d", mPlayer->mRank, mPlayer->mGoalTime / 10000 % 100, mPlayer->mGoalTime / 100 % 100, mPlayer->mGoalTime % 100);
		if (isGoal){
			CText::DrawString(goaltime, 270, 285 - mPlayer->mRank * 17, 10, 12, 2);
		}
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			sprintf(goaltime, "%d      %02d:%02d:%02d", mEnemys[i]->mRank, mEnemys[i]->mGoalTime / 10000 % 100, mEnemys[i]->mGoalTime / 100 % 100, mEnemys[i]->mGoalTime % 100);
			sprintf(name, "CPU%d", i + 1);
			if (mEnemys[i]->isEnemyGoaled){
				if (mEnemys[i]->mpModel == &mCarRed){
					color[0] = 1.0f, color[1] = 0.0f, color[2] = 0.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarGreen){
					color[0] = 0.0f, color[1] = 1.0f, color[2] = 0.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarBlue){
					color[0] = 0.0f, color[1] = 0.0f, color[2] = 1.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarYellow){
					color[0] = 1.0f, color[1] = 1.0f, color[2] = 0.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarCyan){
					color[0] = 0.0f, color[1] = 1.0f, color[2] = 1.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarPink){
					color[0] = 1.0f, color[1] = 0.0f, color[2] = 1.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarWhite){
					color[0] = color[1] = color[2] = 1.0f;
				}
				else if (mEnemys[i]->mpModel == &mCarGray){
					color[0] = color[1] = color[2] = 0.5f;
				}
				else if (mEnemys[i]->mpModel == &mCarBlack){
					color[0] = color[1] = color[2] = 0.0f;
				}
				else {
					color[0] = color[1] = color[2] = 0.75f;
				}
				glColor4fv(color);
				CText::DrawString(name, 270 + 40, 285 - mEnemys[i]->mRank * 17, 10, 12, 2);
				//�F����U�A���ɖ߂�
				color[0] = color[1] = color[2] = 1.0f;
				glColor4fv(color);
				CText::DrawString(goaltime, 270, 285 - mEnemys[i]->mRank * 17, 10, 12, 2);
			}
		}
	}
	//�F���������ɖ߂�
	color[3] = 1.0f;
	glColor4fv(color);

	//�|�[�Y���ɕ\������镶��
	if (isPause){

		if (mPauseScreen == EOPTION){
			//�\���ꏊ���ω����Ȃ��e�L�X�g
			CText::DrawString("OPTION", 250, 300, 10 * 3, 12 * 3, 3);
			CText::DrawString("Shadow", 260, 200, 10, 12, 2);
			CText::DrawString("BackMirror", 260, 170, 10, 12, 2);
			CText::DrawString("MiniMap", 260, 140, 10, 12, 2);
			CText::DrawString("Speedometer", 260, 110, 10, 12, 2);

			//�\���ꏊ���ω�����e�L�X�g
			if (mPause_OptionCarsol == 5){
				CText::DrawString("[", 373, 230 - 30 * mPause_OptionCarsol - 10, 14, 21, 1);
				CText::DrawString("]", 432, 230 - 30 * mPause_OptionCarsol - 10, 14, 21, 1);
			}
			else{
				CText::DrawString("[", 493, 230 - 30 * mPause_OptionCarsol, 14, 21, 1);
				CText::DrawString("]", 547, 230 - 30 * mPause_OptionCarsol, 14, 21, 1);
			}

			for (int i = 1; i <= 5; i++){
				if (mPause_OptionCarsol == i){
					color[0] = color[1] = color[2] = 1.0f;
				}
				else{
					color[0] = color[1] = color[2] = 0.5f;
				}
				glColor4fv(color);

				if (i == 1){
					if (isEnableShadow){
						CText::DrawString("ON", 510, 200, 10, 12, 2);
					}
					else{
						CText::DrawString("OFF", 502, 200, 10, 12, 2);
					}
				}
				else if (i == 2){
					if (isEnableBackMirror){
						CText::DrawString("ON", 510, 170, 10, 12, 2);
					}
					else{
						CText::DrawString("OFF", 502, 170, 10, 12, 2);
					}
				}
				else if (i == 3){
					if (isEnableMiniMap){
						CText::DrawString("ON", 510, 140, 10, 12, 2);
					}
					else{
						CText::DrawString("OFF", 502, 140, 10, 12, 2);
					}
				}
				else if (i == 4){
					if (isEnableSpeedometer){
						CText::DrawString("ON", 510, 110, 10, 12, 2);
					}
					else{
						CText::DrawString("OFF", 502, 110, 10, 12, 2);
					}
				}
				if (i == 5){
					CText::DrawString("OK", 390, 70, 13, 15, 2);
				}
			}
			color[0] = color[1] = color[2] = 1.0f;
			glColor4fv(color);
		}
		else if (mPauseScreen == EPAUSE){
			CText::DrawString("[", 336, 230 - 30 * mPause_SelectCarsol, 14, 21, 1);
			CText::DrawString("]", 462, 230 - 30 * mPause_SelectCarsol, 14, 21, 1);
			CText::DrawString("PAUSE", 280, 300, 10 * 3, 12 * 3, 3);
			//�J�[�\���̈ʒu
			for (int i = 1; i <= 3; i++){
				if (mPause_SelectCarsol == i){
					color[0] = color[1] = color[2] = 1.0f;
				}
				else{
					color[0] = color[1] = color[2] = 0.5f;
				}
				glColor4fv(color);
				if (i == 1){
					CText::DrawString("Resume", 350, 200, 10, 12, 2);
				}
				else if (i == 2){
					CText::DrawString("Option", 350, 170, 10, 12, 2);
				}
				else if (i == 3){
					CText::DrawString("Quit", 370, 140, 10, 12, 2);
				}
			}
			color[0] = color[1] = color[2] = 1.0f;
			glColor4fv(color);
		}
	}
	//�t�F�[�h�C��
	if (isFadeIn){
		FadeIn();
	}
	//�t�F�[�h�C��
	if (isFadeOut){
		SceneChange();
	}
	//2D�`��I��
	End2D();
}

/* �}�b�v�ォ��̎��_ */
void CSceneRace::RenderMiniMap() {	
	glPushMatrix();
	glLoadIdentity();
	//�ꎞ�I��2D���_�ɕύX����
	glViewport(MINIMAP_AREA);//��ʂ̕`��G���A�̎w��
	glMatrixMode(GL_PROJECTION);//�s����v���W�F�N�V�������[�h�֕ύX
	//�s��ޔ�
	glPushMatrix();
	glLoadIdentity();			//�s���������
	//2D��ʂ̐ݒ�ύX
	float size = 20000.0f;//�R�[�X�̏k�ڐݒ�
	bool canscrollmap = false;//�v���C���[�ɍ��킹���}�b�v�ړ��̗L��
	if (CSceneTitle::mCource_Number == 1){
		size = 3600.0f;
	}
	else if (CSceneTitle::mCource_Number == 2){
		size = 5600.0f;
	}
	else if (CSceneTitle::mCource_Number == 3){
		size = 7600.0f;
	}
	else if (CSceneTitle::mCource_Number == 4){
		size = 3600.0f;
	}
	else if (CSceneTitle::mCource_Number == 5){
		size = 5500.0f;
		canscrollmap = true;
	}
	//��ʔ䗦��800x600��4:3��1.33:1
	float ratio = 1.33f;
	if (canscrollmap){
		glOrtho(-size*ratio + mPlayer->mPosition.mX, size*ratio + mPlayer->mPosition.mX, -size - mPlayer->mPosition.mZ, size - mPlayer->mPosition.mZ, -size, size);//glOrtho(���A�E�A���A��A��O�A��)
	}
	else{
		glOrtho(-size*ratio, size*ratio, -size, size, -size, size);//glOrtho(���A�E�A���A��A��O�A��)
	}	
	glMatrixMode(GL_MODELVIEW);		//�s������f���r���[���[�h�֕ύX
	glLoadIdentity();				//�s���������
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	//X-Z���ʂ�X-Y���ʂ�
		
	glDisable(GL_DEPTH_TEST);
	CTaskManager::Get()->Render();
		
	if (CSceneTitle::mCource_Number == 3){
		//�~�j�}�b�v�ɃS�[���A�C�R����`��
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 1.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(90)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(140.0f, 0.0f, 60.0f);
		mMiniGoal.Render(matminig);
		CMatrix matenemys[ENEMYS_AMOUNT];
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			matenemys[i] = CMatrix().Scale(70.0f, 1.0f, 70.0f) //* mPlayer->mMatrixScale
				* CMatrix().RotateX(0)
				* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
				* CMatrix().RotateZ(0)
				* mEnemys[i]->mMatrixTranslate;
			mCarsol_Enemy.Render(matenemys[i]);
		}
		//�~�j�}�b�v��Ƀv���C���[�������J�[�\����`��
		CMatrix matplayer;
		matplayer = CMatrix().Scale(70.0f, 1.0f, 70.0f) //* mPlayer->mMatrixScale
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mPlayer->mMatrixTranslate;
		mCarsol.Render(matplayer);
	}
	else if (CSceneTitle::mCource_Number == 2){
		//�~�j�}�b�v�ɃS�[���A�C�R����`��
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 1.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(0)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(2211.0f, 0.0f, -2300.0f);
			//* CMatrix().Translate(-3200.0f, 0.0f, 341.7f);
		mMiniGoal.Render(matminig);
		CMatrix matenemys[ENEMYS_AMOUNT];
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
				* CMatrix().RotateX(0)
				* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
				* CMatrix().RotateZ(0)
				* mEnemys[i]->mMatrixTranslate;
			mCarsol_Enemy.Render(matenemys[i]);
		}
		//�~�j�}�b�v��Ƀv���C���[�������J�[�\����`��
		CMatrix matplayer;
		matplayer = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mPlayer->mMatrixTranslate;
		mCarsol.Render(matplayer);
	}
	else if (CSceneTitle::mCource_Number == 5){
		//�~�j�}�b�v�ɃS�[���A�C�R����`��
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 100.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(-145.3)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(-3862.5f, 30.0f, 15925.0f);
		mMiniGoal.Render(matminig);

		CMatrix matenemys[ENEMYS_AMOUNT];
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			matenemys[i] = CMatrix().Scale(45.0f, 1.0f, 45.0f) //* mPlayer->mMatrixScale
				* CMatrix().RotateX(0)
				* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
				* CMatrix().RotateZ(0)
				* mEnemys[i]->mMatrixTranslate;
			mCarsol_Enemy.Render(matenemys[i]);
		}
		//�~�j�}�b�v��Ƀv���C���[�������J�[�\����`��
		CMatrix matplayer;
		matplayer = CMatrix().Scale(45.0f, 1.0f, 45.0f) //* mPlayer->mMatrixScale
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mPlayer->mMatrixTranslate;
		mCarsol.Render(matplayer);
	}
	else{
		//�~�j�}�b�v�ɃS�[���A�C�R����`��
		CMatrix matminig;
		matminig = CMatrix().Scale(20.0f, 1.0f, 20.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(0)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(550.0f, 0.0f, -10.0f);
		mMiniGoal.Render(matminig);

		CMatrix matenemys[ENEMYS_AMOUNT];
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
				* CMatrix().RotateX(0)
				* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
				* CMatrix().RotateZ(0)
				* mEnemys[i]->mMatrixTranslate;
			mCarsol_Enemy.Render(matenemys[i]);
		}

		//�~�j�}�b�v��Ƀv���C���[�������J�[�\����`��
		CMatrix matplayer;
		matplayer = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mPlayer->mMatrixTranslate;
		mCarsol.Render(matplayer);
	}
	
	glPopMatrix();
	glViewport(0, 0, 800, 600); //��ʂ̕`��G���A�̎w��
	glEnable(GL_DEPTH_TEST);

	//3D���_�ɖ߂�
	glViewport(0, 0, 800, 600);	//��ʂ̕`��G���A�̎w��
	glMatrixMode(GL_PROJECTION);	//�s����v���W�F�N�V�������[�h�֕ύX
	glLoadIdentity();				//�s���������
	//gluOrtho2D(-200 / 2, 200 / 2, -150 / 2, 150 / 2);	//2D�̉�ʂ�ݒ�
	gluPerspective(75.0, 800.0 / 600.0, 1.0, 10000.0);	//3D�̉�ʂ�ݒ�
	glMatrixMode(GL_MODELVIEW);		//�s������f���r���[���[�h�֕ύX
	glLoadIdentity();
}
//�o�b�N�~���[��\������
void CSceneRace::RenderBackMirror()
{
	//�s��̑ޔ�
	glPushMatrix();
	glLoadIdentity();
	//�J�����̐ݒ�
	CVector e, c, u;//���_�A�����_�A�����
	e = CVector(0.0f, 17.0f + 13.0f, 35.0f - 36.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		//* mPlayer->mMatrixRotate
		* mPlayer->mMatrixTranslate;
	c = mPlayer->mPosition + CVector(0.0f, 17.0f + 13.0f, 40.0f - 42.0f)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//* mPlayer->mMatrixRotate;
	u = CVector(0.0f, 1.0f, 0.0f);
	//�o�b�N�~���[�̃J�����̐ݒ�
	gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	
	

	//�����_�[�e�N�X�`���J�n
	mRenderTexture.Start();

	//�o�b�N�~���[�̕`��
	if (isEnableShadow){
		RenderShadow();
	}

	//�I�u�W�F�N�g�̕`��
	CTaskManager::Get()->Render();
	//�����_�[�e�N�X�`���I��
	mRenderTexture.End();

	//�o�b�N�~���[�̃t���[�������͈̔�	
	glViewport(BACKMIRROR_FRAME_AREA);
	//2D�`��J�n
	Start2D(-1, 1, -1, 1);
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glColor4fv(color);
	//��L��2D�`��͈͂̎w��l���傫�߂ɔ��w�i��`�悷��
	int expand = 100; color[3] = 0.0f;
	//���w�i�̂�����ɍ��g�ƂȂ���̂�`�悷��
	glBegin(GL_TRIANGLES);
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(800 + expand, 600 + expand);
	glVertex2d(0 - expand, 600 + expand);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(800 + expand, 0 - expand);
	glVertex2d(800 + expand, 600 + expand);
	glEnd();
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	//2D�`��I��
	End2D();

	//�o�b�N�~���[�ɉf��i�F��`��G���A�̎w��
	glViewport(BACKMIRROR_VIEW_AREA);
	//2D�`��J�n
	Start2D(-1, 1, -1, 1);	
	// �e�N�X�`���}�b�s���O��L���ɂ���
	glEnable(GL_TEXTURE_2D);
	//�����_�[�e�N�X�`���̃e�N�X�`�����o�C���h����
	glBindTexture(GL_TEXTURE_2D, mRenderTexture.GetTexture());	
	// �����`��`��
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(-1.0, -1.0);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(1.0, -1.0);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(1.0, 1.0);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(-1.0, 1.0);
	glEnd();
	//2D�`��I��
	End2D();

	//���؂ꂽ���g�̕�����⊮����
	glViewport(BACKMIRROR_EXTRAFRAME_AREA);
	//2D�`��J�n
	Start2D(0, SCREENSIZE_X, 0, SCREENSIZE_Y);
	color[0] = color[1] = color[2] = 0.0f;
	glColor4fv(color);
	//��L��2D�`��͈͂̎w��l���傫�߂ɔ��w�i��`�悷��
	expand = 100;
	//���؂ꂽ���g�̕�����`�悷��
	glBegin(GL_TRIANGLES);
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
	glVertex2d(0 - expand, SCREENSIZE_Y + expand);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(SCREENSIZE_X + expand, 0 - expand);
	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
	glEnd();
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	//2D�`��I��
	End2D();

	// �e�N�X�`���}�b�s���O�𖳌��ɂ���
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//��ʂ̕`��G���A�����C���̉�ʂɖ߂�	glViewport
	glViewport(0, 0, SCREENSIZE_X, SCREENSIZE_Y); //��ʂ̕`��G���A�����C���̉�ʂɖ߂�

	//�s���߂�
	glPopMatrix();
}

//�e�̕`��
void CSceneRace::RenderShadow(){	
	//Shadow Map ************************************

	GLint	viewport[4]; //�r���[�|�[�g�̕ۑ��p
	CMatrix	modelview; //���f���r���[�ϊ��s��̕ۑ��p
	CMatrix	projection; //�����ϊ��s��̕ۑ��p

	/*
	** ��P�X�e�b�v�F�f�v�X�e�N�X�`���̍쐬
	*/

	/* �f�v�X�o�b�t�@���N���A���� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* ���݂̃r���[�|�[�g��ۑ����Ă��� */
	glGetIntegerv(GL_VIEWPORT, viewport);

	/* �r���[�|�[�g���e�N�X�`���̃T�C�Y�ɐݒ肷�� */
	glViewport(0, 0, TEXWIDTH, TEXHEIGHT);

	/* �����ϊ��s���ݒ肷�� */
	glMatrixMode(GL_PROJECTION); //�����ϊ��s��ɐ؂�ւ�
	glPushMatrix(); //���݂̐ݒ�̓X�^�b�N�ɕۑ�
	glLoadIdentity(); //�s��̏�����

	/* Depth�e�N�X�`���̓����ϊ��s���ۑ����Ă��� */
	gluPerspective(75.0, (GLdouble)TEXWIDTH / (GLdouble)TEXHEIGHT, 1.0, 100000.0);
	glGetFloatv(GL_PROJECTION_MATRIX, projection.mM[0]); //�����ϊ��s��̕ۑ�

	GLfloat lightpos[] = { 0.0f, 200.0f, 200.0f, 0.0f }; //���C�g�̈ʒu�f�[�^
	lightpos[2] = 0.0f; //���C�g�̈ʒu�f�[�^
	if (CSceneTitle::mCource_Number == 1){
		lightpos[1] = 2000.0f*2; //���C�g�̈ʒu�f�[�^
	}
	else if (CSceneTitle::mCource_Number == 2){
		lightpos[1] = 10000.0f; //���C�g�̈ʒu�f�[�^
	}
	else if (CSceneTitle::mCource_Number == 5){
		lightpos[1] = 24000.0f; //���C�g�̈ʒu�f�[�^
	}
	else{
		//�����������قǉe�̉掿���e���Ȃ��Ă��܂�
	}

	lightpos[0] = mPlayer->mPosition.mX; //���C�g�̈ʒu�f�[�^
	lightpos[1] = mPlayer->mPosition.mY+1400.0f; //���C�g�̈ʒu�f�[�^
	lightpos[2] = mPlayer->mPosition.mZ; //���C�g�̈ʒu�f�[�^

	/* �����ʒu�����_�Ƃ��V�[��������Ɏ��܂�悤���f���r���[�ϊ��s���ݒ肷�� */
	glMatrixMode(GL_MODELVIEW); //���f���r���[�s��ɐ؂�ւ�
	glPushMatrix(); //���݂̐ݒ�̓X�^�b�N�ɕۑ�
	glLoadIdentity(); //�s��̏�����
	//���C�g�ʒu���猩��悤�ɍs���ݒ肷��
	gluLookAt(lightpos[0], lightpos[1], lightpos[2], lightpos[0] - 1, 0, lightpos[2] - 1, 0.0, 1.0, 0.0);
	/* �ݒ肵�����f���r���[�ϊ��s���ۑ����Ă��� */
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.mM[0]);

	/* �f�v�X�o�b�t�@�̓��e�������擾����̂Ńt���[���o�b�t�@�ɂ͏������܂Ȃ� */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* ���������ĉA�e�t�����s�v�Ȃ̂Ń��C�e�B���O���I�t�ɂ��� */
	glDisable(GL_LIGHTING);

	/* �f�v�X�o�b�t�@�ɂ͔w�ʂ̃|���S���̉��s�����L�^����悤�ɂ��� */
	glCullFace(GL_FRONT);
	//************************************ Shadow Map

	//Depth�e�N�X�`�����쐬����`��
	CTaskManager::Get()->Render();

	//Shadow Map ************************************
	/* �e�N�X�`�����j�b�g�P�ɐ؂�ւ��� */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
	/* �f�v�X�o�b�t�@�̓��e���e�N�X�`���������ɓ]������ */
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXWIDTH, TEXHEIGHT);

	/* �ʏ�̕`��̐ݒ�ɖ߂� */
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glMatrixMode(GL_PROJECTION); //�����ϊ��s��ɐ؂�ւ�
	glPopMatrix(); //�ݒ���X�^�b�N����߂�

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	/*
	** ��Q�X�e�b�v�F�S�̂̕`��
	*/

	/* �t���[���o�b�t�@�ƃf�v�X�o�b�t�@���N���A���� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* ���f���r���[�ϊ��s��̐ݒ� */
	glMatrixMode(GL_MODELVIEW); //���f���r���[�s��ɐ؂�ւ�
	glPopMatrix(); //�X�^�b�N���猳�ɖ߂�
	/* ���f���r���[�ϊ��s���ۑ����Ă��� */
	CMatrix modelviewCamera;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewCamera.mM[0]);

	/* �����̈ʒu��ݒ肷�� */
	//glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	/* �e�N�X�`���ϊ��s���ݒ肷�� */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* �e�N�X�`�����W�� [-1,1] �͈̔͂� [0,1] �͈̔͂Ɏ��߂� */
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	/* �e�N�X�`���̃��f���r���[�ϊ��s��Ɠ����ϊ��s��̐ς������� */
	glMultMatrixf(projection.mM[0]);
	glMultMatrixf(modelview.mM[0]);

	/* ���݂̃��f���r���[�ϊ��̋t�ϊ��������Ă��� */
	glMultMatrixf(modelviewCamera.GetInverse().mM[0]);

	/* ���f���r���[�ϊ��s��ɖ߂� */
	glMatrixMode(GL_MODELVIEW);

	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎���������L���ɂ��� */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	/* �����̖��邳������̕����ł̖��邳�ɐݒ� */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
	//************************************ Shadow Map

	
	//�e�̕`��
	if (isEnableShadow){
		//�R�[�X�̉e�̕`��
		if (isEnableShadow_Cource){
			for (int i = 0; i < GROUND_AMOUNT; i++){
				if (mpGrounds[i] != NULL){
					//�e�N�X�`�����j�b�g0�ɐ؂�ւ���
					glActiveTexture(GL_TEXTURE0);
					mpGrounds[i]->Render();
					//�e�N�X�`�����j�b�g1�ɐ؂�ւ���
					glActiveTexture(GL_TEXTURE1);
				}
			}
		}
		//�Ԃ̉e��ݒ�
		if (isEnableShadow_Car){
			for (int i = 0; i < ENEMYS_AMOUNT + 1; i++){
				if (mCarShadow[i] != NULL){
					//�e�N�X�`�����j�b�g0�ɐ؂�ւ���
					glActiveTexture(GL_TEXTURE0);
					mCarShadow[i]->Render();
					//�e�N�X�`�����j�b�g1�ɐ؂�ւ���
					glActiveTexture(GL_TEXTURE1);
				}
			}
		}		
	}
	

	//Shadow Map ************************************
	/* �e�N�X�`���}�b�s���O�ƃe�N�X�`�����W�̎��������𖳌��ɂ��� */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);
	//�e�N�X�`������������
	glBindTexture(GL_TEXTURE_2D, 0);
	/* �e�N�X�`���ϊ��s���ݒ肷�� */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glActiveTexture(GL_TEXTURE0);
	//************************************ Shadow Map
}

void CSceneRace::FadeIn(){
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//
	if (isBlackOutTime < 60){
		isBlackOutTime++;
	}
	else{
		isFadeIn = false;
	}
	color[0] = color[1] = color[2] = 0.0f;
	color[3] = 1.5f - 0.03f*isBlackOutTime;
	glColor4fv(color);
	CRectangle::Render(400, 300, 400, 300);
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
}

//�ʂ̃V�[���ɉf��ۂ̏���
void CSceneRace::SceneChange(){
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//�Ó]
	if (isBlackOutTime < 60){
		isBlackOutTime++;
	}
	else{
		isFadeOut = false;
		//�^�C�g����ʂɖ߂�
		mScene = ETITLE;
	}
	color[0] = color[1] = color[2] = 0.0f;
	color[3] = 0.0f + 0.025f*isBlackOutTime;
	glColor4fv(color);
	CRectangle::Render(400, 300, 400, 300);
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
}

//���̃V�[���̎擾
CScene::EScene CSceneRace::GetNextScene(){
	return mScene;
}

void CSceneRace::PutCPUColor(){
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		int cpu_number = i + 1;
		if (mEnemys[i]->mpModel == &mCarRed){
			printf("CPU%d RED\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarBlue){
			printf("CPU%d BLUE\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarBlack){
			printf("CPU%d BLACK\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarCyan){
			printf("CPU%d CYAN\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarGreen){
			printf("CPU%d GREEN\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarGray){
			printf("CPU%d GRAY\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarPink){
			printf("CPU%d PINK\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarWhite){
			printf("CPU%d WHITE\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarYellow){
			printf("CPU%d YELLOW\n", cpu_number);
		}
	}
}
