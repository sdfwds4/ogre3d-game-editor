

#pragma once
 
using namespace Ogre;
 
class MaterialSwitcher;
 
/**
 We need this attached to the depth target, otherwise we get problems with the compositor
 MaterialSwitcher should NOT be running all the time - rather only when we're
 specifically rendering the target that needs it
*/
class SelectionRenderListener : public RenderTargetListener
{
public:
  SelectionRenderListener(MaterialSwitcher *switcher);
  ~SelectionRenderListener();
 
  virtual void preRenderTargetUpdate(const RenderTargetEvent &evt);
  virtual void postRenderTargetUpdate(const RenderTargetEvent &evt);

private:
	MaterialSwitcher *mMaterialListener;
};
 