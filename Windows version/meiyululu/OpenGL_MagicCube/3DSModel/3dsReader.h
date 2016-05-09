// 3dsReader.h: interface for the C3dsReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSREADER_H__D75DC1D0_3A9E_45A1_8B5D_6FE30B426A9E__INCLUDED_)
#define AFX_3DSREADER_H__D75DC1D0_3A9E_45A1_8B5D_6FE30B426A9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "glStructures.h"	//���ݽṹ����
#include "3DSModel.h"		//CTriList��˵��
#include "Node.h"		//CTriObject˵��
#include "basicRead.h"		//���������ݶ��뺯������

class C3DSReader  
{
public:
	C3DSReader();
	virtual ~C3DSReader();
	int ReadKFTrackTag(long fileSize, long tagStart, long tagSize, FILE * fp,  char* nodeName, Vector3* pivot, Chunk3DS chunk);
	int ReadKFObjectNode( long fileSize, long nodeStart, long nodeSize, FILE* fp);
	int ReadKFDATA( long fileSize, long kfdataStart, long kfdataSize, FILE* fp);
	int Read3DSChunk(FILE* fp, Chunk3DS& chunk);
	int Read3DSString(FILE* fp, char* name, int len = 256);
	int ReadPercentage(FILE* fp, float& value);
	int ReadColor(FILE* fp, float& red, float& green, float& blue);
	int ReadPointArray (CNode*	newchild, long fileSize, FILE *fp);
	int ReadFaceArray (CNode* newchild, long unsigned fileSize, FILE* fp);
	int ReadMeshMatGroup(CNode* newchild, MaterialDict* matdict, long fileSize, FILE* fp);
	int ReadTriObject(MaterialDict* matdict, long fileSize, FILE* fp, long triStart, long triSize, char* groupName);
	int ReadNamedObject(MaterialDict* matdict, long fileSize, long namedStart, long namedSize, FILE* fp);
	int ReadMatEntry(MaterialDict* matdict, long fileSize, long matStart, long matSize, FILE* fp);
	int ReadMDATA(MaterialDict* matdict, long fileSize, long mdataStart, long mdataSize, FILE* fp);
	int Read3DSFile(long fileSize, long fileStart, long fileLen, FILE* fp);
	int Is3DSFile(FILE* fp);
	BOOL Reader(const char* filename, C3DSModel* _list);
private:
	C3DSModel* DaList;

};

#endif // !defined(AFX_3DSREADER_H__D75DC1D0_3A9E_45A1_8B5D_6FE30B426A9E__INCLUDED_)
