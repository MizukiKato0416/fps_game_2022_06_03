//===========================================
//�T�E���h����
//Authore:��������
//===========================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include "main.h"
#include <thread>

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
class CSound
{
public:
	CSound();				//�R���X�g���N�^
	~CSound();				//�f�X�g���N�^

	//�T�E���h�t�@�C��
	enum class SOUND_LABEL
	{
		GAME_BGM = 0,			//�Q�[��BGM
		GAME_LAST_SPURT_BGM,	//���X�g�X�p�[�gBGM
		RESULT_BGM,				//���U���gBGM
		TITLE_BGM,				//�^�C�g��BGM
		RELOAD_SE,				//�����[�hSE
		RELOAD_FIN_SE,			//�����[�h����SE
		HIT_SE,					//�q�b�g��SE
		KILL_SE,				//�L��SE
		GAME_OVER_SE,			//�Q�[���I�[�o�[SE
		SHOT_SE,				//�V���b�gSE
		START_SHOT_SE,			//�X�^�[�g���̃o�L���[��SE
		TITLE_WIND_SE,			//�^�C�g���̕�SE
		COUNT_DOWN_SE,			//�J�E���g�_�E��SE
		FINISH_SE,				//�t�B�j�b�V��SE
		START_SE,				//�X�^�[�gSE
		DECIDE_SE,				//����SE
		MAX,
	};

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	HRESULT Play(const SOUND_LABEL &label);
	void Stop(const SOUND_LABEL &label);
	void Stop(void);
	void ControllVoice(const SOUND_LABEL &label, const float &fVolume) { m_apSourceVoice[(int)label]->SetVolume(fVolume); }	//���ʒ���

private:
	//�p�����[�^�[�\����
	typedef struct
	{
		char *filename;		// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;

	//�����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunksize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;										// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[(int)SOUND_LABEL::MAX] = {};			// �\�[�X�{�C�X
	BYTE *m_apDataAudio[(int)SOUND_LABEL::MAX] = {};							// �I�[�f�B�I�f�[�^
	DWORD m_asizeAudio[(int)SOUND_LABEL::MAX] = {};							// �I�[�f�B�I�f�[�^�T�C�Y

	static PARAM m_aParam[(int)SOUND_LABEL::MAX];								// �e���f�ނ̃p�����[�^
};

//================================================
//�v���g�^�C�v�錾
//================================================
#endif // !_SOUND_H_