#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

// include Direct3D Library in Object file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

class DXRenderer
{
public:
	DXRenderer(HWND mainWnd);
	~DXRenderer();
	// Forward declarations of D3D11 functions
	void RenderFrame(void);			// render a single frame
	unsigned char gfx[64 * 32];		// pixel buffer

private:
	void InitD3D(HWND hWnd);		// sets up and initializes D3D
	void InitPipeline(void);		// initialize render pipeline
	void InitGraphics(void);		// initialize graphics
	void CleanD3D(void);			// closes D3D and releases memory

	// Global Declarations for D3D11, make sure to release all COMs in CleanD3D
	IDXGISwapChain * swapchain;							// the pointer to the swap chain interface
	ID3D11Device *dev;									// the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;						// the pointer to our Direct3D device context
	ID3D11RenderTargetView *backbuffer;					// the pointer to the backbuffer
	ID3D11VertexShader *pVS;							// the vertex shader
	ID3D11PixelShader *pPS;								// the pixel shader
	struct VERTEX { FLOAT X, Y, Z;  FLOAT Color[4]; };	// a struct to define a vertex
	ID3D11Buffer *pVBuffer;								// the vertex buffer
	ID3D11InputLayout *pLayout;							// the vertex input layout
};

