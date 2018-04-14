

#include <iostream>
#include <exception>
#include <SDL.h>
#include <tuple>
#include <unordered_map>

#include "sdl_helpers.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

const Uint32 fps = 60;
const Uint32 minimumFrameTime = 1000 / fps;

typedef std::tuple<int,int> point;
typedef std::unordered_map<point,int, hash_tuple::hash<point>> board;

board points;


void render(renderer_ptr& sdl_renderer)
{
    int width = 0 ;
    int height = 0;  
    if(SDL_GetRendererOutputSize(sdl_renderer.get(),&width,&height))
    {
        throw std::runtime_error(SDL_GetError());
    }
    
    Uint8 r = 0,b = 0,g = 0;
    SDL_SetRenderDrawColor(sdl_renderer.get(),r,b,g,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer.get());

    SDL_SetRenderDrawColor(sdl_renderer.get(),240,240,240,SDL_ALPHA_OPAQUE);
    SDL_Rect rect;
    rect.w = 5;
    rect.h = 5;
    
    for(const auto& p : points)
    {
        rect.x = std::get<0>(p.first)*5;
        rect.y = std::get<1>(p.first)*5;        
        SDL_RenderFillRect(sdl_renderer.get(),&rect);
    }
    
    
    SDL_RenderPresent(sdl_renderer.get());
}

void initialize_game()
{
    points[{10,10}]= 0;
    points[{12,10}]= 0;
    points[{11,11}]= 0;    
    points[{11,12}]= 0;
    points[{12,11}]= 0;    
}

void compute_next_generation()
{
    board new_points;
    for(const auto& p : points)
    {
        int x = std::get<0>(p.first);
        int y = std::get<1>(p.first);
        
        for(int i=x-1;i<=x+1;i++) 
        {
            for(int j=y-1;j<=y+1;j++)
            {
                if(i==x&&j==y) continue;                
                point new_p{i,j};
                if(new_points.find(new_p) == new_points.end())
                {
                    new_points[new_p] = 1;
                }
                else
                {
                     new_points[new_p]++;
                }
            }
        }
    }
    board old_points = points;
    points.clear();
    for(const auto& p : new_points)
    {
        if( (old_points.find(p.first) != old_points.end() && ( p.second == 2 || p.second == 3 )) 
            || (old_points.find(p.first) == old_points.end() && p.second == 3)
        )
        {
            points[p.first] = 0;
        }
    }
}

int main() {
    window_ptr window(nullptr,sdl_window_deleter());  
    int return_value = 0;
    
    try
    {
        if(SDL_Init(SDL_INIT_EVERYTHING))
        {
            throw std::runtime_error(SDL_GetError());
        }
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); 
        SDL_LogSetPriority(SDL_LOG_CATEGORY_RENDER,SDL_LOG_PRIORITY_DEBUG);

        window.reset(SDL_CreateWindow("Game of Life",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT,
                                    SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE));

        if(!window)
        {
            throw std::runtime_error(SDL_GetError());
        }
        SDL_GLContext mContext = SDL_GL_GetCurrentContext();
        SDL_GL_MakeCurrent(window.get(), mContext);    
        
        renderer_ptr  sdl_renderer(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE), sdl_renderer_deleter());
        
        initialize_game();
        
        SDL_Event event{0};
        Uint32 frameTime;
        bool quit = false;
        Uint32 lastUpdate = SDL_GetTicks();
        Uint32 speed = 1000;
        while(!quit)
        {
            frameTime = SDL_GetTicks();
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                    quit = true;
                    break;
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym ) 
                        {
                            case SDLK_q:
                                quit = true;
                                break;
                            case SDLK_DOWN:
                                speed += 100;
                                break;
                            case SDLK_UP:
                                if(speed > 100) {
                                    speed -= 100;
                                }
                                break;
                        }
                    break;
                }
            }
            render(sdl_renderer);
            if( (SDL_GetTicks() - lastUpdate) > speed) {
                compute_next_generation();
                lastUpdate = SDL_GetTicks();
            }
    //        deltaTime = frameTime - lastFrameTime;
    //        lastFrameTime = frameTime;
         
            if ((SDL_GetTicks() - frameTime) < minimumFrameTime) {
                SDL_Delay(minimumFrameTime - (SDL_GetTicks() - frameTime));
            }
        }

    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return_value = -1;
    }
    
    SDL_Quit();
    return return_value;
}

