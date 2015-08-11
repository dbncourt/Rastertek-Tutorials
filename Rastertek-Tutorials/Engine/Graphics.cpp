////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Graphics.h"


Graphics::Graphics()
{
	this->m_Direct3D = nullptr;
	this->m_Camera = nullptr;
	this->m_Model = nullptr;
	this->m_TextureShader = nullptr;
	this->m_HorizontalBlurShader = nullptr;
	this->m_VerticalBlurShader = nullptr;
	this->m_RenderTexture = nullptr;
	this->m_DownSampleTexture = nullptr;
	this->m_HorizontalBlurTexture = nullptr;
	this->m_VerticalBlurTexture = nullptr;
	this->m_UpSampleTexture = nullptr;
	this->m_SmallWindow = nullptr;
	this->m_FullScreenWindow = nullptr;
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

	// Create the Direct3D object.
	this->m_Direct3D = new Direct3D();
	if (!this->m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = this->m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the Camera object.
	this->m_Camera = new Camera();
	if (!this->m_Camera)
	{
		return false;
	}

	// Set the initial position of the Camera.
	this->m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -10.0f));

	// Create the Model object.
	this->m_Model = new Model();
	if (!this->m_Model)
	{
		return false;
	}

	// Initialize the Model object.
	result = this->m_Model->Initialize(this->m_Direct3D->GetDevice(), "cube.txt", L"seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Model object.", L"Error", MB_OK);
		return false;
	}

	// Create the TextureShader object.
	this->m_TextureShader = new TextureShader();
	if (!this->m_TextureShader)
	{
		return false;
	}

	// Initialize the TextureShader object.
	result = m_TextureShader->Initialize(this->m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the TextureShader object.", L"Error", MB_OK);
		return false;
	}

	// Create the HorizontalBlurShader object.
	this->m_HorizontalBlurShader = new HorizontalBlurShader();
	if (!this->m_HorizontalBlurShader)
	{
		return false;
	}

	// Initialize the HorizontalBlurShader object.
	result = this->m_HorizontalBlurShader->Initialize(this->m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the HorizontalBlurShader object.", L"Error", MB_OK);
		return false;
	}

	// Create the VerticalBlurShader object.
	this->m_VerticalBlurShader = new VerticalBlurShader();
	if (!this->m_VerticalBlurShader)
	{
		return false;
	}

	// Initialize the VerticalBlurShader object.
	result = this->m_VerticalBlurShader->Initialize(this->m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the VerticalBlurShader object.", L"Error", MB_OK);
		return false;
	}

	// Create the RenderTexture object.
	this->m_RenderTexture = new RenderTexture();
	if (!this->m_RenderTexture)
	{
		return false;
	}

	// Initialize the RenderTexture object.
	result = this->m_RenderTexture->Initialize(this->m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the RenderTexture object.", L"Error", MB_OK);
		return false;
	}

	// Create the DownSample RenderTexture object.
	this->m_DownSampleTexture = new RenderTexture();
	if (!this->m_DownSampleTexture)
	{
		return false;
	}

	int downSampleWidth = screenWidth / 2;
	int downSampleHeight = screenHeight / 2;

	// Initialize the DownSample RenderTexture object.
	result = this->m_DownSampleTexture->Initialize(this->m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the DownSample RenderTexture object.", L"Error", MB_OK);
		return false;
	}

	// Create the HorizontalBlur RenderTexture object.
	this->m_HorizontalBlurTexture = new RenderTexture();
	if (!this->m_HorizontalBlurTexture)
	{
		return false;
	}

	// Initialize the HorizontalBlur RenderTexture object.
	result = this->m_HorizontalBlurTexture->Initialize(this->m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the HorizontalBlur RenderTexture object.", L"Error", MB_OK);
		return false;
	}

	// Create the VerticalBlur RenderTexture object.
	this->m_VerticalBlurTexture = new RenderTexture();
	if (!this->m_VerticalBlurTexture)
	{
		return false;
	}

	// Initialize the VerticalBlur RenderTexture object.
	result = this->m_VerticalBlurTexture->Initialize(this->m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the VerticalBlur RenderTexture object.", L"Error", MB_OK);
		return false;
	}

	// Create the UpSample RenderTexture object.
	this->m_UpSampleTexture = new RenderTexture();
	if (!this->m_UpSampleTexture)
	{
		return false;
	}

	// Initialize the UpSample RenderTexture object.
	result = this->m_UpSampleTexture->Initialize(this->m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the UpSample RenderTexture object.", L"Error", MB_OK);
		return false;
	}

	// Create the Small OrthoWindow object.
	this->m_SmallWindow = new OrthoWindow();
	if (!this->m_SmallWindow)
	{
		return false;
	}

	// Initialize the Small OrthoWindow object.
	result = this->m_SmallWindow->Initialize(this->m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Small OrthoWindow object.", L"Error", MB_OK);
		return false;
	}

	// Create the FullScreen OrthoWindow object.
	this->m_FullScreenWindow = new OrthoWindow();
	if (!this->m_FullScreenWindow)
	{
		return false;
	}

	// Initialize the FullScreen OrthoWindow object.
	result = this->m_FullScreenWindow->Initialize(this->m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the FullScreen OrthoWindow object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Shutdown()
{
	// Release the FullScreen OrthoWindow object.
	if (this->m_FullScreenWindow)
	{
		this->m_FullScreenWindow->Shutdown();
		delete this->m_FullScreenWindow;
		this->m_FullScreenWindow = nullptr;
	}

	// Release the Small OrthoWindow object.
	if (this->m_SmallWindow)
	{
		this->m_SmallWindow->Shutdown();
		delete this->m_SmallWindow;
		this->m_SmallWindow = nullptr;
	}

	// Release the UpSample RenderTexture object.
	if (this->m_UpSampleTexture)
	{
		this->m_UpSampleTexture->Shutdown();
		delete m_UpSampleTexture;
		m_UpSampleTexture = nullptr;
	}

	// Release the VerticalBlur RenderTexture object.
	if (this->m_VerticalBlurTexture)
	{
		this->m_VerticalBlurTexture->Shutdown();
		delete this->m_VerticalBlurTexture;
		this->m_VerticalBlurTexture = nullptr;
	}

	// Release the HorizontalBlur RenderTexture object.
	if (this->m_HorizontalBlurTexture)
	{
		this->m_HorizontalBlurTexture->Shutdown();
		delete this->m_HorizontalBlurTexture;
		this->m_HorizontalBlurTexture = nullptr;
	}

	// Release the DownSample RenderTexture object.
	if (this->m_DownSampleTexture)
	{
		this->m_DownSampleTexture->Shutdown();
		delete this->m_DownSampleTexture;
		this->m_DownSampleTexture = nullptr;
	}

	// Release the RenderTexture object.
	if (this->m_RenderTexture)
	{
		this->m_RenderTexture->Shutdown();
		delete this->m_RenderTexture;
		this->m_RenderTexture = nullptr;
	}

	// Release the VerticalBlurShader object.
	if (this->m_VerticalBlurShader)
	{
		this->m_VerticalBlurShader->Shutdown();
		delete this->m_VerticalBlurShader;
		this->m_VerticalBlurShader = nullptr;
	}

	// Release the HorizontalBlurShader object.
	if (this->m_HorizontalBlurShader)
	{
		this->m_HorizontalBlurShader->Shutdown();
		delete this->m_HorizontalBlurShader;
		this->m_HorizontalBlurShader = nullptr;
	}

	// Release the TextureShader object.
	if (this->m_TextureShader)
	{
		this->m_TextureShader->Shutdown();
		delete this->m_TextureShader;
		this->m_TextureShader = nullptr;
	}

	// Release the Model object.
	if (this->m_Model)
	{
		this->m_Model->Shutdown();
		delete this->m_Model;
		this->m_Model = nullptr;
	}

	// Release the Camera object.
	if (this->m_Camera)
	{
		delete this->m_Camera;
		this->m_Camera = nullptr;
	}

	// Release the D3D object.
	if (this->m_Direct3D)
	{
		this->m_Direct3D->Shutdown();
		delete this->m_Direct3D;
		this->m_Direct3D = nullptr;
	}

	return;
}


bool Graphics::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Graphics::Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool Graphics::Render(float rotation)
{
	bool result;


	// First render the scene to a render texture.
	result = Graphics::RenderSceneToTexture(rotation);
	if (!result)
	{
		return false;
	}

	// Next down sample the render texture to a smaller sized texture.
	result = Graphics::DownSampleTexture();
	if (!result)
	{
		return false;
	}

	// Perform a horizontal blur on the down sampled render texture.
	result = Graphics::RenderHorizontalBlurToTexture();
	if (!result)
	{
		return false;
	}

	// Now perform a vertical blur on the horizontal blur render texture.
	result = Graphics::RenderVerticalBlurToTexture();
	if (!result)
	{
		return false;
	}

	// Up sample the final blurred render texture to screen size again.
	result = Graphics::UpSampleTexture();
	if (!result)
	{
		return false;
	}

	// Render the blurred up sampled render texture to the screen.
	result = Graphics::Render2DTextureScene();
	if (!result)
	{
		return false;
	}

	return true;
}


bool Graphics::RenderSceneToTexture(float rotation)
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;

	// Set the render target to be the render to texture.
	this->m_RenderTexture->SetRenderTarget(this->m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	this->m_RenderTexture->ClearRenderTarget(this->m_Direct3D->GetDeviceContext(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	this->m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the cube will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->m_Model->Render(this->m_Direct3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = this->m_TextureShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, this->m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	this->m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	this->m_Direct3D->ResetViewport();

	return true;
}


bool Graphics::DownSampleTexture()
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX orthoMatrix;

	// Set the render target to be the render to texture.
	this->m_DownSampleTexture->SetRenderTarget(this->m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	this->m_DownSampleTexture->ClearRenderTarget(this->m_Direct3D->GetDeviceContext(), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	this->m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	this->m_DownSampleTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	this->m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->m_SmallWindow->Render(this->m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = this->m_TextureShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, this->m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	this->m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	this->m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	this->m_Direct3D->ResetViewport();

	return true;
}


bool Graphics::RenderHorizontalBlurToTexture()
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX orthoMatrix;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	float screenSizeX = (float)this->m_HorizontalBlurTexture->GetTextureWidth();

	// Set the render target to be the render to texture.
	this->m_HorizontalBlurTexture->SetRenderTarget(this->m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	this->m_HorizontalBlurTexture->ClearRenderTarget(this->m_Direct3D->GetDeviceContext(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	this->m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	this->m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	this->m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->m_SmallWindow->Render(this->m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = this->m_HorizontalBlurShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, this->m_DownSampleTexture->GetShaderResourceView(), screenSizeX);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	this->m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	this->m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	this->m_Direct3D->ResetViewport();

	return true;
}


bool Graphics::RenderVerticalBlurToTexture()
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX orthoMatrix;

	// Store the screen height in a float that will be used in the vertical blur shader.
	float screenSizeY = (float)this->m_VerticalBlurTexture->GetTextureHeight();

	// Set the render target to be the render to texture.
	this->m_VerticalBlurTexture->SetRenderTarget(this->m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	this->m_VerticalBlurTexture->ClearRenderTarget(this->m_Direct3D->GetDeviceContext(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	this->m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	this->m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	this->m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->m_SmallWindow->Render(this->m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	result = this->m_VerticalBlurShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, this->m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	this->m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	this->m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	this->m_Direct3D->ResetViewport();

	return true;
}


bool Graphics::UpSampleTexture()
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX orthoMatrix;

	// Set the render target to be the render to texture.
	this->m_UpSampleTexture->SetRenderTarget(this->m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	this->m_UpSampleTexture->ClearRenderTarget(this->m_Direct3D->GetDeviceContext(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	this->m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	this->m_UpSampleTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	this->m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->m_FullScreenWindow->Render(this->m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = this->m_TextureShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_FullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, this->m_VerticalBlurTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	this->m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	this->m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	this->m_Direct3D->ResetViewport();

	return true;
}


bool Graphics::Render2DTextureScene()
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX orthoMatrix;

	// Clear the buffers to begin the scene.
	this->m_Direct3D->BeginScene(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));

	// Generate the view matrix based on the camera's position.
	this->m_Camera->Render();

	// Get the world, view, and ortho matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	this->m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->m_FullScreenWindow->Render(this->m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	result = this->m_TextureShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_FullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, this->m_UpSampleTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	this->m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	this->m_Direct3D->EndScene();

	return true;
}