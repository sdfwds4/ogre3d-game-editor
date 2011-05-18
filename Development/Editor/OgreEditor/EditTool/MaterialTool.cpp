

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "MaterialTool.h"

IMPLEMENT_DYNCREATE(CMaterialTool, CEditTool)

CMaterialTool::CMaterialTool() :mpParent(NULL)
{
	mName = "材质工具";

}

CMaterialTool::~CMaterialTool()
{

}

void CMaterialTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CMaterialTool::EndTool()
{

}