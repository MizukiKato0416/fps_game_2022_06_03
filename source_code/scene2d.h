//*****************************************************************************
// �V�[��2D���� [Scene2D.h]
// Author : �����G��
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define MAX_TEXTURE (16)		//�ő�Ŏg����2D�p�̉摜��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�t�H�[�}�b�g�ɍ��킹���\����
typedef struct
{
	D3DXVECTOR3 pos;	// ���_����
	float rhw;			// 1.0�ŌŒ�
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`���̍��W
} VERTEX_2D;

//*****************************************************************************
//�I�u�W�F�N�g�N���X
//*****************************************************************************
class CScene2D : public CObject
{
public:
	CScene2D(PRIORITY priorty = PRIORITY::EFFECT3D);
	~CScene2D();

	HRESULT Init();	//������
	void Uninit();				//�I��
	void Update();				//�X�V
	void Draw();				//�`��

	void SetPosition(D3DXVECTOR3 pos);		//pos�X�V�p
	D3DXVECTOR3 GetPosition() { return m_pos; }				//pos�X�V���邽�߂ɃQ�b�g

	float GetHight() { return  m_Size.y; }			//pos�X�V���邽�߂ɃQ�b�g
	float GetWhidth() { return  m_Size.x; }			//pos�X�V���邽�߂ɃQ�b
	void SetHight(float Hight);		//�傫���X�V
	void SetWhidth(float Whidth);	//�傫���X�V

	void ColorChange(D3DCOLORVALUE color);	//�J���[�ύX

	void FadeColorChange(int Alpha);		//�t�F�[�h�p���ύX

	void SetTex(int Texpos, float TexSize);						//�e�N�X�`���p�^�[���ԍ��A�p�^�[���P�̃T�C�Y
	void TexMove(float TexMoveU, float TexMoveV);				//�e�N�X�`�����W�̍X�V

	void SetGaugeScele(float Whidth, float Hight);				//�Q�[�W�^�C�v�̃X�P�[���ύX
	void SetRotate(D3DXVECTOR3 pos, float Rotate, float Rotate2, float Vectol);			//��]

protected:
	D3DXVECTOR3 m_pos;
	string m_pTexture;					//���

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR2 m_Size;				//�T�C�Y


	int m_Texpos;					//�e�N�X�`�����W
	float m_TexSize;				//�e�N�X�`���T�C�Y

	float m_TexMoveU;				//�e�N�X�`���ړ��l
	float m_TexMoveV;				//�e�N�X�`���ړ��l

	float m_PosTexU;				//�e�N�X�`��U���W
	float m_PosTexV;				//�e�N�X�`��V���W

									//�J���[�l
};
#endif