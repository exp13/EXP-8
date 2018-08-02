#pragma once
#include "stdafx.h"
#include "C8Core.h"
#include <fstream>


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
	dxR->UpdatePixels();
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

	// set appropriate quirk flags
	if (path.find("BLINKY") != std::string::npos)
	{
		load_quirk = true;
		shift_quirk = true;
	}
	else
	{
		load_quirk = false;
		shift_quirk = false;
	}

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
	opcode = (memory[pc] << 8) | memory[pc + 1];

	// Decode opcode
	unsigned short switchCode = (opcode & 0xF000);

	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x0F00)
			{
				case 0x0000:
					switch (opcode & 0x000F)
					{
						case 0x0000:
							OutputDebugString(L"Calling 0x00E0 \n");
							_0x00E0();	
							break;
						case 0x000E:
							OutputDebugString(L"Calling 0x00EE \n");
							_0x00EE();	
							break;
						default:
							OutputDebugString(L"Unknown opcode\n");
							break;
					}
					break;
				default:
					OutputDebugString(L"Calling 0x0NNN, NOT IMPLEMENTED \n");
					break;
			}
			break;
		case 0x1000:
			OutputDebugString(L"Calling 0x1NNN \n");
			_0x1NNN();
			break;
		case 0x2000:
			OutputDebugString(L"Calling 0x2NNN \n");
			_0x2NNN();		
			break;
		case 0x3000:
			OutputDebugString(L"Calling 0x3XNN \n");
			_0x3XNN();
			break;
		case 0x4000:
			OutputDebugString(L"Calling 0x4XNN \n");
			_0x4XNN();
			break;
		case 0x5000:
			OutputDebugString(L"Calling 0x5XY0 \n");
			_0x5XY0();
			break;
		case 0x6000:
			OutputDebugString(L"Calling 0x6XNN \n");
			_0x6XNN();
			break;
		case 0x7000:
			OutputDebugString(L"Calling 0x7XNN \n");
			_0x7XNN();
			break;
		case 0x8000:
			switch (opcode & 0x000F)
			{
				case 0x0000:
					OutputDebugString(L"Calling 0x8XY0 \n");
					_0x8XY0();	
					break;
				case 0x0001:
					OutputDebugString(L"Calling 0x8XY1 \n");
					_0x8XY1();
					break;
				case 0x0002:
					OutputDebugString(L"Calling 0x8XY2 \n");
					_0x8XY2();
					break;
				case 0x0003:
					OutputDebugString(L"Calling 0x8XY3 \n");
					_0x8XY3();
					break;
				case 0x0004:
					OutputDebugString(L"Calling 0x8XY4 \n");
					_0x8XY4();	
					break;
				case 0x0005:
					OutputDebugString(L"Calling 0x8XY5 \n");
					_0x8XY5();
					break;
				case 0x0006:
					OutputDebugString(L"Calling 0x8XY6 \n");
					_0x8XY6();
					break;
				case 0x0007:
					OutputDebugString(L"Calling 0x8XY7 \n");
					_0x8XY7();
					break;
				case 0x000E:
					OutputDebugString(L"Calling 0x8XYE \n");
					_0x8XYE();
					break;
				default:
					OutputDebugString(L"Unknown opcode\n");
					break;
			}
			break;
		case 0x9000:
			OutputDebugString(L"Calling 0x9XY0 \n");
			_0x9XY0();
			break;
		case 0xA000:
			OutputDebugString(L"Calling 0xANNN \n");
			_0xANNN();		
			break;
		case 0xB000:
			OutputDebugString(L"Calling 0xBNNN \n");
			_0xBNNN();
			break;
		case 0xC000:
			OutputDebugString(L"Calling 0xCXNN \n");
			_0xCXNN();
			break;
		case 0xD000:
			OutputDebugString(L"Calling 0xDXYN \n");
			_0xDXYN();		
			break;
		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x009E:
					OutputDebugString(L"Calling 0xEX9E \n");
					_0xEX9E();	
					break;
				case 0x00A1:
					OutputDebugString(L"Calling 0xEXA1 \n");
					_0xEXA1();
					break;
				default:
					OutputDebugString(L"Unknown opcode\n");
					break;
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF)
			{
				case 0x0007:
					OutputDebugString(L"Calling 0xFX07 \n");
					_0xFX07();	
					break;
				case 0x000A:
					OutputDebugString(L"Calling 0xFX0A \n");
					_0xFX0A();
					break;
				case 0x0015:
					OutputDebugString(L"Calling 0xFX15 \n");
					_0xFX15();
					break;
				case 0x0018:
					OutputDebugString(L"Calling 0xFX18 \n");
					_0xFX18();
					break;
				case 0x001E:
					OutputDebugString(L"Calling 0xFX1E \n");
					_0xFX1E();
					break;
				case 0x0029:
					OutputDebugString(L"Calling 0xFX29 \n");
					_0xFX29();
					break;
				case 0x0033:
					OutputDebugString(L"Calling 0xFX33 \n");
					_0xFX33();
					break;
				case 0x0055:
					OutputDebugString(L"Calling 0xFX55 \n");
					_0xFX55();
					break;
				case 0x0065:
					OutputDebugString(L"Calling 0xFX65 \n");
					_0xFX65();
					break;
				default:
					OutputDebugString(L"Unknown opcode\n");
					break;
			}
			break;
		default:
			OutputDebugString(L"Unknown opcode\n");
			break;
	}

	// Update timers
	clockCounter++;
	if (clockCounter >= 9)
	{
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

		clockCounter = 0;
	}
}

