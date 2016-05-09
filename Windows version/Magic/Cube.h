#pragma once

#include <string>
#include <vector>
#include <d3dx9.h>

#define CUBE_LEN     (100.0)
#define CUBE_FACE_NUMBER  (6)     //正方体面数
#define CUBE_VERT_NUMBER  (8)     //正方体顶点数
#define SPEER_RADIO_FAC  (0.7) 

using namespace std;

class CCube
{

private:
	string m_XFileName;
	ID3DXMesh*                 m_Mesh;
	vector<D3DMATERIAL9>       m_Mtrls;
	vector<IDirect3DTexture9*> m_Textures;

	D3DXMATRIX				   m_Rotation;
	D3DXMATRIX				   m_Transform;

	bool					   m_ShowWire;
	float					   m_PositionX;
	float					   m_PositionY;
	float					   m_PositionZ;

	IDirect3DDevice9*		   m_Device;
public:
	CCube(void);
	~CCube(void);

	void SetDevice(IDirect3DDevice9*		Device);
	bool LoadXFile(string FileName);
	void Draw();

	float GetPPositionX();
	float GetPPositionY();
	float GetPPositionZ();

	void SetPositon(float x, float y, float z);
	void SetRotation(float AngleX, float AngleY, float AngleZ);
	void SetShowWire(bool Wire);
	bool GetShowWire();

	bool TestHit(int x, int y);

	ID3DXMesh* m_Sphere;
	int  GetFaceColor(int Face);;
	int  GetFaceVertex(int Face, D3DXVECTOR3 p[]);
};
