#include "CRectangle.h"
#include "CTexture.h"
//Vector型のインクルード
#include <vector>

void CRectangle::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2){
	//アルファブレンドを有効にする
	glEnable(GL_BLEND);
	//三角形の描画
	glBegin(GL_TRIANGLES);
	glVertex2d(x0, y0);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
	//アルファブレンドを有効にする
	glDisable(GL_BLEND);
}
void CRectangle::DrawRectangle(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3){
	DrawTriangle(x0, y0, x1, y1, x2, y2);
	DrawTriangle(x0, y0, x2, y2, x3, y3);
}
void CRectangle::Render(int x, int y, int w, int h){
	DrawRectangle(x + w, y + h, x - w, y + h, x - w, y - h, x + w, y - h);
}