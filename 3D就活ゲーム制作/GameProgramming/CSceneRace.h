#ifndef CSCENERACE_H
#define CSCENERACE_H
#include "CScene.h"
#include "CModel.h"
#include "CPlayer.h"
#include "CEnemy.h"
//CSound.h���C���N���[�h�����特�̍Đ��̓��삪�y���Ȃ����H
#include "CSound.h"
#include "CSceneGame.h"

#define ENEMYS_AMOUNT 8 //0�ȉ���NG
/*
�Q�[���̃V�[��
*/
class CSceneRace : public CScene {
public:
	//���̃V�[���̎擾
	EScene GetNextScene();

	~CSceneRace();
	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
	//�~�j�}�b�v�֘A�̏���
	void RenderMiniMap();
	
	CModel mSky;
	CModel mRock;
	CModel mRover;
	CModel mCarRed;
	CModel mCarBlue;
	CModel mCarGreen;
	CModel mCarYellow;
	CModel mCarPink;
	CModel mCarCyan;
	CModel mCarWhite;
	CModel mCarBlack;//���͂�V���G�b�g
	CModel mCube;
	CModel mPlane;
	CModel mStairs;//�K�i
	CModel mTarget;//�I
	CModel mOnBlock;//OFF�ɂȂ�Ə�����

	CModel mItem;
	

	CModel mCube2;//�{���ɓ���
	CModel mCube3;//�����ۂ��F

	CModel mCheckPoint;//�����y���ׂ̈�1�������̃|���S���ŁA�����^�C����\��

	CModel mWater;//���^�C��

	CModel mSpringS;//�΂�
	CModel mSpringL;//�΂ːL��

	CModel mTileBlack;//������
	CModel mTileWhite;//������

	CModel mCarsol;//���(�~�j�}�b�v)
	CModel mCarsol_Enemy;//�G�̖��(�~�j�}�b�v)
	CModel mMiniGoal;//�S�[��Icon(�~�j�}�b�v)

	CModel mCource01;//�R�[�X01
	CModel mGrass01;//�Ő�01
	CModel mFence01;//��
	CModel mRWTile;//�Ő��ƃA�X�t�@���g�̋���
	
	CModel mFenceTop;//��̏��
	CModel mFenceSide;//��̑���

	CModel mPole;//�|�[��

	CModel mDashBoard;//�������鏰

	CPlayer *mPlayer;
	CEnemy *mEnemys[ENEMYS_AMOUNT];
	

	CSound BGM;
	CSound SoundCountDown;
	CSound SoundStart;
	CSound SoundGoal;
	

	int mBlockAmoX;//X���W�̍ő�u���b�N��
	int mBlockAmoY;//Y���W�̍ő�u���b�N��
	int mBlockAmoZ;//Z���W�̍ő�u���b�N��
	int mBlockAmount;//��̎O�̐ςɂ���

	float mCamY;//�v���C���[�̎�����O���O��(����������)

	bool mPutCol;

	int mFrame;
	int mCountDown;
	bool isStartRace, isGoal;
	int mTime;
	static int mBestTime;
	int mLap, mMaxLap;
	bool isNewRecord;
	static int mRecord_A, mRecord_B, mRecord_C, mRecord_D;

	CModel mSumple;
	int mCamPoV;

	//�R�[�X2�̃}�e���A��
	CModel msumple2;

	int mTextBlinkTime;
	bool isRendPoint;
	
	bool isPause;

	int mRanking;
	int mAfterGoalTime;
};

#endif
