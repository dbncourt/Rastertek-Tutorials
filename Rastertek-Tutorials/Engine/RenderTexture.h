////////////////////////////////////////////////////////////////////////////////
// Filename: RenderTexture.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: RenderTexture
////////////////////////////////////////////////////////////////////////////////
class RenderTexture
{
private:
	int m_textureWidth;
	int m_textureHeight;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;

public:
	RenderTexture();
	RenderTexture(const RenderTexture& other);
	~RenderTexture();

	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, D3DXCOLOR color);
	ID3D11ShaderResourceView* GetShaderResourceView();

	void GetProjectionMatrix(_Out_ D3DXMATRIX& projectionMatrix);
	void GetOrthoMatrix(_Out_ D3DXMATRIX& orthoMatrix);

	int GetTextureWidth();
	int GetTextureHeight();
};
#endif