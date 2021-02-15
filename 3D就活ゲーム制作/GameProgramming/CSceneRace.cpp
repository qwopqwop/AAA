#include "CSceneRace.h"
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
#include "CItem.h"
//
#include "CKey.h"
//
#include "CBullet.h"

//スマートポインタの生成
std::shared_ptr<CTexture> TextureExp(new CTexture());
std::shared_ptr<CTexture> TextureHit(new CTexture());
std::shared_ptr<CTexture> TextureBoost(new CTexture());

extern CSound BGM;
extern CSound SoundCountDown;
extern CSound SoundStart;
extern CSound SoundGoal;

//ベストタイムの初期記録は2分00秒00
int CSceneRace::mBestTime = 20000;
int CSceneRace::mRecord_A = 10000;
int CSceneRace::mRecord_B = 13000;
int CSceneRace::mRecord_C = 20000;
int CSceneRace::mRecord_D = 30000;

CSceneRace::~CSceneRace() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneRace::Init() {
	//的の残数の初期化
	CItem::mTargetAmount = 0;

	//爆発テクスチャの読み込み
	TextureExp->Load("exp.tga");
	//衝突テクスチャの読み込み
	TextureHit->Load("effect\\[Attack]Hit01_panop.tga");
	//加速テクスチャの読み込み
	TextureBoost->Load("effect\\boost01.tga");
	////TextureBoost->Load("effect\\[Magic]SonicWave01_panop.tga");

	//テキストフォントの読み込みと設定
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	//mFont3:最後にmFont3に設定したシーンでのフォント(≒Title画面の時)

	//背景の読み込み
	mSky.Load("sky.obj", "sky.mtl");
	//岩の読み込み
	mRock.Load("Rock1.obj", "Rock1.mtl");
	//車の読み込み
	mRover.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");//プレイヤー
	mCarRed.Load("Rover1.obj", "material\\racing_mat\\single_color\\red.mtl");//以下敵の車
	mCarBlue.Load("Rover1.obj", "material\\racing_mat\\single_color\\blue.mtl");
	mCarGreen.Load("Rover1.obj", "material\\racing_mat\\single_color\\green.mtl");
	mCarYellow.Load("Rover1.obj", "material\\racing_mat\\single_color\\yellow.mtl");
	mCarPink.Load("Rover1.obj", "material\\racing_mat\\single_color\\pink.mtl");
	mCarCyan.Load("Rover1.obj", "material\\racing_mat\\single_color\\cyan.mtl");
	mCarWhite.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");
	mCarBlack.Load("Rover1.obj", "material\\racing_mat\\single_color\\black.mtl");
	//立方体の読み込み
	mCube.Load("cube.obj", "material\\cube.mtl");
	//地面の読み込み
	mPlane.Load("plane.obj", "plane.mtl");
	//階段の読み込み
	mStairs.Load("cube.obj", "material\\stairs.mtl");
	//的の読み込み
	mTarget.Load("Rock1.obj", "material\\target.mtl");
	//ONブロック(？)の読み込み
	mOnBlock.Load("cube.obj", "material\\on.mtl");

	mCube2.Load("cube.obj", "cube2.mtl");
	mCube3.Load("cube.obj", "cube3.mtl");
	//中間地点の読み込み(透明、ポリゴン1枚のみ)
	mCheckPoint.Load("cube.obj", "cube2.mtl");

	mWater.Load("cube.obj", "water_sumple.mtl");


	//バネの読み込み
	mSpringS.Load("ばね.obj", "cube.mtl");
	mSpringL.Load("ばね伸び.obj", "cube.mtl");

	//床タイルの読み込み
	mTileBlack.Load("cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("cube.obj", "material\\racing_mat\\tile_white.mtl");
	mRWTile.Load("material\\racing_mat\\NewNewR-W.obj", "material\\racing_mat\\NewNewR-W.mtl");
	//コースの読み込み
	//mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");
	mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");
	mCource02Road.Load("material\\racing_mat\\cource2nd\\cource02road.obj", "material\\racing_mat\\cource2nd\\cource02road.mtl");
	mCource02Wall.Load("material\\racing_mat\\cource2nd\\cource02wall.obj", "material\\racing_mat\\cource2nd\\cource02wall.mtl");
	mCource02Jump.Load("material\\racing_mat\\cource2nd\\cource02jumper.obj", "material\\racing_mat\\cource2nd\\cource02jumper.mtl");
	mCource03.Load("material\\racing_mat\\stage3\\cource3.obj", "material\\racing_mat\\stage3\\cource3.mtl");
	mCource04.Load("material\\racing_mat\\cource2nd\\track01.obj", "material\\racing_mat\\cource2nd\\track01.mtl");	//借り物
	
	//芝生の読み込み
	mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");
	//柵(壁)の読み込み
	mFence01.Load("material\\racing_mat\\Cource01Wall.obj", "material\\racing_mat\\Cource01Wall.mtl");
	//柵の読み込み
	mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");//上面
	mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");//壁
	mStuff.Load("material\\racing_mat\\fencestuff01.obj", "cube2.mtl");//詰め物(?)

	mPole.Load("cube.obj", "material\\soil.mtl");

	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");
	//ジャンプ台の読み込み
	mJumper01.Load("cube.obj", "cube.mtl");

	//ミニマップ上でのプレイヤー・敵のカーソルの読み込み
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//プレイヤー
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//敵
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");

	//ステージ1BGMの読み込み
	if (CSceneTitle::mMode == 1){
		BGM.Load("BGM\\(音量調整版)Popsギター_No.01.wav");
		//mMaxLap = 1;
		mBestTime = mRecord_A;
	}
	else if (CSceneTitle::mMode == 2){
		BGM.Load("BGM\\調整後game_maoudamashii_1_battle34.wav");
		//mMaxLap = 2;
		mBestTime = mRecord_B;
	}
	else if (CSceneTitle::mMode == 3){
		BGM.Load("BGM\\(調整後)bgm_maoudamashii_neorock33.wav");
		//mMaxLap = 3;
		mBestTime = mRecord_C;
	}
	else if (CSceneTitle::mMode == 4){
		BGM.Load("BGM\\調整後Crazy_Machine.wav");
		//mMaxLap = 5;
		mBestTime = mRecord_D;
	}

	//効果音の読み込み
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");

	mSumple.Load("UnderGround\\passage2.obj", "UnderGround\\passage2.mtl");

	/*
	////物理演算(笑)するオブジェクト No.2001
	//new CObj(&mTileWhite, CVector(500.0f, -13.1f + 23.0f, 900.0f), CVector(0.0f, 0.0f, 0.0f), CVector(11.0f, 11.0f, 11.0f), 2001);
	//new CObj(&mTileWhite, CVector(550.0f, -13.1f + 0.0f, 900.0f), CVector(0.0f, 0.0f, 0.0f), CVector(11.0f, 11.0f, 11.0f), 2001);
	//for (int i = 0; i < 5; i++){
	//	new CObj(&mTileWhite, CVector(280.0f + 22.0f*i, -13.1f + 22.0f*i, 200.0f), CVector(0.0f, 0.0f, 0.0f), CVector(11.0f, 11.0f, 11.0f), 2001);
	//}	
	//new CRigidObj(&mTileWhite, CVector(400.0f, -13.1f, 400.0f), CVector(0.0f, 0.0f, 0.0f), CVector(21.0f, 21.0f, 21.0f));

	//流れるダッシュボード達
	for (int i = 0; i < 16; i++){
		new CObj(&mDashBoard, CVector(260.0f - 40.0f*i , 13.1f + 10.0f, 800.0f), CVector(0.0f, 270.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 31);
		new CObj(&mDashBoard, CVector(260.0f - 40.0f*i-20.0f, 13.1f + 10.0f, 800.0f), CVector(0.0f, 270.0f, 0.0f), CVector(0.9f, 0.9f, 0.9f), 31);
	}
	*/


	//カメラ視点のY座標
	mCamY = 0.0f;
	//衝突判定の描画(デバッグ用)
	mPutCol = false;

	//一部テキストの点滅時間
	mTextBlinkTime = 0;

	//レースのカウントダウン関連
	mFrame = 0;
	mCountDown = 3+1;
	//レースはカウントダウンが終わってから開始
	isStartRace = isGoal = false;
	//時間のリセット
	mTime = 0;
		
	//ラップ数の初期化
	mLap = 1;
	//3周でゴール(全コース共通)
	mMaxLap = 3;
	//記録更新してない状態
	isNewRecord = false;
	
	//※最終順位の初期化
	mRanking = 1;
	//プレイヤーがゴール後の経過時間
	mAfterGoalTime = 0;

	//カメラ視点
	mCamPoV = 1;

	//バックミラーの描画
	isRender_BackMirror = true;

	//初期状態では敵の目標地点は描画しない
	isRendPoint = false;
	//初期状態ではポーズ状態無効
	isPause = false;

	//BGMはループ
	BGM.Repeat();
}


void CSceneRace::Update() {
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	if (mCamPoV == 1){
		e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
	}
	else if (mCamPoV == 2){
		e = CVector(0.0f, 0.0f + 0.5f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////注視点を求める
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}
	else if (mCamPoV == 3){//後方を映す視点
		e = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////注視点を求める
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}
	else{//1〜3以外の数値が入っている時はとりあえず前方視点(1と同じ)
		e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
	}
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);// *mPlayer->mMatrixRotate;	
	//カメラの設定
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	//タスクマネージャの更新・描画
	if (isPause == false){
		CTaskManager::Get()->Update();
	}
	CTaskManager::Get()->Render();	
	//衝突処理
	CTaskManager::Get()->TaskCollision();
	//削除処理
	CTaskManager::Get()->Delete();

	//デバッグ用
#ifdef _DEBUG
	if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
		printf("ベストタイム:%d\n", mBestTime);
	}
	if (CKey::Once('2')){//Playerの座標を出力する
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Once('3')){//強制的に的の残数を0にする(本来の的は消えない)
		CItem::mTargetAmount = 0;
	}
	if (CKey::Once('4')){//バックミラーのON・OFF切り替え
		isRender_BackMirror = !isRender_BackMirror;
	}
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
	if (CKey::Once('8')){
		//敵車すべてのmVPointの値を出力
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			//printf("mEnemys[%d]->mVPoint…X:%.1f Y:%.1f Z:%.1f\n", i, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mZ);
			printf("mEnemys[%d]の中間地点…%d  %d週目\n", i, mEnemys[i]->mChecks, mEnemys[i]->mEnemyLap);
		}
	}
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

	//敵の中継地点の表示ON・OFF切り替え
	if (CKey::Once('O')){		
		/*if (isRendPoint){
			isRendPoint = false;
		}
		else{
			isRendPoint = true;
		}*/
		//もっと簡潔に切り替える方法
		isRendPoint = !isRendPoint;
	}
	//BGMを停止する
	if (CKey::Once('M')){
		BGM.Stop();
	}
	//Bキーを押している間、メイン画面も後方視点になる
	if (CKey::Push('B')){
		mCamPoV = 3;
	}
	else{
		mCamPoV = 1;
	}
