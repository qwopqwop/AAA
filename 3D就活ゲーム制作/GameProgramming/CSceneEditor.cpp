#include "CSceneEditor.h"
//
#include "CSceneTitle.h"
//
#include "CCamera.h"
//
#include "CText.h"
//
#include "CTaskManager.h"
//
#include "CCollisionManager.h"
////
#include "CRock.h"
//
#include "CObj.h"
//
#include "CObjWall.h"
//
#include "CItem.h"
//
#include "CKey.h"
//
#include "CBullet.h"


#include "CRaceCourceD.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CObjGrass.h"
#include "CObjBoost.h"
#include "CObjNonCol.h"

////�X�}�[�g�|�C���^�̐���
//std::shared_ptr<CTexture> TextureExp(new CTexture());
//std::shared_ptr<CTexture> TextureHit(new CTexture());
//std::shared_ptr<CTexture> TextureBoost(new CTexture());

extern CSound BGM;
extern CSound SoundCountDown;
extern CSound SoundStart;
extern CSound SoundGoal;

////������mBestTime�̒l�͊֌W�Ȃ�(mRecord_ �̒l�����邽��)
//int CSceneEditor::mBestTime = 0;
//int CSceneEditor::mRecord_A = 10000;
//int CSceneEditor::mRecord_B = 13000;
//int CSceneEditor::mRecord_C = 22000;
//int CSceneEditor::mRecord_D = 460000;
//int CSceneEditor::mRecord_E = 40000;
//int CSceneEditor::mRecord_F = 43300;

//��ʃT�C�Y��800x600��z��
#define SCREENSIZE_X 800
#define SCREENSIZE_Y 600
//�`��G���A�̎w��(���[��X���W,���[��Y���W,��,����)
#define MINIMAP_AREA 590,10,200,150
#define MINIMAP_AREA_EDITMODE 0,0,800,600
#define BACKMIRROR_FRAME_AREA 286,491,229,154
#define BACKMIRROR_BG_WHITE_AREA 288,493,225,150
#define BACKMIRROR_VIEW_AREA 288,493,225,150
#define BACKMIRROR_EXTRAFRAME_AREA 286,598,228,5

