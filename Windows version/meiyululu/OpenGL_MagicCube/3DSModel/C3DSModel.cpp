// TriList.cpp: implementation of the CTriList class.
//
//////////////////////////////////////////////////////////////////////

#include "3DSModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DSModel::C3DSModel()
{
	numobjects = 0;
}

C3DSModel::~C3DSModel()
{
	for (int i=0; i <Objects.size(); i++)
	{
		if (Objects[i] != NULL)
		{
			delete (Objects[i]);
		}
	}
}

BOOL C3DSModel::Add(CNode * _object)
{
	_object->ApplyNormals();
	Objects.push_back(_object);

	return true;
}

void C3DSModel::Draw()
{
	for (int i=0; i <Objects.size(); i++)
	{
		Objects[i]->Draw();
	}	
}



void C3DSModel::Init()
{
	Objects.clear();
}

void C3DSModel::doAfterMath()
{
	for (int i=0; i <numobjects; i++)
	{
		Objects[i]->ApplyNormals();
	}
}

void C3DSModel::RemoveAllObjects()
{
	for (int i=0; i <Objects.size(); i++)
	{
		delete (Objects[i]);
	}

	Objects.clear();
}

CNode* C3DSModel::GetObjectByName(char * name)
{
	char* objname;
	for (int i=0; i <numobjects; i++)
	{
		Objects[i]->getName(objname);
		if(strcmp(name, objname) == 0) 
		{
			return Objects[i];
		}
	}
	
	return NULL;
}
