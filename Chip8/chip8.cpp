#include "chip8.h"
#include <fstream> // file loader
// computes each instructtion according to the Chip8 system
Chip8::Chip8() {}
Chip8::~Chip8() {}

uint8_t chip8Fonts[80] =
{	// tiny font
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

void Chip8::Tick()
{
	uint16_t opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode >> 12)
	{
	case 0x00:
		Proc0(opcode);
		break;
	case 0x01:
		Proc1(opcode);
		break;
	case 0x02:
		Proc2(opcode);
		break;
	case 0x03:
		Proc3(opcode);
		break;
	case 0x04:
		Proc4(opcode);
		break;
	case 0x05:
		Proc5(opcode);
		break;
	case 0x06:
		Proc6(opcode);
		break;
	case 0x07:
		Proc7(opcode);
		break;
	case 0x08:
		Proc8(opcode);
		break;
	case 0x09:
		Proc9(opcode);
		break;
	case 0x0A:
		ProcA(opcode);
		break;
	case 0x0B:
		ProcB(opcode);
		break;
	case 0x0C:
		ProcC(opcode);
		break;
	case 0x0D:
		ProcD(opcode);
		break;
	case 0x0E:
		ProcE(opcode);
		break;
	case 0x0F:
		ProcF(opcode);
		break;
	default:
		break;
	}
}

void Chip8::TicksPer60FPS()
{
	if (delayTimer > 0)
		delayTimer--;
}

void Chip8::Init()
{
	for (int i = 0; i < MEMORY_SIZE; i++)
		memory[i] = 0;

	for (int i = 0; i < REGISTER_SIZE; i++)
		v[i] = 0;

	this->i = 0;
	pc = START_ADDRESS;

	for (int i = 0; i < STACK_SIZE; i++)
		stack[i] = 0;

	this->sp = 0;
	this->delayTimer = 0;

	for (int y = 0; y < SCREEN_HEIGHT; y++)
		for (int x = 0; x < SCREEN_WIDTH; x++)
			videoBuffer[y][x] = false;

	for (int i = 0; i < KEYBOARD_KEYS_SIZE; i++)
		keyboard[i] = 0;

	for (int i = 0; i < MEMORY_SIZE; ++i)
		memory[i] = 0;

	for (int i = 0; i < 80; ++i)
		memory[i] = chip8Fonts[i];

	srand(static_cast<unsigned int>(time(nullptr)));
}

bool Chip8::loadRom(const char* filename)
{
	Init();

	std::ifstream file;
	size_t fileSize;

	file.open(filename, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();

		if (fileSize > 0xFFF - START_ADDRESS)
		{
			file.close();
			return false;
		}
		else
		{
			char* data= (char*)(&(memory[START_ADDRESS]));
			file.seekg(0, std::ios::beg);
			file.read(data, fileSize);
			file.close();
			return true;
		}
	}
	else
		return false;
	
	return true;
}
// all operations are explained at the link below :
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
void Chip8::Proc0(const uint16_t &opcode)
{
	uint8_t nn = opcode & 0x00FF;
	// CLS
	if (nn == 0xE0)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y++)
			for (int x = 0; x < SCREEN_WIDTH; x++)
				videoBuffer[y][x] = false;
			
		pc += 2;
	}
	else if (nn == 0xEE) // RET
	{
		--sp;
		pc = stack[sp];
		pc += 2;
	}
}

void Chip8::Proc1(const uint16_t &opcode)
{	// JMP to new address
	pc = opcode & 0x0FFF;
}

void Chip8::Proc2(const uint16_t &opcode)
{	// CALL subroutine
	stack[sp] = pc;
	++sp;
	pc = opcode & 0x0FFF;
}

void Chip8::Proc3(const uint16_t &opcode)
{	// SE
	v[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF) ? pc += 4 : pc += 2;
}

void Chip8::Proc4(const uint16_t &opcode)
{	// SNE
	v[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF) ? pc += 4 : pc += 2;
}

void Chip8::Proc5(const uint16_t &opcode)
{	// SE
	v[(opcode & 0x0F00) >> 8] == v[(opcode & 0x00F0) >> 4] ? pc += 4 : pc += 2;
}

