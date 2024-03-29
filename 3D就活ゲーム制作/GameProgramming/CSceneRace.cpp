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

#include "glut.h"

//乱数を実装するインクルード群
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//スマートポインタの生成
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
extern CSound SoundFinalResult_GetTrophy;
extern CSound SoundFinalResult_NotGetTrophy;
extern CSound SoundApplause;

int CSceneRace::mBestTime = 0;//ここのmBestTimeの値は関係ない
int CSceneRace::mRecords[6] = { 0, 10000, 20000, 23000, 595959, 40000};//{ｴﾃﾞｨﾀ,A,B,C,D,E}
int CSceneRace::mTotalPoint_Enemys[ENEMYS_AMOUNT] = { 0 };

int CSceneRace::mTotalPoint = 0;
int CSceneRace::mCurrent_RaceNumber = 0;

//オプション画面から変更ができる変数
bool CSceneRace::isEnableShadow = true;//影
bool CSceneRace::isEnableMiniMap = true;//ミニマップ
bool CSceneRace::isEnableBackMirror = true;//バックミラー
bool CSceneRace::isEnableSpeedometer = true;//速度計

//画面サイズは800x600を想定
#define SCREENSIZE_X 800
#define SCREENSIZE_Y 600
//描画エリアの指定(左端のX座標,下端のY座標,幅,高さ)
#define MINIMAP_AREA 590,10,200,150
#define BACKMIRROR_FRAME_AREA 286,491,229,154
#define BACKMIRROR_VIEW_AREA 288,493,225,150
#define BACKMIRROR_EXTRAFRAME_AREA 286,598,228,5

#define TEXWIDTH (8192)
#define TEXHEIGHT (6144)

#define OPENINGTIME 5*60
#define WAITTIME_ENTER 4*60

//アルファテスト
#define USEALPHA 0

CRenderTexture mRenderTexture;

CSceneRace::~CSceneRace() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneRace::Init() {
	//オブジェクトの数の初期化
	CObj::mObjectNum = 0;
	
	for (int i = 0; i < CObj::mObject_Limit; i++){
		CObj::mpGrounds[i] = NULL;
	}

	//爆発テクスチャの読み込み
	TextureExp->Load("effect\\exp.tga");
	//衝突テクスチャの読み込み
	TextureHit->Load("effect\\Hit01_panop.tga");
	//加速テクスチャの読み込み
	TextureBoost->Load("effect\\boost01.tga");

	//テキストフォントの読み込みと設定
	CText::mFont.Load("font\\FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font\\font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	CText::mFont3.Load("font\\FontDIY.tga");
	CText::mFont3.SetRowCol(8, 176 / 11);

	//現在のコース数が初期化されていた場合はプレイヤー、ライバルのポイントを初期化
	if (mCurrent_RaceNumber == 0){
		mTotalPoint = 0;
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			mTotalPoint_Enemys[i] = 0;
		}		
	}
	mCurrent_RaceNumber++;//現コース数+1
	
	/*コース共通のマテリアルはCSceneRaceで読み込む*/
	//車の読み込み
	mCar.Load("material\\Car2.obj", "material\\single_color\\white.mtl");//プレイヤー

	mRover.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\white.mtl");//プレイヤー
	mCarRed.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\red.mtl");//以下敵の車
	mCarBlue.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\blue.mtl");
	mCarGreen.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\green.mtl");
	mCarYellow.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\yellow.mtl");
	mCarPink.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\pink.mtl");
	mCarCyan.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\cyan.mtl");
	mCarWhite.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\white.mtl");
	mCarBlack.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\black.mtl");
	mCarGray.Load("material\\common_mat\\Rover1.obj", "material\\single_color\\gray.mtl");
	mCheckPoint.Load("material\\common_mat\\cube.obj", "material\\single_color\\invisible.mtl");//中間地点(透明、ポリゴン1枚)
	//床タイルの読み込み
	mTileBlack.Load("material\\common_mat\\cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("material\\common_mat\\cube.obj", "material\\racing_mat\\tile_white.mtl");	
	//地面の読み込み
	mPlane.Load("material\\common_mat\\plane.obj", "material\\common_mat\\plane.mtl");
	mSign_Left.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnLeft.mtl");//標識:左折
	mSign_Right.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnRight.mtl");//標識:右折
	mGoalGate.Load("material\\racing_mat\\goalflag.obj", "material\\racing_mat\\goalflag.mtl");//ゴールゲート
	//ミニマップ上でのプレイヤー・敵のカーソル、ゴール地点の読み込み
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//プレイヤー
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//敵
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");//ゴールアイコン
	mPole.Load("material\\common_mat\\cube.obj", "material\\common_mat\\soil.mtl");//ポール(木製)
	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");//加速床
	mJumper01.Load("material\\common_mat\\cube.obj", "material\\common_mat\\on.mtl");//ジャンプ台
	
	//for (int i = 1; i <= COURCE_TOTAL; i++){
	//	if (CSceneTitle::mCource == i){
	//		//ベストタイムの読み込み
	//		mBestTime = mRecords[i];
	//		//BGMの読み込み
	//		/*if (i == 1)BGM.Load("BGM\\revolumed_PopsGuitar_No.01.wav");
	//		if (i == 2)BGM.Load("BGM\\revolumed_game_maoudamashii_1_battle34.wav");
	//		if (i == 3)BGM.Load("BGM\\revolumed_bgm_maoudamashii_neorock33.wav");
	//		if (i == 4)BGM.Load("BGM\\revolumed_Spring_Breeze.wav");
	//		if (i == 5)BGM.Load("BGM\\revolumed_Go_on_the_mountain_road.wav");*/
	//	}		
	//}

	//ジングル(短BGM)の読み込み
	JingleOpening.Load("SE\\jingle19.wav");
	//効果音の読み込み//主にレース中
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");
	//主にポーズ中
	SoundMoveCarsol.Load("SE\\Carsol2.wav");
	SoundDecide.Load("SE\\Decision_Small(SF).wav");
	SoundPauseOn.Load("SE\\button79.wav");
	SoundPauseOff.Load("SE\\button80.wav");
	SoundFinalResult_GetTrophy.Load("SE\\Onmtp-Flash06-1.wav");
	SoundFinalResult_NotGetTrophy.Load("SE\\Cyber21-1.wav");
	SoundApplause.Load("SE\\Cheers.wav");
	
	Texture_GoldTrophy.Load("texture\\trophy_gold.tga");
	Texture_SilverTrophy.Load("texture\\trophy_silver.tga");
	Texture_BronzeTrophy.Load("texture\\trophy_bronze.tga");

	//カメラ視点のY座標
	mCamY = 0.0f;
	//衝突判定の描画(デバッグ用)
	mPutCol = false;

	//一部テキストが点滅する時間
	mTextBlinkTime = 0;

	isOpening = true;
	mTime_Opening = 0;
	//レースのカウントダウン関連
	mFrame = 0;
	mCountDown = 4;//countdownは3秒から始まる
	//カウントダウン後にレース開始
	isStartRace = isGoal = false;
	//時間のリセット
	mTime = 0;
	mTime_Output = 0;
		
	//ラップ数の初期化
	mLap = 1;
	//n周でゴール(基本は3周)
	mMaxLap = 3;
	if (CSceneTitle::mCource == 5){
		mMaxLap = 2;//コース5は2周(1周が長い為)
	}
	//記録更新してない状態
	isNewRecord = false;
	
	//※最終順位の初期化
	mRanking = 1;
	//プレイヤーがゴール後の経過時間
	mAfterGoalTime = 0;
	
	//初期状態ではポーズ状態無効
	isPause = false;
	mPause_SelectCarsol = 1;
	mPause_OptionCarsol = 1;
	mPauseScreen = EPAUSE;

	//プレイヤー、ライバル車のスタート時の配置順を決める
	if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
		mStartPos = ESTARTPOS_RANDOM;
	}
	else if (CSceneTitle::mMode == CSceneTitle::EMODE_TIMEATTACK){
		mStartPos = ESTARTPOS_TOP;
	}

	isGoaledAll = false;
	isResult_FinalRace = false;
	mPushEnter_WaitTime = 0;
	canPushEnter = false;

	//乱数の初期化
	srand(time(NULL));

	//スタート位置をランダムで決める
	for (int i = 0; i < LIST_SIZE; i++) {
		list[i] = i;
	}
	//ランダムな順位からのスタート
	if (mStartPos == ESTARTPOS_RANDOM){
		for (int i = 0; i < LIST_SIZE; i++) {
			int j = rand() % LIST_SIZE;
			int t = list[i];
			list[i] = list[j];
			list[j] = t;
		}		
	}
	//プレイヤーが一番前の配置でスタート
	else if (mStartPos == ESTARTPOS_TOP){
		//list配列内の順番はそのまま0,1,2…
	}

	isFadeIn = true;
	isFadeOut = false;
	isBlackOutTime = 0;	

	mCameraAngle = EANGLE_THIRDPERSON;//カメラ位置…俯瞰視点
	
	mRenderTexture.Init();
	

	/* テクスチャユニット１をDepthテクスチャで使用 */
	glActiveTexture(GL_TEXTURE1);
	//テクスチャの生成
	glGenTextures(1, &mDepthTextureID);
	//使用するテクスチャのバインド
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);

	/* Depthテクスチャの割り当て */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXWIDTH, TEXHEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	/* テクスチャを拡大・縮小する方法の指定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* テクスチャの繰り返し方法の指定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* 書き込むポリゴンのテクスチャ座標値のＲとテクスチャとの比較を行うようにする */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	/* もしＲの値がテクスチャの値以下なら真（つまり日向） */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