// Begin opcode functions
// -----------------------------------------------------------------------

void C8Core::_0x00E0()
{
	for (int i = 0; i < 2048;i++)
	{
		dxR->gfx[i] = 0;
	}

	dxR->UpdatePixels();

	pc += 2;
}

void C8Core::_0x00EE()
{
	--sp;
	pc = stack[sp] + 2;
	stack[sp] = 0;
}

void C8Core::_0x1NNN()
{
	pc = opcode & 0x0FFF;
}

void C8Core::_0x2NNN()
{
	stack[sp] = pc;
	++sp;
	pc = opcode & 0x0FFF;
}

void C8Core::_0x3XNN()
{
	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void C8Core::_0x4XNN()
{
	if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void C8Core::_0x5XY0()
{
	if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void C8Core::_0x6XNN()
{
	V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);

	pc += 2;
}

void C8Core::_0x7XNN()
{
	V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);

	pc += 2;
}

void C8Core::_0x8XY0()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];

	pc += 2;
}

void C8Core::_0x8XY1()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];

	pc += 2;
}

void C8Core::_0x8XY2()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];

	pc += 2;
}

void C8Core::_0x8XY3()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];

	pc += 2;
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

void C8Core::_0x8XY5()
{
	if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
	{
		V[0xF] = 0; // borrow
	}
	else
	{
		V[0xF] = 1;
	}

	V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];

	pc += 2;
}

void C8Core::_0x8XY6()
{
	if (shift_quirk)
	{
		V[0xF] = (V[(opcode & 0x0F00) >> 8] & 1);

		V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] >> 1);
	}
	else
	{
		V[0xF] = (V[(opcode & 0x00F0) >> 4] & 1);

		V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] >> 1);
	}

	pc += 2;
}

void C8Core::_0x8XY7()
{
	if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
	{
		V[0xF] = 0; // borrow
	}
	else
	{
		V[0xF] = 1;
	}

	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];

	pc += 2;
}

void C8Core::_0x8XYE()
{
	if (shift_quirk)
	{
		V[0xF] = (V[(opcode & 0x0F00) >> 8] & 128);

		V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] << 1);
	}
	else 
	{
		V[0xF] = (V[(opcode & 0x00F0) >> 4] & 128);

		V[(opcode & 0x00F0) >> 4] = (V[(opcode & 0x00F0) >> 4] << 1);
		V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
	}

	pc += 2;
}

void C8Core::_0x9XY0()
{
	if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void C8Core::_0xANNN()
{
	I = opcode & 0x0FFF;
	pc += 2;
}

void C8Core::_0xBNNN()
{
	pc = (opcode & 0x0FFF) + V[0x0];
}

void C8Core::_0xCXNN()
{
	V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x00FF);

	pc += 2;
}

void C8Core::_0xDXYN()
{
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	if (x > 63 || y > 31 || I > 4095)
	{
		OutputDebugString(L"draw out of range \n");
		return;
	}

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
	}

	// update frame since we changed gfx
	Render();
	// increment program counter
	pc += 2;
}

void C8Core::_0xEX9E()
{
	if (key[V[(opcode & 0x0F00) >> 8]] != 0)
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void C8Core::_0xEXA1()
{
	if (key[V[(opcode & 0x0F00) >> 8]] == 0)
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void C8Core::_0xFX07()
{
	V[(opcode & 0x0F00) >> 8] = delay_timer;

	pc += 2;
}

void C8Core::_0xFX0A()
{
	int downKey = -1;
	for (int i = 0; i < 16; i++)
	{
		if (key[i] != 0)
		{
			downKey = i;
			break;
		}
	}

	if (downKey > -1)
	{
		V[(opcode & 0x0F00) >> 8] = downKey;
		pc += 2;
	}
}

void C8Core::_0xFX15()
{
	delay_timer = V[(opcode & 0x0F00) >> 8];

	pc += 2;
}

void C8Core::_0xFX18()
{
	sound_timer = V[(opcode & 0x0F00) >> 8];

	pc += 2;
}

void C8Core::_0xFX1E()
{
	I += V[(opcode & 0x0F00) >> 8];

	pc += 2;
}

void C8Core::_0xFX29()
{
	I = V[(opcode & 0x0F00) >> 8] * 5;

	pc += 2;
}

void C8Core::_0xFX33()
{
	memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
	memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;

	pc += 2;
}

void C8Core::_0xFX55()
{
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
	{
		memory[I + i] = V[i];
	}

	if (load_quirk)
	{
		I += ((opcode & 0x0F00) >> 8);
	}

	pc += 2;
}

void C8Core::_0xFX65()
{
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
	{
		V[i] = memory[I + i];
	}

	if (load_quirk)
	{
		I += ((opcode & 0x0F00) >> 8);
	}

	pc += 2;
}
