#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
#include "chip8.h"
#include <iostream>
#include <filesystem> // ISO C++ 17

class Chip8Machine : public gck::xGameEngine
{
private:
	Chip8 chip8;
	SimpleTimer drawTimer;
	SimpleTimer cpuTimer;
	std::string romFilePath;
	int clockRate;
	double cpuTickTime;
	double drawTickTime;
	// manages here colors
	gck::Pixel backgroundPixel = gck::BLACK;
	gck::Pixel foregroundPixel = gck::WHITE;

public:
	Chip8Machine()
	{
		sAppName = "Chip8 Emulator";
	
		clockRate = 500;
		cpuTickTime = 1000.0 / clockRate;
		drawTickTime = 1000.0 / 60;
	}

	void reset()
	{
		if (romFilePath.length() != 0)
		{
			chip8.loadRom(romFilePath.c_str());

			drawTimer.SetTargetElapsedTimer(drawTickTime);
			drawTimer.Start();

			cpuTimer.SetTargetElapsedTimer(cpuTickTime);
			cpuTimer.Start();
		}
	}

	bool appCreate() override
	{
		return true;
	}

	bool appUpdate(float fElapsedTime) override
	{
		if (GetKey(gck::Key::ESCAPE).bPressed)
			return false;

		handleInput();
		
		if (cpuTimer.CheckTargetTimer())
		{
			chip8.Tick();
			cpuTimer.Restart();
		}
		
		if (drawTimer.CheckTargetTimer())
		{		
			chip8.TicksPer60FPS();

			for (int y = 0; y < chip8.SCREEN_HEIGHT; y++)
				for (int x = 0; x < chip8.SCREEN_WIDTH; x++)
					if (chip8.videoBuffer[y][x])
						Draw(gck::vi2d(x, y), foregroundPixel);
					else
						Draw(gck::vi2d(x, y), backgroundPixel);

			drawTimer.Restart();
		}
		
		return true;
	}

	void handleInput()
	{
		if (GetKey(gck::Key::BACK).bPressed)
			reset();

		if (GetKey(gck::Key::K1).bPressed)
			chip8.keyboard[0x1] = 1;
		else if (GetKey(gck::Key::K1).bReleased)
			chip8.keyboard[0x1] = 0;

		if (GetKey(gck::Key::K2).bPressed)
			chip8.keyboard[0x2] = 1;
		else if (GetKey(gck::Key::K2).bReleased)
			chip8.keyboard[0x2] = 0;

		if (GetKey(gck::Key::K3).bPressed)
			chip8.keyboard[0x3] = 1;
		else if (GetKey(gck::Key::K3).bReleased)
			chip8.keyboard[0x3] = 0;

		if (GetKey(gck::Key::K4).bPressed)
			chip8.keyboard[0xC] = 1;
		else if (GetKey(gck::Key::K4).bReleased)
			chip8.keyboard[0xC] = 0;

		if (GetKey(gck::Key::Q).bPressed)
			chip8.keyboard[0x4] = 1;
		else if (GetKey(gck::Key::Q).bReleased)
			chip8.keyboard[0x4] = 0;

		if (GetKey(gck::Key::W).bPressed)
			chip8.keyboard[0x5] = 1;
		else if (GetKey(gck::Key::W).bReleased)
			chip8.keyboard[0x5] = 0;

		if (GetKey(gck::Key::E).bPressed)
			chip8.keyboard[0x6] = 1;
		else if (GetKey(gck::Key::E).bReleased)
			chip8.keyboard[0x6] = 0;

		if (GetKey(gck::Key::R).bPressed)
			chip8.keyboard[0xD] = 1;
		else if (GetKey(gck::Key::R).bReleased)
			chip8.keyboard[0xD] = 0;

		if (GetKey(gck::Key::A).bPressed)
			chip8.keyboard[0x7] = 1;
		else if (GetKey(gck::Key::A).bReleased)
			chip8.keyboard[0x7] = 0;

		if (GetKey(gck::Key::S).bPressed)
			chip8.keyboard[0x8] = 1;
		else if (GetKey(gck::Key::S).bReleased)
			chip8.keyboard[0x8] = 0;

		if (GetKey(gck::Key::D).bPressed)
			chip8.keyboard[0x9] = 1;
		else if (GetKey(gck::Key::D).bReleased)
			chip8.keyboard[0x9] = 0;

		if (GetKey(gck::Key::F).bPressed)
			chip8.keyboard[0xE] = 1;
		else if (GetKey(gck::Key::F).bReleased)
			chip8.keyboard[0xE] = 0;

		if (GetKey(gck::Key::Z).bPressed)
			chip8.keyboard[0xA] = 1;
		else if (GetKey(gck::Key::Z).bReleased)
			chip8.keyboard[0xA] = 0;

		if (GetKey(gck::Key::X).bPressed)
			chip8.keyboard[0x0] = 1;
		else if (GetKey(gck::Key::X).bReleased)
			chip8.keyboard[0x0] = 0;

		if (GetKey(gck::Key::C).bPressed)
			chip8.keyboard[0xB] = 1;
		else if (GetKey(gck::Key::C).bReleased)
			chip8.keyboard[0xB] = 0;

		if (GetKey(gck::Key::V).bPressed)
			chip8.keyboard[0xF] = 1;
		else if (GetKey(gck::Key::V).bReleased)
			chip8.keyboard[0xF] = 0;
	}

	bool tryToLoadRom(const char* filePath)
	{
		if (chip8.loadRom(filePath))
		{
			romFilePath = (std::string)(filePath);
			drawTimer.SetTargetElapsedTimer(drawTickTime);
			drawTimer.Start();

			cpuTimer.SetTargetElapsedTimer(cpuTickTime);
			cpuTimer.Start();

			return true; // ok
		}
		else
			return false;		
	}
};

void listFolder(std::string folder, std::string ext)
{	// check if folder exists
	bool checkFolder = std::filesystem::is_directory(folder);
	short i = 1;
	// get it!
	if (checkFolder) {
		std::cout << "Available ROMS list :" << std::endl;

		for (auto& p : std::filesystem::recursive_directory_iterator(folder))
			if (p.path().extension() == ext)
			{   // list files according to the extension ch8
				std::cout << i << ". " << p.path().stem().string() << ext << std::endl;
				++i;
			}

		std::cout << std::endl;
	}
	else { // no folder?
		std::cout << "Folder With Codes Does Not Exist" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	std::string dir = argv[0];
	std::string folder = "\\ROMS\\";
	std::string romPath;
	std::string selectedRom;

	dir.erase(dir.find_last_of('\\') + 1);
	// in our context ROM has for extension .ch8
	listFolder("." + folder, ".ch8");

	std::cout << "Select the ROM that will be executed - (ex : TicTacToe.ch8)" << std::endl;
	std::cin >> selectedRom;
	romPath = dir + folder + selectedRom;
	// create virtual machine
	Chip8Machine chip8Machine;

	int pixelScale = 20;
	// DON'T USE VSYNC 
	if (chip8Machine.Construct(Chip8::SCREEN_WIDTH, Chip8::SCREEN_HEIGHT, pixelScale, pixelScale, true, false, false))
		if (chip8Machine.tryToLoadRom(romPath.c_str()))
			chip8Machine.Start();

	return 0;
}