////////////////////////////////////////////////////////////////////////////////
// Filename: GraphicsClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Direct3D.h"
#include "Camera.h"
#include "Model.h"
#include "TextureShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "RenderTexture.h"
#include "OrthoWindow.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
private:
	Direct3D* m_Direct3D;
	Camera* m_Camera;
	Model* m_Model;
	TextureShader* m_TextureShader;
	HorizontalBlurShader* m_HorizontalBlurShader;
	VerticalBlurShader* m_VerticalBlurShader;
	RenderTexture *m_RenderTexture;
	RenderTexture *m_DownSampleTexture;
	RenderTexture *m_HorizontalBlurTexture;
	RenderTexture *m_VerticalBlurTexture;
	RenderTexture *m_UpSampleTexture;
	OrthoWindow *m_SmallWindow;
	OrthoWindow* m_FullScreenWindow;

public:
	Graphics();
	Graphics(const Graphics& other);
	~Graphics();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render(float rotation);

	bool RenderSceneToTexture(float rotation);
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool Render2DTextureScene();
};

#endif