#if USEALPHA
	/*比較の結果をアルファ値として得る*/
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
	/* アルファテストの比較関数（しきい値） */
	glAlphaFunc(GL_GEQUAL, 0.5f);
#else
	/*比較の結果を輝度値として得る*/
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
#endif

	/* テクスチャ座標に視点座標系における物体の座標値を用いる */
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	/* 生成したテクスチャ座標をそのまま (S, T, R, Q) に使う */
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

	//テクスチャの解除
	glBindTexture(GL_TEXTURE_2D, 0);


	//フレームバッファ追加
	//* フレームバッファオブジェクトを生成して結合する 
	glGenFramebuffersEXT(1, &mFb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFb);
	
	//* フレームバッファオブジェクトにデプスバッファ用のテクスチャを結合する 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT,
		GL_TEXTURE_2D, mDepthTextureID, 0);
	//* カラーバッファが無いので読み書きしない 
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//* フレームバッファオブジェクトの結合を解除する 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//テクスチャユニットを0に戻す
	glActiveTexture(GL_TEXTURE0);		
}


void CSceneRace::Update() {
	//タスクマネージャの更新・描画
	if (isPause == false){
		CTaskManager::Get()->Update();
	}

	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	if (mCameraAngle == EANGLE_FRONTCAR){
		e = mPlayer->mPosition + CVector(0.0f, 6.0f, 5.0f)* mPlayer->mMatrixScale* CMatrix().RotateY(mPlayer->mRotation.mY);;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 45.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
	}
	else if (mCameraAngle == EANGLE_THIRDPERSON){
		e = CCameraPos::mpCamera->mPosition;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 75.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
	}
	u = CVector(0.0f, 1.0f, 0.0f);//*mPlayer->mMatrixRotate;

	//カメラの設定
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	RenderShadow();//先に影を描画

#if USEALPHA
	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat lightdim[] = { 0.4f, 0.4f, 0.4f, 0.2f };
	const GLfloat lightblk[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	/* 光源の明るさを影の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdim);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightblk);
	
	//描画処理
	CTaskManager::Get()->Render();//タスク	

	/* 光源の明るさを影の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
#else
	//描画処理
	CTaskManager::Get()->Render();//タスク	
#endif

	

	//衝突処理
	CTaskManager::Get()->TaskCollision();
	//削除処理
	CTaskManager::Get()->Delete();

	//デバッグ用
#ifdef _DEBUG
	if (CKey::Once('2')){//Playerの座標を出力
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Push('5')){//Playerの回転値を出力
		printf("%f:%f:%f\n", CPlayer::mpPlayer->mRotation.mX, CPlayer::mpPlayer->mRotation.mY, CPlayer::mpPlayer->mRotation.mZ);
	}
	if (CKey::Once('6')){//Playerのチェックポイントの状態を出力
		printf("%d\n", CPlayer::mpPlayer->mChecks);
	}
	if (CKey::Once('7')){//Player飛行モード切り替え
		if (CPlayer::mpPlayer->mFlyingMode){
			CPlayer::mpPlayer->mFlyingMode = false;
		}
		else{
			CPlayer::mpPlayer->mFlyingMode = true;
		}
	}//当たり判定描画の切り替え
	if (CKey::Once('9')){
		if (mPutCol){
			mPutCol = false;
		}
		else{
			mPutCol = true;
		}
	}
	if (mPutCol){
		//衝突判定の描画
		CollisionManager.Render();
	}
	//Playerを即時トップスピードに
	if (CKey::Once('G')){
		mPlayer->mCarSpeed = 20.0f;
	}
	//チェックポイントを通過したことにする
	if (CKey::Once('V')){
		if (mPlayer->mChecks < 3){
			mPlayer->mChecks++;
		}
	}
	if (CKey::Once('8')){
		printf("オブジェクトの数：%d\n", CObj::mObjectNum);		
	}
	if (CKey::Once('0')){
		BGM.Stop();
	}
#endif	

	//ポーズ画面に入っていない時
	if (isPause == false){
		if (isStartRace || isGoal){
			//mTime:ゴール後も内部的にタイマーは作動し続ける(ゲーム画面には表示されない)
			//59:59:59が最大時間
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
			if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
				for (int i = 0; i < ENEMYS_AMOUNT; i++){
					if (mEnemys[i]->isEnemyGoaled == false){
						mEnemys[i]->mPointTime++;
					}
				}
			}			
		}
		if (isStartRace){
			//mTime_Output:ゲーム画面に表示されるタイム(ゴール後にタイマー停止)
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
	//レース前のオープニング
	if (isOpening){
		//OPジングル再生
		if (mTime_Opening == 0)
			JingleOpening.Play();
		mTime_Opening++;
		if (mTime_Opening > OPENINGTIME)
			isOpening = false;
	}
	//カウントダウン開始、「GO!」で操作の受付開始
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

	//待ち時間の消化
	if (mPushEnter_WaitTime > 0){
		mPushEnter_WaitTime--;
	}

	//カウント0で全員走行可能
	if (mCountDown == 0){
		CPlayer::mpPlayer->CanMove = true;

		if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
			for (int i = 0; i < ENEMYS_AMOUNT; i++){
				mEnemys[i]->CanMove = true;
			}
		}		
	}

	//バックミラーの描画
	if (isEnableBackMirror){
		RenderBackMirror();
	}
	//ミニマップの描画
	if (isEnableMiniMap){
		RenderMiniMap();
	}
	//テキスト、フェードイン等の描画
	Render();
	
	//プレイヤーがゴール地点を通過した時の処理
	if ((CPlayer::mpPlayer->isTouchGoal)
		&& (CPlayer::mpPlayer->mChecks == 3)
		&& (isStartRace)){
		if (mLap == mMaxLap){
			
			mPlayer->mRank = mRanking;

			if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
				mTotalPoint += (ENEMYS_AMOUNT + 1 - mRanking);
				if (mRanking >= ENEMYS_AMOUNT + 1){
					isGoaledAll = true;
				}
			}
			//TIMEATTACK MODEの時のみベストタイムが記録される
			else if (CSceneTitle::mMode == CSceneTitle::EMODE_TIMEATTACK){
				//ベストタイム更新時
				if (mTime < mBestTime){
					mBestTime = mTime;
					isNewRecord = true;
					//そのコースの記録を更新する
					for (int i = 1; i <= COURCE_TOTAL; i++){
						if (CSceneTitle::mCource == i){
							mRecords[i] = mBestTime;
						}
					}
				}
			}			

			mRanking++;
			isStartRace = false;
			isGoal = true;
			BGM.Stop();
			SoundGoal.Play();
			//CPlayer::mpPlayer->CanMove = false;//動きストップ
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

	if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
		//CPUの車がゴール地点を通過した時の処理
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			if (mEnemys[i]->isTouchGoal
				&& (mEnemys[i]->mChecks == 3)
				&& (mEnemys[i]->isEnemyGoaled == false)){
				//その敵が最終ラップだった場合
				if (mEnemys[i]->mEnemyLap == mMaxLap){
					mTotalPoint_Enemys[i] += (ENEMYS_AMOUNT + 1 - mRanking);					
					mEnemys[i]->mRank = mRanking;
					if (mRanking >= ENEMYS_AMOUNT + 1){
						isGoaledAll = true;
					}
					mRanking++;
					mEnemys[i]->isTouchGoal = false;
					mEnemys[i]->isEnemyGoaled = true;
					mEnemys[i]->mGoalTime = mTime;					
				}
				//まだ最終ラップでない場合
				else{
					//チェックポイントをリセットし、次の周スタート
					mEnemys[i]->mChecks = 0;
					mEnemys[i]->isTouchGoal = false;
					mEnemys[i]->mEnemyLap++;
				}
			}
		}
	}	

	//カメラアングルの変更
	if (CKey::Once('C')){
		switch (mCameraAngle){
		case EANGLE_FRONTCAR:
			mCameraAngle = EANGLE_THIRDPERSON;
			break;
		case EANGLE_THIRDPERSON:
			mCameraAngle = EANGLE_FRONTCAR;
			break;
		default:
			printf("視点変更に失敗しました\n");
			break;
		}
	}

	if (CKey::Once('P')){
		//カウントダウン終了後、ポーズの切り替えが可能になる。
		if (mCountDown < 0){
			//ゴール後は切り替え不可
			if (isGoal)return;

			if (isPause&&mPauseScreen == EOPTION)return;

			//「タイトルに戻る」選択後はカーソルを動かせない
			if (isFadeOut)return;

			//ポーズのON・OFF切り替え
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

	/*シーン切り替え系の処理*/
	//ゴール後Enterキー押下→タイトル画面移行
	if (isGoal){
		if (CKey::Once(VK_RETURN) && canPushEnter  && mPushEnter_WaitTime <= 0){
			SoundDecide.Play();			
			if (isGoaledAll == false && CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
				//まだ全ライバルがゴールしてない
			}
			else if (mCurrent_RaceNumber == RACES_PER_1CUP && isResult_FinalRace == false){
				mPushEnter_WaitTime = WAITTIME_ENTER;
				isResult_FinalRace = true;
			}			
			else{
				isFadeOut = true;
				isBlackOutTime = 0;
				canPushEnter = false;
			}

			//まだゴールしてない敵の順位をランダムに決める
			if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
				//まだ全員ゴールしてない時
				if (isGoaledAll == false){
					int ranks[ENEMYS_AMOUNT];
					//スタート位置をランダムで決める
					for (int i = 0; i < ENEMYS_AMOUNT; i++) {
						ranks[i] = i;
					}
					//ランダムな順位からのスタート
					if (mStartPos == ESTARTPOS_RANDOM){
						for (int i = 0; i < ENEMYS_AMOUNT; i++) {
							int j = rand() % ENEMYS_AMOUNT;
							int t = ranks[i];
							ranks[i] = ranks[j];
							ranks[j] = t;
						}
					}
					for (int i = 0; i < ENEMYS_AMOUNT; i++){
						if (mEnemys[ranks[i]]->isEnemyGoaled == false){
							mTotalPoint_Enemys[ranks[i]] += (ENEMYS_AMOUNT + 1 - mRanking);							
							mEnemys[ranks[i]]->mRank = mRanking;
							mRanking++;
							mEnemys[ranks[i]]->isTouchGoal = false;
							mEnemys[ranks[i]]->isEnemyGoaled = true;
							mEnemys[ranks[i]]->mGoalTime = mTime;
						}
					}
					isGoaledAll = true;
				}
			}			
		}
	}
	//ポーズ中Escキー押下→タイトル画面移行
	if (isPause){
		//「タイトルに戻る」選択して暗転が始まるとカーソルを動かせない
		if (isFadeOut)return;
			
		//ポーズの中で、設定画面を開いている時
		if (mPauseScreen == EOPTION){
			//↑キー
			if (CKey::Once(VK_UP)){
				if (mPause_OptionCarsol > 1){
					mPause_OptionCarsol--;
					SoundMoveCarsol.Play();
				}
			}
			//↓キー
			if (CKey::Once(VK_DOWN)){
				if (mPause_OptionCarsol < 5){
					mPause_OptionCarsol++;
					SoundMoveCarsol.Play();
				}
			}
			//Enterキー
			if (CKey::Once(VK_RETURN)){
				//影の描画のON・OFF
				if (mPause_OptionCarsol == 1){
					isEnableShadow = !isEnableShadow;
				}
				//バックミラー表示のON・OFF
				if (mPause_OptionCarsol == 2){
					isEnableBackMirror = !isEnableBackMirror;
				}
				//ミニマップ表示のON・OFF
				if (mPause_OptionCarsol == 3){
					isEnableMiniMap = !isEnableMiniMap;
				}
				////スピードメーター表示のON・OFF
				if (mPause_OptionCarsol == 4){
					isEnableSpeedometer = !isEnableSpeedometer;
				}
				//設定画面を閉じる
				if (mPause_OptionCarsol == 5){
					mPauseScreen = EPAUSE;
				}
				SoundDecide.Play();
			}
		}
		else{
			//↑キー
			if (CKey::Once(VK_UP)){
				if (mPause_SelectCarsol > 1){
					mPause_SelectCarsol--;
					SoundMoveCarsol.Play();
				}
			}
			//↓キー
			if (CKey::Once(VK_DOWN)){
				if (mPause_SelectCarsol < 3){
					mPause_SelectCarsol++;
					SoundMoveCarsol.Play();
				}
			}
			//Enterキー
			if (CKey::Once(VK_RETURN)){
				if (mPause_SelectCarsol == 1){
					//ポーズ解除
					isPause = !isPause;
					mPause_SelectCarsol = 1;
					SoundPauseOff.Play();
				}
				else if (mPause_SelectCarsol == 2){
					//オプション画面表示
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
	//2D描画開始
	Start2D(0, 800, 0, 600);

	//順位の描画
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//レース開始前のOP中にコース名を左下に表示
	if (isOpening){
		char race_status[16];//ラップ数
		sprintf(race_status, "COURCE%d  %d/%d", CSceneTitle::mCource, mCurrent_RaceNumber, RACES_PER_1CUP);
		CText::DrawString(race_status, 20, 20, 10, 12, 2);
	}

	//集中線の描画
	//2D座標の原点(0,0)を画面中央へ移動
	glTranslated(SCREENSIZE_X / 2, SCREENSIZE_Y / 2, 0.0f);
	color[0] = color[1] = color[2] = 1.0f;
	color[3] = (mPlayer->mCarSpeed - 20.0f) * 0.1f;//プレイヤーの非ブースト時の最高速度
	if (color[3] > 0.3f){
		color[3] = 0.3f;
	}
	glColor4fv(color);
	//ランダムに各線の角度、長さを決める
	for (int i = 0; i < 90; i++){
		int degree = rand() % 360;
		glRotated(degree, 0.0f, 0.0f, 1.0f);
		CRectangle::Render(0, 400, 1, 100 + rand() % 50);
		glRotated(-degree, 0.0f, 0.0f, 1.0f);
	}
	//ずらした座標分とカラーを元に戻す
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	glTranslated(-SCREENSIZE_X / 2, -SCREENSIZE_Y / 2, 0.0f);
		

	//スピードメーターの描画
	//原点(0,0)をずらす
	glTranslated(715, 520, 0.0f);
	//複数の描画に分けてメーターを表現
	for (int ii = 0; ii < 4; ii++){
		color[3] = 1.0f;//プレイヤーの非ブースト時の最高速度	
		if (ii % 2 == 1){
			color[0] = color[1] = color[2] = 0.0f;
		}
		else{
			color[0] = color[1] = color[2] = 1.0f;			
		}
		glColor4fv(color);

		int circle_size = 0;
		if (ii == 0){
			circle_size = 32;
		}
		else if (ii == 1){
			circle_size = 30;
		}
		else if (ii == 2){
			circle_size = 16;
		}
		else if (ii == 3){
			circle_size = 15;
		}

		for (int i = 0; i < 90; i++){
			int degree = i * 4;
			glRotated(degree, 0.0f, 0.0f, 1.0f);
			CRectangle::Render(0, circle_size, 5, circle_size);
			glRotated(-degree, 0.0f, 0.0f, 1.0f);
		}

		if(ii == 1){
			color[0] = color[1] = color[2] = 1.0f;
			glColor4fv(color);
			int degree = 200;
			glRotated(degree, 0.0f, 0.0f, 1.0f);
			CRectangle::Render(0, 58, 2, 6);
		//	CText::DrawString("0", 0, 48, 7, 9, 2);
			glRotated(-degree, 0.0f, 0.0f, 1.0f);

			degree = 0;
			glRotated(degree, 0.0f, 0.0f, 1.0f);
			CRectangle::Render(0, 58, 2, 6);
		//	CText::DrawString("20", 0, 48, 7, 9, 2, -3);
			glRotated(-degree, 0.0f, 0.0f, 1.0f);

			degree = 100;
			glRotated(degree, 0.0f, 0.0f, 1.0f);
			CRectangle::Render(0, 58, 2, 6);
		//	CText::DrawString("10", 0, 48, 7, 9, 2, -3);
			glRotated(-degree, 0.0f, 0.0f, 1.0f);

			//degree = -90;
			//glRotated(degree, 0.0f, 0.0f, 1.0f);
			//CRectangle::Render(0, 46, 2, 14);
			////CText::DrawString("10", 0, 48, 7, 9, 2);
			//glRotated(-degree, 0.0f, 0.0f, 1.0f);
			for (int i = 0; i < 30; i++){
				degree = -90-i;
				glRotated(degree, 0.0f, 0.0f, 1.0f);
				CRectangle::Render(0, 46, 1, 15);
				//CText::DrawString("10", 0, 48, 7, 9, 2);
				glRotated(-degree, 0.0f, 0.0f, 1.0f);
			}

			for (int i = 0; i < 90; i++){
				degree = 0 - i;
				color[0] = 0.0f + i*0.01f;
				color[1] = color[2] = 0.0f;
				glColor4fv(color);
				glRotated(degree, 0.0f, 0.0f, 1.0f);
				CRectangle::Render(0, 46, 1, 15);
				//CText::DrawString("10", 0, 48, 7, 9, 2);
				glRotated(-degree, 0.0f, 0.0f, 1.0f);
			}

			for (int i = 0; i < 4; i++){
				color[0] = color[1] = color[2] = 0.7f;
				glColor4fv(color);
				degree = i*20+20;
				glRotated(degree, 0.0f, 0.0f, 1.0f);
				CRectangle::Render(0, 57, 1, 3);
				//CText::DrawString("10", 0, 48, 7, 9, 2);
				glRotated(-degree, 0.0f, 0.0f, 1.0f);
			}
			for (int i = 0; i < 4; i++){
				color[0] = color[1] = color[2] = 0.7f;
				glColor4fv(color);
				degree = i * 20 + 20 + 100;
				glRotated(degree, 0.0f, 0.0f, 1.0f);
				CRectangle::Render(0, 57, 1, 3);
				//CText::DrawString("10", 0, 48, 7, 9, 2);
				glRotated(-degree, 0.0f, 0.0f, 1.0f);
			}
			for (int i = 0; i < 4; i++){
				color[0] = color[1] = color[2] = 0.7f;
				glColor4fv(color);
				degree = i * 20 + 20 - 100;
				glRotated(degree, 0.0f, 0.0f, 1.0f);
				CRectangle::Render(0, 57, 1, 3);
				//CText::DrawString("10", 0, 48, 7, 9, 2);
				glRotated(-degree, 0.0f, 0.0f, 1.0f);
			}
			for (int i = 0; i < 1; i++){
				color[0] = color[1] = color[2] = 0.7f;
				glColor4fv(color);
				degree = i * 20 + 20 + 200;
				glRotated(degree, 0.0f, 0.0f, 1.0f);
				CRectangle::Render(0, 57, 1, 3);
				//CText::DrawString("10", 0, 48, 7, 9, 2);
				glRotated(-degree, 0.0f, 0.0f, 1.0f);
			}
		}

	}
	color[0] = 1.0f;
	color[1] = color[2] = 0.0f;
	glColor4fv(color);
	//メーターの線
	float degree = mPlayer->mCarSpeed * 10.0f - 200;
	//メーターの線の可動域を決める
	if (degree > 90.0f){
		degree = 90.0f;
	}
	if (degree < -240.0f){
		degree = -240.0f;
	}
	glRotated(-degree, 0.0f, 0.0f, 1.0f);
	//CRectangle::Render(0, 30, 1, 30);
	CRectangle::Render(0, 46, 1, 14);
	glRotated(degree, 0.0f, 0.0f, 1.0f);
	//ずらした座標分とカラーを元に戻す
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	glTranslated(-715, -520, 0.0f);
	
	//プレイヤーの車の速度表示
	char carspeed[8];
	sprintf(carspeed, "%4.1f", CPlayer::mpPlayer->mCarSpeed);
	if (isEnableSpeedometer){
		//CText::DrawString(carspeed, 20 + 580, 20, 10, 12);
		CText::DrawString(carspeed, 697, 520, 7, 9,2,-1);

		CText::DrawString("0", 715+17, 528-50-3, 7, 9, 2, -3);
		CText::DrawString("10", 715-49, 528-16, 7, 9, 2, -3);
		CText::DrawString("20", 715-5, 528+50-10, 7, 9, 2, -3);
	}
	////プレイヤーの車の速度表示
	//char carspeed[33];
	//sprintf(carspeed, "SPEED:%4.1f", CPlayer::mpPlayer->mCarSpeed);
	//if (isEnableSpeedometer){
	//	//CText::DrawString(carspeed, 20 + 580, 20, 10, 12);
	//}
	
	char besttime[20];//ベストタイム
	sprintf(besttime, "BEST:%02d:%02d:%02d", mBestTime / 10000 % 100, mBestTime / 100 % 100, mBestTime % 100);
	char time[20];//経過時間
	sprintf(time, "TIME:%02d:%02d:%02d", mTime_Output / 10000 % 100, mTime_Output / 100 % 100, mTime_Output % 100);
	char lap[19];//ラップ数
	sprintf(lap, "LAP%d/%d", mLap, mMaxLap);
	if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){		
		CText::DrawString(time, 20, 580, 10, 12,2);		
		CText::DrawString(lap, 20, 530, 10, 12, 2);
	}
	else if (CSceneTitle::mMode == CSceneTitle::EMODE_TIMEATTACK){
		CText::DrawString(besttime, 20, 580, 10, 12,2);
		CText::DrawString(time, 20, 530, 10, 12,2);
		CText::DrawString(lap, 20, 500, 10, 12, 2);
	}

	//カウントダウンの表示
	char mcountd[7];
	//残り3秒までの間、表示
	sprintf(mcountd, "%d", mCountDown);
	if (mCountDown > 0 && mCountDown <= 3){
		CText::DrawString(mcountd, 400, 300, 25, 30);
	}
	else if (mCountDown == 0){
		CText::DrawString("GO!", 400 - 40, 300, 25, 30);
	}
	
	
	
	//ゴール後、継続して実行する処理
	if (mLap == mMaxLap && isStartRace == false && isGoal){
		//60fで一巡
		if (mTextBlinkTime < 60){
			mTextBlinkTime++;
		}
		else{
			mTextBlinkTime = 0;
		}
		//Enterキー入力でタイトル画面や次のレースへの移行などを伝えるテキスト
		if (mTextBlinkTime < 30){
			if (mPushEnter_WaitTime <= 0 && canPushEnter){
				//状況に合わせてテキストの表示内容が変化する
				if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
					if (isGoaledAll == false){
						CText::DrawString("Press Enter to Skip Result", 147, 77, 10, 12, 2);
					}
					else if (mCurrent_RaceNumber == RACES_PER_1CUP){
						if (isResult_FinalRace){
							CText::DrawString("Press Enter to Title", 222, 77, 10, 12, 2);
						}
						else{
							CText::DrawString("Press Enter to Final Result", 137, 77, 10, 12, 2);
						}
					}
					else{
						CText::DrawString("Press Enter to Next Race", 159, 77, 10, 12, 2);
					}
				}
				else if (CSceneTitle::mMode == CSceneTitle::EMODE_TIMEATTACK){
					CText::DrawString("Press Enter to Title", 222, 77, 10, 12, 2);
				}				
			}			
		}
		//記録を更新した時
		if (isNewRecord){
			//CText::DrawString("FINISH!", 400 - 20 * 6, 300, 20, 24);
			if (mTextBlinkTime < 15 || mTextBlinkTime >= 30 && mTextBlinkTime < 45){
				CText::DrawString("NEW RECORD!", 55, 551, 8, 9, 2);
			}
		}
	}

	//ゴール後に表示される文字
	if (isGoal){

		if (isResult_FinalRace){
			//並び変えを行う為のリストを作成する
			int spoints[LIST_SIZE];//得点
			int jun[LIST_SIZE];//順位
			int carnumber[LIST_SIZE];//番号
			for (int i = 0; i < LIST_SIZE; i++){
				if (i == 0){
					spoints[i] = mTotalPoint;
				}
				else{
					spoints[i] = mTotalPoint_Enemys[i - 1];
				}
				jun[i] = carnumber[i] = i + 1;//no.1:プレイヤー 以降ライバル
			}
			
			char list_info[32];
			//合計点の降順に並び変え
			for (int i = 0; i < LIST_SIZE; i++) {				
				for (int j = i + 1; j < LIST_SIZE; j++){
					if (spoints[i] < spoints[j]) {
						int tmp;
						tmp = spoints[i];
						spoints[i] = spoints[j];
						spoints[j] = tmp;

						tmp = carnumber[i];
						carnumber[i] = carnumber[j];
						carnumber[j] = tmp;
					}
				}
			}
			int rank = 1;
			for (int i = 0; i < LIST_SIZE; i++){				
				if (carnumber[i] == 1){
					rank = jun[i];
				}
			}
						
			char result[8];
			char trophy_name[32];
			CText::DrawString("Final Result", 280, 420, 12, 15, 2);

			if (mPushEnter_WaitTime == WAITTIME_ENTER - 60){
				if (rank <= 3){
					SoundFinalResult_GetTrophy.Play();
				}
				else{
					SoundFinalResult_NotGetTrophy.Play();
				}
			}
			if (mPushEnter_WaitTime < WAITTIME_ENTER - 60){
				if (rank == 1){      //1st
					sprintf(result, "%dst", rank);
					CRectangle::RenderImage(Texture_GoldTrophy, 400, 300, 100, 100, 555, 555);
				}
				else if (rank == 2){ //2nd
					sprintf(result, "%dnd", rank);
					CRectangle::RenderImage(Texture_SilverTrophy, 400, 300, 100, 100, 555, 555);
				}
				else if (rank == 3){ //3rd
					sprintf(result, "%drd", rank);
					CRectangle::RenderImage(Texture_BronzeTrophy, 400, 300, 100, 100, 555, 555);
				}
				else{ //4th,5th,...
					sprintf(result, "%dth", rank);
				}
				CText::DrawString(result, 357, 337, 21, 25, 2);
			}
			if (mPushEnter_WaitTime == WAITTIME_ENTER - 120){
				if (rank <= 3)SoundApplause.Play();//3位以内入賞で拍手
			}
			if (mPushEnter_WaitTime < WAITTIME_ENTER - 120){
				//順位に応じて激励の言葉をかける
				if (rank == 1){      //1st					
					CText::DrawString("Congratulations!", 175, 195, 14, 17, 2);
					sprintf(trophy_name, "You won the %s Trophy!", "Gold");
					CText::DrawString(trophy_name, 70, 160, 14, 17, 2);
				}
				else if (rank == 2){ //2nd
					CText::DrawString("Congratulations!", 175, 195, 14, 17, 2);
					sprintf(trophy_name, "You won the %s Trophy!", "Silver");
					CText::DrawString(trophy_name, 64, 132, 14, 17, 2);
				}
				else if (rank == 3){ //3rd
					CText::DrawString("Congratulations!", 175, 195, 14, 17, 2);
					sprintf(trophy_name, "You won the %s Trophy!", "Bronze");
					CText::DrawString(trophy_name, 64, 132, 14, 17, 2);
				}
				else{ //4th,5th,...
					CText::DrawString("Nice Try!", 300, 180, 14, 17, 2);
				}
				//一位から順に総得点を表示する
				color[0] = color[1] = color[2] = 0.0f;
				glColor4fv(color);
				CRectangle::Render(610, 310, 108, 86);
				color[0] = color[1] = color[2] = 1.0f;
				glColor4fv(color);
				CRectangle::Render(610, 310, 107, 85);
				for (int i = 0; i < LIST_SIZE; i++){
					char name[6];
					if (carnumber[i] == 1){
						sprintf(name, "YOU");
					}
					else{
						sprintf(name, "CPU%d", carnumber[i] - 1);
					}
					sprintf(list_info, "%d %-4s %2dpt", jun[i], name, spoints[i]);
					CText::DrawString(list_info, 530, 375 - 26 * i, 8, 10, 2);
				}
			}
			//過去最高となる順位の更新
			if (CSceneTitle::RecordHigh_Ranking > rank)
				CSceneTitle::RecordHigh_Ranking = rank;

		}
		else{
			//GPモード時
			if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
				if (mAfterGoalTime < 999){
					mAfterGoalTime++;
				}
				if (mAfterGoalTime > 85){
					color[3] = 1.0f - 0.1f * (mAfterGoalTime - 85);
					glColor4fv(color);
				}
				if (mAfterGoalTime == 100){
					canPushEnter = true;
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

				//ゴールした車の記録を表示
				char goaltime[30];
				char name[8];
				sprintf(goaltime, "%d YOU  %02d:%02d:%02d %2dpt", mPlayer->mRank, mPlayer->mGoalTime / 10000 % 100, mPlayer->mGoalTime / 100 % 100, mPlayer->mGoalTime % 100, mTotalPoint);
				if (isGoal){
					CText::DrawString(goaltime, 215, 285 - mPlayer->mRank * 17, 10, 12, 2);
				}
				for (int i = 0; i < ENEMYS_AMOUNT; i++){
					sprintf(goaltime, "%d      %02d:%02d:%02d %2dpt", mEnemys[i]->mRank, mEnemys[i]->mGoalTime / 10000 % 100, mEnemys[i]->mGoalTime / 100 % 100, mEnemys[i]->mGoalTime % 100, mTotalPoint_Enemys[i]);
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
						CText::DrawString(name, 255, 285 - mEnemys[i]->mRank * 17, 10, 12, 2);
						//色を一旦、白に戻す
						color[0] = color[1] = color[2] = 1.0f;
						glColor4fv(color);
						CText::DrawString(goaltime, 215, 285 - mEnemys[i]->mRank * 17, 10, 12, 2);
					}
				}
			}
			//TAモード時
			else if (CSceneTitle::mMode == CSceneTitle::EMODE_TIMEATTACK){
				if (mAfterGoalTime < 999){
					mAfterGoalTime++;
				}
				if (mAfterGoalTime > 85){
					color[3] = 1.0f - 0.1f * (mAfterGoalTime - 85);
					glColor4fv(color);
				}
				if (mAfterGoalTime == 120){
					canPushEnter = true;
				}
				CText::DrawString("FINISH!", 400 - 25 * 6, 300, 25, 30);

				color[3] = 0.05f * (mAfterGoalTime - 100);
				glColor4fv(color);
				//ゴールした車の記録を表示
				char goaltime[30];
				sprintf(goaltime, "Record  %02d:%02d:%02d", mPlayer->mGoalTime / 10000 % 100, mPlayer->mGoalTime / 100 % 100, mPlayer->mGoalTime % 100);
				if (isGoal){
					CText::DrawString(goaltime, 270 - 55, 285 - mPlayer->mRank * 17, 13, 16, 2);
				}
			}
		}

	}
	//色合いを元に戻す
	color[3] = 1.0f;
	glColor4fv(color);

	//ポーズ中に表示される文字
	if (isPause){

		if (mPauseScreen == EOPTION){
			//表示場所が変化しないテキスト
			CText::DrawString("OPTION", 250, 300, 10 * 3, 12 * 3, 3);
			CText::DrawString("Shadow", 260, 200, 10, 12, 2);
			CText::DrawString("BackMirror", 260, 170, 10, 12, 2);
			CText::DrawString("MiniMap", 260, 140, 10, 12, 2);
			CText::DrawString("Speedometer", 260, 110, 10, 12, 2);

			//表示場所が変化するテキスト
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
			//カーソルの位置
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
	//フェードイン
	if (isFadeIn){
		FadeIn();
	}
	//フェードイン
	if (isFadeOut){
		SceneChange();
	}
	//2D描画終了
	End2D();
}

/* マップ上からの視点 */
void CSceneRace::RenderMiniMap() {	
	glPushMatrix();
	glLoadIdentity();
	//一時的に2D視点に変更する
	glViewport(MINIMAP_AREA);//画面の描画エリアの指定
	glMatrixMode(GL_PROJECTION);//行列をプロジェクションモードへ変更
	//行列退避
	glPushMatrix();
	glLoadIdentity();			//行列を初期化
	//2D画面の設定変更
	float size = 20000.0f;//コースの縮尺設定
	bool canscrollmap = false;//プレイヤーに合わせたマップ移動の有無
	if (CSceneTitle::mCource == 1){
		size = 3600.0f;
	}
	else if (CSceneTitle::mCource == 2){
		size = 5600.0f;
	}
	else if (CSceneTitle::mCource == 3){
		size = 7600.0f;
	}
	else if (CSceneTitle::mCource == 4){
		size = 3600.0f;
	}
	else if (CSceneTitle::mCource == 5){
		size = 5500.0f;
		canscrollmap = true;
	}
	//画面比率は800x600→4:3→1.33:1
	float ratio = 1.33f;
	if (canscrollmap){
		glOrtho(-size*ratio + mPlayer->mPosition.mX, size*ratio + mPlayer->mPosition.mX, -size - mPlayer->mPosition.mZ, size - mPlayer->mPosition.mZ, -size, size);//glOrtho(左、右、下、上、手前、奥)
	}
	else{
		glOrtho(-size*ratio, size*ratio, -size, size, -size, size);//glOrtho(左、右、下、上、手前、奥)
	}	
	glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
	glLoadIdentity();				//行列を初期化
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	//X-Z平面をX-Y平面へ
		
	glDisable(GL_DEPTH_TEST);
	CTaskManager::Get()->Render();
	
	if (CSceneTitle::mCource == 2){
		//ミニマップにゴールアイコンを描画
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 1.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(0)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(2211.0f, 0.0f, -2300.0f);
			//* CMatrix().Translate(-3200.0f, 0.0f, 341.7f);
		mMiniGoal.Render(matminig);
		//ミニマップ状に敵の位置を示すカーソルを描画
		if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
			CMatrix matenemys[ENEMYS_AMOUNT];
			for (int i = 0; i < ENEMYS_AMOUNT; i++){
				matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
					* CMatrix().RotateX(0)
					* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
					* CMatrix().RotateZ(0)
					* mEnemys[i]->mMatrixTranslate;
				mCarsol_Enemy.Render(matenemys[i]);
			}
		}		
		//ミニマップ状にプレイヤーの位置を示すカーソルを描画
		CMatrix matplayer;
		matplayer = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mPlayer->mMatrixTranslate;
		mCarsol.Render(matplayer);
	}
	else if (CSceneTitle::mCource == 5){
		//ミニマップにゴールアイコンを描画
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 100.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(-145.3)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(-3862.5f, 30.0f, 15925.0f);
		mMiniGoal.Render(matminig);
		//ミニマップ状に敵の位置を示すカーソルを描画
		if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
			CMatrix matenemys[ENEMYS_AMOUNT];
			for (int i = 0; i < ENEMYS_AMOUNT; i++){
				matenemys[i] = CMatrix().Scale(45.0f, 1.0f, 45.0f) //* mPlayer->mMatrixScale
					* CMatrix().RotateX(0)
					* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
					* CMatrix().RotateZ(0)
					* mEnemys[i]->mMatrixTranslate;
				mCarsol_Enemy.Render(matenemys[i]);
			}
		}
		
		//ミニマップ状にプレイヤーを示すカーソルを描画
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
		//ミニマップにゴールアイコンを描画
		CMatrix matminig;
		matminig = CMatrix().Scale(20.0f, 1.0f, 20.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(0)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(550.0f, 0.0f, -10.0f);
		mMiniGoal.Render(matminig);
				
		//ミニマップ状に敵の位置を示すカーソルを描画
		if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
			CMatrix matenemys[ENEMYS_AMOUNT];
			for (int i = 0; i < ENEMYS_AMOUNT; i++){
				matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
					* CMatrix().RotateX(0)
					* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
					* CMatrix().RotateZ(0)
					* mEnemys[i]->mMatrixTranslate;
				mCarsol_Enemy.Render(matenemys[i]);
			}
		}

		//ミニマップ状にプレイヤーを示すカーソルを描画
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
	glViewport(0, 0, 800, 600); //画面の描画エリアの指定
	glEnable(GL_DEPTH_TEST);

	//3D視点に戻す
	glViewport(0, 0, 800, 600);	//画面の描画エリアの指定
	glMatrixMode(GL_PROJECTION);	//行列をプロジェクションモードへ変更
	glLoadIdentity();				//行列を初期化
	//gluOrtho2D(-200 / 2, 200 / 2, -150 / 2, 150 / 2);	//2Dの画面を設定
	gluPerspective(75.0, 800.0 / 600.0, 1.0, 10000.0);	//3Dの画面を設定
	glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
	glLoadIdentity();
}
//バックミラーを表示する
void CSceneRace::RenderBackMirror()
{
	//行列の退避
	glPushMatrix();
	glLoadIdentity();
	//カメラの設定
	CVector e, c, u;//視点、注視点、上方向
	e = CVector(0.0f, 17.0f + 13.0f, 35.0f - 36.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		//* mPlayer->mMatrixRotate
		* mPlayer->mMatrixTranslate;
	c = mPlayer->mPosition + CVector(0.0f, 17.0f + 13.0f, 40.0f - 42.0f)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//* mPlayer->mMatrixRotate;
	u = CVector(0.0f, 1.0f, 0.0f);
	//バックミラーのカメラの設定
	gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	
	//

	//レンダーテクスチャ開始
	mRenderTexture.Start();

	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat lightdim[] = { 0.4f, 0.4f, 0.4f, 0.2f };
	const GLfloat lightblk[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//バックミラーの描画
	if (isEnableShadow){
		RenderShadowBM();
	}
#if USEALPHA
	//オブジェクトの描画
	/* 光源の明るさを影の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdim);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightblk);
	CTaskManager::Get()->Render();
	/* 光源の明るさを影の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
#else
	CTaskManager::Get()->Render();
#endif
	
	//レンダーテクスチャ終了
	mRenderTexture.End();

	//バックミラーのフレーム部分の範囲	
	glViewport(BACKMIRROR_FRAME_AREA);
	//2D描画開始
	Start2D(-1, 1, -1, 1);
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glColor4fv(color);
	//上記の2D描画範囲の指定値より大きめに白背景を描画する
	int expand = 100; color[3] = 0.0f;
	//白背景のよりも先に黒枠となるものを描画する
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
	//2D描画終了
	End2D();

	//バックミラーに映る景色を描画エリアの指定
	glViewport(BACKMIRROR_VIEW_AREA);
	//2D描画開始
	Start2D(-1, 1, -1, 1);	
	// テクスチャマッピングを有効にする
	glEnable(GL_TEXTURE_2D);
	//レンダーテクスチャのテクスチャをバインドする
	glBindTexture(GL_TEXTURE_2D, mRenderTexture.GetTexture());	
	// 正方形を描く
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
	//2D描画終了
	End2D();

	//見切れた黒枠の部分を補完する
	glViewport(BACKMIRROR_EXTRAFRAME_AREA);
	//2D描画開始
	Start2D(0, SCREENSIZE_X, 0, SCREENSIZE_Y);
	color[0] = color[1] = color[2] = 0.0f;
	glColor4fv(color);
	//上記の2D描画範囲の指定値より大きめに白背景を描画する
	expand = 100;
	//見切れた黒枠の部分を描画する
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
	//2D描画終了
	End2D();

	// テクスチャマッピングを無効にする
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//画面の描画エリアをメインの画面に戻す	glViewport
	glViewport(0, 0, SCREENSIZE_X, SCREENSIZE_Y); //画面の描画エリアをメインの画面に戻す

	//行列を戻す
	glPopMatrix();
}

//影の描画
void CSceneRace::RenderShadow(){
	//Shadow Map ************************************

	//GLint	viewport[4]; //ビューポートの保存用
	//CMatrix	modelview; //モデルビュー変換行列の保存用
	//CMatrix	projection; //透視変換行列の保存用

	/*
	** 第１ステップ：デプステクスチャの作成
	*/
	//* フレームバッファオブジェクトへのレンダリング開始
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFb);

	/* デプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* 現在のビューポートを保存しておく */
	glGetIntegerv(GL_VIEWPORT, viewport);

	/* ビューポートをテクスチャのサイズに設定する */
	glViewport(0, 0, TEXWIDTH, TEXHEIGHT);

	/* 透視変換行列を設定する */
	glMatrixMode(GL_PROJECTION); //透視変換行列に切り替え
	glPushMatrix(); //現在の設定はスタックに保存
	glLoadIdentity(); //行列の初期化

	/* Depthテクスチャの透視変換行列を保存しておく */
	gluPerspective(75.0, (GLdouble)TEXWIDTH / (GLdouble)TEXHEIGHT, 1.0, 100000.0);
	glGetFloatv(GL_PROJECTION_MATRIX, projection.mM[0]); //透視変換行列の保存

	GLfloat lightpos[] = { 0.0f, 200.0f, 200.0f, 0.0f }; //ライトの位置データ
	lightpos[2] = 0.0f; //ライトの位置データ
	if (CSceneTitle::mCource == 1){
		lightpos[1] = 2000.0f*2; //ライトの位置データ
	}
	else if (CSceneTitle::mCource == 2){
		lightpos[1] = 10000.0f; //ライトの位置データ
	}
	else if (CSceneTitle::mCource == 5){
		lightpos[1] = 24000.0f; //ライトの位置データ
	}
	else{
		//光源が遠いほど影の画質が粗くなってしまう
	}

	lightpos[0] = mPlayer->mPosition.mX; //ライトの位置データ
	lightpos[1] = mPlayer->mPosition.mY+1400.0f*5; //ライトの位置データ
	lightpos[2] = mPlayer->mPosition.mZ; //ライトの位置データ

	/* 光源位置を視点としシーンが視野に収まるようモデルビュー変換行列を設定する */
	glMatrixMode(GL_MODELVIEW); //モデルビュー行列に切り替え
	glPushMatrix(); //現在の設定はスタックに保存
	glLoadIdentity(); //行列の初期化
	//ライト位置から見るように行列を設定する
	gluLookAt(lightpos[0], lightpos[1], lightpos[2], lightpos[0] - 1, 0, lightpos[2] - 1, 0.0, 1.0, 0.0);
	/* 設定したモデルビュー変換行列を保存しておく */
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.mM[0]);

	/* デプスバッファの内容だけを取得するのでフレームバッファには書き込まない */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* したがって陰影付けも不要なのでライティングをオフにする */
	glDisable(GL_LIGHTING);

	/* デプスバッファには背面のポリゴンの奥行きを記録するようにする */
	glCullFace(GL_FRONT);
	//************************************ Shadow Map

	//Depthテクスチャを作成する描画
	CTaskManager::Get()->Render();

	//Shadow Map ************************************
	/* フレームバッファオブジェクトへのレンダリング終了 */
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/* テクスチャユニット１に切り替える */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
	///* デプスバッファの内容をテクスチャメモリに転送する */
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXWIDTH, TEXHEIGHT);

	/* 通常の描画の設定に戻す */
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glMatrixMode(GL_PROJECTION); //透視変換行列に切り替え
	glPopMatrix(); //設定をスタックから戻す

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	/*
	** 第２ステップ：全体の描画
	*/

	/* フレームバッファとデプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW); //モデルビュー行列に切り替え
	glPopMatrix(); //スタックから元に戻す
	/* モデルビュー変換行列を保存しておく */
	CMatrix modelviewCamera;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewCamera.mM[0]);

	/* 光源の位置を設定する */
	//glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

#if USEALPHA
	//プレイヤー、ライバルを先に描画
	for (int i = 0; i < ENEMYS_AMOUNT; i++)
	{
		if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
			mEnemys[i]->Render();
		}
	}
	mPlayer->Render();
