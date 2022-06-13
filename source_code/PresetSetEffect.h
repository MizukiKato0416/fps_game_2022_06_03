//*****************************************************************************
// �G�t�F�N�g���̊i�[���ݒu���� [PresetEffect.h]
// Author : �����G��
//*****************************************************************************
#ifndef _PRESETEFFECT_H_
#define _PRESETEFFECT_H_
#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define MAX_EFFECTPATTERN_2D (8)
#define MAX_EFFECTPATTERN_3D (8)

#define MAX_ORDER_3D (8)

class CPresetEffect : CObject
{
public:
	//2D�p�̃G�t�F�N�g�X�e�[�^�X�W
	typedef struct
	{
		int m_nPattern;			//�����̃p�^�[��
		D3DXVECTOR3 m_pos;		//�ʒu
		float m_fRotate;		//��]
		D3DXVECTOR3 m_Endpos;	//�������ׂ��ʒu
		D3DXVECTOR2 m_move;		//�ړ���
		D3DXVECTOR2 m_Addmove;	//�ړ��ʉ��Z
		int m_nDiffusion;		//�g�U��
		int m_nDestroyvec;		//������x�N�g��
		float m_fSize;			//�T�C�Y
		float m_fAddSize;		//�T�C�Y���Z
		D3DCOLORVALUE m_Col;			//�J���[
		D3DCOLORVALUE m_Changecolor;	//�J���[���Z
		int m_nLife;				//����
		int m_nDensity;			//���x
		bool m_bColorRandR;		//�F�����_��R
		bool m_bColorRandG;		//�F�����_��G
		bool m_bColorRandB;		//�F�����_��B
		bool m_bMousePos;
		int Synthetic;	//����
		int nTexture;	//�e�N�X�`��
	} EFFECT_STATE2D;

	//3D�̏��
	typedef struct
	{
		int m_nPattern;			//�����̃p�^�[��
		float m_fRotate;		//��]
		float m_move;		//�ړ���
		float m_Addmove;	//�ړ��ʉ��Z
		int m_nDiffusion;		//�g�U��
		float m_fSize;			//�T�C�Y
		float m_fAddSize;		//�T�C�Y���Z
		float m_MaxSize;		//�ő�T�C�Y
		float m_fParticleSize;			//�T�C�Y
		float m_fParticleAddSize;		//�T�C�Y���Z
		int m_Active;
		D3DCOLORVALUE m_Col;			//�J���[
		D3DCOLORVALUE m_Changecolor;	//�J���[���Z
		D3DCOLORVALUE m_SecondCol;			//2�ԖڃJ���[
		D3DCOLORVALUE m_SecondChangecolor;	//2�ԖڃJ���[���Z
		int m_ParticleSynthetic;	//�p�[�e�B�N������
		int m_nLife;				//����
		int m_nDensity;			//���x
		int m_nDistance;		//��������
		int TrajectTop;	//�������f���P
		int TrajectCur;	//�������f��2
		D3DXVECTOR3 move3d;	//�ړ�
		int RandMove;	//�����_���ړ��l
		bool m_bColorRandR;		//�F�����_��R
		bool m_bColorRandG;		//�F�����_��G
		bool m_bColorRandB;		//�F�����_��B
		int Synthetic;	//����
		int nTexture;	//�e�N�X�`��
		int ParticleTime;
		D3DXVECTOR3 pos;	//�ʒu
		float m_fActiveAddSize;	//�A�N�e�B�u���̑傫���ύX
		int m_FieldTime;	//�t�B�[���h�����Ԋu
		bool m_fieldCreate;	//�t�B�[���h�������邩
		int mCreatePreset;	//�ǂ̃v���Z�b�g�𐶐����邩
		int m_nSecondTime;	//2�Ԗڂ̎��Ԍv�Z
		int m_nVtx;	//���_��
		int m_nType;	//�^�C�v
		D3DXVECTOR2 m_TexMove;	//�e�N�X�`���ړ�
		D3DXVECTOR2 m_TexNum;	//�e�N�X�`������
		int m_SecondType;	
		D3DXVECTOR2 m_TexSplit;	//������
		int AnimCnt;	//�A�j���[�V�����J�E���g
		float m_fHigth;	//����
		int m_AnimPatternType;
		D3DXVECTOR3 m_ControlBezier;
		D3DCOLORVALUE m_TherdCol;			//3�ԖڃJ���[
		D3DCOLORVALUE m_TherdChangecolor;	//3�ԖڃJ���[���Z
		int m_SecondTex;
	} EFFECT_STATE3D;


