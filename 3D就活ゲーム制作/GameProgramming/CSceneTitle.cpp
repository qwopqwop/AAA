#include "CSceneTitle.h"
#include "CRectangle.h"

#define WAITTIME_STARTLOAD 150 //選択決定後、次のシーン移行までの時間
#define WAITTIME_NOWLOADING  110 //「NOW LOADING」が表示されるまでの時間
#define WAITTIME_STARTFADEOUT  WAITTIME_STARTLOAD - 90 //「NOW LOADING」がフェードアウトが始まるまでの時間

int CSceneTitle::mCource_Number = 1; //コースやLap数の初期設定
int CSceneTitle::mCPU_Level = 2; //敵AIの強さの初期設定　　1:EASY　2:NORMAL　3:HARD

//次のシーンの取得
CScene::EScene CSceneTitle::GetNextScene(){
	return mScene;
}


//初期化処理のオーバーライド
void CSceneTitle::Init() {
	//テクスチャファイルを読み込む
	//フォントの設定
	CText::mFont.Load("font\\font2nd.tga");
	CText::mFont.SetRowCol(8, 256 / 16);
	CText::mFont2.Load("font\\FontG.tga");
	CText::mFont2.SetRowCol(1, 4096 / 64);
	CText::mFont3.Load("font\\FontDIY.tga");//自作フォント
	CText::mFont3.SetRowCol(8, 176 / 11);
	
	//シーンの設定
	mScene = ETITLE;

	mSelect_Step = 1;
	mCarsol_Pos = 0;
	mStart = false;
	mStartWaitTime = 0;
	SoundMoveCarsol.Load("SE\\Carsol2.wav");
	SoundDecide.Load("SE\\Decision_Small(SF).wav");
	SoundCancel.Load("SE\\Cancel2.wav");
}
//更新処理のオーバーライド
void CSceneTitle::Update() {
	if (CKey::Once(VK_RETURN)){
		//開始フラグが建ったら連打しようが音は鳴らない
		if (mStart == false){
			SoundDecide.Play();
		}
		if (mSelect_Step < 2){
			//次に選ぶ項目へ
			mSelect_Step++;
		}
		else{
			//選び終えたらゲーム開始
			mStart = true;
		}
	}
	if (mStart){
		if (mStartWaitTime < WAITTIME_STARTLOAD){
			mStartWaitTime++;
		}
		else{
			//次のシーンはレース画面
			printf("選択したコース番号:No.%d  ", mCource_Number);
			printf("選択したCPUのレベル:%d\n", mCPU_Level);
			//選択したコースに対応するシーンへ移行			
			if (mCource_Number == 1){
				//次のシーンはコース1
				mScene = ERACE1;
			}
			else if (mCource_Number == 2){
				//次のシーンはコース2
				mScene = ERACE2;
			}
			else if (mCource_Number == 3){
				//次のシーンはコース3
				mScene = ERACE3;
			}
			else if (mCource_Number == 4){
				//次のシーンはコース3
				mScene = ERACE4;
			}
			else if (mCource_Number == 5){
				//次のシーンはコース3
				mScene = ERACE5;
			}
			else if (mCource_Number == 127){
				//コースエディタに移行
				mScene = EEDIT;
			}
		}
	}

	//まだ選択してない時
	if (mStart == false){
		//矢印キーで選択
		//選択画面1:コースの選択
		if (mSelect_Step == 1){
			//←,→キーでコース選択
			if (CKey::Once(VK_LEFT)){
				//次のシーンはゲーム
				if (mCarsol_Pos > 0){
					mCarsol_Pos -= 1;
					SoundMoveCarsol.Play();
				}
			}
			if (CKey::Once(VK_RIGHT)){
				//次のシーンはゲーム
				if (mCarsol_Pos < 2){
					mCarsol_Pos += 1;
					SoundMoveCarsol.Play();
				}
			}
		}
		//選択画面2：敵AIの強さの設定
		else if (mSelect_Step == 2){
			if (CKey::Once(VK_LEFT)){
				if (mCPU_Level > 1){
					mCPU_Level--;					
				}
				else{
					mCPU_Level = 3;
				}
				SoundMoveCarsol.Play();
			}
			if (CKey::Once(VK_RIGHT)){
				if (mCPU_Level < 3){
					mCPU_Level++;
				}
				else{
					mCPU_Level = 1;
				}
				SoundMoveCarsol.Play();
			}
			//Escキーか、BackSpaceキーで、前の選択画面に戻る
			if (CKey::Once(VK_BACK) || CKey::Once(VK_ESCAPE)){
				mSelect_Step--;
				SoundCancel.Play();
			}
		}
	}
	if (mCarsol_Pos == 0){
		mCource_Number = 1;
	}
	else if (mCarsol_Pos == 1){
		mCource_Number = 2;
	}
	else if (mCarsol_Pos == 2){
		mCource_Number = 5;
	}
	Render();//テキスト等の描画
}