#endif	

	//ポーズ画面に入っていない時
	if (isPause == false){
		if (isStartRace){
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
		}
	}
	
	//ミニマップの描画
	RenderMiniMap();
	//バックミラーの描画
	if (isRender_BackMirror){
		RenderBackMirror();
	}

	//2D描画開始
	Start2D(0, 800, 0, 600);

	//順位の描画
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//////コースによって左下のコース名が変わる
	////if (CSceneTitle::mCource == CSceneTitle::ECOURCE1){
	////	CText::DrawString("COURCE 1", 20, 20, 10, 12);
	////}
	////else if (CSceneTitle::mCource == CSceneTitle::ECOURCE2){
	////	CText::DrawString("COURCE 2", 20, 20, 10, 12);
	////}
	////else if (CSceneTitle::mCource == CSceneTitle::ECOURCE3){
	////	CText::DrawString("COURCE 3", 20, 20, 10, 12);
	////}
	////else if (CSceneTitle::mCource == CSceneTitle::ECOURCE4){
	////	CText::DrawString("COURCE 4", 20, 20, 10, 12);
	////}
	if (CSceneTitle::mMode == 1){
		CText::DrawString("COURCE 1", 20, 20, 10, 12);
	}
	else if (CSceneTitle::mMode == 2){
		CText::DrawString("COURCE 2", 20, 20, 10, 12);
	}
	else if (CSceneTitle::mMode == 3){
		CText::DrawString("COURCE 3", 20, 20, 10, 12);
	}
	else if (CSceneTitle::mMode == 4){
		CText::DrawString("COURCE 4", 20, 20, 10, 12);
	}

	//時間の表示
	char mti[70];// :も含めた最大文字数の設定