	CPresetEffect(PRIORITY priorty = PRIORITY::EFFECT3D);
	~CPresetEffect();


	//�ǂݍ��񂾃G�t�F�N�g�̏����i�[������
	static void SetEffectState2D(int nPattern,
		D3DXVECTOR3 pos,
		float fRotate,
		D3DXVECTOR2 move,
		D3DXVECTOR2 Addmove,
		int Diffusion,
		int Destroyvec,
		float fSize,
		float m_fAddSize,
		D3DCOLORVALUE col,
		D3DCOLORVALUE Changecolor,
		int nLife,
		int nDensity,
		bool bColorRandR,
		bool bColorRandG,
		bool bColorRandB,
		bool bMousePos,
		int Synthetic,
		int Texture);

	//�ǂݍ��񂾃G�t�F�N�g�̏����i�[������3D
	static void SetEffectState3D(
		int nPattern,
		float m_fRotate,
		float m_move,
		float m_Addmove,
		int m_nDiffusion,
		float m_fSize,
		float m_fAddSize,
		float m_MaxSize,
		float m_fParticleSize,
		float m_fParticleAddSize,
		int m_Active,
		D3DCOLORVALUE m_Col,
		D3DCOLORVALUE m_Changecolor,
		D3DCOLORVALUE m_SecondCol,
		D3DCOLORVALUE m_SecondChangecolor,
		int m_ParticleSynthetic,
		int m_nLife,
		int m_nDensity,
		int TrajectTop,
		int TrajectCur,
		D3DXVECTOR3 move3d,
		int RandMove,
		bool bColorRandR,
		bool bColorRandG,
		bool bColorRandB,
		int Synthetic,
		int Texture,
		int nDistance,
		int ParticleTime,
		D3DXVECTOR3 pos,
		float m_fActiveAddSize,
		int m_FieldTime,
		bool m_fieldCreate,
		int mCreatePreset,
		int m_nSecondTime,
		int m_nVtx,
		int m_nType,
		D3DXVECTOR2 m_TexMove,
		D3DXVECTOR2 m_TexNum,
		int m_nSecondType,
		D3DXVECTOR2 m_TexSplit,
		int AnimCnt,
		float fHigth,
		int AnimPatternType,
		D3DXVECTOR3 ControlBezier,
		D3DCOLORVALUE TherdCol,
		D3DCOLORVALUE TherdChangecolor,
		int SecondTex);

	typedef struct
	{
		int nDeley;
		int nPresetNum;
		int m_nOrder[MAX_ORDER_3D];
		D3DXVECTOR3 pos[MAX_ORDER_3D];
		D3DXVECTOR3 Endpos[MAX_ORDER_3D];
		bool bOne[MAX_ORDER_3D];
	} ORDER_PRESET;

	static void SetEffect2D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos);
	static void SetEffect3D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos);


	//�f�B���C�t���Đ�
	void SetOrderPreset(int nDeley, int nPresetNum);
	static void CallOrder3D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos);
	//void DeleySet(int nPattern);

	void CPresetEffect::SetOrder(int nOrder, int nPattern);

	static CPresetEffect *CreateOrderMenu(int nDeley, int nPresetNum, int nOrder[MAX_ORDER_3D]);

	static void ResetPattern() { m_nEffectPattern = 0; }
	static void ResetOrder() { m_nMaxOrderCount = 0; }

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void ResetDeley(int n) { nCntDeley[n]= 0; }
private:
	static EFFECT_STATE2D m_EffectState2D[MAX_EFFECTPATTERN_2D];
	static EFFECT_STATE3D m_EffectState3D[MAX_EFFECTPATTERN_3D];
	static ORDER_PRESET m_Order3D[MAX_ORDER_3D][MAX_ORDER_3D];

	static int m_nEffectPattern;	//�����̂�ǂݍ��߂�悤�ɂ�����

	static int m_nMaxOrderCount;	//�Ăяo���ő吔�J�E���g
	static int m_PrticleCreateTime;	//�p�[�e�B�N�������Ԋu�v�Z

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_EndPos;
	int nCntDeley[MAX_ORDER_3D];
};
#endif // !_PRESETEFFECT_H_