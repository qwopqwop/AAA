#ifndef CSCENETITLE_H
#define CSCENETITLE_H
#include "CText.h"
#include "CScene.h"
#include "CKey.h"
#include <Windows.h>
#include "CSound.h"

class CSceneTitle : public CScene {
public:
	//初期化処理のオーバーライド
	void Init() {
		//テクスチャファイルを読み込む
		//フォントの設定
		/*CText::mFont.Load("FontG.tga");
		CText::mFont.SetRowCol(1, 4096 / 64);*/

		CText::mFont.Load("font2nd.tga");
		CText::mFont.SetRowCol(8, 256 / 16);

		//シーンの設定
		mScene = ETITLE;

		mVariable1 = 0;
		mPrevVariable1 = mVariable1;
		mVariable2 = 0;
		mPrevVariable2 = mVariable2;
		mStart = false;
		mStartWaitTime = 0;
		SoundMoveCarsol.Load("SE\\カーソル2.wav");
		SoundDecide.Load("SE\\決定＿小決定（SF系）.wav");
	}
	//更新処理のオーバーライド
	void Update() {
		//2D描画開始
		Start2D(0, 800, 0, 600);
		//文字列の描画
		CText::DrawString("3D-RACE", -278 + 400 + 68, 10 + 400, 36, 36);
		CText::DrawString("Push Enter Key", 200, 150, 16, 16);
		
		float c[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		if (mStartWaitTime > 20 || mStart == false){
			c[0] = c[1] = c[2] = 1.0f;
		}
		else if(mStartWaitTime % 8 < 4){
			c[0] = c[1] = c[2] = 0.5f;
		}
		glColor4fv(c);
		CText::DrawString("O", 170 + mVariable2 * 250, 300 + mVariable1 * 50, 10, 10);


		
		if (mVariable1 == 0 && mVariable2 == 0){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartA", 200, 300, 16, 16);
		if (mVariable1 == 0 && mVariable2 == 1){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartB", 450, 300, 16, 16);
		if (mVariable1 == -1 && mVariable2 == 0){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartC", 200, 250, 16, 16);
		if (mVariable1 == -1 && mVariable2 == 1){
			c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		}
		else{
			c[0] = c[1] = c[2] = 0.5f; c[3] = 1.0f;
		}
		glColor4fv(c);
		CText::DrawString("StartD", 450, 250, 16, 16);
		c[0] = c[1] = c[2] = 1.0f; c[3] = 1.0f;
		glColor4fv(c);
		//2D描画終了
		End2D();
		
		if (CKey::Once(VK_RETURN)){
			mStart = true;
			SoundDecide.Play();
		}
		if (mStart){
			if (mStartWaitTime < 60){
				mStartWaitTime++;
			}
			else{
				//次のシーンはゲーム
				mScene = ERACE1;
			}
		}

		if (CKey::Once(VK_UP)){
			//次のシーンはゲーム
			if (mVariable1 < 0){
				mVariable1 += 1;
			}
		}
		if (CKey::Once(VK_DOWN)){
			//次のシーンはゲーム
			if (mVariable1 > -1){
				mVariable1 -= 1;
			}
		}
		if (CKey::Once(VK_LEFT)){
			//次のシーンはゲーム
			if (mVariable2 > 0){
				mVariable2 -= 1;
			}
		}
		if (CKey::Once(VK_RIGHT)){
			//次のシーンはゲーム
			if (mVariable2 < 1){
				mVariable2 += 1;
			}
		}

		if (mPrevVariable1 != mVariable1 || mPrevVariable2 != mVariable2){
			SoundMoveCarsol.Play();
			mPrevVariable1 = mVariable1;
			mPrevVariable2 = mVariable2;
		}
	}
	//描画処理絶対君主
	void Render();

	//次のシーンの取得
	EScene GetNextScene();

	int mVariable1;
	int mPrevVariable1;
	int mVariable2;
	int mPrevVariable2;
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	bool mStart;
	int mStartWaitTime;
	
};

#endif