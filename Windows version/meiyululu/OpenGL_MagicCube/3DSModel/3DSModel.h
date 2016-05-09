// TriList.h: interface for the CTriList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRILIST_H__F4D23E1D_92BF_4F04_A498_761E19114FC0__INCLUDED_)
#define AFX_TRILIST_H__F4D23E1D_92BF_4F04_A498_761E19114FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include <vector>

using namespace std;


class C3DSModel  
{
public:
	C3DSModel();
	virtual ~C3DSModel();

	CNode* GetObjectByName(char* name);
	void RemoveAllObjects();
	void doAfterMath();
	void Init();
	int	 getNumObjects() {return numobjects;};

	void Draw();
	BOOL Add(CNode* _object);
	
private:
	int numobjects;
	int free;
	int maxobjects;
	vector<CNode*> Objects;	


};

#endif // !defined(AFX_TRILIST_H__F4D23E1D_92BF_4F04_A498_761E19114FC0__INCLUDED_)
