#include "Cube.h"

CCube::CCube(void)
{
	D3DXMatrixIdentity(&m_Rotation);
	D3DXMatrixIdentity(&m_Transform);

	m_ShowWire = false;
}

void CCube::SetShowWire(bool Wire)
{
	m_ShowWire = Wire;
}

bool CCube::GetShowWire()
{
	return m_ShowWire;
}

CCube::~CCube(void)
{
}

void CCube::SetDevice(IDirect3DDevice9*		Device)
{
	m_Device =  Device;
}

bool CCube::LoadXFile(string FileName)
{
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer  = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls   = 0;

	m_XFileName = FileName;
	int pos = m_XFileName.rfind('\\') + 1;
	string Path = m_XFileName.substr(0, pos);

	hr = D3DXLoadMeshFromX(  
		m_XFileName.c_str(),
		D3DXMESH_MANAGED,
		m_Device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&m_Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			m_Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != NULL )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = NULL;

				string TexName = "";
				TexName = Path + mtrls[i].pTextureFilename;
				D3DXCreateTextureFromFile(
					m_Device,
					TexName.c_str(),
					&tex);

				// save the loaded texture
				m_Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				m_Textures.push_back( 0 );
			}
		}
	}


	mtrlBuffer->Release();
	mtrlBuffer = NULL;

	//
	// Optimize the mesh.
	//

	hr = m_Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	adjBuffer->Release();
	adjBuffer = NULL;

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}

	D3DXCreateSphere(m_Device, CUBE_LEN*SPEER_RADIO_FAC, 20, 20, &m_Sphere, 0);
}

void CCube::Draw()
{
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, m_PositionX, m_PositionY, m_PositionZ);

	Tran = Tran*m_Rotation;

	m_Device->SetTransform(D3DTS_WORLD, &m_Transform);
	m_Device->BeginScene();

	/*if (m_ShowWire)
	{
		m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else*/
	{
		m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	for(int i = 0; i < m_Mtrls.size(); i++)
	{
		
		m_Device->SetMaterial( &m_Mtrls[i] );
		m_Device->SetTexture(0, m_Textures[i]);
		m_Mesh->DrawSubset(i);
	}	

	/*m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXVECTOR4 U(0.0, 0.0, 0.0, 1.0);
	D3DXVec4Transform(&U, &U, &m_Transform);
	D3DXMatrixTranslation(&Tran, U.x, U.y, U.z);
	m_Device->SetTransform(D3DTS_WORLD, &Tran);
	m_Sphere->DrawSubset(0);*/

	m_Device->EndScene();
}

void CCube::SetRotation(float AngleX, float AngleY, float AngleZ)
{
	
	D3DXMATRIX Tem;
	D3DXMatrixRotationX(&Tem, AngleX);
	m_Rotation = m_Rotation*Tem;

	D3DXMatrixRotationY(&Tem, AngleY);
	m_Rotation = m_Rotation*Tem;

	D3DXMatrixRotationZ(&Tem, AngleZ);
	m_Rotation = m_Rotation*Tem;

	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, m_PositionX, m_PositionY, m_PositionZ);
	m_Transform = Tran*m_Rotation;
}

void  CCube::SetPositon(float x, float y, float z)
{
	 m_PositionX = x;
	 m_PositionY = y;
	 m_PositionZ = z;

	 D3DXMatrixTranslation(&m_Transform, m_PositionX, m_PositionY, m_PositionZ);
}

float CCube::GetPPositionX()
{
	D3DXVECTOR4 V(0.0, 0.0, 0.0, 1.0);
	D3DXVec4Transform(&V, &V, &m_Transform);

	return V.x;
}


float CCube::GetPPositionY()
{
	D3DXVECTOR4 V(0.0, 0.0, 0.0, 1.0);
	D3DXVec4Transform(&V, &V, &m_Transform);

	return V.y;
}

float CCube::GetPPositionZ()
{
	D3DXVECTOR4 V(0.0, 0.0, 0.0, 1.0);
	D3DXVec4Transform(&V, &V, &m_Transform);

	return V.z;
}

bool  CCube::TestHit(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	m_Device->GetViewport(&vp);

	D3DXMATRIX proj;
	m_Device->GetTransform(D3DTS_PROJECTION, &proj);

	px = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	D3DXVECTOR3 origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 direction = D3DXVECTOR3(px, py, 1.0f);

	
	D3DXMATRIX view;
	m_Device->GetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse,	0, &view);

	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&origin,
		&origin,
		&viewInverse);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&direction,
		&direction,
		&viewInverse);

	// normalize the direction
	D3DXVec3Normalize(&direction, &direction);

	float radius = CUBE_LEN*SPEER_RADIO_FAC;
	D3DXVECTOR4 U(0.0, 0.0, 0.0, 1.0);
	D3DXVec4Transform(&U, &U, &m_Transform);

	D3DXVECTOR3 center(U.x, U.y, U.z);

	D3DXVECTOR3 v;
	v = origin - center;

	float b = 2.0f * D3DXVec3Dot(&direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (radius * radius);

	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if( discriminant < 0.0f )
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if( s0 >= 0.0f || s1 >= 0.0f )
		return true;
}

int CCube::GetFaceColor(int Face)
{
	if (Face < 1  || Face > CUBE_FACE_NUMBER)
	{
		return -1;
	}

	return Face;
}

int  CCube::GetFaceVertex(int Face, D3DXVECTOR3 p[])
{

//		     /4------------3/
//		    /              /|
//	       /       5      / |
//        /              /  |
//	     /1------------2/   |
//        |            |    |
//        |      1     | 2 / 7    
//        |            |  /
//        |5----------6| /
//
//


	if (Face < 1  || Face > CUBE_FACE_NUMBER)
	{
		return -1;
	}


	int L = CUBE_LEN;
	float Start = -L*0.5;
	float End   = L*0.5;

	D3DXVECTOR3 Vertexs[CUBE_VERT_NUMBER] = 
	{
		D3DXVECTOR3(Start, End, Start),  //1
		D3DXVECTOR3(End,   End, Start),
		D3DXVECTOR3(End,   End, End),
		D3DXVECTOR3(Start, End, End), //4
		D3DXVECTOR3(Start, Start, Start),
		D3DXVECTOR3(End,   Start, Start),//6
		D3DXVECTOR3(End,   Start, End),
		D3DXVECTOR3(Start, Start, End),
	};

	int FaceVertex[CUBE_FACE_NUMBER][3] = {
		{1, 2, 6},   // 1
		{6, 2, 3},
		{4, 3, 7},
		{5, 1, 4}, //4
		{1, 2, 4},
		{5, 6, 7},
	};

	int Index = Face - 1;

	for (int i=0; i<3; i++)
	{
		
		int k = FaceVertex[Index][i]-1;
		
		D3DXVECTOR4 V(Vertexs[k].x, Vertexs[k].y, Vertexs[k].z, 1.0);
		D3DXVec4Transform(&V, &V, &m_Transform);

		 p[i].x = V.x;  p[i].y = V.y;  p[i].z = V.z;
	}

	return 1;
}
