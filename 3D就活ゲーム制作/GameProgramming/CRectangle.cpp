#include "CRectangle.h"
#include "CTexture.h"
//Vector�^�̃C���N���[�h
#include <vector>
///*
//�ϒ��z��̒�`
//std;;vector<�z��v�f�̃f�[�^�^> �z��
//*/
//std::vector<CRectangle*> VectorRect;
//
//CRectangle::CRectangle()
////:mTag(EBLOCK)  //�����l�u���b�N
////, mEnabled(false)
//{
//	///*
//	//�ϒ��z��Ɏ��g�̃A�h���X�i�|�C���^�j��ǉ�����
//	//�z��.push_back(�l)
//	//�l��z��̍Ō�ɒǉ�����
//	//this:���g�̃A�h���X�i�|�C���^�j
//	//*/
//	//VectorRect.push_back(this);
//
//}

void CRectangle::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2){
	//�A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	//�O�p�`�̕`��
	glBegin(GL_TRIANGLES);
	glVertex2d(x0, y0);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
	//�A���t�@�u�����h��L���ɂ���
	glDisable(GL_BLEND);
}
void CRectangle::DrawRectangle(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3){
	DrawTriangle(x0, y0, x1, y1, x2, y2);
	DrawTriangle(x0, y0, x2, y2, x3, y3);
}
void CRectangle::Render(int x, int y, int w, int h){
	DrawRectangle(x + w, y + h, x - w, y + h, x - w, y - h, x + w, y - h);
}

//void CRectangle::Render2D(const CTexture &t, int left, int right, int bottom, int top){
//	t.DrawImage2D(200 - 100, 200 + 100, 222 - 111, 222 + 111, left, right, bottom, top);
//}