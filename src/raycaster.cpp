#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int MAPWIDTH = 24;
constexpr int MAPHEIGHT = 24;
constexpr int LAG_BUFFER = 15;

enum Tside {
  EastWest,
  NorthSouth,
};

template <typename T>
struct Point {
  T x {};
  T y {};
};

constexpr int terminate ( std::string error ) {
  std::cerr << error << SDL_GetError();
  return 1;
}

struct Color {
  uint8_t r, g, b, a;
};

namespace colors {
  constexpr Color red    {255, 0, 0, 255};
  constexpr Color green  {0, 255, 0, 255};
  constexpr Color blue   {0, 0, 255, 255};
  constexpr Color white  {255, 255, 255, 255};
  constexpr Color yellow {255, 255, 0, 255};
}

void vertline(SDL_Renderer& renderer, int x, int startY, int endY, Color color)
{
  uint8_t r = color.r; uint8_t g = color.g;
  uint8_t b = color.b; uint8_t a = color.a;
  SDL_SetRenderDrawColor(&renderer,r, g, b, a);
  SDL_RenderDrawLine(&renderer, x, startY, x, endY);
}

void clearScreen(SDL_Renderer& renderer)
{
  SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
  SDL_RenderClear(&renderer); 
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
  {{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }}
}};

Color colorHit(Point<int>& map)
{
  switch(WORLDMAP[map.x][map.y])
  {
    case 1:  return colors::red;
    case 2:  return colors::green;
    case 3:  return colors::blue;
    case 4:  return colors::white;
    default: return colors::yellow;
  }
}

int inter(double D)
{
  return static_cast<int>(D);
} 

