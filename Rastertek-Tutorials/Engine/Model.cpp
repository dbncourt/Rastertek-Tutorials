////////////////////////////////////////////////////////////////////////////////
// Filename: Model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Model.h"


Model::Model()
{
	this->m_vertexBuffer = nullptr;
	this->m_indexBuffer = nullptr;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* device)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = Model::InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	return true;
}

void Model::Shutdown()
{
	// Shutdown the vertex and index buffers
	Model::ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	Model::RenderBuffers(deviceContext);
}

int Model::GetIndexCount()
{
	return this->m_indexCount;
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;

	//Set the number of vertices in the vertex array
	this->m_vertexCount = 3;

	//Set the number of indices in the index array
	this->m_indexCount = 3;

	//Create the vertex array
	VertexType* vertices = new VertexType[this->m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//Create the index array
	UINT* indices = new UINT[this->m_indexCount];
	if (!indices)
	{
		return false;
	}

	//Load the vertex array with data
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); //Bottom left
	vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f); //Top middle
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); //Bottom right
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//Load the index array with data
	indices[0] = 0;	//Bottom left
	indices[1] = 1; //Top right
	indices[2] = 2; //Bottom right

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	//Set up the description of the static vertex buffer
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * this->m_vertexCount;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));

	//Give the sub-resource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	//Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));

	//Give the sub-resource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Model::ShutdownBuffers()
{
	//Release the Index Buffer
	if (this->m_indexBuffer)
	{
		this->m_indexBuffer->Release();
		this->m_indexBuffer = nullptr;
	}

	//Release the Vertex Buffer
	if (this->m_vertexBuffer)
	{
		this->m_vertexBuffer->Release();
		this->m_vertexBuffer = nullptr;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//Set vertex buffer stride and offset
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(this->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}