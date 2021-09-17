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


//ここのmBestTimeの値は関係ない(mRecord_ の値を入れるため)
int CSceneRace::mBestTime = 0;
int CSceneRace::mRecord_A = 10000;
int CSceneRace::mRecord_B = 13000;
int CSceneRace::mRecord_C = 22000;
int CSceneRace::mRecord_D = 460000;
int CSceneRace::mRecord_E = 40000;

//オプション画面から変更ができる変数
bool CSceneRace::isEnableShadow = true;//影
bool CSceneRace::isEnableMiniMap = true;//ミニマップ
bool CSceneRace::isEnableBackMirror = true;//バックミラー
bool CSceneRace::isEnableSpeedometer = false;//速度計

//画面サイズは800x600を想定
#define SCREENSIZE_X 800
#define SCREENSIZE_Y 600
//描画エリアの指定(左端のX座標,下端のY座標,幅,高さ)
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
	for (int i = 0; i < ENEMYS_AMOUNT + 1; i++){//+1はプレイヤー分
		mCarShadow[i] = NULL;
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
	
	/*全コース共通のマテリアル*/
	//車の読み込み
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
	//ミニマップ上でのプレイヤー・敵のカーソル、ゴール地点の読み込み
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//プレイヤー
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//敵
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");//ゴールアイコン
	mPole.Load("material\\common_mat\\cube.obj", "material\\common_mat\\soil.mtl");//ポール(木製)
	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");//加速床
	mJumper01.Load("material\\common_mat\\cube.obj", "material\\common_mat\\on.mtl");//ジャンプ台
	/*コース個別に読み込むマテリアル*/
	if (CSceneTitle::mCource_Number == 1){
		mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");//路面
		mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");//芝生
		mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");//柵(上面)
		mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");//柵(壁)
		mCurb01.Load("material\\racing_mat\\Curb01.obj", "material\\racing_mat\\Curb01.mtl");//紅白タイル
		mGoalTile01.Load("material\\racing_mat\\cource01_goaltile.obj", "material\\racing_mat\\cource01_goaltile.mtl");//＝白黒タイル
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
		//コースエディターのタイルの読み込み
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
		//コース05の読み込み
		mCource05Wall.Load("material\\racing_mat\\stage5\\cource05wall.obj", "material\\racing_mat\\stage5\\cource05wall.mtl");
		mCource05Mountain.Load("material\\racing_mat\\stage5\\cource05mountain.obj", "material\\racing_mat\\stage5\\cource05mountain.mtl");//全ての山共通
		mCource05Road.Load("material\\racing_mat\\stage5\\cource05road2.obj", "material\\racing_mat\\stage5\\cource05road2.mtl");
		mCource05Lake.Load("material\\racing_mat\\stage5\\cource05_lake.obj", "material\\racing_mat\\stage5\\cource05_lake.mtl");
		mCource05Grass_Floor.Load("material\\racing_mat\\stage5\\cource05grassF03.obj", "material\\racing_mat\\stage5\\cource05grassF03.mtl");
		mCource05Grass_Wall.Load("material\\racing_mat\\stage5\\cource05grass_wall.obj", "material\\racing_mat\\stage5\\cource05grass_wall.mtl");
		mCource05GoalTile.Load("material\\racing_mat\\stage5\\Checker_Tile.obj", "material\\racing_mat\\stage5\\Checker_Tile.mtl");
	}

	//ステージ1BGMの読み込み
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
	//効果音の読み込み//主にレース中
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");
	//主にポーズ中
	SoundMoveCarsol.Load("SE\\Carsol2.wav");
	SoundDecide.Load("SE\\Decision_Small(SF).wav");
	SoundPauseOn.Load("SE\\button79.wav");
	SoundPauseOff.Load("SE\\button80.wav");
	

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
	if (CSceneTitle::mCource_Number == 5){
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

	isEnableShadow_Cource = true;//ゲーム内での切り替え不可
	isEnableShadow_Car = false;//現時点では非表示推奨

	isFadeIn = true;
	isFadeOut = false;
	isBlackOutTime = 0;
	
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

	/* 比較の結果を輝度値として得る */
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

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
	//テクスチャユニットを0に戻す
	glActiveTexture(GL_TEXTURE0);	
}


void CSceneRace::Update() {
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向

	e = CCameraPos::mpCamera->mPosition;
	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 35.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	u = CVector(0.0f, 1.0f, 0.0f);//*mPlayer->mMatrixRotate;
	//カメラの設定
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	//タスクマネージャの更新・描画
	if (isPause == false){
		CTaskManager::Get()->Update();
	}

	RenderShadow();

	CTaskManager::Get()->Render();	
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
	//最後のチェックポイントを通過した扱いにする
	if (CKey::Once('V')){
		mPlayer->mChecks = 3;
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
			for (int i = 0; i < ENEMYS_AMOUNT; i++){
				if (mEnemys[i]->isEnemyGoaled == false){
					mEnemys[i]->mPointTime++;
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
	//カウント0で全員が走行可能に
	if (mCountDown == 0){
		CPlayer::mpPlayer->CanMove = true;
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			mEnemys[i]->CanMove = true;
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
			//ベストタイム更新時
			if (mTime < mBestTime){
				mBestTime = mTime;
				isNewRecord = true;
				//コースによって新しく記録する
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
	//CPUの車がゴール地点を通過した時の処理
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		if (mEnemys[i]->isTouchGoal
			&& (mEnemys[i]->mChecks == 3)
			&& (mEnemys[i]->isEnemyGoaled == false)){
			//その敵が最終ラップだった場合
			if (mEnemys[i]->mEnemyLap == mMaxLap){
				mEnemys[i]->mRank = mRanking;
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
		if (CKey::Once(VK_RETURN)){
			//次のシーンはゲーム
			mScene = ETITLE;
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

	//時間の表示
	char time[20];// :も含めた最大文字数の設定
	sprintf(time, "TIME:%02d:%02d:%02d", mTime_Output / 10000 % 100, mTime_Output / 100 % 100, mTime_Output % 100);
	CText::DrawString(time, 20, 530, 10, 12);
	//ベストタイムの表示
	char besttime[20];// :も含めた最大文字数の設定
	sprintf(besttime, "BEST:%02d:%02d:%02d", mBestTime / 10000 % 100, mBestTime / 100 % 100, mBestTime % 100);
	CText::DrawString(besttime, 20, 580, 10, 12);

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
	char lap[19];
	sprintf(lap, "LAP%d/%d", mLap, mMaxLap);
	CText::DrawString(lap, 20, 500, 10, 12, 2);
	//プレイヤーの車の速度表示
	char carspeed[33];
	sprintf(carspeed, "SPEED:%4.1f", CPlayer::mpPlayer->mCarSpeed);
	if (isEnableSpeedometer){
		CText::DrawString(carspeed, 20 + 580, 20, 10, 12);
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
		//Enterキー入力でタイトル画面に戻れることを伝えるテキスト
		if (mTextBlinkTime < 30){
			CText::DrawString("Press Enter to Title", 222, 77, 10, 12, 2);
		}
		//新記録をたたき出した時
		if (isNewRecord){
			//CText::DrawString("FINISH!", 400 - 20 * 6, 300, 20, 24);
			if (mTextBlinkTime < 15 || mTextBlinkTime >= 30 && mTextBlinkTime < 45){
				CText::DrawString("NEW RECORD!", 55, 551, 8, 9, 2);
			}
		}
	}

	//ゴール後に表示される文字
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

		//ゴールした車の記録を表示
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
				//色を一旦、白に戻す
				color[0] = color[1] = color[2] = 1.0f;
				glColor4fv(color);
				CText::DrawString(goaltime, 270, 285 - mEnemys[i]->mRank * 17, 10, 12, 2);
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
		
	if (CSceneTitle::mCource_Number == 3){
		//ミニマップにゴールアイコンを描画
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
		//ミニマップ状にプレイヤーを示すカーソルを描画
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
		CMatrix matenemys[ENEMYS_AMOUNT];
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
				* CMatrix().RotateX(0)
				* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
				* CMatrix().RotateZ(0)
				* mEnemys[i]->mMatrixTranslate;
			mCarsol_Enemy.Render(matenemys[i]);
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
	else if (CSceneTitle::mCource_Number == 5){
		//ミニマップにゴールアイコンを描画
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

		CMatrix matenemys[ENEMYS_AMOUNT];
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
				* CMatrix().RotateX(0)
				* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
				* CMatrix().RotateZ(0)
				* mEnemys[i]->mMatrixTranslate;
			mCarsol_Enemy.Render(matenemys[i]);
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
	
	

	//レンダーテクスチャ開始
	mRenderTexture.Start();

	//バックミラーの描画
	if (isEnableShadow){
		RenderShadow();
	}

	//オブジェクトの描画
	CTaskManager::Get()->Render();
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

	GLint	viewport[4]; //ビューポートの保存用
	CMatrix	modelview; //モデルビュー変換行列の保存用
	CMatrix	projection; //透視変換行列の保存用

	/*
	** 第１ステップ：デプステクスチャの作成
	*/

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
	if (CSceneTitle::mCource_Number == 1){
		lightpos[1] = 2000.0f*2; //ライトの位置データ
	}
	else if (CSceneTitle::mCource_Number == 2){
		lightpos[1] = 10000.0f; //ライトの位置データ
	}
	else if (CSceneTitle::mCource_Number == 5){
		lightpos[1] = 24000.0f; //ライトの位置データ
	}
	else{
		//光源が遠いほど影の画質が粗くなってしまう
	}

	lightpos[0] = mPlayer->mPosition.mX; //ライトの位置データ
	lightpos[1] = mPlayer->mPosition.mY+1400.0f; //ライトの位置データ
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
	/* テクスチャユニット１に切り替える */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
	/* デプスバッファの内容をテクスチャメモリに転送する */
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXWIDTH, TEXHEIGHT);

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

	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	/* 光源の明るさを日向の部分での明るさに設定 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
	//************************************ Shadow Map

	
	//影の描画
	if (isEnableShadow){
		//コースの影の描画
		if (isEnableShadow_Cource){
			for (int i = 0; i < GROUND_AMOUNT; i++){
				if (mpGrounds[i] != NULL){
					//テクスチャユニット0に切り替える
					glActiveTexture(GL_TEXTURE0);
					mpGrounds[i]->Render();
					//テクスチャユニット1に切り替える
					glActiveTexture(GL_TEXTURE1);
				}
			}
		}
		//車の影を設定
		if (isEnableShadow_Car){
			for (int i = 0; i < ENEMYS_AMOUNT + 1; i++){
				if (mCarShadow[i] != NULL){
					//テクスチャユニット0に切り替える
					glActiveTexture(GL_TEXTURE0);
					mCarShadow[i]->Render();
					//テクスチャユニット1に切り替える
					glActiveTexture(GL_TEXTURE1);
				}
			}
		}		
	}
	

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
	color[3] = 1.5f - 0.03f*isBlackOutTime;
	glColor4fv(color);
	CRectangle::Render(400, 300, 400, 300);
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
}

//別のシーンに映る際の処理
void CSceneRace::SceneChange(){
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//暗転
	if (isBlackOutTime < 60){
		isBlackOutTime++;
	}
	else{
		isFadeOut = false;
		//タイトル画面に戻る
		mScene = ETITLE;
	}
	color[0] = color[1] = color[2] = 0.0f;
	color[3] = 0.0f + 0.025f*isBlackOutTime;
	glColor4fv(color);
	CRectangle::Render(400, 300, 400, 300);
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
}

//次のシーンの取得
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
