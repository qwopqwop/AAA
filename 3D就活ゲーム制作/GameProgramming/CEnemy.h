#ifndef CENEMY_H
#define CENEMY_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CSound.h"
#include "CPoint.h"

class CEnemy :public CCharacter{
public:
	static CEnemy*mpEnemy;
	//�f�t�H���g�R���X�g���N�^
	CEnemy();

	//�X�V
	void Update();

	//�v���C���[�̑̂̓����蔻��
	CCollider mColBody;
	//�v���C���[�̃^�C���̓����蔻��
	CCollider mColTire;

	//�Փˏ���
	void Collision(CCollider *mc, CCollider *yc);

	//�W�����v���x�̕ϐ�
	float mVelocityJump;
	//�W�����v��(�W�����v����)�̕ϐ�
	float mJumpV0;

	//�ړ����x
	float mMoveSpeed;
	float mADMoveX;
	float mWSMoveZ;
	float mCarSpeed;

	//��]���x
	float mTurnSpeed;

	bool mCanJump;//�W�����v�\��(���n�ʂɑ��������Ă邩)

	CSound SoundBoost;
	CSound SoundEngine;//��Ԃ̓��
	bool isSoundEngine;
	CSound SoundCollision;
	CSound SoundCollisionSmall;

	float mStartPoint[3];//{ X,Y,Z }

	int mChecks;

	bool isBoost;//�u�[�X�g����(�������ň�莞�ԗL��)
	float mBoostMaxSpeed;//�u�[�X�g�Œ�グ�����ō����x�̐��l
	int mBoostTime;//�u�[�X�g��Ԃ��p������鎞��

	bool CanMove;//�v���C���[������\���ۂ�(�J�E���g�_�E���O�E�S�[����Ȃǂ͔�)

	static int RenderType;//�Q�[����ʂ̕`��ƃ~�j�}�b�v�̕`��𕪂���ϐ�

	//�R���C�_�̍X�V
	void TaskCollision();



	//�U���|�C���g//���S�A�S�G�l�~�[���ʂ̃|�C���g
	static CPoint *mPoint;
	static CPoint *mPoint2;
	static CPoint *mPoint3;
	static CPoint *mPoint4;
	static CPoint *mPoint5;
	static CPoint *mPoint6;
	static CPoint *mPoint7;
	static CPoint *mPoint8;
	static CPoint *mPoint9;
	static CPoint *mPoint10;
	static CPoint *mPoint11;
	static CPoint *mPoint12;
	//�����ł̃u�����܂ޗU���|�C���g
	CPoint *mPointRand;
	CPoint *mPointRand2;
	CPoint *mPointRand3;
	CPoint *mPointRand4;
	CPoint *mPointRand5;
	CPoint *mPointRand6;
	CPoint *mPointRand7;
	CPoint *mPointRand8;
	CPoint *mPointRand9;
	CPoint *mPointRand10;
	CPoint *mPointRand11;
	CPoint *mPointRand12;

	static int mPointSize;//�|�C���g�̐�
	CPoint *mpPoint;//�ڎw���|�C���g
	int mPointCnt;//�|�C���g�̃J�E���^

	//�R���C�_
	CCollider mSearch;

	int mRangeX, mRangeZ;

	CPoint *mpPoint2;
};


#endif