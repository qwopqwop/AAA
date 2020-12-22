#ifndef CSCENETITLE_H
#define CSCENETITLE_H
#include "CText.h"
#include "CScene.h"
#include "CKey.h"
#include <Windows.h>

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
	}
	//更新処理のオーバーライド
	void Update() {
		//2D描画開始
		Start2D(0, 800, 0, 600);
		//文字列の描画
		CText::DrawString("RACE GAME", -278 + 400, 10 + 400, 36, 36);
		CText::DrawString("Push Enter Key", 200, 150, 16, 16);
		//2D描画終了
		End2D();
		
		if (CKey::Once(VK_RETURN)){
			//次のシーンはゲーム
			mScene = ERACE1;
		}
	}
	//描画処理絶対君主
	void Render();

	//次のシーンの取得
	EScene GetNextScene();
};

#endif