#pragma once
#include <cstdint>
#include <Windows.h>

class SimpleTimer
{
private:
	__int64 freq;
	__int64 startTime;
	__int64 lastTime;
	double targetElapsedTime;

public:
	void SetTargetElapsedTimer(double time)
	{
		targetElapsedTime = time;
	}

	void Start()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		freq = static_cast<__int64>(double(li.QuadPart) / 1000.0);
		QueryPerformanceCounter(&li);
		startTime = li.QuadPart;
	}

	void Restart()
	{
		startTime = lastTime;
	}

	bool CheckTargetTimer()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		lastTime = static_cast<__int64>(double(li.QuadPart));

		if ((lastTime - startTime) >= (targetElapsedTime * freq))
			return true;
		else
			return false;
	}
};

class Chip8
{
public:
	Chip8();
	~Chip8();

	const static int MEMORY_SIZE = 4096;
	const static int REGISTER_SIZE = 16;
	const static int STACK_SIZE = 16;
	const static int KEYBOARD_KEYS_SIZE = 16;
	const static int START_ADDRESS = 0x200;
	const static int SCREEN_WIDTH = 64;
	const static int SCREEN_HEIGHT = 32;

	uint8_t memory[MEMORY_SIZE];
	uint8_t v[REGISTER_SIZE];
	uint16_t i;
	uint16_t pc;
	uint16_t stack[KEYBOARD_KEYS_SIZE];
	uint16_t sp;
	uint8_t delayTimer;

	bool videoBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

	uint16_t opcode;
	uint8_t keyboard[KEYBOARD_KEYS_SIZE];

	
private:
	void Proc0(const uint16_t &opcode);
	void Proc1(const uint16_t &opcode);
	void Proc2(const uint16_t &opcode);
	void Proc3(const uint16_t &opcode);
	void Proc4(const uint16_t &opcode);
	void Proc5(const uint16_t &opcode);
	void Proc6(const uint16_t &opcode);
	void Proc7(const uint16_t &opcode);
	void Proc8(const uint16_t &opcode);
	void Proc9(const uint16_t &opcode);
	void ProcA(const uint16_t &opcode);
	void ProcB(const uint16_t &opcode);
	void ProcC(const uint16_t &opcode);
	void ProcD(const uint16_t &opcode);
	void ProcE(const uint16_t &opcode);
	void ProcF(const uint16_t &opcode);
	
public:
	void Tick();
	void TicksPer60FPS();
	void Init();
	bool loadRom(const char * filename);
};
