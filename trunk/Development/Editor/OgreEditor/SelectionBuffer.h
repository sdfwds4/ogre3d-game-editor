


#pragma once

using namespace Ogre;

class MaterialSwitcher;
class SelectionRenderListener;

class SelectionBuffer
{
public:
	SelectionBuffer( );
	~SelectionBuffer( );

	Entity *OnSelectionClick(int x, int y);
	void ShowOverlay(bool show);

private:
	SceneManager *mSceneMgr;
	Camera *mCamera;
	RenderTarget *mRenderTarget;

	// This is the material listener - Note: it is controlled by a separate
	// RenderTargetListener, not applied globally to all targets
	MaterialSwitcher *mMaterialSwitchListener;

	SelectionRenderListener *mSelectionTargetListener;

	TexturePtr mTexture;
	RenderTexture *mRenderTexture;
	uint8 *mBuffer;
	PixelBox *mPixelBox;
	Overlay *mSelectionDebugOverlay;

	void CreateRTTOverlays();
	void Update();
	void UpdateBufferSize();
};

