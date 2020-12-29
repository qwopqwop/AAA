#ifndef CPLAYER_H
#define CPLAYER_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"
#include "CSound.h"

class CPlayer :public CCharacter{
public:
	static CPlayer*mpPlayer;
	//�f�t�H���g�R���X�g���N�^
	CPlayer();

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

	//�W�����v�𓖂��蔻����������������������
	int mJumpPrio;

	//�ړ����x
	float mMoveSpeed;
	float mADMoveX;
	float mWSMoveZ;
	float mCarSpeed;

	//��]���x
	float mTurnSpeed;

	bool mCanJump;//�W�����v�\��(���n�ʂɑ��������Ă邩)
	int mCanSwim;//���j�\��
	int mCanFire;//���C�\��
	//�e�̍Ĕ��ˊԊu
	int FireCount;

	CSound SoundJump;
	CSound SoundShot;
	CSound SoundItemGet;
	CSound SoundBoost;
	CSound SoundEngine;//��Ԃ̓��
	bool isSoundEngine;
	CSound SoundHorn;
	CSound ShutUp;
	int mBuzzerCount;
	CSound SoundCollision;
	CSound SoundCollisionSmall;

	float mStartPoint[3];//{ X,Y,Z }

	bool mIsGetKey;
	bool mHaveBoat;

	bool mFlyingMode;

	int mChecks;

	bool isBoost;//�u�[�X�g����(�������ň�莞�ԗL��)
	float mBoostMaxSpeed;//�u�[�X�g�Œ�グ�����ō����x�̐��l
	int mBoostTime;//�u�[�X�g��Ԃ��p������鎞��

	bool CanMove;//�v���C���[������\���ۂ�(�J�E���g�_�E���O�E�S�[����Ȃǂ͔�)

	static int RenderType;//�Q�[����ʂ̕`��ƃ~�j�}�b�v�̕`��𕪂���ϐ�


	//�R���C�_�̍X�V
	void TaskCollision();
};


#endif