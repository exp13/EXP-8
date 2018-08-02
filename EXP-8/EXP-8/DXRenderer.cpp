#include "stdafx.h"
#include "DXRenderer.h"

// define screen resolution
#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	480

DXRenderer::DXRenderer(HWND mainWnd)
{
	for (int i = 0; i < 2048; i++)
	{
		gfx[i] = 0;
	}

	InitD3D(mainWnd);
	InitPipeline();
	InitGraphics();
}


DXRenderer::~DXRenderer()
{
	CleanD3D();
}

// This function initializes and prepares D3D for use
void DXRenderer::InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swapchain
	DXGI_SWAP_CHAIN_DESC scd;

	// clears out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the chain description struct
	scd.BufferCount = 1;									// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;					// set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;					// set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how swap chain is to be used
	scd.OutputWindow = hWnd;								// the window to be used
	scd.SampleDesc.Count = 4;								// how many multisamples
	scd.Windowed = TRUE;									// windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// allow full screen switching

															// create a device, device context, and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, nullptr, &devcon);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, nullptr, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, nullptr);

	// set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);
}

void DXRenderer::InitPipeline()
{
	// load and compile the two shaders
	ID3DBlob *VS, *PS;
	D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}

void DXRenderer::InitGraphics()
{
	// create vertices
	float Xdiff = 2.0f / 64.0f;
	float Ydiff = 2.0f / 32.0f;
	float vX = -1.0f;
	float vY = 1.0f;

	// TODO: Make OurVertices a private member and write method to update colors
	//		and the vertex buffer
	
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			// Vertices for strips go in this order: -x,-y -x,+y, +x,-y +x,+y
			OurVertices[y * 64 * 4 + x * 4].X = vX;
			OurVertices[y * 64 * 4 + x * 4].Y = vY - Ydiff;
			OurVertices[y * 64 * 4 + x * 4].Z = 0.0f;

			OurVertices[y * 64 * 4 + x * 4 + 1].X = vX;
			OurVertices[y * 64 * 4 + x * 4 + 1].Y = vY;
			OurVertices[y * 64 * 4 + x * 4 + 1].Z = 0.0f;

			OurVertices[y * 64 * 4 + x * 4 + 2].X = vX + Xdiff;
			OurVertices[y * 64 * 4 + x * 4 + 2].Y = vY - Ydiff;
			OurVertices[y * 64 * 4 + x * 4 + 2].Z = 0.0f;

			OurVertices[y * 64 * 4 + x * 4 + 3].X = vX + Xdiff;
			OurVertices[y * 64 * 4 + x * 4 + 3].Y = vY;
			OurVertices[y * 64 * 4 + x * 4 + 3].Z = 0.0f;

			// set color of 4 corners of pixel to white or black based on colorW
			for (int i = 0; i < 4; i++)
			{
				if (gfx[y * 64 + x] == 1)
				{
					OurVertices[y * 64 * 4 + x * 4 + i].Color[0] = 1.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[1] = 1.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[2] = 1.0f;
				}
				else
				{
					OurVertices[y * 64 * 4 + x * 4 + i].Color[0] = 0.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[1] = 0.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[2] = 0.0f;
				}
				OurVertices[y * 64 * 4 + x + i].Color[3] = 1.0f;
			}

			vX += Xdiff;
		}
		
		vX = -1.0f;
		vY -= Ydiff;
	}

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;				// write access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 64 * 32 * 4;	// size is the VERTEX struct * # of vertices
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);	// create the buffer

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));					// copy the data
	devcon->Unmap(pVBuffer, NULL);										// unmap the buffer
	
	RenderFrame();
}

void DXRenderer::UpdatePixels()
{
	float Xdiff = 2.0f / 64.0f;
	float Ydiff = 2.0f / 32.0f;
	float vX = -1.0f;
	float vY = 1.0f;

	// update colors of vertices
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			// set color of 4 corners of pixel to white or black based on colorW
			for (int i = 0; i < 4; i++)
			{
				if (gfx[y * 64 + x] == 1)
				{
					OurVertices[y * 64 * 4 + x * 4 + i].Color[0] = 1.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[1] = 1.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[2] = 1.0f;
				}
				else
				{
					OurVertices[y * 64 * 4 + x * 4 + i].Color[0] = 0.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[1] = 0.0f;
					OurVertices[y * 64 * 4 + x * 4 + i].Color[2] = 0.0f;
				}
				OurVertices[y * 64 * 4 + x + i].Color[3] = 1.0f;
			}

			vX += Xdiff;
		}

		vX = -1.0f;
		vY -= Ydiff;
	}

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));					// copy the data
	devcon->Unmap(pVBuffer, NULL);										// unmap the buffer

	RenderFrame();
}

// this is the function that cleans up D3D and COM
void DXRenderer::CleanD3D()
{
	swapchain->SetFullscreenState(FALSE, nullptr); // switch to windowed mode

	// close and release all existing COM objects
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	pLayout->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

// this is the function used to render a single frame
void DXRenderer::RenderFrame()
{
	// clear the back buffer to a deep blue
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);

	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// select which primitive type we are using
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// draw the vertex buffer to the back buffer
	//devcon->Draw(4, 0);
	//devcon->Draw(4, 4);

	// 64 * 32 * 4 = 8192
	for (int i = 0; i < 8192; i+=4)
	{
		devcon->Draw(4, i);
	}

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}