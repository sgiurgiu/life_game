

#include <iostream>
#include <exception>
#include <tuple>
#include <unordered_map>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include "hash.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

const int32_t fps = 60;
const int32_t minimumFrameTime = 1000 / fps;



typedef std::tuple<int,int> point;
typedef std::unordered_map<point,int, hash_tuple::hash<point>> board;

board points;


void render(sf::RenderWindow& window)
{
    
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(5, 5));
    rectangle.setFillColor(sf::Color(240,240,240));

    
    for(const auto& p : points)
    {
        rectangle.setPosition(std::get<0>(p.first)*5,std::get<1>(p.first)*5);
        window.draw(rectangle);
    }
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

    int return_value = 0;
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),"Game of Life",sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.resetGLStates(); 
    initialize_game();
    
    bool quit = false;
    int32_t delay = 1000;
    
    sf::Clock computeClock;
    sf::Clock renderClock;
    sf::Color bgColor(0,0,0);
    while(!quit)
    {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
            ) {
                quit = true;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
            {
                delay += 100;
            }                
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
            {
                if(delay > 100) {
                    delay -= 100;
                }
            }                                
        }
        
        window.clear(bgColor);
        render(window);
        window.display();
        
        auto computeTime = computeClock.getElapsedTime();
        if( computeTime.asMilliseconds() > delay) {
            compute_next_generation();
            computeClock.restart();
        }
        auto renderTime = renderClock.restart();
        if (renderTime.asMilliseconds() < minimumFrameTime) {
            sf::sleep(sf::milliseconds(minimumFrameTime - renderTime.asMilliseconds()));
        }
    }
    
    window.close();
    
    return return_value;
}

