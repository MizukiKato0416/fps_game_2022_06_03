//===========================================
//�e������
//Author:��������
//===========================================
#ifndef _BALLISTIC_H_
#define _BALLISTIC_H_

#include "object.h"

//================================================
//�O���錾
//================================================
class CObject3D;

//================================================
//�}�N����`
//================================================
#define BALLISTIC_MAX_ORBIT		(2)		//�e�̋O���G�t�F�N�g�̐�

//========================================================
//�\���̂̒�`
//========================================================

//================================================
//�N���X�̒�`
//================================================
//�N���X�̒�`
class CBallistic : public CObject
{
public:
	CBallistic(PRIORITY priorty = PRIORITY::BULLET);				//�R���X�g���N�^
	~CBallistic();				//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//��������
	static CBallistic *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 rot, const D3DXVECTOR3 endPos,
		                   const float fSpeed, const string sTexPas1, const string sTexPas2);

private:
	D3DXVECTOR3 m_bigenPos;						//�e�̎n�_
	D3DXVECTOR3 m_endPos;						//�e�̏I�_
	D3DXVECTOR3 m_size;							//�e�̃T�C�Y
	D3DXVECTOR3 m_rot;							//�e�̌���
	float m_fSpeed;								//�e�̑���
	string m_sTexPas1;							//�e�̃e�N�X�`��
	string m_sTexPas2;							//�e�̃e�N�X�`��
	CObject3D *m_apOrbit[BALLISTIC_MAX_ORBIT];	//�O���G�t�F�N�g�̃|�C���^
	bool m_bMove;								//�ړ����n�߂邩�ǂ���
};

//================================================
//�v���g�^�C�v�錾
//================================================
#endif // !_BALLISTIC_H_