#endif
	

	/* テクスチャ変換行列を設定する */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める */
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	/* テクスチャのモデルビュー変換行列と透視変換行列の積をかける */
	glMultMatrixf(projection.mM[0]);
	glMultMatrixf(modelview.mM[0]);

	/* 現在のモデルビュー変換の逆変換をかけておく */
	glMultMatrixf(modelviewCamera.GetInverse().mM[0]);

	/* モデルビュー変換行列に戻す */
	glMatrixMode(GL_MODELVIEW);

	/* テクスチャマッピングとテクスチャ座標の自動生成を有効にする */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

#if USEALPHA
	/* アルファテストを有効にして影の部分だけを描画する */
	glEnable(GL_ALPHA_TEST);
	///* 日向の部分がもとの図形に重ねて描かれるように奥行きの比較関数を変更する */
	glDepthFunc(GL_LEQUAL);
#endif
	
#if USEALPHA
	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat lightdim[] = { 0.2f, 0.2f, 0.2f, 0.2f };
	const GLfloat lightblk[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	/* 光源の明るさを日向の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
	//影の描画
	if (isEnableShadow){
		////コースの影の描画
		//for (int i = 0; i < CObj::mObject_Limit; i++){
		//	if (CObj::mpGrounds[i] != NULL){
		//		//テクスチャユニット0に切り替える
		//		glActiveTexture(GL_TEXTURE0);
		//		CObj::mpGrounds[i]->Render();				
		//		//テクスチャユニット1に切り替える
		//		glActiveTexture(GL_TEXTURE1);
		//	}
		//}
		//テクスチャユニット0に切り替える
		glActiveTexture(GL_TEXTURE0);
		CTaskManager::Get()->Render();
		//テクスチャユニット1に切り替える
		glActiveTexture(GL_TEXTURE1);
	}
#else
	//const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//const GLfloat lightdim[] = { 0.2f, 0.2f, 0.2f, 0.2f };
	//const GLfloat lightblk[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	///* 光源の明るさを日向の部分での明るさに設定 */
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
	//影の描画
	if (isEnableShadow){
		//コースの影の描画
		for (int i = 0; i < CObj::mObject_Limit; i++){
			if (CObj::mpGrounds[i] != NULL){
				//テクスチャユニット0に切り替える
				glActiveTexture(GL_TEXTURE0);
				CObj::mpGrounds[i]->Render();				
				//テクスチャユニット1に切り替える
				glActiveTexture(GL_TEXTURE1);
			}
		}
		/*テクスチャユニット0に切り替える
		glActiveTexture(GL_TEXTURE0);
		CTaskManager::Get()->Render();
		テクスチャユニット1に切り替える
		glActiveTexture(GL_TEXTURE1);*/
	}
#endif

	

#if USEALPHA
	/* 奥行きの比較関数を元に戻す */
	glDepthFunc(GL_LESS);
	/* アルファテストを無効にする */
	glDisable(GL_ALPHA_TEST);
#endif	

	//Shadow Map ************************************
	/* テクスチャマッピングとテクスチャ座標の自動生成を無効にする */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

	//テクスチャを解除する
	glBindTexture(GL_TEXTURE_2D, 0);
	/* テクスチャ変換行列を設定する */
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
	/*color[3] = 1.5f - 0.03f*isBlackOutTime;
	glColor4fv(color);
	CRectangle::Render(400, 300, 400, 300);*/

	int x_tiles = 20;
	int y_tiles = 15;
	int tile_width = 400 / x_tiles;
	int tile_height = 300 / y_tiles;
	for (int i = 0; i < (x_tiles * y_tiles); i++){
		////c[3] = (0.025f - 0.0025f*(i / x_tiles)) * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		//c[3] = -0.05f + -0.005f * i + 0.05f * (mStartWaitTime - WAITTIME_STARTFADEOUT);
		//glColor4fv(c);
		//int wid = mStartWaitTime - i;
		//if (wid > tile_width){
		//	wid = tile_width;
		//}
		//int hei = mStartWaitTime - i;
		//if (hei > tile_height){
		//	hei = tile_height;
		//}
		//wid = tile_width;
		//hei = tile_height;
		////hei = tile_height;
		//CRectangle::Render(0 + tile_width +  tile_width * 2 * (i / x_tiles % x_tiles),
		//	0 + tile_height + tile_height * 2 * (i % y_tiles), wid, hei);

		//c[3] = (0.025f - 0.0025f*(i / x_tiles)) * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		color[3] = 1-(-0.01f + -0.01f * i + 0.075f * (isBlackOutTime));
		/*if (c[3] > 0.3f){
		c[3] = 0.3f;
		}*/
		glColor4fv(color);

		int wid = tile_width - i + (isBlackOutTime);
		int hei = tile_height;
		//hei = tile_height;
		if (wid > tile_width){
			wid = tile_width;
		}
		if (hei > tile_height){
			hei = tile_height;
		}
		wid = tile_width;
		hei = tile_height;

		CRectangle::Render(0 + tile_width + tile_width * 2 * (i / y_tiles),
			0 + tile_height + tile_height * 2 * (i % y_tiles), wid, hei);
	}


	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
}