CSceneEditor::~CSceneEditor() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneEditor::Init() {
	//�I�̎c���̏�����
	CItem::mTargetAmount = 0;

	////�����e�N�X�`���̓ǂݍ���
	//TextureExp->Load("exp.tga");
	////�Փ˃e�N�X�`���̓ǂݍ���
	//TextureHit->Load("effect\\[Attack]Hit01_panop.tga");
	////�����e�N�X�`���̓ǂݍ���
	//TextureBoost->Load("effect\\boost01.tga");

	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	//mFont3:�Ō��mFont3�ɐݒ肵���V�[���ł̃t�H���g(=Title��ʂ̃t�H���g)

	/*�S�R�[�X���ʂ̃}�e���A��*/
	mSky.Load("sky.obj", "sky.mtl");//�w�i
	mRock.Load("Rock1.obj", "Rock1.mtl");//��
	//�Ԃ̓ǂݍ���
	mRover.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");//�v���C���[
	mCarRed.Load("Rover1.obj", "material\\racing_mat\\single_color\\red.mtl");//�ȉ��G�̎�
	mCarBlue.Load("Rover1.obj", "material\\racing_mat\\single_color\\blue.mtl");
	mCarGreen.Load("Rover1.obj", "material\\racing_mat\\single_color\\green.mtl");
	mCarYellow.Load("Rover1.obj", "material\\racing_mat\\single_color\\yellow.mtl");
	mCarPink.Load("Rover1.obj", "material\\racing_mat\\single_color\\pink.mtl");
	mCarCyan.Load("Rover1.obj", "material\\racing_mat\\single_color\\cyan.mtl");
	mCarWhite.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");
	mCarBlack.Load("Rover1.obj", "material\\racing_mat\\single_color\\black.mtl");
	mCarGray.Load("Rover1.obj", "material\\racing_mat\\single_color\\gray.mtl");
	mCheckPoint.Load("cube.obj", "cube2.mtl");//���Ԓn�_(�����A�|���S��1��)
	//���^�C���̓ǂݍ���
	mTileBlack.Load("cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("cube.obj", "material\\racing_mat\\tile_white.mtl");
	mRWTile.Load("material\\racing_mat\\NewNewR-W.obj", "material\\racing_mat\\NewNewR-W.mtl");
	//�����̂̓ǂݍ���
	mCube.Load("cube.obj", "material\\cube.mtl");//��
	mCube2.Load("cube.obj", "cube2.mtl");//����
	mCube3.Load("cube.obj", "cube3.mtl");//?
	//�n�ʂ̓ǂݍ���
	mPlane.Load("plane.obj", "plane.mtl");
	//�K�i�̓ǂݍ���
	mStairs.Load("cube.obj", "material\\stairs.mtl");
	//�I�̓ǂݍ���
	mTarget.Load("Rock1.obj", "material\\target.mtl");
	//ON�u���b�N(�H)�̓ǂݍ���
	mOnBlock.Load("cube.obj", "material\\on.mtl");
	mSign_Left.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnLeft.mtl");//�W��:����
	mSign_Right.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnRight.mtl");//�W��:�E��
	//�~�j�}�b�v��ł̃v���C���[�E�G�̃J�[�\���A�S�[���n�_�̓ǂݍ���
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//�v���C���[
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//�G
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");
	mPole.Load("cube.obj", "material\\soil.mtl");//�|�[��(�ؐ�)
	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");//������
	mJumper01.Load("cube.obj", "cube.mtl");//�W�����v��
	/*�R�[�X�ʂɓǂݍ��ރ}�e���A��*/
	if (CSceneTitle::mMode == 1){
		mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");//�H��
		mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");//�Ő�
		mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");//��(���)
		mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");//��(��)
	}
	else if (CSceneTitle::mMode == 2){
		mCource02Road.Load("material\\racing_mat\\cource2nd\\cource02road.obj", "material\\racing_mat\\cource2nd\\cource02road.mtl");
		mCource02Wall.Load("material\\racing_mat\\cource2nd\\cource02wall.obj", "material\\racing_mat\\cource2nd\\cource02wall.mtl");
		mCource02Jump.Load("material\\racing_mat\\cource2nd\\cource02jumper.obj", "material\\racing_mat\\cource2nd\\cource02jumper.mtl");
	}
	else if (CSceneTitle::mMode == 3){
		mCource03Road.Load("material\\racing_mat\\stage3\\cource03road.obj", "material\\racing_mat\\stage3\\cource03road.mtl");
		mCource03Wall.Load("material\\racing_mat\\stage3\\cource03wall.obj", "material\\racing_mat\\stage3\\cource03wall.mtl");
		mCource03Fence.Load("material\\racing_mat\\stage3\\cource03fence.obj", "material\\racing_mat\\stage3\\cource03fence.mtl");
	}
	else if (CSceneTitle::mMode == 4){
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
	else if (CSceneTitle::mMode == 5){
		//�R�[�X05�̓ǂݍ���
		mCource05Wall.Load("material\\racing_mat\\stage5\\cource05wall.obj", "material\\racing_mat\\stage5\\cource05wall.mtl");
		mCource05Mountain.Load("material\\racing_mat\\stage5\\cource05mountain.obj", "material\\racing_mat\\stage5\\cource05mountain.mtl");//�S�Ă̎R����
		mCource05Road.Load("material\\racing_mat\\stage5\\cource05road2.obj", "material\\racing_mat\\stage5\\cource05road2.mtl");
		mCource05Lake.Load("material\\racing_mat\\stage5\\cource05_lake.obj", "material\\racing_mat\\stage5\\cource05_lake.mtl");
		mCource05Grass_Floor.Load("material\\racing_mat\\stage5\\cource05grassF03.obj", "material\\racing_mat\\stage5\\cource05grassF03.mtl");
		mCource05Grass_Wall.Load("material\\racing_mat\\stage5\\cource05grass_wall.obj", "material\\racing_mat\\stage5\\cource05grass_wall.mtl");
		mCource05GoalTile.Load("material\\racing_mat\\stage5\\Checker_Tile.obj", "material\\racing_mat\\stage5\\Checker_Tile.mtl");
	}
	else if (CSceneTitle::mMode == 6){
		//
	}
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

	BGM.Load("BGM\\revolumed_PopsGuitar_No.01.wav");
	
	//���ʉ��̓ǂݍ���
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");

	//�J�������_��Y���W
	mCamY = 0.0f;
	//�Փ˔���̕`��(�f�o�b�O�p)
	mPutCol = false;

	//�ꕔ�e�L�X�g���_�ł��鎞��
	mTextBlinkTime = 0;


	//�J�������_
	mCamPoV = 1;


	//������Ԃł͓G�̖ڕW�n�_�͕`�悵�Ȃ�
	isRendPoint = false;
	//������Ԃł̓|�[�Y��Ԗ���
	isPause = false;

	//BGM�̓��[�v
	BGM.Repeat();


	//�V�[���̐ݒ�
	mScene = EEDIT;
	//�v���C���[�̐���
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//�J�����̐���
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();


	int ess[3][4][5] = {
		{
			{ 00, 00, 00, 00, 21 },
			{ 00, 24, 00, 00, 00 },
			{ 00, 00, 00, 00, 00 },
			{ 00, 00, 00, 00, 00 },
		},
		{
			{ 00, 00, 00, 32, 00 },
			{ 00, 00, 34, 21, 33 },
			{ 00, 33, 00, 00, 00 },
			{ 00, 23, 22, 00, 00 },
		},
		{
			{ 24, 12, 32, 00, 00 },
			{ 31, 24, 21, 00, 00 },
			{ 33, 11, 31, 33, 33 },
			{ 23, 22, 00, 23, 22 },
		}
	};
	int size = 10;//�R�[�X�̏k��
	//for (int i = 3-1; i >= 0; i--){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 4; j++){
			for (int k = 0; k < 5; k++){
				if (ess[i][j][k] == 11){//����:I��
					new CObjFloor(&mTile_Straight01_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Straight01_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 12){//����:�ꎚ
					new CObjFloor(&mTile_Straight02_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Straight02_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 21){//�J�[�u:0���`3��
					new CObjFloor(&mTile_Curve03_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve03_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 22){//�J�[�u:3���`6��
					new CObjFloor(&mTile_Curve04_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve04_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 23){//�J�[�u:6���`9��
					new CObjFloor(&mTile_Curve01_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve01_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 24){//�J�[�u:9���`12��
					new CObjFloor(&mTile_Curve02_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve02_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 31){//��:
					new CObjFloor(&mTile_Slope01_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope01_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 32){//��:
					new CObjFloor(&mTile_Slope02_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope02_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 33){//��:
					new CObjFloor(&mTile_Slope03_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope03_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 34){//��:
					new CObjFloor(&mTile_Slope04_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope04_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 91){//�L����
					new CObjFloor(&mTile_Wide_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Wide_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 99){//�ł����u���b�N
					new CObjFloor(&mBlock_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mBlock_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
			}
		}
	}


	//�D��x�ύX
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
}


void CSceneEditor::Update() {
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����

	////���_�����߂�
	//if (mCamPoV == 1){
	//	e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//	//e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	//	//* mPlayer->mMatrixTranslate
	//	//	*CCameraPos::mpCamera->mMatrixTranslate
	//	//	+ CVector(0.0f, 0.0f, 0.0f);
	//	//c = CCameraPos::mpCamera->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	//	//e = CVector(0.0f, 0.0f, 0.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	//	*CCameraPos::mpCamera->mMatrixTranslate;
	//	//c = CCameraPos::mpCamera->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	//	
	//}
	//else if (mCamPoV == 2){
	//	e = CVector(0.0f, 0.0f + 0.5f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	////�����_�����߂�
	//	//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//		//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	//}
	//else if (mCamPoV == 3){//������f�����_
	//	e = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	////�����_�����߂�
	//	//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//		//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	//}
	//else{//1�`3�ȊO�̐��l�������Ă��鎞�͂Ƃ肠�����O�����_(1�Ɠ���)
	//	e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//}
	////e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	////	* CMatrix().RotateY(mPlayer->mRotation.mY)
	////	*mCam->mMatrixTranslate
	////	//* mPlayer->mMatrixTranslate
	////	+ CVector(0.0f, 0.0f, 0.0f);
	////c = mCam->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	////	* CMatrix().RotateY(mPlayer->mRotation.mY);
	////����������߂�
	//u = CVector(0.0f, 1.0f, 0.0f);// *mPlayer->mMatrixRotate;

	//e = CCameraPos::mpCamera->mPosition;
	e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
				* CMatrix().RotateY(mPlayer->mRotation.mY)
				* mPlayer->mMatrixTranslate
				+ CVector(0.0f, 0.0f, 0.0f);

	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	u = CVector(0.0f, 1.0f, 0.0f);//*mPlayer->mMatrixRotate;
	//�J�����̐ݒ�
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	//�^�X�N�}�l�[�W���̍X�V�E�`��
	if (isPause == false){
		CTaskManager::Get()->Update();
	}
	CTaskManager::Get()->Render();
	//�Փˏ���
	CTaskManager::Get()->TaskCollision();
	//�폜����
	CTaskManager::Get()->Delete();

	//�f�o�b�O�p
#ifdef _DEBUG
	/*if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
		printf("�x�X�g�^�C��:%d\n", mBestTime);
	}*/
	if (CKey::Once('2')){//Player�̍��W���o�͂���
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	//if (CKey::Once('3')){//�����I�ɓI�̎c����0�ɂ���(�{���̓I�͏����Ȃ�)
	//	for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//		printf("X:%f Y:%f Z:%f\n", mEnemys[i]->mPosition.mX, mEnemys[i]->mPosition.mY, mEnemys[i]->mPosition.mZ);
	//	}
	//	//CItem::mTargetAmount = 0;
	//}
	//if (CKey::Once('4')){//�o�b�N�~���[��ON�EOFF�؂�ւ�
	//	isRender_BackMirror = !isRender_BackMirror;
	//}
	if (CKey::Push('5')){
		printf("%f:%f:%f\n", CPlayer::mpPlayer->mRotation.mX, CPlayer::mpPlayer->mRotation.mY, CPlayer::mpPlayer->mRotation.mZ);
	}
	if (CKey::Once('6')){
		printf("%d\n", CPlayer::mpPlayer->mChecks);
	}
	if (CKey::Once('7')){
		if (CPlayer::mpPlayer->mFlyingMode){
			CPlayer::mpPlayer->mFlyingMode = false;
		}
		else{
			CPlayer::mpPlayer->mFlyingMode = true;
		}
	}
	//if (CKey::Once('8')){
	//	//�G�Ԃ��ׂĂ�mVPoint�̒l���o��
	//	for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//		//printf("mEnemys[%d]->mVPoint�cX:%.1f Y:%.1f Z:%.1f\n", i, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mZ);
	//		printf("mEnemys[%d]�̒��Ԓn�_�c%d  %d�T��\n", i, mEnemys[i]->mChecks, mEnemys[i]->mEnemyLap);
	//	}
	//}
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
	//GetNearestColor(1,2);

	//�G�̒��p�n�_�̕\��ON�EOFF�؂�ւ�
	if (CKey::Once('O')){
		/*if (isRendPoint){
		isRendPoint = false;
		}
		else{
		isRendPoint = true;
		}*/
		//�����ƊȌ��ɐ؂�ւ�����@
		isRendPoint = !isRendPoint;
	}
	//BGM���~����
	if (CKey::Once('M')){
		BGM.Stop();
	}
	//B�L�[�������Ă���ԁA���C����ʂ�������_�ɂȂ�
	if (CKey::Push('B')){
		mCamPoV = 3;
	}
	else{
		mCamPoV = 1;
	}
	//�����g�b�v�X�s�[�h��
	if (CKey::Once('G')){
		mPlayer->mCarSpeed = 20.0f;
	}
	//
	if (CKey::Once('V')){
		mPlayer->mChecks = 3;
	}
#endif

	if (CKey::Once('P')){
		isPause = !isPause;
	}

	////�o�b�N�~���[�̕`��
	//if (isRender_BackMirror){
	//	RenderBackMirror();
	//}
	//�~�j�}�b�v�̕`��
	RenderMiniMap();


	//2D�`��J�n
	Start2D(0, 800, 0, 600);

	//���ʂ̕`��
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//�R�[�X���������ɕ\��
	CText::DrawString("COURCE EDIT", 20, 20, 10, 12);
		
	CPlayer::mpPlayer->CanMove = true;

	//�|�[�Y���\������镶��
	if (isPause){
		CText::DrawString("PAUSE", 280, 300, 10 * 3, 12 * 3, 3);
		CText::DrawString("P - Resume", 290, 200, 10, 12, 2);
		CText::DrawString("Esc - Back to Title", 250, 170, 10, 12, 2);
	}
	//2D�`��I��
	End2D();
		
	//�|�[�Y��Esc�L�[�������^�C�g����ʈڍs
	if (isPause){
		if (CKey::Once(VK_BACK) || CKey::Once(VK_ESCAPE)){
			//���̃V�[���̓Q�[��
			mScene = ETITLE;
		}
	}

	return;
}

/* �}�b�v�ォ��̎��_ */
void CSceneEditor::RenderMiniMap() {
	glPushMatrix();
	glLoadIdentity();
	//�ꎞ�I��2D���_�ɕύX����
	glViewport(MINIMAP_AREA);//��ʂ̕`��G���A�̎w��
	glMatrixMode(GL_PROJECTION);	//�s����v���W�F�N�V�������[�h�֕ύX
	//�s��ޔ�
	glPushMatrix();
	glLoadIdentity();				//�s���������
	//2D��ʂ̐ݒ�ύX
	float size = 4000.0f;//�R�[�X�̏k�ڐݒ�
	bool canscrollmap = false;//�v���C���[�ɍ��킹���}�b�v�ړ��̗L��
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

	


	if (isRendPoint == true){
		/*�f�o�b�O�p*/
		//�ݒ肵���G�̖ڕW�n�_���ׂĂ��~�j�}�b�v��ɕ`�悷��
		CMatrix point;
		for (int i = 1; i <= 12; i++){//�|�C���g�̐������������s
			point = CMatrix().Scale(111.0f, 1.0f, 111.0f)
				* CMatrix().RotateY(45);
			//* CEnemy::mPoint->mMatrixTranslate;
			//1��菬�������͑����
			if (i < 1){
				break;
			}
			if (i == 1){
				point = point * CEnemy::mPoint->mMatrixTranslate;
			}
			else if (i == 2){
				point = point * CEnemy::mPoint2->mMatrixTranslate;
			}
			else if (i == 3){
				point = point * CEnemy::mPoint3->mMatrixTranslate;
			}
			else if (i == 4){
				point = point * CEnemy::mPoint4->mMatrixTranslate;
			}
			else if (i == 5){
				point = point * CEnemy::mPoint5->mMatrixTranslate;
			}
			else if (i == 6){
				point = point * CEnemy::mPoint6->mMatrixTranslate;
			}
			else if (i == 7){
				point = point * CEnemy::mPoint7->mMatrixTranslate;
			}
			else if (i == 8){
				point = point * CEnemy::mPoint8->mMatrixTranslate;
			}
			else if (i == 9){
				point = point * CEnemy::mPoint9->mMatrixTranslate;
			}
			else if (i == 10){
				point = point * CEnemy::mPoint10->mMatrixTranslate;
			}
			else if (i == 11){
				point = point * CEnemy::mPoint11->mMatrixTranslate;
			}
			else if (i == 12){
				point = point * CEnemy::mPoint12->mMatrixTranslate;
			}
			else{
				break;
			}
			mTileWhite.Render(point);
		}
	}
	//�~�j�}�b�v�ɃS�[���A�C�R����`��
	CMatrix matminig;
	matminig = CMatrix().Scale(20.0f, 1.0f, 20.0f)
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(0)
		* CMatrix().RotateZ(0)
		* CMatrix().Translate(550.0f, 0.0f, -10.0f);
	mMiniGoal.Render(matminig);
	//CMatrix matenemys[ENEMYS_AMOUNT];
	//for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//	matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
	//		* CMatrix().RotateX(0)
	//		* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
	//		* CMatrix().RotateZ(0)
	//		* mEnemys[i]->mMatrixTranslate;
	//	mCarsol_Enemy.Render(matenemys[i]);
	//}
	//�~�j�}�b�v��Ƀv���C���[�������J�[�\����`��
	CMatrix matplayer;
	matplayer = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		* CMatrix().RotateZ(0)
		* mPlayer->mMatrixTranslate;
	mCarsol.Render(matplayer);




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
////�o�b�N�~���[��\��
//void CSceneEditor::RenderBackMirror(){
//	glDisable(GL_CULL_FACE);//�ꎞ�I�ɗ��ʂ�`��\�ɂ���
//	glDisable(GL_DEPTH_TEST);
//	glViewport(BACKMIRROR_FRAME_AREA);
//	//2D�`��J�n
//	Start2D(0, 800, 0, 600);
//	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//	glColor4fv(color);
//	//��L��2D�`��͈͂̎w��l���傫�߂ɔ��w�i��`�悷��
//	int expand = 100; color[3] = 0.0f;
//	//���w�i�̂�����ɍ��g�ƂȂ���̂�`�悷��
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(800 + expand, 600 + expand);
//	glVertex2d(0 - expand, 600 + expand);
//	glEnd();
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(800 + expand, 0 - expand);
//	glVertex2d(800 + expand, 600 + expand);
//	glEnd();
//	color[0] = color[1] = color[2] = color[3] = 1.0f;
//	glColor4fv(color);
//	//2D�`��I��
//	End2D();
//
//	glViewport(BACKMIRROR_BG_WHITE_AREA);
//	//2D�`��J�n
//	Start2D(0, SCREENSIZE_X, 0, SCREENSIZE_Y);
//	color[0] = color[1] = color[2] = 0.8f; color[3] = 0.0f;
//	glColor4fv(color);
//	//��L��2D�`��͈͂̎w��l���傫�߂ɔ��w�i��`�悷��
//	expand = 100;
//	//���w�i��"���"�`�悷��
//	glBegin(GL_TRIANGLES);//�v���Ԃ�
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glVertex2d(0 - expand, 600 + expand);
//	glEnd();
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glEnd();
//	color[0] = color[1] = color[2] = color[3] = 1.0f;
//	glColor4fv(color);
//	//2D�`��I��
//	End2D();
//
//	//�s���ޔ�������
//	glPushMatrix();
//	//�s���P�ʍs��ɂ���
//	glLoadIdentity();
//	glViewport(BACKMIRROR_VIEW_AREA);
//	//�J�����̃p�����[�^���쐬����
//	CVector e, c, u;//���_�A�����_�A�����
//	e = CVector(0.0f, 17.0f + 13.0f, 40.0f - 41.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale
//		* CMatrix().RotateY(mPlayer->mRotation.mY)
//		* mPlayer->mMatrixTranslate;
//	c = mPlayer->mPosition + CVector(0.0f, 17.0f + 12.8f, 40.0f - 42.0f)* mPlayer->mMatrixScale
//		* CMatrix().RotateY(mPlayer->mRotation.mY);
//	u = CVector(0.0f, 1.0f, 0.0f);
//	//�J�����̂�X���W�𔽓]������	
//	e = e * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
//	c = c * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
//	u = u * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
//	//�o�b�N�~���[�̃J�����̐ݒ�
//	gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
//	GLfloat translate[] = {
//		-1, 0, 0, 0,
//		0, 1, 0, 0,
//		0, 0, 1, 0,
//		0, 0, 0, 1
//	};
//	glMultMatrixf(translate);
//	CTaskManager::Get()->Render();
//
//	//���g�̌��؂�̕⊮
//	glViewport(BACKMIRROR_EXTRAFRAME_AREA);
//	//2D�`��J�n
//	Start2D(0, SCREENSIZE_X, 0, SCREENSIZE_Y);
//	color[0] = color[1] = color[2] = 0.0f;
//	glColor4fv(color);
//	//��L��2D�`��͈͂̎w��l���傫�߂ɔ��w�i��`�悷��
//	expand = 100;
//	//���i�̏�ɍ��g��`�悷��
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glVertex2d(0 - expand, SCREENSIZE_Y + expand);
//	glEnd();
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glEnd();
//	color[0] = color[1] = color[2] = color[3] = 1.0f;
//	glColor4fv(color);
//	//2D�`��I��
//	End2D();
//
//	glEnable(GL_DEPTH_TEST);
//	glViewport(0, 0, SCREENSIZE_X, SCREENSIZE_Y); //��ʂ̕`��G���A�����C���̉�ʂɖ߂�
//	glEnable(GL_CULL_FACE);//�\�ʂ݂̂̕`��ɖ߂�
//
//	//�s���߂�
//	glPopMatrix();
//}
//���̃V�[���̎擾
CScene::EScene CSceneEditor::GetNextScene(){
	return mScene;
}