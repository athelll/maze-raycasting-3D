#include <SDL2/SDL.h>
#include <iostream>
#include <array>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int MAPWIDTH = 24;
constexpr int MAPHEIGHT = 24;

struct Point {
  double x {};
  double y {};
};

constexpr int terminate ( std::string error ) {
  std::cerr << error << SDL_GetError();
  return 1;
}

constexpr std::array<std::array<int, MAPWIDTH>, MAPHEIGHT> WORLDMAP {{
  {{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1 }},
  {{ 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1 }},
  {{ 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }},
  {{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}
}};

int main(int argc, char* argv[])
{
  Point player { 22, 12 };
  Point DirVector { -1, 0 };
  Point PlaneVector { 0, 0.66 };

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  if (SDL_Init ( SDL_INIT_VIDEO ) < 0)
    return terminate("SDL could not be initilaized. SDL ERROR: ");

  window = SDL_CreateWindow(
    "Raycaster",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (window == nullptr) return terminate("Window could not be initialized. SDL ERROR: ");

  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
  if (renderer == nullptr) return terminate("Renderer could not be initialized. SDL ERROR: ");
  
  // main loop
  SDL_Event event; bool running = true;
  while ( running ) 
  {
    while ( SDL_PollEvent ( &event ) )
    { 
      if ( event.type == SDL_QUIT ) 
        running = false; 
    }

    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      double offset = 2 * x std::static_cast<double>(SCREEN_WIDTH) -1;
      Point rayDir {};
      rayDir.x = dirVector.x + planeVector.x*offset;
      rayDir.y = dirVector.y + planeVector.y*offset;
    }
  }

  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
}
