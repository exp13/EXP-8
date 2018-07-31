#pragma once
#include "stdafx.h"
#include "C8Core.h"
#include <fstream>


C8Core::C8Core(HWND mainWnd)
{
	dxR = new DXRenderer(mainWnd);

	// Render beginning frame
	dxR->RenderFrame();
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
	// might need to figure out how to read in unsigned char
	// holding on to this till im sure I want to delete it
	/*char buffer[4096 - 512] = { 0 };
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
	}*/

	// open file
	std::ifstream inf(path.c_str(), std::ifstream::binary);
	// get length of file and set buffer to file length
	inf.seekg(0, inf.end);
	int length = inf.tellg();
	inf.seekg(0, inf.beg);
	char * buffer = new char[length];

	inf.read(buffer, length);

	if (inf)
	{
		OutputDebugString(L"all characters read successfully\n");
	}
	else
	{
		OutputDebugString(L"file not fully read\n");
	}

	
	for (int i = 0; i < length; i++)
	{
		memory[i + 512] = (unsigned char) buffer[i];
	}

	delete[] buffer;
	inf.close();
}

void C8Core::EmuCycle()
{
	// Fetch opcode
	opcode = memory[pc] | memory[pc + 1];

	// Decode opcode
	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x000F)
			{
				case 0x0000:
					//_0x00E0();	// clears the screen
					break;
				case 0x000E:
					//_0x00EE();	// returns from subroutine
					break;
				default:
					OutputDebugString(L"Unknown opcode\n");
					break;
			}
			break;
		case 0x2000:
			_0x2NNN();		// calls subroutine at address 0x0NNN
			break;
		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0004:
					_0x8XY4();	// add VX to VY, set VF to 1 if carry otherwise 0
					break;
				default:
					OutputDebugString(L"Unknown opcode\n");
					break;
			}
			break;
		case 0xA000:
			_0xANNN();		// sets I to the address NNN
			break;
		case 0xD000:
			_0xDXYN();		// draws sprite at XY of height N, sprite stored at I
			break;
		default:
			OutputDebugString(L"Unknown opcode\n");
			break;
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

// Begin opcode functions
// -----------------------------------------------------------------------

void C8Core::_0xDXYN()
{
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	V[0xF] = 0;
	for (int yL = 0; yL < height; yL++)
	{
		pixel = memory[I + yL];
		for (int xL = 0; xL < 8; xL++)
		{
			// check if pixel is 1 or 0, (0x80 >> xL) scans 1 at a time
			if ((pixel & (0x80 >> xL)) != 0)
			{
				// if gfx value is already 1, set collision flag on VF
				if (dxR->gfx[((y + yL) * 64 + x + xL)] == 1)
				{
					V[0xF] = 1;
				}
				dxR->gfx[((y + yL) * 64 + x + xL)] ^= 1;
			}
		}

		// update frame since we changed gfx
		dxR->RenderFrame();
		// increment program counter
		pc += 2;
	}
}

void C8Core::_0xANNN()
{
	I = opcode & 0x0FFF;
	pc += 2;
}

void C8Core::_0x2NNN()
{
	stack[sp] = pc;
	++sp;
	pc = opcode & 0x0FFF;
}

void C8Core::_0x8XY4()
{
	if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
	{
		V[0xF] = 1; // carry
	}
	else
	{
		V[0xF] = 0;
	}

	V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
	pc += 2;
}