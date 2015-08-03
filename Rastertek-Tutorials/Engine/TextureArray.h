////////////////////////////////////////////////////////////////////////////////
// Filename: TextureArray.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureArray
////////////////////////////////////////////////////////////////////////////////
class TextureArray
{
private:
	ID3D11ShaderResourceView* m_textures[3];

public:
	TextureArray();
	TextureArray(const TextureArray& other);
	~TextureArray();

	bool Initialize(ID3D11Device* device, WCHAR* baseTextureFileName, WCHAR* bumpMapTextureFileName, WCHAR* specularMapTextureFileName);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();
};

#endif