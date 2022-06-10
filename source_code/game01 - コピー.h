//===========================================
//�Q�[������
//Author:��������
//===========================================
#ifndef _GAME01_H_
#define _GAME01_H_
#include "object.h"
#include "main.h"

//�O���錾
class CPlayer;
class CFloor;
class CRoad;
class CGauge;
class CBg;
class CObject2D;
class CRocket;
class CModelSingle;
class CLetter;
class CNextDialogUI;
class CDialog;
class CPause;

//================================================
//�}�N����`
//================================================

//========================================================
//�\���̂̒�`
//========================================================

//================================================
//�N���X�̒�`
//================================================
class CGame01 : public CObject
{
public:
	CGame01(CObject::PRIORITY Priority = CObject::PRIORITY::NONE);		//�R���X�g���N�^
	~CGame01();									//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void LoadModelTxt(const string &Pas);			//���f�����[�h����
		
	//�����o�ϐ�
	CPlayer *m_pPlayer;					//�v���C���[�̃|�C���^
};	

#endif // !_GAME01_H_