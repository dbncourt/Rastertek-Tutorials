////////////////////////////////////////////////////////////////////////////////
// Filename: VerticalBlurShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VERTICALBLURSHADER_H_
#define _VERTICALBLURSHADER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: VerticalBlurShader
////////////////////////////////////////////////////////////////////////////////
class VerticalBlurShader
{

private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct ScreenSizeBufferType
	{
		float screenHeight;
		D3DXVECTOR3 padding;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11SamplerState* m_samplerState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_screenSizeBuffer;


public:
	VerticalBlurShader();
	VerticalBlurShader(const VerticalBlurShader& other);
	~VerticalBlurShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float screenHeight);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vertexShaderFileName, WCHAR* pixelShaderFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float screenHeight);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};
#endif