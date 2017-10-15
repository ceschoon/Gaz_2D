#include "rendering.h"
#include <vector>
#include <math.h>
#include <iostream>

/**
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
**/


void renderParticles(sf::RenderWindow &window, Gas &gas)
{
    std::vector<std::vector<double>> particles = gas.Getparticles();

    int radius0 = particles[0][5];
    sf::CircleShape shape0(radius0);
    shape0.setFillColor(sf::Color::Blue);

    for (int i=0; i<particles.size(); i++)
    {
        int radius = particles[i][5];
        if (radius!=radius0)
        {
            sf::CircleShape shape(radius);
            shape.setFillColor(sf::Color::Blue);

            if (i==particles.size()-1 && gas.GetbrownianMotion())
            {
                shape.setFillColor(sf::Color::Green);
            }

            shape.setPosition(sf::Vector2f(particles[i][0]-radius,particles[i][1]-radius));
            window.draw(shape);
        }
        else
        {
            shape0.setPosition(sf::Vector2f(particles[i][0]-radius,particles[i][1]-radius));
            window.draw(shape0);
        }
    }
}

void renderDistribution(sf::RenderWindow &window, std::vector<int> &speeds)
{
    int numOfParticles = 0;

    for (int i=0; i<speeds.size(); i++)
    {
        numOfParticles += speeds[i];
    }

    for (int i=0; i<speeds.size(); i++)
    {
        double width = double(window.getSize().x)/double(speeds.size());
        double height = double(speeds[i])/double(numOfParticles)*double(window.getSize().y) * double(speeds.size())/6.0 ;   // arbitrary factor at the end
        int rectWidth = int(width)+1;
        //if (rectWidth==0){rectWidth=1;}
        int rectHeight = int(height);
        sf::RectangleShape rect(sf::Vector2f(rectWidth,rectHeight));
        rect.setFillColor(sf::Color::Blue);
        int posHeight = 0;
        if (window.getSize().y-rectHeight > 0)
        {
            posHeight = window.getSize().y-rectHeight;
        }
        rect.setPosition(sf::Vector2f(rectWidth*i,posHeight));
        window.draw(rect);
    }
}
