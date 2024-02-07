#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>


const int thickness = 15;
const int paddleH = thickness * 8;

struct Vector2
{
    float x;
    float y;
};

class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);

    bool mIsRunning;
    Uint32 mTicksCount;
    int mPaddleDir;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Vector2 mPaddlePos;
    Vector2 mBallPos;
    Vector2 mBallVel;
    bool mUpdatingActors;
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
};