//	sprintf(mti, "%06d", mTime);
	sprintf(mti, "TIME:%02d:%02d:%02d", mTime / 10000 % 100, mTime / 100 % 100, mTime % 100);
	CText::DrawString(mti, 20, 530, 10, 12);
	//ベストタイムの表示
	char mbestti[70];// :も含めた最大文字数の設定
	//	sprintf(mti, "%06d", mTime);
	sprintf(mbestti, "BEST:%02d:%02d:%02d", mBestTime / 10000 % 100, mBestTime / 100 % 100, mBestTime % 100);
	CText::DrawString(mbestti, 20, 580, 10, 12);
	
	//カウントダウン開始、GO!で操作の受付開始
	if (mFrame < 60){
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
		}
		mFrame = 0;
	}
	//カウントダウン表示
	char mcountd[7];
	//残り3秒までの間、表示
	sprintf(mcountd, "%d", mCountDown);
	if (mCountDown > 0 && mCountDown <= 3){
		CText::DrawString(mcountd, 400, 300, 25, 30);
	}
	else if (mCountDown == 0){
		CText::DrawString("GO!", 400-40, 300, 25, 30);
		CPlayer::mpPlayer->CanMove = true;
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			mEnemys[i]->CanMove = true;
		}
	}

	char lap[19];
	sprintf(lap, "LAP%d/%d", mLap, mMaxLap);
	CText::DrawString(lap, 20, 500, 10, 12, 2);

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
	char carspeed[33];
	sprintf(carspeed, "SPEED:%4.1f", CPlayer::mpPlayer->mCarSpeed);
	CText::DrawString(carspeed, 20+560, 20, 10, 12);
	char enecarspeed[33];
	sprintf(enecarspeed, "SPEED:%4.1f", mEnemys[0]->mCarSpeed);
	CText::DrawString(enecarspeed, 20 + 560, 20+25, 10, 12);

	char carhandle[33];
	sprintf(carhandle, "%4.2f", CPlayer::mpPlayer->mTurnSpeed);
	CText::DrawString(carhandle, 20 + 260, 20, 10, 12);
	
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
		if (mRanking == 1){      //1st
			sprintf(rank, "%dst", mRanking);
		}
		else if (mRanking == 2){ //2nd
			sprintf(rank, "%dnd", mRanking);
		}
		else if (mRanking == 3){ //3rd
			sprintf(rank, "%drd", mRanking);
		}
		else{ //4th,5th,...
			sprintf(rank, "%dth", mRanking);
		}
		CText::DrawString(rank, 357, 310, 10 * 2, 12 * 2, 2);		
	}
	//色合いを元に戻す
	color[3] = 1.0f;
	glColor4fv(color);

	//ポーズ中に表示される文字
	if (isPause){
		CText::DrawString("PAUSE", 280, 300, 10*3, 12*3, 3);		
		CText::DrawString("P - Resume", 290, 200, 10, 12, 2);
		CText::DrawString("Esc - Back to Title", 250, 170, 10, 12, 2);
	}
	//2D描画終了
	End2D();

	//ゴール地点通過時の処理
	if (CSceneTitle::mMode == 2){
		if ((CPlayer::mpPlayer->mPosition.mX > 2216.0f - 222.0f && CPlayer::mpPlayer->mPosition.mX < 2216.0f + 222.0f)
			&& (CPlayer::mpPlayer->mPosition.mZ > -2300.0f - 30.0f && CPlayer::mpPlayer->mPosition.mZ < -2300.0f + 30.0f)
			&& (CPlayer::mpPlayer->mChecks == 3)
			&& (isStartRace)){
			//new CObj(&mCube, CVector(0.0f, 0.0f, 5700.0f), CVector(-90.0f, 0.0f, 0.0f), CVector(100.0f, 13.0f, 211.0f), 1);//ゴール
			//new CObj(&mTileBlack, CVector(170.0f + 20.0f * -1 + 5.0f, -13.1f + 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(5.0f, 64.0f, 5.0f), 99);//柱
			//new CObj(&mTileBlack, CVector(170.0f + 20.0f * 40 + 5.0f, -13.1f + 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(5.0f, 64.0f, 5.0f), 99);//柱
			if (mLap == mMaxLap){
				//ベストタイム更新時
				if (mTime < mBestTime){
					mBestTime = mTime;
					isNewRecord = true;
					//コースによって新しく記録する
					if (CSceneTitle::mMode == 1){
						mRecord_A = mBestTime;
					}
					else if (CSceneTitle::mMode == 2){
						mRecord_B = mBestTime;
					}
					else if (CSceneTitle::mMode == 3){
						mRecord_C = mBestTime;
					}
					else if (CSceneTitle::mMode == 4){
						mRecord_D = mBestTime;
					}
				}
				isStartRace = false;
				isGoal = true;
				BGM.Stop();
				SoundGoal.Play();
				//CPlayer::mpPlayer->CanMove = false;//動きストップ
				CPlayer::mpPlayer->mChecks = 0;
			}
			else{
				mLap++;
				CPlayer::mpPlayer->mChecks = 0;
			}
		}
		//CPUの車がゴール地点を通過した時の処理
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			if (mEnemys[i]->mPosition.mX>2216.0f - 222.0f && mEnemys[i]->mPosition.mX < 2216.0f + 222.0f
				&& (mEnemys[i]->mPosition.mZ > -2300.0f - 30.0f && mEnemys[i]->mPosition.mZ < -2300.0f + 30.0f)
				&& (mEnemys[i]->mChecks == 3)
				&& (mEnemys[i]->isEnemyGoaled == false)){
				//その敵が最終ラップだった場合
				if (mEnemys[i]->mEnemyLap == mMaxLap){
					//プレイヤーの未ゴール時はプレイヤーの最終順位が落ちる
					if (isGoal == false){
						mRanking++;
					}
					mEnemys[i]->isEnemyGoaled = true;
				}
				//まだ最終ラップでない場合
				else{
					//チェックポイントをリセットし、次の周スタート
					mEnemys[i]->mChecks = 0;
					mEnemys[i]->mEnemyLap++;
				}
			}
		}
	}
	else{
		if ((CPlayer::mpPlayer->mPosition.mX > -55.0f && CPlayer::mpPlayer->mPosition.mX < 1400.0f)
			&& (CPlayer::mpPlayer->mPosition.mZ > -3.1f - 5.0f && CPlayer::mpPlayer->mPosition.mZ < -3.1f + 5.0f + 20.0f)
			&& (CPlayer::mpPlayer->mChecks == 3)
			&& (isStartRace)){
			if (mLap == mMaxLap){
				//ベストタイム更新時
				if (mTime < mBestTime){
					mBestTime = mTime;
					isNewRecord = true;
					//コースによって新しく記録する
					if (CSceneTitle::mMode == 1){
						mRecord_A = mBestTime;
					}
					else if (CSceneTitle::mMode == 2){
						mRecord_B = mBestTime;
					}
					else if (CSceneTitle::mMode == 3){
						mRecord_C = mBestTime;
					}
					else if (CSceneTitle::mMode == 4){
						mRecord_D = mBestTime;
					}
				}
				isStartRace = false;
				isGoal = true;
				BGM.Stop();
				SoundGoal.Play();
				//CPlayer::mpPlayer->CanMove = false;//false:ゴール後も一応走り続けることはできる
				CPlayer::mpPlayer->mChecks = 0;
			}
			else{
				mLap++;
				CPlayer::mpPlayer->mChecks = 0;
			}
		}
		//CPUの車がゴール地点を通過した時の処理
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			if (mEnemys[i]->mPosition.mX>-55.0f && mEnemys[i]->mPosition.mX < 1400.0f
				&& (mEnemys[i]->mPosition.mZ > -3.1f - 5.0f && mEnemys[i]->mPosition.mZ < -3.1f + 5.0f + 20.0f)
				&& (mEnemys[i]->mChecks == 3)
				&& (mEnemys[i]->isEnemyGoaled == false)){
				//その敵が最終ラップだった場合
				if (mEnemys[i]->mEnemyLap == mMaxLap){
					//プレイヤーの未ゴール時はプレイヤーの最終順位が落ちる
					if (isGoal == false){
						mRanking++;
					}
					mEnemys[i]->isEnemyGoaled = true;
				}
				//まだ最終ラップでない場合
				else{
					//チェックポイントをリセットし、次の周スタート
					mEnemys[i]->mChecks = 0;
					mEnemys[i]->mEnemyLap++;
				}
			}
		}
	}
	

	//if ((CPlayer::mpPlayer->mPosition.mX > -55.0f && CPlayer::mpPlayer->mPosition.mX < 1400.0f)
	//	&& (CPlayer::mpPlayer->mPosition.mZ > -3.1f - 5.0f && CPlayer::mpPlayer->mPosition.mZ < -3.1f + 5.0f + 20.0f)
	//	&& (CPlayer::mpPlayer->mChecks == 3)
	//	&& (isStartRace)){	
	//	if (mLap == mMaxLap){
	//		//ベストタイム更新時
	//		if (mTime < mBestTime){
	//			mBestTime = mTime;
	//			isNewRecord = true;
	//			//コースによって新しく記録する
	//			if (CSceneTitle::mMode == 1){
	//				mRecord_A = mBestTime;
	//			}
	//			else if (CSceneTitle::mMode == 2){
	//				mRecord_B = mBestTime;
	//			}
	//			else if (CSceneTitle::mMode == 3){
	//				mRecord_C = mBestTime;
	//			}
	//			else if (CSceneTitle::mMode == 4){
	//				mRecord_D = mBestTime;
	//			}
	//		}
	//		isStartRace = false;
	//		isGoal = true;
	//		BGM.Stop();
	//		SoundGoal.Play();
	//		//CPlayer::mpPlayer->CanMove = false;//動きストップ
	//		CPlayer::mpPlayer->mChecks = 0;
	//	}
	//	else{
	//		mLap++;
	//		CPlayer::mpPlayer->mChecks = 0;
	//	}
	//}
	

	if (CKey::Once('P')){
		//カウントダウン終了後、ポーズの切り替えが可能になる。
		if (mCountDown < 0){
			//ゴール後は切り替え不可
			if (isGoal)return;

			//ポーズのON・OFF切り替え
			isPause = !isPause;
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
		if (CKey::Once(VK_BACK) || CKey::Once(VK_ESCAPE)){
			//次のシーンはゲーム
			mScene = ETITLE;
		}
	}


	return;
}

