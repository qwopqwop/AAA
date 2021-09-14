#ifndef CSCENERACE_H
#define CSCENERACE_H
#include "CScene.h"
#include "CModel.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CCameraRange.h"
#include "CCameraPos.h"
#include "CRigidObj.h"
//CSound.hをインクルードしたら音の再生の動作が軽くなった？
#include "CSound.h"

#include "CObjFloor.h"

#define ENEMYS_AMOUNT 5-4//0以下には設定できない

#define GROUND_AMOUNT 128

#include <stdlib.h>

/*
ゲームのシーン
*/
class CSceneRace : public CScene {
private:
	static bool isEnableShadow;//影の描画の有無
	static bool isEnableMiniMap;//ミニマップ表示のON・OFF
	static bool isEnableBackMirror;//バックミラー表示のON・OFF
	static bool isEnableSpeedometer;//速度計のON・OFF

	GLuint mDepthTextureID;

	float mCamY;//プレイヤーの周りを回転(水平方向に)	

	//スタート前のカウントダウン
	int mFrame;//60f=1秒
	int mCountDown;
	//コースタイム、ラップ関連
	bool isStartRace, isGoal;
	int mTime, mTime_Output;


	static int mBestTime;
	int mLap, mMaxLap;
	bool isNewRecord;
	int mTextBlinkTime;//テキストの点滅に関わる変数

	bool isPause;
	int mPause_SelectCarsol;
	int mPause_OptionCarsol;
	enum EPauseScreen{
		EPAUSE,//ポーズ画面(選択肢がresume,option,quit)
		EOPTION,//オプション画面(設定を変更できる画面)
	};
	EPauseScreen mPauseScreen;

	int mRanking;
	int mAfterGoalTime;

	//デバッグコマンド用の変数
	bool mPutCol;//当たり判定の描画のON・OFF

	bool isEnableShadow_Cource;
	bool isEnableShadow_Car;

	bool isFadeIn, isFadeOut;
	int isBlackOutTime;

	bool isOpening;
	int mTime_Opening;

protected:
	CModel mRover;
	CModel mCarRed;
	CModel mCarBlue;
	CModel mCarGreen;
	CModel mCarYellow;
	CModel mCarPink;
	CModel mCarCyan;
	CModel mCarWhite;
	CModel mCarBlack;
	CModel mCarGray;
	CModel mPlane;
	CModel mCheckPoint;//ポリゴン1枚のみ、透明
	CModel mTileBlack;//黒い床
	CModel mTileWhite;//白い床
	CModel mCarsol;//矢印(ミニマップ)
	CModel mCarsol_Enemy;//敵の矢印(ミニマップ)
	CModel mMiniGoal;//ゴールIcon(ミニマップ)
	CModel mCource01;//コース01
	CModel mGrass01;//芝生01
	CModel mGoalTile01;
	CModel mCurb01;//紅白の縁石
	CModel mFenceTop;//柵の上面
	CModel mFenceSide;//柵の側面
	CModel mPole;//ポール
	CModel mDashBoard;//加速床

	CPlayer *mPlayer;
	CEnemy *mEnemys[ENEMYS_AMOUNT];
	CCameraRange *mCamRange;
	CCameraPos *mCam;

	CSound BGM;
	CSound JingleOpening;
	CSound SoundCountDown;
	CSound SoundStart;
	CSound SoundGoal;
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	CSound SoundPauseOn;
	CSound SoundPauseOff;


	//コース2,3のモデル
	CModel mCource02Road;
	CModel mCource02Wall;
	CModel mCource02Jump;
	CModel mCource03Road;
	CModel mCource03Wall;
	CModel mCource03Fence;

	CModel mJumper01;//ジャンプ台
	//平らなタイル
	CModel mTile_Curve01_Floor;
	CModel mTile_Curve02_Floor;
	CModel mTile_Curve03_Floor;
	CModel mTile_Curve04_Floor;
	CModel mTile_Straight01_Floor;
	CModel mTile_Straight02_Floor;
	CModel mTile_Curve01_Wall;
	CModel mTile_Curve02_Wall;
	CModel mTile_Curve03_Wall;
	CModel mTile_Curve04_Wall;
	CModel mTile_Straight01_Wall;
	CModel mTile_Straight02_Wall;
	//坂系
	CModel mTile_Slope01_Floor;
	CModel mTile_Slope02_Floor;
	CModel mTile_Slope03_Floor;
	CModel mTile_Slope04_Floor;
	CModel mTile_Slope01_Wall;
	CModel mTile_Slope02_Wall;
	CModel mTile_Slope03_Wall;
	CModel mTile_Slope04_Wall;
	//でかめのタイル
	CModel mTile_Wide_Floor;
	CModel mTile_Wide_Wall;
	CModel mBlock_Floor;
	CModel mBlock_Wall;
	//コース5のモデル
	CModel mCource05Mountain;
	CModel mCource05Wall;
	CModel mCource05Road;
	CModel mCource05Lake;
	CModel mCource05Grass_Floor;
	CModel mCource05Grass_Wall;
	CModel mCource05GoalTile;
	//標識
	CModel mSign_Left;
	CModel mSign_Right;

	CObj *mpGrounds[GROUND_AMOUNT];
	CCharacter *mCarShadow[ENEMYS_AMOUNT + 1];
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
	//バックミラーの描画
	void RenderBackMirror();
	//影の描画
	void RenderShadow();
	//フェードインしてゲーム画面に突入する
	void FadeIn();
	//他シーンに行く際の演出・処理
	void SceneChange();
	
	//各コースのベストタイム
	static int mRecord_A, mRecord_B, mRecord_C, mRecord_D, mRecord_E, mRecord_F;

	FILE *outputfile;	
};
#endif
