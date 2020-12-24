#ifndef CENEMY_H
#define CENEMY_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CSound.h"

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
};


#endif