////////////////////////////////////////////////////////////////////////////////
// Filename: RenderTexture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "RenderTexture.h"


RenderTexture::RenderTexture()
{
	this->m_renderTargetTexture = nullptr;
	this->m_renderTargetView = nullptr;
	this->m_shaderResourceView = nullptr;
	this->m_depthStencilBuffer = nullptr;
	this->m_depthStencilView = nullptr;
}

RenderTexture::RenderTexture(const RenderTexture& other)
{
}


RenderTexture::~RenderTexture()
{
}

bool RenderTexture::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc;
	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	// Setup the render target texture description.
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Width = textureWidth;

	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, nullptr, &this->m_renderTargetTexture);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(this->m_renderTargetTexture, &renderTargetViewDesc, &this->m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	//Create the shader resource view
	result = device->CreateShaderResourceView(this->m_renderTargetTexture, &shaderResourceViewDesc, &this->m_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	// Initialize the description of the DepthBuffer
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.MiscFlags = 0;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.Width = textureWidth;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, nullptr, &this->m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(this->m_depthStencilBuffer, &depthStencilViewDesc, &this->m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
	ZeroMemory(&this->m_viewport, sizeof(D3D11_VIEWPORT));

	this->m_viewport.Height = (FLOAT)textureHeight;
	this->m_viewport.MaxDepth = 1.0f;
	this->m_viewport.MinDepth = 0.0f;
	this->m_viewport.TopLeftX = 0.0f;
	this->m_viewport.TopLeftY = 0.0f;
	this->m_viewport.Width = (FLOAT)textureWidth;

	// Setup the projection matrix.
	D3DXMatrixPerspectiveFovLH(&this->m_projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&this->m_orthoMatrix, (float)textureWidth, (float)textureHeight, screenNear, screenDepth);

	return true;
}

void RenderTexture::Shutdown()
{
	if (this->m_depthStencilView)
	{
		this->m_depthStencilView->Release();
		this->m_depthStencilView = nullptr;
	}

	if (this->m_depthStencilBuffer)
	{
		this->m_depthStencilBuffer->Release();
		this->m_depthStencilBuffer = nullptr;
	}

	if (this->m_shaderResourceView)
	{
		this->m_shaderResourceView->Release();
		this->m_shaderResourceView = nullptr;
	}

	if (this->m_renderTargetView)
	{
		this->m_renderTargetView->Release();
		this->m_renderTargetView = nullptr;
	}

	if (this->m_renderTargetTexture)
	{
		this->m_renderTargetTexture->Release();
		this->m_renderTargetTexture = nullptr;
	}
}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &this->m_renderTargetView, this->m_depthStencilView);

	// Set the viewport.
	deviceContext->RSSetViewports(1, &this->m_viewport);
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* deviceContext, D3DXCOLOR renderTargetViewColor)
{
	float color[4] = {
		renderTargetViewColor.r,
		renderTargetViewColor.g,
		renderTargetViewColor.b, 
		renderTargetViewColor.a };
	
	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(this->m_renderTargetView, renderTargetViewColor);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(this->m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


ID3D11ShaderResourceView* RenderTexture::GetShaderResourceView()
{
	return this->m_shaderResourceView;
}

void RenderTexture::GetOrthoMatrix(_Out_ D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = this->m_orthoMatrix;
}

void RenderTexture::GetProjectionMatrix(_Out_ D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = this->m_projectionMatrix;
}

int RenderTexture::GetTextureWidth()
{
	return this->m_textureWidth;
}

int RenderTexture::GetTextureHeight()
{
	return this->m_textureHeight;
}