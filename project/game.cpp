#include <SDL.h>
#include "game.h"
#include <iostream>



bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100, // Top left x-coordinate of window
        100, // Top left y-coordinate of window
        1024, // Width of window
        768, // Height of window
        0 // Flags (0 for no flags set)
    );

    mRenderer = SDL_CreateRenderer(
        mWindow, // Window to create renderer for
        -1, // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }    

    mIsRunning = true;
    return true;
}

Game::Game() {
    mPaddlePos.x = thickness*2;
    mPaddlePos.y = static_cast<float>(768) / 2;

    mBallPos.x = 1024 / 2;
    mBallPos.y = 768 / 2;

    mTicksCount = 0;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::ProcessInput() {
    SDL_Event event;

    // Quits by pressing X on window
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    // Get state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
    }
}
void Game::UpdateGame() {
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    // Delta time is the difference in ticks from last frame (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    if (mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 100.0f * deltaTime;
        // Make sure paddle doesn't move off screen!
        if (mPaddlePos.y < (paddleH / 2.0f + thickness))
        {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
        {
            mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    // Update tick counts (for next frame)
    // mTicksCount = SDL_GetTicks();
}
void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer,
        0, // R
        0, // G
        0, // B
        255 // A
    );
    SDL_RenderClear(mRenderer);
    SDL_Rect wall{
        0, // Top left x
        0, // Top left y
        1024, // Width
        15 // Height
    };

    SDL_Rect wall2{
        0, // Top left x
        768 - 15, // Top left y
        1024, // Width
        55 // Height
    };

    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x - thickness / 2),
        static_cast<int>(mPaddlePos.y - paddleH / 2),
        thickness,
        paddleH
    };

    SDL_Rect ball{
        static_cast<int>(mBallPos.x - thickness / 2),
        static_cast<int>(mBallPos.y - thickness / 2),
        thickness,
        thickness
    };

    //std::cout << mBallPos.x << " " << mBallPos.y;

    SDL_SetRenderDrawColor(mRenderer, 192, 192, 192, 255);
    SDL_RenderFillRect(mRenderer, &wall);
    SDL_RenderFillRect(mRenderer, &wall2);

    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle);
    // Present the renderer
    SDL_RenderPresent(mRenderer);
}