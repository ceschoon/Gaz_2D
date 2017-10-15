#include "toolFunctions.h"
#include <cstring>


std::vector<std::string> extractParametreAndValue(char* argument)
{
    std::string parametre = "";
    std::string value = "";
    bool foundEqual = false;

    for (int j=2; j<std::strlen(argument); j++)
    {
        if (argument[j]!='=')
        {
            if (!foundEqual){parametre += argument[j];}
            else {value += argument[j];}
        }
        else
        {
            foundEqual = true;
        }
    }

    return std::vector<std::string> {parametre, value};
}