//別のシーンに映る際の処理
void CSceneRace::SceneChange(){
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	color[0] = color[1] = color[2] = 0.0f;
	color[3] = 0.0f + 0.025f*isBlackOutTime;
	glColor4fv(color);

	

	int x_tiles = 20;
	int y_tiles = 15;
	int tile_width = 400 / x_tiles;
	int tile_height = 300 / y_tiles;
	for (int i = 0; i < (x_tiles * y_tiles); i++){
		////c[3] = (0.025f - 0.0025f*(i / x_tiles)) * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		//c[3] = -0.05f + -0.005f * i + 0.05f * (mStartWaitTime - WAITTIME_STARTFADEOUT);
		//glColor4fv(c);
		//int wid = mStartWaitTime - i;
		//if (wid > tile_width){
		//	wid = tile_width;
		//}
		//int hei = mStartWaitTime - i;
		//if (hei > tile_height){
		//	hei = tile_height;
		//}
		//wid = tile_width;
		//hei = tile_height;
		////hei = tile_height;
		//CRectangle::Render(0 + tile_width +  tile_width * 2 * (i / x_tiles % x_tiles),
		//	0 + tile_height + tile_height * 2 * (i % y_tiles), wid, hei);

		//c[3] = (0.025f - 0.0025f*(i / x_tiles)) * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		color[3] = -0.01f + -0.01f * i + 0.075f * (isBlackOutTime);
		/*if (c[3] > 0.3f){
		c[3] = 0.3f;
		}*/
		glColor4fv(color);

		int wid = tile_width - i + (isBlackOutTime);
		int hei = tile_height;
		//hei = tile_height;
		if (wid > tile_width){
			wid = tile_width;
		}
		if (hei > tile_height){
			hei = tile_height;
		}
		wid = tile_width;
		hei = tile_height;

		CRectangle::Render(0 + tile_width + tile_width * 2 * (i / y_tiles),
			0 + tile_height + tile_height * 2 * (i % y_tiles), wid, hei);
	}


	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);

	//暗転
	if (isBlackOutTime < 60){
		isBlackOutTime++;
		if (isBlackOutTime >= 60)
			CText::DrawString("Please Wait...", 555, 14, 9, 11, 2);
	}
	else{
		//isFadeOut = false;		

		//ポーズ画面からのシーン遷移はタイトル
		if (isPause){
			//タイトル画面に戻る
			mScene = ETITLE;
		}
		//レース終了後からのシーン遷移は次のレース場所orタイトル
		else{
			if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
				//次のコースに移る
				if (mCurrent_RaceNumber == 1){					
					CSceneTitle::mCource = CSceneTitle::ECOURCE2;
					mScene = ERACE2;
				}
				else if (mCurrent_RaceNumber == 2){
					CSceneTitle::mCource = CSceneTitle::ECOURCE5;
					mScene = ERACE5;
				}
				else if (mCurrent_RaceNumber == 3){	
					//
				}
				//規定数レースを行ったらグランプリ終結、タイトルに戻る
				if (mCurrent_RaceNumber == RACES_PER_1CUP){
					mScene = ETITLE;
				}
			}
			else if (CSceneTitle::mMode == CSceneTitle::EMODE_TIMEATTACK){
				//タイムアタック終了時はタイトルへ戻る
				mScene = ETITLE;
			}			
		}
	}	
}

