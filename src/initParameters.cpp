#include "initParameters.h"
#include "toolFunctions.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>


bool initParameters(int argc, char* argv[], int &argNumOfParticles, int &argRadiusOfParticles, double &initSpeedParticles, int &numOfLoopsRemembered, int &resolution, double &millisecPerLoop, bool &moduloSpace, bool &distribution, bool &computeMaxwell, bool &brownianMotion, double &brownianMass, int &brownianRadius)
{
    std::vector<std::string> parametres = {"help", "number", "radius", "speed", "loopsRemembered", "resolution", "msPerLoop", "modulo", "noDistribution", "computeMaxwell", "brownianMotion", "brownianMass", "brownianRadius"};

    for (int i=1; i<argc; i++)
    {
        if (argv[i][0] != '-'){}

        if (argv[i][0] == '-' && argv[i][1] != '-'){}

        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            std::vector<std::string> argument = extractParametreAndValue(argv[i]);

            if (argument[0] == parametres[0])
            {
                std::cout << "Usable parameters:" << std::endl;

                for (int j=0; j<parametres.size(); j++)
                {
                    std::cout << "--" << parametres[j] << " ";
                }

                std::cout << std::endl;

                return true;
            }

            if (argument[0] == parametres[1])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                argNumOfParticles = std::atoi(valueInChars);

                if (argNumOfParticles<1)
                {
                    std::cerr << "Parameter \"" << parametres[1] << "\" must be a non-zero positive integer" << std::endl;
                    return true;
                }
            }

            if (argument[0] == parametres[2])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                argRadiusOfParticles = std::atoi(valueInChars);

                if (argRadiusOfParticles<1)
                {
                    std::cerr << "Parameter \"" << parametres[2] << "\" must be a non-zero positive integer" << std::endl;
                    return true;
                }
            }

            if (argument[0] == parametres[3])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                initSpeedParticles = std::atof(valueInChars);
            }

            if (argument[0] == parametres[4])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                numOfLoopsRemembered = std::atoi(valueInChars);

                if (numOfLoopsRemembered<1)
                {
                    std::cerr << "Parameter \"" << parametres[4] << " must be a non-zero positive integer" << std::endl;
                    return true;
                }
            }

            if (argument[0] == parametres[5])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                resolution = std::atoi(valueInChars);
            }

            if (argument[0] == parametres[6])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                millisecPerLoop = std::atof(valueInChars);

                if (numOfLoopsRemembered<0)
                {
                    std::cerr << "Parameter \"" << parametres[6] << " must be a positive integer" << std::endl;
                    return true;
                }
            }

            if (argument[0] == parametres[7])
            {
                moduloSpace = !moduloSpace;
            }

            if (argument[0] == parametres[8])
            {
                distribution = !distribution;
            }

            if (argument[0] == parametres[9])
            {
                computeMaxwell = !computeMaxwell;
            }

            if (argument[0] == parametres[10])
            {
                brownianMotion = !brownianMotion;
            }

            if (argument[0] == parametres[11])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                brownianMass = std::atof(valueInChars);

                if (brownianMass<=0)
                {
                    std::cerr << "Parameter \"" << parametres[11] << "\" must be a non-zero positive real number" << std::endl;
                    return true;
                }
            }

            if (argument[0] == parametres[12])
            {
                char valueInChars[argument[1].size()];
                for (int j=0; j<argument[1].size(); j++)
                {
                    valueInChars[j] = argument[1][j];
                }

                brownianRadius = std::atoi(valueInChars);

                if (brownianRadius<1)
                {
                    std::cerr << "Parameter \"" << parametres[12] << "\" must be a non-zero positive integer" << std::endl;
                    return true;
                }
            }
        }
    }

    return false;
}
