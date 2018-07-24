#pragma once
#include "DXRenderer.h"

class C8Core
{
public:
	C8Core(HWND mainWnd);
	~C8Core();
	void Render(void);
	void Initialize(void);
	void Load(std::string path);
	void EmuCycle(void);

private:
	// current opcode, opcodes are 2 bytes (unsigned short)
	unsigned short opcode;
	// 4K memory, unsigned chars are 1 byte
	unsigned char memory[4096];
	// 16 CPU Registers, V1-VE, VF is carry flag, 1 byte each
	unsigned char V[16];
	// Index register (values 0x000 - 0xFFF)
	unsigned short I;
	// program counter (values 0x000 - 0xFFF)
	unsigned short pc;
	// Timers that count at 60hz, when set above 0 they count down to 0
	unsigned char delay_timer;
	// the system buzzer sounds when the sound_timer reaches 0, *Needs Implementation*
	unsigned char sound_timer;
	// stack and stack pointer to remember addresses for subroutine jumps
	unsigned short stack[16];
	unsigned short sp;
	// current state of input key (values 0x0-0xF)
	unsigned char key[16];
	// object pointer for DXRenderer class for gfx
	DXRenderer *dxR;
	// CHIP-8 fontset
	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	// Memory Map
	// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0 - F)
	// 0x200-0xFFF - Program ROM and work RAM

};

