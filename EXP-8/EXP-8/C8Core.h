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
	// Functions for opcodes
	void _0x00E0(void);		// clears the screen NOT DONE
	void _0x00EE(void);		// returns from subroutine NOT DONE
	void _0x1NNN(void);		// jumps to address	NNN
	void _0x2NNN(void);		// calls subroutine at address 0x0NNN
	void _0x3XNN(void);		// skip next instruction if VX equals NN
	void _0x4XNN(void);		// skip next instruction if VX does not equal NN
	void _0x5XY0(void);		// skip next instruction if VX equals VY
	void _0x6XNN(void);		// set VX to NN
	void _0x7XNN(void);		// adds NN to VX (carry flag is not changed)
	void _0x8XY0(void);		// sets VX to the value of VY
	void _0x8XY1(void);		// sets VX to VX OR VY (bitwise OR)
	void _0x8XY2(void);		// sets VX to VX AND VY (bitwise AND)
	void _0x8XY3(void);		// sets VX to VX XOR VY (bitwise XOR)
	void _0x8XY4(void);		// add VX to VY, set VF to 1 if carry otherwise 0
	void _0x8XY5(void);		// sub VY from VX, set VF to 0 when borrow otherwise 1
	void _0x8XY6(void);		// shift VY right 1 and store in VX, VF is least bit before shift (VY unchanged)
	void _0x8XY7(void);		// set VX to VY - VX, set VF to 0 when borrow otherwise 1
	void _0x8XYE(void);		// shift VY left 1 and copy to VX, VF is highest bit before shift
	void _0x9XY0(void);		// skip next instruction if VX does not equal VY
	void _0xANNN(void);		// sets I to the address NNN
	void _0xBNNN(void);		// jump to the address NNN + V0
	void _0xCXNN(void);		// set VX to bitwise AND of rand(0-255) and NN
	void _0xDXYN(void);		// draws sprite at XY of height N, sprite stored at I
	void _0xEX9E(void);		// skips next instruction if key in VX is pressed
	void _0xEXA1(void);		// skips next instruction if key in VX isnt pressed
	void _0xFX07(void);		// set VX to the value of the delay timer
	void _0xFX0A(void);		// a key press is awaited and stored in VX (blocks until input)
	void _0xFX15(void);		// set delay timer to VX
	void _0xFX18(void);		// set sound timer to VX
	void _0xFX1E(void);		// adds VX to I
	void _0xFX29(void);		// set I to location of sprite for char in VX in fontset
	void _0xFX33(void);		// stores binary-coded decimal of VX at I, I+1, I+2 in memory[]
	void _0xFX55(void);		// store V0 to VX in memory starting at I. Offset I by 1 for each but I is unchanged
	void _0xFX65(void);		// fill V0 to VX from memory starting at I. Offset I by 1 for each but I is unchanged


	// Memory Map
	// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0 - F)
	// 0x200-0xFFF - Program ROM and work RAM

};

