//=============================================================================
// ���ʏ��� [plane.h]
// Author : �����G��
//=============================================================================
#ifndef _PLANE_H_
#define _PLANE_H_

#include "main.h"
#include "object.h"

class CPlane : public CObject
{
public:
	CPlane(PRIORITY priorty = PRIORITY::EFFECT3D);
	~CPlane();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	D3DXVECTOR3 GetSize() { return m_Oliginsize; }
	void SetSize(D3DXVECTOR3 size);

	LPDIRECT3DVERTEXBUFFER9 GetVtx() { return m_pVtxBuff; }
	void ChangeColor(D3DXCOLOR col);

	//�G�t�F�N�g�p�֐�
	void ColorChange(D3DCOLORVALUE color);	//�F�ύX
	void ChangeSize(D3DXVECTOR3 size);	//�T�C�Y�ύX
	void BillboardSize(float size);	//�T�C�Y�ύX

	void TexturMove(D3DXVECTOR2 MoveTex);

	void SetTexAnim(D3DXVECTOR2 TexPattern, D3DXVECTOR2 TexAnimSize);
	void SetPosField(D3DXVECTOR3 pos, D3DXVECTOR3 Size, float Rotate,float Rotate2);

	void SetPosBill(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 pos4);

	static CPlane *Create();


	//�e�N�X�`���j��
	static void UninitTexture();
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL; //���_�o�b�t�@�ւ̃|�C���^

	static int m_Synthetic;
protected:
	string m_pTexture;	//�e�N�X�`���̃p�X

	D3DXVECTOR2 m_TexNum;
	D3DXVECTOR2 m_TexMove;
	D3DXVECTOR2 m_TexSize;
	D3DXVECTOR2 m_PatternSize;

	int m_nAnimCount;
	int m_nSetAnimCnt;
	D3DXVECTOR2 m_nSplit;
	D3DXVECTOR2 m_MaxSplit;

	D3DXVECTOR3 m_Oliginpos;

	D3DXVECTOR3 m_Oliginsize;
	D3DXVECTOR2 m_OliginTex;

};

#endif