//次のシーンの取得
CScene::EScene CSceneRace::GetNextScene(){
	return mScene;
}

//敵の車カラーを出力する
void CSceneRace::PutCPUColor(){
	if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX
		&& mCurrent_RaceNumber == 1){
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
}

//影の描画
void CSceneRace::RenderShadowBM(){
	/*全体の描画*/

	/* テクスチャユニット１に切り替える */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
	/* フレームバッファとデプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///* モデルビュー変換行列の設定 */
	//glMatrixMode(GL_MODELVIEW); //モデルビュー行列に切り替え
	//glPopMatrix(); //スタックから元に戻す
	/* モデルビュー変換行列を保存しておく */
	CMatrix modelviewCamera;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewCamera.mM[0]);


#if USEALPHA
	for (int i = 0; i < ENEMYS_AMOUNT; i++)
	{
		if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
			mEnemys[i]->Render();
		}
	}
	mPlayer->Render();
#endif
	

	/* テクスチャ変換行列を設定する */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	/* テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める */
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	/* テクスチャのモデルビュー変換行列と透視変換行列の積をかける */
	glMultMatrixf(projection.mM[0]);
	glMultMatrixf(modelview.mM[0]);

	/* 現在のモデルビュー変換の逆変換をかけておく */
	glMultMatrixf(modelviewCamera.GetInverse().mM[0]);

	

	/* モデルビュー変換行列に戻す */
	glMatrixMode(GL_MODELVIEW);

	

	/* テクスチャマッピングとテクスチャ座標の自動生成を有効にする */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	
