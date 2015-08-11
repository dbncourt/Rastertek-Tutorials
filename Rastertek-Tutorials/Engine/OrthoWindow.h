////////////////////////////////////////////////////////////////////////////////
// Filename: OrthoWindow.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ORTHOWINDOW_H_
#define _ORTHOWINDOW_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: OrthoWindow
////////////////////////////////////////////////////////////////////////////////
class OrthoWindow
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	ULONG m_vertexCount;
	ULONG m_indexCount;

public:
	OrthoWindow();
	OrthoWindow(const OrthoWindow& other);
	~OrthoWindow();

	bool Initialize(ID3D11Device* device, int windowWidth, int windowHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
};
#endif