void Chip8::Proc6(const uint16_t &opcode)
{	// LD
	v[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
	pc += 2;
}

void Chip8::Proc7(const uint16_t &opcode)
{	// ADD
	v[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
	pc += 2;
}

void Chip8::Proc8(const uint16_t &opcode)
{	// arithmetic operators
	switch (opcode & 0x000F)
	{
	case 0x0:
		v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x00F0) >> 4];
		pc += 2; // LD
		break;
	case 0x1:
		v[(opcode & 0x0F00) >> 8] |= v[(opcode & 0x00F0) >> 4];
		pc += 2; // OR
		break;
	case 0x2:
		v[(opcode & 0x0F00) >> 8] &= v[(opcode & 0x00F0) >> 4];
		pc += 2; // AND
		break;
	case 0x3:
		v[(opcode & 0x0F00) >> 8] ^= v[(opcode & 0x00F0) >> 4];
		pc += 2; // XOR
		break;
	case 0x4:
		v[(opcode & 0x0F00) >> 8] > (0xFF - v[(opcode & 0x00F0) >> 4]) ? v[0xF] = 1 : v[0xF] = 0;
		v[(opcode & 0x0F00) >> 8] += v[(opcode & 0x00F0) >> 4];
		pc += 2; // ADD
		break;
	case 0x5:
		v[(opcode & 0x0F00) >> 8] > v[(opcode & 0x00F0) >> 4] ? v[0xF] = 1 : v[0xF] = 0;
		v[(opcode & 0x0F00) >> 8] -= v[(opcode & 0x00F0) >> 4];
		pc += 2; // SUB
		break;
	case 0x6:
		v[0xF] = v[(opcode & 0x0F00) >> 8] & 0x1;
		v[(opcode & 0x0F00) >> 8] >>= 1;
		pc += 2; // SHR
		break;
	case 0x7:
		v[(opcode & 0x00F0) >> 4] > v[(opcode & 0x0F00) >> 8] ? v[0xF] = 1 : v[0xF] = 0;
		v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x00F0) >> 4] - v[(opcode & 0x0F00) >> 8];
		pc += 2; // SUBN
		break;
	case 0xE:
		v[0xF] = v[(opcode & 0x0F00) >> 8] >> 7;
		v[(opcode & 0x0F00) >> 8] <<= 1;
		pc += 2; // SHL
		break;
	}
}

void Chip8::Proc9(const uint16_t &opcode)
{	// SNE
	v[(opcode & 0x0F00) >> 8] != v[(opcode & 0x00F0) >> 4] ? pc += 4 : pc += 2;
}

void Chip8::ProcA(const uint16_t &opcode)
{	// LD
	i = (uint16_t)(opcode & 0x0FFF);
	pc += 2;
}

void Chip8::ProcB(const uint16_t &opcode)
{	// JMP
	pc = (opcode & 0x0FFF) + v[0];
}

void Chip8::ProcC(const uint16_t &opcode)
{	// RND (randomize value)
	v[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & (rand() % 0xFF);
	pc += 2;
}

void Chip8::ProcD(const uint16_t &opcode)
{	// DRW - draw sprite according to a memory zone
	uint16_t x = (uint16_t)(v[(opcode & 0x0F00) >> 8] % SCREEN_WIDTH);
	uint16_t y = (uint16_t)(v[(opcode & 0x00F0) >> 4] % SCREEN_HEIGHT);
	uint16_t spriteHeight = (uint16_t)(opcode & 0x000F);
	uint8_t spriteLine;

	v[0xF] = 0;
	for (int h = 0; h < spriteHeight; h++)
	{
		spriteLine = memory[i + h];
		for (int bit = 0; bit < 8; bit++)
			if ((spriteLine & (0x80 >> bit)) != 0)
			{
				if (videoBuffer[((y + h) % SCREEN_HEIGHT)][(x + bit) % SCREEN_WIDTH])
					v[0xF] = 1;
				
				videoBuffer[((y + h) % SCREEN_HEIGHT)][(x + bit) % SCREEN_WIDTH] ^= true;
			}
	}

	pc += 2;
}

void Chip8::ProcE(const uint16_t &opcode)
{	
	switch (opcode & 0x00FF)
	{
	case 0x9E: // SKP
		keyboard[v[(opcode & 0x0F00) >> 8]] != 0 ? pc += 4 : pc += 2;
		break;
	case 0xA1: // SKNP
		keyboard[v[(opcode & 0x0F00) >> 8]] == 0 ? pc += 4 : pc += 2;
		break;
	}
}

void Chip8::ProcF(const uint16_t &opcode)
{	// LD (for timer)
	switch (opcode & 0x00FF)
	{
	case 0x07:
		v[(opcode & 0x0F00) >> 8] = delayTimer;
		pc += 2;
		break;
	case 0x0A:
	{
		bool keyPress = false;
		for (int keyIndex = 0; keyIndex < 16; keyIndex++)
		{
			if (keyboard[keyIndex] != 0)
			{
				v[(opcode & 0x0F00) >> 8] = keyIndex;
				keyPress = true;
			}
		}

		if (!keyPress)
			return;
		pc += 2;
	}
		break;
	case 0x15:
		delayTimer = v[(opcode & 0x0F00) >> 8];
		pc += 2;
		break;
	case 0x18:
		// no sound here
		pc += 2;
		break;
	case 0x1E:
		i += v[(opcode & 0x0F00) >> 8];
		pc += 2;
		break;
	case 0x29:
		i = (uint16_t)(v[(opcode & 0x0F00) >> 8] * 0x5);
		pc += 2;
		break;
	case 0x33:
		memory[i] = v[(opcode & 0x0F00) >> 8] / 100;
		memory[i + 1] = (v[(opcode & 0x0F00) >> 8] / 10) % 10;
		memory[i + 2] = (v[(opcode & 0x0F00) >> 8] % 100) % 10;
		pc += 2;
		break;
	case 0x55:
		for (int vi = 0; vi <= ((opcode & 0x0F00) >> 8); vi++)
			memory[i + vi] = v[vi];		
		pc += 2;
		break;
	case 0x65:
		for (int vi = 0; vi <= ((opcode & 0x0F00) >> 8); vi++)
			v[vi] = memory[i + vi];		
		pc += 2;
		break;
	}
}