void CSceneTitle::Render(){
	//2D描画開始
	Start2D(0, 800, 0, 600);	
	float c[] = { 1.0f, 1.0f, 1.0f, 1.0f };//{ R,G,B,α }

	//文字列の描画
	CText::DrawString("3D-RACE", 190, 430, 36, 36);
	CText::DrawString("Push Enter Key", 200, 177, 16, 16);
	//
	if (mSelect_Step == 1){
		CText::DrawString("[", 66 + mCarsol_Pos * 250, 284, 20, 30, 2);
		CText::DrawString("]", 262 + mCarsol_Pos * 250, 284, 20, 30, 2);
	}
	if (mSelect_Step == 2 && mStart == false){
		if (mCPU_Level == 1){
			CText::DrawString("[", 190, 102, 16, 24, 2);
			CText::DrawString("]", 278, 102, 16, 24, 2);
		}
		if (mCPU_Level == 2){
			CText::DrawString("[", 338, 102, 16, 24, 2);
			CText::DrawString("]", 474, 102, 16, 24, 2);
		}
		if (mCPU_Level == 3){
			CText::DrawString("[", 534, 102, 16, 24, 2);
			CText::DrawString("]", 622, 102, 16, 24, 2);
		}
	}

	//ここより前は選択時に点滅しない
	if (mStartWaitTime > 20 || mStart == false){
		c[0] = c[1] = c[2] = 1.0f;
	}
	else if (mStartWaitTime % 8 < 4){
		c[0] = c[1] = c[2] = 0.5f;
	}
	glColor4fv(c);
	//ここから先が選択完了時に点滅する

	//コース選択系のテキスト
	for (int i = 0; i <= 2; i++){
		if (mCarsol_Pos == i){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		//選択時に点滅する
		if (mStart && mCarsol_Pos == i){
			if (mStartWaitTime > 20){
				c[0] = c[1] = c[2] = 1.0f;
			}
			else if (mStartWaitTime % 8 < 4){
				c[0] = c[1] = c[2] = 0.5f;
			}
		}
		glColor4fv(c);
		if (i == 0){
			CText::DrawString("COURCE-1", 80, 280, 12, 14);
		}
		else if (i == 1){
			CText::DrawString("COURCE-2", 330, 280, 12, 14);
		}
		else if (i == 2){
			CText::DrawString("COURCE-3", 580, 280, 12, 14);
		}
	}
	//敵のレベル系のテキスト
	for (int i = 1; i <= 3; i++){		
		if (mCPU_Level == i && mSelect_Step == 2){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		//選択時に点滅する
		if (mStart && mCPU_Level == i){
			if (mStartWaitTime > 20){
				c[0] = c[1] = c[2] = 1.0f;
			}
			else if (mStartWaitTime % 8 < 4){
				c[0] = c[1] = c[2] = 0.5f;
			}
		}
		glColor4fv(c);
		if (i == 1){
			CText::DrawString("EASY", 200, 100, 12, 12);
		}
		else if (i == 2){
			CText::DrawString("NORMAL", 347, 100, 12, 12);
		}
		else if (i == 3){
			CText::DrawString("HARD", 543, 100, 12, 12);
		}		
	}		
	//全ての値を1.0fに戻しておく
	c[0] = c[1] = c[2] = c[3] = 1.0f;
	glColor4fv(c);

	char mrecord[16];// :も含めた最大文字数の設定
	if (mCource_Number == 1){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_A / 10000 % 100, CSceneRace::mRecord_A / 100 % 100, CSceneRace::mRecord_A % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mCource_Number == 2){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_B / 10000 % 100, CSceneRace::mRecord_B / 100 % 100, CSceneRace::mRecord_B % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mCource_Number == 3){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_C / 10000 % 100, CSceneRace::mRecord_C / 100 % 100, CSceneRace::mRecord_C % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mCource_Number == 4){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_D / 10000 % 100, CSceneRace::mRecord_D / 100 % 100, CSceneRace::mRecord_D % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mCource_Number == 5){
		sprintf(mrecord, "BEST:%02d:%02d:%02d", CSceneRace::mRecord_E / 10000 % 100, CSceneRace::mRecord_E / 100 % 100, CSceneRace::mRecord_E % 100);
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	else if (mCource_Number == 127){
		sprintf(mrecord, "EDIT");
		CText::DrawString(mrecord, 20, 580, 10, 12);
	}
	c[0] = c[1] = c[2] = 1.0f;
	glColor4fv(c);
	
	//フェードアウトの表現
	if (mStartWaitTime >= WAITTIME_STARTFADEOUT){
		c[0] = c[1] = c[2] = 0.0f;
		c[3] = 0.025f * (mStartWaitTime - (WAITTIME_STARTFADEOUT));
		glColor4fv(c);
		CRectangle::Render(400, 300, 400, 300);
	}
	//全ての値を1.0fに戻しておく
	c[0] = c[1] = c[2] = c[3] = 1.0f;
	glColor4fv(c);
	
	//シーンが移行する直前で表示
	if (mStartWaitTime > WAITTIME_NOWLOADING){
		CText::DrawString("Please Wait...", 555, 14, 9, 11);
	}
	//2D描画終了
	End2D();
}