

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "MaterialTool.h"

IMPLEMENT_DYNCREATE(CMaterialTool, CEditTool)

CMaterialTool::CMaterialTool() :mpParent(NULL)
{
	mName = "���ʹ���";

}

CMaterialTool::~CMaterialTool()
{

}

void CMaterialTool::BeginTool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}

void CMaterialTool::EndTool()
{

}