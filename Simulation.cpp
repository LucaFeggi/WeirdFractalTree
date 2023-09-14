#include "Simulation.hpp"
#include "DrawingFunctions.hpp"

#include <iostream>
#include <cmath>


inline int SDL_GetWindowRefreshRate(SDL_Window *Window){
    SDL_DisplayMode Mode;
    int DisplayIndex = SDL_GetWindowDisplayIndex(Window);
    int DefaultRefreshRate = 60;
    if(SDL_GetDesktopDisplayMode(DisplayIndex, &Mode) != 0){
        return DefaultRefreshRate;
    }
    if(Mode.refresh_rate == 0){
        return DefaultRefreshRate;
    }
    return Mode.refresh_rate;
}

Simulation::Simulation() : WindowWidth(1280), WindowHeight(720), SmallestBranchDimension(5){
	SDL_Init(SDL_INIT_VIDEO);
	Window = SDL_CreateWindow("Fractal Tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	Surface = SDL_GetWindowSurface(Window);
	FPS = SDL_GetWindowRefreshRate(Window);
	Quit = false;
	Angle = M_PI/6.0f;
	this->CreateTree();
	SDL_UpdateWindowSurface(Window);
}

Simulation::~Simulation(){
	SDL_FreeSurface(Surface);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Simulation::GetCommands(){
	while(SDL_WaitEvent(&Event)){
		if(Event.type == SDL_QUIT){
			Quit = true;
			break;
		}
		if(Event.key.state == SDL_PRESSED){
			if(Event.key.keysym.sym == SDLK_ESCAPE){
				Quit = true;
				break;
			}
		}
		if(Event.type == SDL_MOUSEWHEEL){
			if(Event.wheel.y > 0){	//scroll up
				Angle += 0.01f;
			}
			else if(Event.wheel.y < 0){	//scroll down
				Angle -= 0.01f;
			}
			if(Angle > 2*M_PI)
				Angle = Angle/(2*M_PI) - 1.0f;
			if(Angle < -(2*M_PI))
				Angle = -Angle/(2*M_PI) - 1.0f;
			this->ClearSurface();
			this->CreateTree();
			SDL_UpdateWindowSurface(Window);
		}
	}
}

void Simulation::CreateBranch(Branch ParentBranch, float Length, float Angle){
	if(Length < SmallestBranchDimension)
		return;
	Branch LeftBranch(ParentBranch.x, ParentBranch.y, Length, -Angle);
	Branch RightBranch(ParentBranch.x, ParentBranch.y, Length, Angle);
	DrawLine(ParentBranch.x, ParentBranch.y, LeftBranch.x, LeftBranch.y, Surface, WindowWidth, White);
	DrawLine(ParentBranch.x, ParentBranch.y, RightBranch.x, RightBranch.y, Surface, WindowWidth, White);
	this->CreateBranch(LeftBranch, Length*0.67f, Angle - M_PI/4.0f);
	this->CreateBranch(RightBranch, Length*0.67f, Angle + M_PI/4.0f);
}

void Simulation::CreateTree(){
	float InitialLength = WindowHeight/4;
	int InitialX = WindowWidth/2;
	int InitialY = WindowHeight-1;
	Branch FirstBranch(InitialX, InitialY, InitialLength, 0.0f);
	DrawLine(InitialX, InitialY, FirstBranch.x, FirstBranch.y, Surface, WindowWidth, White);
	this->CreateBranch(FirstBranch, InitialLength*0.67f, Angle);
}

void Simulation::ClearSurface(){
	SDL_FillRect(Surface, NULL, 0x000000);
}

void Simulation::Cycle(){
	while (!Quit){
		double IterationStart = SDL_GetPerformanceCounter();
		this->GetCommands();
		if(Quit)
			break;
		double IterationEnd = SDL_GetPerformanceCounter();
		double ElapsedSeconds = (IterationEnd-IterationStart) / (double)SDL_GetPerformanceFrequency();
		double Delay =  (1000.0/(double)FPS) - (ElapsedSeconds*1000.0);
		if(Delay > 0)
			SDL_Delay(std::max(0, (int)round(Delay)));
	}
}
