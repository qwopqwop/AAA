#ifndef CSCENERACE_H
#define CSCENERACE_H
#include "CScene.h"
#include "CModel.h"
#include "CPlayer.h"
#include "CEnemy.h"
//CSound.hをインクルードしたら音の再生の動作が軽くなった？
#include "CSound.h"
#include "CSceneGame.h"

#define ENEMYS_AMOUNT 8 //0以下はNG
/*
ゲームのシーン
*/
class CSceneRace : public CScene {
public:
	//次のシーンの取得
	EScene GetNextScene();

	~CSceneRace();
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
	//ミニマップ関連の処理
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
	CModel mCarBlack;//もはやシルエット
	CModel mCube;
	CModel mPlane;
	CModel mStairs;//階段
	CModel mTarget;//的
	CModel mOnBlock;//OFFになると消える

	CModel mItem;
	

	CModel mCube2;//本当に透明
	CModel mCube3;//窓っぽい色

	CModel mCheckPoint;//処理軽減の為に1枚だけのポリゴンで、透明タイルを表現

	CModel mWater;//水タイル

	CModel mSpringS;//ばね
	CModel mSpringL;//ばね伸び

	CModel mTileBlack;//黒い床
	CModel mTileWhite;//白い床

	CModel mCarsol;//矢印(ミニマップ)
	CModel mCarsol_Enemy;//敵の矢印(ミニマップ)
	CModel mMiniGoal;//ゴールIcon(ミニマップ)

	CModel mCource01;//コース01
	CModel mGrass01;//芝生01
	CModel mFence01;//柵
	CModel mRWTile;//芝生とアスファルトの境目
	
	CModel mFenceTop;//柵の上面
	CModel mFenceSide;//柵の側面

	CModel mPole;//ポール

	CModel mDashBoard;//加速する床

	CPlayer *mPlayer;
	CEnemy *mEnemys[ENEMYS_AMOUNT];
	

	CSound BGM;
	CSound SoundCountDown;
	CSound SoundStart;
	CSound SoundGoal;
	

	int mBlockAmoX;//X座標の最大ブロック数
	int mBlockAmoY;//Y座標の最大ブロック数
	int mBlockAmoZ;//Z座標の最大ブロック数
	int mBlockAmount;//上の三つの積にする

	float mCamY;//プレイヤーの周りをグルグル(水平方向に)

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

	//コース2のマテリアル
	CModel msumple2;

	int mTextBlinkTime;
	bool isRendPoint;
	
	bool isPause;

	int mRanking;
	int mAfterGoalTime;
};

#endif
