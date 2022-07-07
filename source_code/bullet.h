//===========================================
//�e����
//Author:��������
//===========================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object.h"

//================================================
//�O���錾
//================================================
class CObject3D;

//================================================
//�}�N����`
//================================================

//========================================================
//�\���̂̒�`
//========================================================

//================================================
//�N���X�̒�`
//================================================
//�N���X�̒�`
class CBullet : public CObject
{
public:
	CBullet(PRIORITY priorty = PRIORITY::NONE);				//�R���X�g���N�^
	~CBullet();				//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBullet *Create(void);		//��������

	//���������G�̃v���C���[�ԍ��擾����
	int GetHitPlayerNum(void) { return m_nPlayer; }
	//�_���[�W�擾����
	int GetDamage(void) { return m_nDamage; }

private:
	D3DXVECTOR3 m_bigenPos;						//�e�̎n�_
	D3DXVECTOR3 m_endPos;						//�e�̏I�_
	D3DXVECTOR3 m_rayVec;						//���C�̕���
	float m_fDiffer;							//�I�_�܂ł̋���
	int m_nPlayer;								//�v���C���[�ԍ�
	int m_nDamage;								//�_���[�W
};
//================================================
//�v���g�^�C�v�錾
//================================================
#endif // !_BULLET_H_