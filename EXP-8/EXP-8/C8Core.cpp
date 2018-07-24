#include "stdafx.h"
#include "C8Core.h"


C8Core::C8Core(HWND mainWnd)
{
	dxR = new DXRenderer(mainWnd);
}


C8Core::~C8Core()
{
	delete dxR;
}

void C8Core::Render()
{
	dxR->RenderFrame();
}

void C8Core::Initialize()
{
	pc = 0x200;			// program counter starts at 0x200
	opcode = 0;			// reset current opcode
	I = 0;				// reset index register
	sp = 0;				// reset stack pointer

	// Clear display (handled in dxR constructor)

	// Clear stack and registers
	for (int i = 0; i < 16; i++)
	{
		stack[i] = 0;
		V[i] = 0;
	}

	// Clear memory
	for (int i = 0; i < 4096; i++)
	{
		memory[i] = 0;
	}
	
	// Load fontset
	for (int i = 0; i < 80; i++)
	{
		memory[i] = chip8_fontset[i];
	}

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;
}

void C8Core::Load(std::string path)
{
	char buffer[4096 - 512] = { 0 };
	FILE *pROM;
	fopen_s(&pROM, path.c_str(), "rb");

	if (pROM == NULL) OutputDebugString(L"Error opening file\n");
	else
	{
		OutputDebugString(L"Loaded file \n");
		while (!feof(pROM))
		{
			if (fgets(buffer, (4096 - 512), pROM) == NULL) break;
		}
		fclose(pROM);
	}
	
	for (int i = 0; i < (4096 - 512); i++)
	{
		memory[i + 512] = buffer[i];
	}
	
}

void C8Core::EmuCycle()
{
	// Fetch opcode
	opcode = memory[pc] | memory[pc + 1];

	// Decode opcode
	switch (opcode & 0xF000)
	{

	}

	// Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
		{
			// put audio beep here
		}
		--sound_timer;
	}
}