int main(int argc, char* argv[])
{
  Point<double> player { 22, 12 };
  Point<double> dirVector { -1, 0 };
  Point<double> planeVector { 0, 0.66 };

  double currentTime {};
  double oldTime {};

  if (SDL_Init ( SDL_INIT_VIDEO ) < 0)
    return terminate("SDL could not be initilaized. SDL ERROR: ");

  SDL_Window* window = nullptr;
  window = SDL_CreateWindow(
    "Raycaster",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (window == nullptr) 
    return terminate("Window could not be initialized. SDL ERROR: ");

  SDL_Renderer* renderer = nullptr;
  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
  if (renderer == nullptr) 
    return terminate("Renderer could not be initialized. SDL ERROR: ");
  
  SDL_Event event;
  bool running = true;
  oldTime = static_cast<double>(SDL_GetTicks()) / 1000;

  while ( running )
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      Point<double> rayDir {};
      Point<double> deltaDistance{};
      Point<double> sideDist{};
      Point<int>    map {};
      Point<int>    stepper {};
      double        perpWallDist {};

      double offset = 2 * x / static_cast<double>(SCREEN_WIDTH) -1;

      rayDir.x = dirVector.x + planeVector.x*offset;
      rayDir.y = dirVector.y + planeVector.y*offset;

      deltaDistance.x = std::abs( 1 / rayDir.x );
      deltaDistance.y = std::abs( 1 / rayDir.y );

      map.x = static_cast<int>(player.x);
      map.y = static_cast<int>(player.y);

      bool hit = false;
      Tside sideHit {};
 
      if (rayDir.x < 0) {
        stepper.x = -1;
        sideDist.x = (player.x - map.x) * deltaDistance.x;
      } else {
        stepper.x = +1;
        sideDist.x = (map.x + 1 - player.x) * deltaDistance.x;
      }
      if (rayDir.y < 0) {
        stepper.y = -1;
        sideDist.y = (player.y - map.y) * deltaDistance.y;
      } else {
        stepper.y = +1;
        sideDist.y = (player.y + 1 - player.y) * deltaDistance.y;
      }
      
      while(hit == false)
      {
        if (sideDist.x < sideDist.y) {
          sideDist.x += deltaDistance.x;
          map.x += stepper.x;
          sideHit = NorthSouth;
        } else {          
          sideDist.y += deltaDistance.y;
          map.y += stepper.y;
          sideHit = EastWest;
        }
        if (WORLDMAP[map.x][map.y] > 0) hit = true;
      }

      if (sideHit == NorthSouth)
        perpWallDist = sideDist.x - deltaDistance.x;
      else
        perpWallDist = sideDist.y - deltaDistance.y;

      int lineHeight  = static_cast<int>(SCREEN_HEIGHT / perpWallDist);

      int drawStart   =   SCREEN_HEIGHT / 2 - lineHeight / 2;
      if (drawStart < 0) drawStart = 0;
      int drawEnd     =   SCREEN_HEIGHT / 2 + lineHeight / 2;
      if (drawEnd < 0) drawEnd = 0;

      Color hitColor = colorHit(map);
      if (sideHit == EastWest) {
        hitColor.r = hitColor.r / 2; hitColor.g = hitColor.g / 2;
        hitColor.b = hitColor.b / 2; hitColor.a = hitColor.a / 2; 
      }
      vertline( *renderer, x, drawStart, drawEnd, hitColor );
    }
    SDL_RenderPresent(renderer);
    clearScreen(*renderer);

    currentTime = static_cast<double>(SDL_GetTicks()) / 1000;
    double frameTime = currentTime - oldTime; double FPS = 1 / frameTime;
    SDL_Delay( LAG_BUFFER );
    oldTime = currentTime;

    double movSpeed = frameTime * 5.0;
    double rotSpeed = frameTime * 3.0;

    while ( SDL_PollEvent ( &event ) )
    {
      if ( event.type == SDL_QUIT )
        running = false;

      if ( event.type == SDL_KEYDOWN )
      {
        switch ( event.key.keysym.sym )
        {
        case SDLK_UP:
          if ( WORLDMAP[inter(player.x + dirVector.x * movSpeed)][inter(player.y)] == false )
            player.x += dirVector.x * movSpeed;
          if ( WORLDMAP[inter(player.x)][inter(player.y + dirVector.y*movSpeed)] == false )
            player.y += dirVector.y * movSpeed;
          break;
         
        case SDLK_DOWN:
          if ( WORLDMAP[inter(player.x - dirVector.x * movSpeed)][inter(player.y)] == false )
            player.x -= dirVector.x * movSpeed;
          if ( WORLDMAP[inter(player.x)][inter(player.y - dirVector.y*movSpeed)] == false )
            player.y -= dirVector.y * movSpeed;
          break;

        case SDLK_RIGHT: {
          double cosAngle = std::cos(-rotSpeed);
          double sinAngle = std::sin(-rotSpeed);

          double oldDirVecX = dirVector.x;
          dirVector.x = dirVector.x * cosAngle - dirVector.y * sinAngle;
          dirVector.y = oldDirVecX  * sinAngle + dirVector.y * cosAngle;

          double oldPlaneVecX = planeVector.x;
          planeVector.x = planeVector.x * cosAngle - planeVector.y * sinAngle;
          planeVector.y = oldPlaneVecX  * sinAngle + planeVector.y * cosAngle;
          }
          break;

        case SDLK_LEFT: {
          double cosAngle = std::cos(rotSpeed);
          double sinAngle = std::sin(rotSpeed);

          double oldDirVecX = dirVector.x;
          dirVector.x = dirVector.x * cosAngle - dirVector.y * sinAngle;
          dirVector.y = oldDirVecX  * sinAngle + dirVector.y * cosAngle;

          double oldPlaneVecX = planeVector.x;
          planeVector.x = planeVector.x * cosAngle - planeVector.y * sinAngle;
          planeVector.y = oldPlaneVecX  * sinAngle + planeVector.y * cosAngle;
          }
          break;
        }
      }
    }
    std::cout << "FPS->"<< FPS << '\n';
    std::cout << "rotSpeed" <<rotSpeed << '\n';
    std::cout << "px-> "<< player.x << " py-> " << player.y << '\n';
    std::cout << "dirx-> " << dirVector.x << " diry-> " << dirVector.y << '\n';
    std::cout << "planex-> "<<planeVector.x << " planey-> " << planeVector.y << '\n';
    std::cout << '\n';
  }

  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
}
