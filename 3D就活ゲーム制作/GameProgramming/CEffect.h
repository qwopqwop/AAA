#ifndef CEFFECT_H
#define CEFFECT_H
//ビルボードクラスのインクルード
#include "CBillBoard.h"
/*
エフェクトクラス
テクスチャのアニメーション
*/
class CEffect : public CBillBoard {
public:
	//行数
	int mRows;
	//列数
	int mCols;
	//1コマのフレーム数
	int mFps;
	//フレームカウンタ
	int mFrame;

	//コンストラクタ
	//CEffect(位置, 幅, 高さ, テクスチャ, 行数, 列数, フレーム数, 画像合成時のブレンド方法(0:アルファ 1:加算 それ以外:アルファ))
	CEffect(const CVector &pos, float w, float h, std::shared_ptr<CTexture> texture, int row = 1, int col = 1, int fps = 1, int blendtype = 0);

	//更新
	void Update();
};

#endif
