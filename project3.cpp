#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;
const int SIZE = 20;

struct Segment {
    int x, y;
    Segment(int xPos, int yPos) : x(xPos), y(yPos) {}
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window* window = SDL_CreateWindow("Snake Game - CodTech", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    std::vector<Segment> snake;
    snake.push_back(Segment(WIDTH / 2, HEIGHT / 2));

    int dirX = SIZE, dirY = 0;
    int foodX = (rand() % (WIDTH / SIZE)) * SIZE;
    int foodY = (rand() % (HEIGHT / SIZE)) * SIZE;

    float speed = 100;
    Uint32 lastMove = SDL_GetTicks();

    srand(time(0));

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if (dirY == 0) { dirX = 0; dirY = -SIZE; } break;
                    case SDLK_DOWN: if (dirY == 0) { dirX = 0; dirY = SIZE; } break;
                    case SDLK_LEFT: if (dirX == 0) { dirX = -SIZE; dirY = 0; } break;
                    case SDLK_RIGHT: if (dirX == 0) { dirX = SIZE; dirY = 0; } break;
                }
            }
        }

        if (SDL_GetTicks() - lastMove > speed) {
            lastMove = SDL_GetTicks();

            // Move body
            for (int i = snake.size() - 1; i > 0; --i)
                snake[i] = snake[i - 1];

            // Move head
            snake[0].x += dirX;
            snake[0].y += dirY;

            // Check collision with walls
            if (snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT)
                running = false;

            // Check collision with self
            for (int i = 1; i < snake.size(); ++i)
                if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
                    running = false;

            // Check food
            if (snake[0].x == foodX && snake[0].y == foodY) {
                snake.push_back(Segment(0, 0));
                foodX = (rand() % (WIDTH / SIZE)) * SIZE;
                foodY = (rand() % (HEIGHT / SIZE)) * SIZE;

                if (speed > 30)
                    speed -= 5; // Increase speed
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw food
        SDL_Rect foodRect = { foodX, foodY, SIZE, SIZE };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &foodRect);

        // Draw snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (auto& s : snake) {
            SDL_Rect r = { s.x, s.y, SIZE, SIZE };
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