#if USEALPHA
	/* アルファテストを有効にして影の部分だけを描画する */
	glEnable(GL_ALPHA_TEST);

	/* 日向の部分がもとの図形に重ねて描かれるように奥行きの比較関数を変更する */
	glDepthFunc(GL_LEQUAL);
#endif



#if USEALPHA
	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	/* 光源の明るさを日向の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);


	//影の描画
	if (isEnableShadow){
		////コースの影の描画
		//for (int i = 0; i < CObj::mObject_Limit; i++){
		//	if (CObj::mpGrounds[i] != NULL){
		//		//テクスチャユニット0に切り替える
		//		glActiveTexture(GL_TEXTURE0);
		//		CObj::mpGrounds[i]->Render();
		//		//テクスチャユニット1に切り替える
		//		glActiveTexture(GL_TEXTURE1);				
		//	}
		//}

		//テクスチャユニット0に切り替える
		glActiveTexture(GL_TEXTURE0);
		CTaskManager::Get()->Render();
		//テクスチャユニット1に切り替える
		glActiveTexture(GL_TEXTURE1);
	}
#else
	//影の描画
	if (isEnableShadow){
		//コースの影の描画
		for (int i = 0; i < CObj::mObject_Limit; i++){
			if (CObj::mpGrounds[i] != NULL){
				//テクスチャユニット0に切り替える
				glActiveTexture(GL_TEXTURE0);
				CObj::mpGrounds[i]->Render();
				//テクスチャユニット1に切り替える
				glActiveTexture(GL_TEXTURE1);				
			}
		}
	}
#endif

	

#if USEALPHA
	/* 奥行きの比較関数を元に戻す */
	glDepthFunc(GL_LESS);

	/* アルファテストを無効にする */
	glDisable(GL_ALPHA_TEST);
