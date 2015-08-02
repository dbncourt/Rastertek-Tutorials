////////////////////////////////////////////////////////////////////////////////
// Filename: ColorShader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ColorShader.h"


ColorShader::ColorShader()
{
	this->m_vertexShader = nullptr;
	this->m_pixelShader = nullptr;
	this->m_inputLayout = nullptr;
	this->m_matrixBuffer = nullptr;
}

ColorShader::ColorShader(const ColorShader& other)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = ColorShader::InitializeShader(device, hwnd, L"ColorVertexShader.hlsl", L"ColorPixelShader.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}

void ColorShader::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	ColorShader::ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projecitonMatrix)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = ColorShader::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projecitonMatrix);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	ColorShader::RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vertexShaderFileName, WCHAR* pixelShaderFileName)
{
	HRESULT result;

	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	//Compile the vertex shader code.
	result = D3DX11CompileFromFile(vertexShaderFileName, nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertexShaderBuffer, &errorMessage, nullptr);
	if (FAILED(result))
	{
		//If the shader failed to compile, it should have written something to the error message
		if (errorMessage)
		{
			ColorShader::OutputShaderErrorMessage(errorMessage, hwnd, vertexShaderFileName);
		}
		//If there was nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, vertexShaderFileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Compile the pixel shader code
	result = D3DX11CompileFromFile(pixelShaderFileName, nullptr, nullptr, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixelShaderBuffer, &errorMessage, nullptr);
	if (FAILED(result))
	{
		//If the shader failed to compile it should have written something to the error message
		if (errorMessage)
		{
			ColorShader::OutputShaderErrorMessage(errorMessage, hwnd, pixelShaderFileName);
		}
		//If there was nothing in the error message then it simply could not find the file itself
		else
		{
			MessageBox(hwnd, pixelShaderFileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &this->m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &this->m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	ZeroMemory(polygonLayout, sizeof(polygonLayout));

	//Now setup the layout of the data that goes into the shader
	//This setup needs to match the VertexType structure in the ModelClass and in the shader
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].SemanticName = "POSITION";

	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].SemanticName = "COLOR";

	// Get a count of the elements in the layout.
	UINT numElements = sizeof(polygonLayout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	//Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &this->m_inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	//Release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &this->m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShader::ShutdownShader()
{
	//Release the Matrix Constant Buffer
	if (this->m_matrixBuffer)
	{
		this->m_matrixBuffer->Release();
		this->m_matrixBuffer = nullptr;
	}

	//Release the InputLayout
	if (this->m_inputLayout)
	{
		this->m_inputLayout->Release();
		this->m_inputLayout = nullptr;
	}

	//Release the PixelShader
	if (this->m_pixelShader)
	{
		this->m_pixelShader->Release();
		this->m_pixelShader = nullptr;
	}

	//Release the VertexShader
	if (this->m_vertexShader)
	{
		this->m_vertexShader->Release();
		this->m_vertexShader = nullptr;
	}
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
{
	char* compileErrors;
	ofstream fOut;

	//Get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//Get the length of the message
	ULONG bufferSize = errorMessage->GetBufferSize();

	//Open a file to write the error message in
	fOut.open("shader-error.txt");

	//Write out the error message
	for (ULONG i = 0; i < bufferSize; i++)
	{
		fOut << compileErrors[i];
	}

	//Close the file
	fOut.close();

	//Release the errorMessage
	errorMessage->Release();
	errorMessage = nullptr;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFileName, MB_OK);
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	HRESULT result;

	//Transpose the matrices to prepare them for the shader
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* matrixDataPtr;

	//Lock the constant buffer so it can be written to
	result = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get a pointer to the data in the constant buffer
	matrixDataPtr = (MatrixBufferType*)mappedResource.pData;

	//Copy the matrices into the constant buffer
	matrixDataPtr->world = worldMatrix;
	matrixDataPtr->view = viewMatrix;
	matrixDataPtr->projection = projectionMatrix;

	//Unlock the constant buffer
	deviceContext->Unmap(this->m_matrixBuffer, 0);

	//Finally set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(0, 1, &this->m_matrixBuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the vertex input layout
	deviceContext->IASetInputLayout(this->m_inputLayout);

	//Set the vertex and pixel shaders that will be used to render this model
	deviceContext->VSSetShader(this->m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(this->m_pixelShader, nullptr, 0);

	//Render the model
	deviceContext->DrawIndexed(indexCount, 0, 0);
}