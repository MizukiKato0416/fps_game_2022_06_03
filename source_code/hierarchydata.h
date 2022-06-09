//====================================================
//
// コミュニケーションデータのヘッダーファイル(communicationdata.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef HIERARCHY_DATA_H_
#define HIERARCHY_DATA_H_
#include "main.h"

//====================================================
// 自作フレームデータ
//====================================================
struct FrameData : public D3DXFRAME
{
	// 合成行列
    D3DXMATRIXA16 CombinedTransformationMatrix;
};

//====================================================
// 自作メッシュコンテナ
//====================================================
struct MeshContainer : public D3DXMESHCONTAINER
{
	vector<LPDIRECT3DTEXTURE9> m_TextureList;	// マテリアルで使用するテクスチャ
	DWORD m_BoneWeightNum;	// ウェイトの数
	DWORD m_BoneNum;	// ボーンの数
	LPD3DXBUFFER m_BoneBuffer;	// ボーン情報のバッファ
	D3DXMATRIX **m_BoneMatrix;	// ボーンのマトリックスのポインタの配列
	D3DXMATRIX *m_BoneOffsetMatrix;	// ボーンのオフセット行列
};

//====================================================
// 階層クラス(ID3DXAllocateHierarchyで使う仮想関数のみ定義)
//====================================================
class HierarchyData : public ID3DXAllocateHierarchy
{
public:
	HierarchyData() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR name, LPD3DXFRAME *new_frame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR name, CONST D3DXMESHDATA *mesh_data, CONST D3DXMATERIAL *material_data, CONST D3DXEFFECTINSTANCE *effect_instance, DWORD material_num, CONST DWORD *adjacency, LPD3DXSKININFO skin_info, LPD3DXMESHCONTAINER *new_mesh_container);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME frame);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER base);
private:
	map<string, LPDIRECT3DTEXTURE9> m_TextureList;
};

#endif