#endif

	//Shadow Map ************************************
	/* テクスチャマッピングとテクスチャ座標の自動生成を無効にする */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);
	//テクスチャを解除する
	glBindTexture(GL_TEXTURE_2D, 0);
	/* テクスチャ変換行列を設定する */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glActiveTexture(GL_TEXTURE0);
	////************************************ Shadow Map
}

//敵車の生成
void CSceneRace::InstantiateEnemy(CVector rot){	
	//GPモード限定で敵生成
	if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
		//敵車の生成
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			mEnemys[i] = new CEnemy();
			if (i % 8 == 0){
				mEnemys[i]->mpModel = &mCarBlue;
			}
			else if (i % 8 == 1){
				mEnemys[i]->mpModel = &mCarPink;
			}
			else if (i % 8 == 2){
				mEnemys[i]->mpModel = &mCarRed;
			}
			else if (i % 8 == 3){
				mEnemys[i]->mpModel = &mCarGreen;
			}
			else if (i % 8 == 4){
				mEnemys[i]->mpModel = &mCarYellow;
			}
			else if (i % 8 == 5){
				mEnemys[i]->mpModel = &mCarBlack;
			}
			else if (i % 8 == 6){
				mEnemys[i]->mpModel = &mCarGray;
			}
			else if (i % 8 == 7){
				mEnemys[i]->mpModel = &mCarCyan;
			}
			//初期の配置座標を設定する
			mEnemys[i]->mPosition = CVector(StartPosVecs[i + 1]);
			mEnemys[i]->mRotation = rot;
			mEnemys[i]->CCharacter::Update();
			if (i == 0){
				mEnemys[i]->mEnemyAI = CEnemy::ENEWBIE;
			}
		}
	}
	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
		}
	}
	//敵車のカラー情報の出力
	PutCPUColor();
}