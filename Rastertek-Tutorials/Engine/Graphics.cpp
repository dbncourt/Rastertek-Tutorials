////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Graphics.h"


Graphics::Graphics()
{
	this->m_Direct3D = nullptr;
	this->m_Camera = nullptr;
	this->m_Model = nullptr;
	this->m_LightMapShader = nullptr;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//Create the Direct3D object
	this->m_Direct3D = new Direct3D();
	if (!this->m_Direct3D)
	{
		return false;
	}

	//Initialize the Direct3D object
	result = this->m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create the Camera object
	this->m_Camera = new Camera();
	if (!this->m_Camera)
	{
		return false;
	}

	//Set the initial position of the camera
	this->m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -10.0f));

	//Create the Model object
	this->m_Model = new Model();
	if (!this->m_Model)
	{
		return false;
	}

	//Initialize the Model object
	result = this->m_Model->Initialize(this->m_Direct3D->GetDevice(), "square.txt", L"stone01.dds", L"light01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Model object", L"Error", MB_OK);
		return false;
	}

	//Create the LightMapShader object
	this->m_LightMapShader = new LightMapShader();
	if (!this->m_LightMapShader)
	{
		return false;
	}

	//Initialize the LightMapShader object
	result = this->m_LightMapShader->Initialize(this->m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the LightMapShader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	//Release the LightMapShader object
	if (this->m_LightMapShader)
	{
		this->m_LightMapShader->Shutdown();
		delete this->m_LightMapShader;
		this->m_LightMapShader = nullptr;
	}

	//Release the Model object
	if (this->m_Model)
	{
		this->m_Model->Shutdown();
		delete this->m_Model;
		this->m_Model = nullptr;
	}

	//Release the Camera object
	if (this->m_Camera)
	{
		delete this->m_Camera;
		this->m_Camera = nullptr;
	}

	//Release the Direct3D object
	if (this->m_Direct3D)
	{
		this->m_Direct3D->Shutdown();
		delete this->m_Direct3D;
		this->m_Direct3D = nullptr;
	}
}

bool Graphics::Frame()
{
	//Set the position of the Camera
	this->m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));

	return true;
}

bool Graphics::Render()
{
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;

	// Clear the buffers to begin the scene
	this->m_Direct3D->BeginScene(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position
	this->m_Camera->Render();

	// Get the World, View and Projection matrices from the Camera and Direct3D objects
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	this->m_Model->Render(this->m_Direct3D->GetDeviceContext());

	// Render the model using the LightMapShader
	this->m_LightMapShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, this->m_Model->GetTextureArray());

	// Present the rendered scene to the screen
	this->m_Direct3D->EndScene();

	return true;
}