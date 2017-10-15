#ifndef RENDERING_H
#define RENDERING_H

#include "Gas.h"
#include <SFML/Graphics.hpp>


void renderParticles(sf::RenderWindow &window, Gas &gas);
void renderDistribution(sf::RenderWindow &window, std::vector<int> &speeds);

#endif // RENDERING_H