/* マップ上からの視点 */
void CSceneRace::RenderMiniMap() {
	glPushMatrix();
	glViewport(600 + 20-30, 450 - 440, 200, 150); //画面の描画エリアの指定
	glLoadIdentity();
	
	if (CSceneTitle::mMode == 3){
		gluLookAt(0, 9400, 0, 0, 0, 0, 0, 0, 1);
	}
	else if (CSceneTitle::mMode == 2){
		gluLookAt(0, 7000, 0, 0, 0, 0, 0, 0, 1);
	}
	else{
		gluLookAt(0, 4800, 0, 0, 0, 0, 0, 0, 1);
	}	
	glDisable(GL_DEPTH_TEST);
//	BackGround.Render(CMatrix());
	//タスクマネージャの描画
//	TaskManager.Render();
	CTaskManager::Get()->Render();
		
	/*switch (CSceneTitle::mCource){
	case CSceneTitle::ECOURCE1:
	default:
	}*/

	if (CSceneTitle::mMode == 3){
		if (isRendPoint == true){
			/*デバッグ用*/
			//設定した敵の目標地点すべてをミニマップ上に描画する
			CMatrix point;
			for (int i = 1; i <= 12; i++){//ポイントの数だけ処理実行
				point = CMatrix().Scale(150.0f, 1.0f, 150.0f)
					* CMatrix().RotateY(45);
				//* CEnemy::mPoint->mMatrixTranslate;
				//1より小さい時は即やめ
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
				//ハードモードではさらに目標地点が細かく設定される
				else if (CSceneTitle::mDifficulty == 2 || CSceneTitle::mDifficulty == 3){
					if (i == 7){
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
				}
				else{
					break;
				}
				mTileWhite.Render(point);
			}
		}
		//ミニマップにゴールアイコンを描画
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 1.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(143)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(-3200.0f, 0.0f, 341.7f);
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
	else if (CSceneTitle::mMode == 2){
		if (isRendPoint == true){
			/*デバッグ用*/
			//設定した敵の目標地点すべてをミニマップ上に描画する
			CMatrix point;
			for (int i = 1; i <= 12; i++){//ポイントの数だけ処理実行
				point = CMatrix().Scale(111.0f, 1.0f, 111.0f)
					* CMatrix().RotateY(45);
				//* CEnemy::mPoint->mMatrixTranslate;
				//1より小さい時は即やめ
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
				//ハードモードではさらに目標地点が細かく設定される
				else if (CSceneTitle::mDifficulty == 2 || CSceneTitle::mDifficulty == 3){
					if (i == 7){
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
				}
				else{
					break;
				}
				mTileWhite.Render(point);
			}
		}
		//ミニマップにゴールアイコンを描画
		CMatrix matminig;
		matminig = CMatrix().Scale(25.0f, 1.0f, 25.0f)
			//* mPlayer->mMatrixRotate
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(143)
			* CMatrix().RotateZ(0)
			* CMatrix().Translate(-3200.0f, 0.0f, 341.7f);
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
	else{
		if (isRendPoint == true){
			/*デバッグ用*/
			//設定した敵の目標地点すべてをミニマップ上に描画する
			CMatrix point;
			for (int i = 1; i <= 12; i++){//ポイントの数だけ処理実行
				point = CMatrix().Scale(111.0f, 1.0f, 111.0f)
					* CMatrix().RotateY(45);
				//* CEnemy::mPoint->mMatrixTranslate;
				//1より小さい時は即やめ
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
				//ハードモードではさらに目標地点が細かく設定される
				else if (CSceneTitle::mDifficulty == 2 || CSceneTitle::mDifficulty == 3){
					if (i == 7){
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
				}
				else{
					break;
				}
				mTileWhite.Render(point);
			}
		}
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
}
//バックミラーを表示
void CSceneRace::RenderBackMirror(){
	glDisable(GL_CULL_FACE);//一時的に両面を描画可能にする
	glDisable(GL_DEPTH_TEST);
	glViewport(800 -400 -150-3, 400 - 7-3, 306, 206); //バックミラーの描画エリアの指定
	////カメラのパラメータを作成する
	//CVector be, bc, bu;//視点、注視点、上方向
	////視点を求める
	//be = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* mPlayer->mMatrixTranslate
	//	+ CVector(0.0f, 0.0f, 0.0f);
	//////注視点を求める
	////c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//bc = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	//bu = CVector(0.0f, 1.0f, 0.0f);
	//be = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* mPlayer->mMatrixTranslate
	//	+ CVector(0.0f, 0.0f, 0.0f);
	//////注視点を求める
	////c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//bc = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	////* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	////カメラの設定
	//Camera3D(be.mX, be.mY, be.mZ, bc.mX, bc.mY, bc.mZ, bu.mX, bu.mY, bu.mZ);
	//Camera.mEye = be;
	
	//2D描画開始
	Start2D(0, 800, 0, 600);
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glColor4fv(color);
	//上記の2D描画範囲の指定値より大きめに白背景を描画する
	int expand = 100;
	//白背景のよりも先に黒枠となるものを描画する
	glBegin(GL_TRIANGLES);//久しぶり
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

	glViewport(800 - 400 - 150, 400 - 7, 300, 200);
	//2D描画開始
	Start2D(0, 800, 0, 600);
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	//上記の2D描画範囲の指定値より大きめに白背景を描画する
	expand = 100;
	//白背景を"先に"描画する
	glBegin(GL_TRIANGLES);//久しぶり
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



	//行列を退避させる
	glPushMatrix();
	//行列を単位行列にする
	glLoadIdentity();

	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//メインカメラと同じ前方視点なはず…
	e = CVector(0.0f, 17.0f + 13.0f, 40.0f - 41.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		* mPlayer->mMatrixTranslate;
	c = mPlayer->mPosition + CVector(0.0f, 17.0f + 12.8f, 40.0f - 42.0f)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	u = CVector(0.0f, 1.0f, 0.0f);
	//カメラののX座標を反転させる	
	e = e * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
	c = c * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
	u = u * CMatrix().Scale(-1.0f, 1.0f, 1.0f);

	//バックミラーのカメラの設定
	gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	
	GLfloat translate[] = {
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixf(translate);
	CTaskManager::Get()->Render();
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600); //画面の描画エリアをメインの画面に戻す
	glEnable(GL_CULL_FACE);//表面のみの描画に戻す

	//行列を戻す
	glPopMatrix();
}
//次のシーンの取得
CScene::EScene CSceneRace::GetNextScene(){
	return mScene;
}