////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Graphics.h"


Graphics::Graphics()
{
	this->m_Direct3D = nullptr;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	//Create the Direct3D object
	this->m_Direct3D = new Direct3D();
	if (!this->m_Direct3D)
	{
		return false;
	}

	//Initialize the Direct3D object
	if (!this->m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	if (this->m_Direct3D)
	{
		this->m_Direct3D->Shutdown();
		delete this->m_Direct3D;
		this->m_Direct3D = nullptr;
	}
}

bool Graphics::Frame()
{
	bool result;

	//Render the graphics scene
	result = this->Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	//Clear the buffers to begin the scene
	this->m_Direct3D->BeginScene(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	//Present the rendered scene to the screen
	this->m_Direct3D->EndScene();


	return true;
}