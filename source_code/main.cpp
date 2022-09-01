//=============================================================================
//
// ���C������ [main.cpp]
// Author : ��������
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "sound.h"

//===========================================================
// �}�N����`
//===========================================================
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Yuggdracil"		// �E�C���h�E�̃L���v�V������

//===========================================================
// �v���g�^�C�v�錾
//===========================================================

//===========================================================
// �O���[�o���ϐ�:
//===========================================================

#ifdef _DEBUG
int	g_nCountFPS;	// FPS�J�E���^
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	//����������(�E�B���h�E�𐶐����Ă���s��)(DirectX�{�̂̐������s��)
	if (FAILED(CManager::GetInstance()->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �t���[���J�E���g������
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b���ƂɎ��s
#ifdef _DEBUG
				// FPS���Z�o
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))
			{// 1/60�b�o��
				dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

				//�V�����E�B���h�E�n���h��
				HWND newhWind;
				//�E�B���h�E�̃T�C�Y��������ϐ�
				RECT lprc = {};
				//�E�B���h�E�̃T�C�Y���擾
				if (GetClientRect(hWnd, &lprc))
				{
					//�E�B���h�E�̃T�C�Y���K�肩��ύX����Ă�����
					if (lprc.right != SCREEN_WIDTH || lprc.bottom != SCREEN_HEIGHT)
					{
						//�E�B���h�E��V������������
						newhWind = CreateWindow(CLASS_NAME,
							                    WINDOW_NAME,
							                    WS_OVERLAPPEDWINDOW,
							                    CW_USEDEFAULT,
							                    CW_USEDEFAULT,
							                    (rect.right - rect.left),
							                    (rect.bottom - rect.top),
							                    NULL,
							                    NULL,
							                    hInstance,
							                    NULL);

						// �E�C���h�E�̕\��
						ShowWindow(newhWind, nCmdShow);
						UpdateWindow(newhWind);

						//�E�B���h�E�̍폜
						//DestroyWindow(hWnd);
						//�㏑��
						//hWnd = newhWind;
						//�}�l�[�W���[�ɐݒ�
						CManager::GetInstance()->SetWindowHandle(hWnd);
						//����炷
						CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::COUNT_DOWN_SE);
					}
				}

				// �X�V����
				CManager::GetInstance()->Update();

				// �`�揈��
				CManager::GetInstance()->Draw();

				dwFrameCount++;
			}
		}
	}

	if (CManager::GetInstance() != NULL)
	{
		// �I������
		CManager::GetInstance()->Uninit();
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ

			//�Q�[�����łȂ�������
			if (CManager::GetInstance()->GetMode() != CManager::MODE::GAME01)
			{
				DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			}
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=======================================================================
//�s����g�����x�N�g���̕ϊ�
//=======================================================================
D3DXVECTOR3 VTransform(D3DXVECTOR3 InV, D3DXMATRIX InM)
{
	D3DXVECTOR3 tmp;
	tmp.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0];
	tmp.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1];
	tmp.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2];

	return tmp;
}

//------------------------
// ��P1P2P3�̓��ς��Z�o
//------------------------
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	return (x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2);
}

//------------------------
// ���C�ƃJ�v�Z���̊ђʓ_���Z�o
//------------------------
bool calcRayCapsule(float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z)
{
	bool Q1inP1 = false;
	bool Q1inP2 = false;
	bool Q1inCld = false;

	// Q1�̌���
	if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) <= 0.0f)
	{
		Q1inP1 = true; // Q1�͋���P1��ɂ���
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) <= 0.0f)
	{
		Q1inP2 = true; // Q1�͋���P2��ɂ���
	}
	else if (calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) > 0.0f)
	{
		Q1inCld = true; // Q1�͉~���ʂɂ���
	}
	else
	{
		return false; // ���C�͏Փ˂��Ă��Ȃ�
	}

	// Q2�̌���
	float tx, ty, tz; // �_�~�[
	if (Q1inP1 && checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1�AQ2����P1��ɂ���
		return true;
	}
	else if (Q1inP2 && checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1�AQ2����P2��ɂ���
		return true;
	}
	else if (Q1inCld &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) > 0.0f)
	{
		// Q1�AQ2�����~���ʂɂ���
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2�͋�P1��ɂ���
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2�͋�P2��ɂ���
		return true;
	}

	// Q2���~����ɂ��鎖���m��
	calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z);

	return true;
}

//------------------------
// ���C�Ɖ~���̊ђʓ_���Z�o
//------------------------
bool calcRayInfCilinder(float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z)
{
	float px = p1x - lx;
	float py = p1y - ly;
	float pz = p1z - lz;
	p2x = p2x - lx;
	p2y = p2y - ly;
	p2z = p2z - lz;
	float sx = p2x - px;
	float sy = p2y - py;
	float sz = p2z - pz;

	// �e����ϒl
	float Dvv = vx * vx + vy * vy + vz * vz;
	float Dsv = sx * vx + sy * vy + sz * vz;
	float Dpv = px * vx + py * vy + pz * vz;
	float Dss = sx * sx + sy * sy + sz * sz;
	float Dps = px * sx + py * sy + pz * sz;
	float Dpp = px * px + py * py + pz * pz;
	float rr = r * r;

	if (Dss == 0.0f)
		return false; // �~������`����Ȃ�

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f)
		return false;

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // ���C���~���ƏՓ˂��Ă��Ȃ�
	s = sqrtf(s);

	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}

//------------------------
// lx, ly, lz : ���C�̎n�_
//------------------------
bool calcRaySphere(float lx, float ly, float lz,
	float vx, float vy, float vz,
	float px, float py, float pz,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z)
{
	px = px - lx;
	py = py - ly;
	pz = pz - lz;

	float A = vx * vx + vy * vy + vz * vz;
	float B = vx * px + vy * py + vz * pz;
	float C = px * px + py * py + pz * pz - r * r;

	if (A == 0.0f)
	{
		return false; // ���C�̒�����0
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false; // �Փ˂��Ă��Ȃ�
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false; // ���C�̔��΂ŏՓ�
	}

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}

#ifdef _DEBUG
//=============================================================================
// FPS�擾����
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
#endif