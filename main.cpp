#include "Gas.h"
#include "rendering.h"
#include "initParameters.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <thread>
#include <iostream>
#include <cstdlib>


int main(int argc, char* argv[])
{
    bool moduloSpace = false;
    bool distribution = true;
    bool computeMaxwell = false;
    bool brownianMotion = false;
    int argNumOfParticles = 150;
    int argRadiusOfParticles = 4;
    int numOfLoopsRemembered = 1000;
    int resolution = 400;
    int brownianRadius = 20;
    double millisecPerLoop = 30.0;
    double initSpeedParticles = 100;
    double brownianMass = 5;

    bool interrupt = initParameters(argc, argv, argNumOfParticles, argRadiusOfParticles, initSpeedParticles, numOfLoopsRemembered, resolution, millisecPerLoop, moduloSpace, distribution, computeMaxwell, brownianMotion, brownianMass, brownianRadius);

    if (interrupt){return 0;}

    // create objects

    sf::RenderWindow window(sf::VideoMode(200, 200), "2D Gas - version 1.4 - by Cédric Schoonen");
    int width = resolution-1;
    if (width > sf::VideoMode::getDesktopMode().width)
    {
        width = sf::VideoMode::getDesktopMode().width;
    }
    sf::RenderWindow windowDistribution(sf::VideoMode(width, 200), "Distribution of velocities");
    windowDistribution.setPosition(sf::Vector2i(0,0));

    window.requestFocus();

    if (!distribution)
    {
        windowDistribution.close();
    }

    Gas gas(window.getSize().x, window.getSize().y, moduloSpace, argNumOfParticles, argRadiusOfParticles, initSpeedParticles, brownianMotion, brownianMass, brownianRadius);

    sf::Clock clockProgram;
    sf::Clock clockProcessing;
    sf::Clock clockMemorising;
    sf::Clock clockRendering;
    double elapsedProcessing = 0;
    double elapsedMemorising = 0;
    double elapsedRendering = 0;
    double lastProgramTime = 0;
    int loopCounter = 0;
    bool windowHasFocus = true;

    std::vector<std::vector<int>> lastSpeeds;
    lastSpeeds.clear();
    std::vector<int> speeds;
    speeds.clear();
    int index = 0;

    // main loop

    while (window.isOpen() && (loopCounter<=numOfLoopsRemembered || !computeMaxwell))
    {
        // timing-related stuff

        loopCounter += 1;
        double programTime = clockProgram.getElapsedTime().asSeconds();
        double elapsedLoopTime = programTime - lastProgramTime;
        lastProgramTime = programTime;

        double timeToSleep = 0;
        if (elapsedLoopTime*1000<millisecPerLoop){timeToSleep = millisecPerLoop-elapsedLoopTime*1000;}
        std::this_thread::sleep_for(std::chrono::milliseconds(int(timeToSleep)));

        // display loop info

        if ((loopCounter*100)%numOfLoopsRemembered==0 && loopCounter<=numOfLoopsRemembered && computeMaxwell)
        {
            std::cout << "Loop number " << loopCounter << " out of " << numOfLoopsRemembered << std::endl;
        }
        if (loopCounter==numOfLoopsRemembered && computeMaxwell)
        {
            std::cout << "Maxwellisation completed" << std::endl;   // TODO: save image with info
        }

        // event handling

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                windowDistribution.close();
            }
            if (event.type == sf::Event::GainedFocus)
            {
                windowHasFocus = true;
            }
            if (event.type == sf::Event::LostFocus)
            {
                windowHasFocus = false;
            }
        }
        while (windowDistribution.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                windowDistribution.close();
            }
        }

        // process objects

        clockProcessing.restart();

        gas.resizeBox(window.getSize().x,window.getSize().y);
        gas.moveParticles(millisecPerLoop/1000.0);
        gas.performCollisions();

        elapsedProcessing += clockProcessing.getElapsedTime().asSeconds();

        // compute speed distribution

        if (windowDistribution.isOpen() && distribution)
        {
            clockMemorising.restart();

            if (lastSpeeds.size()<numOfLoopsRemembered)
            {
                lastSpeeds.push_back(gas.speeds(resolution));
            }
            else if(!computeMaxwell)
            {
                lastSpeeds[index] = gas.speeds(resolution);
                index = (index+1)%numOfLoopsRemembered;
            }

            if (!computeMaxwell || (loopCounter*100)%numOfLoopsRemembered==0)
            {
                speeds = lastSpeeds[0];
                for (int i=1; i<lastSpeeds.size(); i++)
                {
                    for (int j=0; j<speeds.size(); j++)
                    {
                        speeds[j] += lastSpeeds[i][j];
                    }
                }
            }

            elapsedMemorising += clockMemorising.getElapsedTime().asSeconds();
        }

        // render windows

        clockRendering.restart();

        if (windowHasFocus)
        {
            window.setView(sf::View(sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)));
            window.clear(sf::Color::White);
            renderParticles(window, gas);
            window.display();

            if (windowDistribution.isOpen() && distribution && (!computeMaxwell || (loopCounter*100)%numOfLoopsRemembered==0))
            {
                windowDistribution.setView(sf::View(sf::FloatRect(0.f, 0.f, windowDistribution.getSize().x, windowDistribution.getSize().y)));
                windowDistribution.clear(sf::Color::White);
                renderDistribution(windowDistribution, speeds);
                windowDistribution.display();
            }
        }

        elapsedRendering += clockRendering.getElapsedTime().asSeconds();
    }

    // new main loop if finished to compute the distribution

    while (window.isOpen() && computeMaxwell)
    {
        // event handling

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                windowDistribution.close();
            }
            else if (event.type == sf::Event::Resized || event.type == sf::Event::GainedFocus)
            {
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)));
                window.clear(sf::Color::White);
                renderParticles(window, gas);
                window.display();
            }
        }
        while (windowDistribution.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                windowDistribution.close();
            }
            else if (event.type == sf::Event::Resized || event.type == sf::Event::GainedFocus)
            {
                if (distribution)
                {
                    windowDistribution.setView(sf::View(sf::FloatRect(0.f, 0.f, windowDistribution.getSize().x, windowDistribution.getSize().y)));
                    windowDistribution.clear(sf::Color::White);
                    renderDistribution(windowDistribution, speeds);
                    windowDistribution.display();
                }
            }
        }
    }

    // display info about the execution time

    double elapsedProgramTime = clockProgram.getElapsedTime().asSeconds();
    std::cout << "Window active for " << loopCounter << " loops and " << elapsedProgramTime << " seconds" << std::endl;
    std::cout << "Average time between two loops: " << elapsedProgramTime/loopCounter*1000 << " milliseconds. " << std::endl;
    std::cout << "Average time processing: " << elapsedProcessing/loopCounter*1000 << " milliseconds. " << std::endl;
    std::cout << "Average time memorising: " << elapsedMemorising/loopCounter*1000 << " milliseconds. " << std::endl;
    std::cout << "Average time rendering: " << elapsedRendering/loopCounter*1000 << " milliseconds. " << std::endl;
    std::cout << std::endl;

    // proper exit

    std::string exitVar;
    std::cout << "Press enter to exit" << std::endl;
    std::getline(std::cin, exitVar);
    std::cout << std::endl;

    return 0;
}
