

#include "stdafx.h"
#include "SelectionRenderListener.h"
#include "MaterialSwitcher.h"

SelectionRenderListener::SelectionRenderListener(MaterialSwitcher *switcher) 
	: mMaterialListener(switcher)
{
}

SelectionRenderListener::~SelectionRenderListener()
{
}

void SelectionRenderListener::preRenderTargetUpdate(const RenderTargetEvent &evt)
{
	MaterialManager::getSingleton().addListener(mMaterialListener);
}

void SelectionRenderListener::postRenderTargetUpdate(const RenderTargetEvent &evt)
{
	MaterialManager::getSingleton().removeListener(mMaterialListener);
}