#include "CSceneTitle.h"
#include "CRectangle.h"

#define WAITTIME_STARTLOAD 150 //選択決定後、次のシーン移行までの時間
#define WAITTIME_NOWLOADING  110 //「NOW LOADING」が表示されるまでの時間
#define WAITTIME_STARTFADEOUT  WAITTIME_STARTLOAD - 90 //「NOW LOADING」がフェードアウトが始まるまでの時間

//int CSceneTitle::mCource_Number = 1; //コースやLap数の初期設定
CSceneTitle::ECPU_Level CSceneTitle::mCPU_Level = CSceneTitle::ENORMAL;//敵AIの強さの初期設定

//int CSceneTitle::mCource_Number;//コースNo.
CSceneTitle::ECource CSceneTitle::mCource = CSceneTitle::ECOURCE1;

//CSceneTitle::EMode CSceneTitle::mMode = CSceneTitle::EMODE_GRANDPRIX;

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

	mLevel = mCPU_Level;
	mCourceNum = mCource;
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
		if (mSelect_Step == 1){
			//次に選ぶ項目へ
			mSelect_Step++;

			
		}
		else if (mSelect_Step == 2){
			//選び終えたらゲーム開始
			mStart = true;
		}
	}

	if (mStart){
		if (mStartWaitTime < WAITTIME_STARTLOAD){
			mStartWaitTime++;
		}
		else{
			switch (mLevel)
			{
			case 1:
				mCPU_Level = EEASY;
				break;
			case 2:
				mCPU_Level = ENORMAL;
				break;
			case 3:
				mCPU_Level = EHARD;
				break;
			default:
				printf("不明な難易度が選択されました　難易度をnormalに設定します\n");
				mCPU_Level = ENORMAL;//
				break;
			}			
			//次のシーンはレース画面
			printf("選択したコース番号:No.%d  ", mCource);
			printf("選択したCPUのレベル:%d\n", mCPU_Level);
			
			/*if (mMode == EMODE_GRANDPRIX)
				printf("グランプリ\n");
			if (mMode == EMODE_TIMEATTACK)
				printf("タイムアタック\n");
			if (mMode == EMODE_EDITOR)
				printf("エディット\n");*/

			//選択したコースに対応するシーンへ移行			
			if (mCource == 1){
				//次のシーンはコース1
				mScene = ERACE1;
			}
			else if (mCource == 2){
				//次のシーンはコース2
				mScene = ERACE2;
			}
			else if (mCource == 3){
				//次のシーンはコース3
				mScene = ERACE3;
			}
			else if (mCource == 4){
				//次のシーンはコース3
				mScene = ERACE4;
			}
			else if (mCource == 5){
				//次のシーンはコース3
				mScene = ERACE5;
			}
			else if (mCource == 0){
				//コースエディタに移行
				mScene = EEDIT;
			}
		}
	}
	//まだ選択してない時
	if (mStart == false){
		//矢印キーで選択

		//選択画面1:グランプリモード(GP)orタイムアタックモード(TA)の選択

		//選択画面2:GPモード→難易度選択    TAモード→コース選択


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
				if (mLevel > 1){
					mLevel--;
				}
				else{
					mLevel = 3;
				}
				SoundMoveCarsol.Play();
			}
			if (CKey::Once(VK_RIGHT)){
				if (mLevel < 3){
					mLevel++;
				}
				else{
					mLevel = 1;
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
		mCource = ECOURCE1;
	}
	else if (mCarsol_Pos == 1){
		mCource = ECOURCE2;
	}
	else if (mCarsol_Pos == 2){
		mCource = ECOURCE5;
	}

	/*if (mCarsol_Pos == 0){
		mMode = EMODE_GRANDPRIX;
	}
	else if (mCarsol_Pos == 1){
		mMode = EMODE_TIMEATTACK;
	}*/

	Render();//テキスト等の描画
}

void CSceneTitle::Render(){
	//2D描画開始
	Start2D(0, 800, 0, 600);	
	float c[] = { 1.0f, 1.0f, 1.0f, 1.0f };//{ R,G,B,α }
	

	//文字列の描画
	CText::DrawString("3D-RACE", 190, 430, 36, 36);
	CText::DrawString("Push Enter Key", 200, 177, 16, 16);
	if (mSelect_Step == 1){//コース選択のカーソル
		CText::DrawString("[", 66 + mCarsol_Pos * 250, 284, 20, 30, 2);
		CText::DrawString("]", 262 + mCarsol_Pos * 250, 284, 20, 30, 2);
	}
	if (mSelect_Step == 2 && mStart == false){
		if (mLevel == 1){//CPUの強さ選択のカーソル
			CText::DrawString("[", 190, 102, 16, 24, 2);
			CText::DrawString("]", 278, 102, 16, 24, 2);
		}
		if (mLevel == 2){
			CText::DrawString("[", 338, 102, 16, 24, 2);
			CText::DrawString("]", 474, 102, 16, 24, 2);
		}
		if (mLevel == 3){
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

		/*if (i == 0){
			CText::DrawString("GRAND-PRIX MODE", 200, 310, 14, 17);
		}
		else if (i == 1){
			CText::DrawString("TIMEATTACK MODE", 200, 260, 14, 17);
		}*/
	}
	//CPUレベルのテキスト
	for (int i = 1; i <= 3; i++){		
		if (mLevel == i && mSelect_Step == 2){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		//選択時に点滅する
		if (mStart && mLevel == i){
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

	char record[16];
	if (mCource == 0){
		sprintf(record, "EDIT");
		CText::DrawString(record, 20, 580, 10, 12);
	}
	else{
		for (int i = 1; i <= COURCE_TOTAL; i++){
			if (mCource == i){
				sprintf(record, "BEST:%02d:%02d:%02d", CSceneRace::mRecords[i] / 10000 % 100, CSceneRace::mRecords[i] / 100 % 100, CSceneRace::mRecords[i] % 100);
				CText::DrawString(record, 20, 580, 10, 12);
			}			
		}
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