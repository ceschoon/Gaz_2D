#include "Gas.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <ctime>


Gas::Gas(int boxSizeX, int boxSizeY, bool moduloSpace, int numPart, int radiusPart, double speedPart, bool brownianMotion, double brownianMass, int brownianRadius)
{
    m_moduloSpace = moduloSpace;
    m_brownianMotion = brownianMotion;
    m_boxSize.clear();
    m_boxSize.push_back(boxSizeX);
    m_boxSize.push_back(boxSizeY);
    m_particles.clear();
    m_initSpeed = speedPart;

    randPlaceParticles(numPart, radiusPart, speedPart);
    if (brownianMotion) {m_particles.push_back({boxSizeX/2,boxSizeY/2,0,0,brownianMass,brownianRadius});}
}

Gas::~Gas()
{
    //dtor
}

void Gas::moveParticles(double dt)
{
    for (int i=0; i<m_particles.size(); i++)
    {
        m_particles[i][0] = m_particles[i][0] + m_particles[i][2]*dt;
        m_particles[i][1] = m_particles[i][1] + m_particles[i][3]*dt;

        if (m_moduloSpace)
        {
            if (m_particles[i][0] < 0)
            {
                m_particles[i][0] += double(m_boxSize[0]);
            }
            else if (m_particles[i][0] >= double(m_boxSize[0]))
            {
                m_particles[i][0] -= double(m_boxSize[0]);
            }
            else if (m_particles[i][1] < 0)
            {
                m_particles[i][1] += double(m_boxSize[1]);
            }
            else if (m_particles[i][1] >= double(m_boxSize[1]))
            {
                m_particles[i][1] -= double(m_boxSize[1]);
            }
        }
    }
}

void Gas::performCollisions()
{
    for (int i=0; i<m_particles.size(); i++)
    {
        for (int j=i+1; j<m_particles.size(); j++)
        {
            double posXI = m_particles[i][0];
            double posYI = m_particles[i][1];
            double radiusI = m_particles[i][5];
            double posXJ = m_particles[j][0];
            double posYJ = m_particles[j][1];
            double radiusJ = m_particles[j][5];

            //if (sqrt(pow(posXI-posXJ,2)+pow(posYI-posYJ,2)) <= radiusI+radiusJ)       more beautiful but too slow
            if ((posXI-posXJ)*(posXI-posXJ)+(posYI-posYJ)*(posYI-posYJ) <= (radiusI+radiusJ)*(radiusI+radiusJ))
            {
                double velXI = m_particles[i][2];
                double velYI = m_particles[i][3];
                double massI = m_particles[i][4];
                double velXJ = m_particles[j][2];
                double velYJ = m_particles[j][3];
                double massJ = m_particles[j][4];

                double collAngle;

                if (posXJ==posXI && posYJ==posYI)
                {
                    collAngle = double(rand()%1000)/1000*atan(1)*8;
                }
                else
                {
                    collAngle = atan2(posXJ-posXI,posYJ-posYI);
                }

                std::vector<double> velI = {velXI, velYI};
                std::vector<double> velJ = {velXJ, velYJ};
                std::vector<double> vel2I = vecRotation(velI,-collAngle);    // in new coord system where collision along axisX
                std::vector<double> vel2J = vecRotation(velJ,-collAngle);
                std::vector<double> Vel2I = vel2I;  // after collision
                std::vector<double> Vel2J = vel2J;

                Vel2I[0] = vel2I[0]*(massI-massJ)/(massI+massJ) + vel2J[0]*(2*massJ)/(massI+massJ);
                Vel2I[1] = vel2I[1];
                Vel2J[0] = vel2J[0]*(massJ-massI)/(massI+massJ) + vel2I[0]*(2*massI)/(massI+massJ);
                Vel2J[1] = vel2J[1];

                velI = vecRotation(Vel2I,collAngle);
                velJ = vecRotation(Vel2J,collAngle);
                m_particles[i][2] = velI[0];
                m_particles[i][3] = velI[1];
                m_particles[j][2] = velJ[0];
                m_particles[j][3] = velJ[1];
            }
        }
    }

    if (!m_moduloSpace)
    {
        // following not correct if particles knocks a corner
        for (int i=0; i<m_particles.size(); i++)
        {
            if (m_particles[i][0]-m_particles[i][5]<=0)
            {
                m_particles[i][2] = -m_particles[i][2];
                m_particles[i][0] = m_particles[i][5];
            }
            else if (m_particles[i][1]-m_particles[i][5]<=0)
            {
                m_particles[i][3] = -m_particles[i][3];
                m_particles[i][1] = m_particles[i][5];
            }
            else if (m_particles[i][0]+m_particles[i][5]>=double(m_boxSize[0]))
            {
                m_particles[i][2] = -m_particles[i][2];
                m_particles[i][0] = double(m_boxSize[0])-m_particles[i][5];
            }
            else if (m_particles[i][1]+m_particles[i][5]>=double(m_boxSize[1]))
            {
                m_particles[i][3] = -m_particles[i][3];
                m_particles[i][1] = double(m_boxSize[1])-m_particles[i][5];
            }
        }
    }
}

std::vector<double> Gas::vecRotation(std::vector<double> vec, double angle)
{
    std::vector<double> out = {0,0};

    out[0] = vec[0]*cos(angle) - vec[1]*sin(angle);
    out[1] = vec[0]*sin(angle) + vec[1]*cos(angle);

    return out;
}

void Gas::resizeBox(int sizeX, int sizeY)
{
    m_boxSize[0] = sizeX;
    m_boxSize[1] = sizeY;
}

std::vector<int> Gas::speeds(int numOfSegments)
{
    //double vMax = m_initSpeed*2.5;
    double vMax = 400;

    std::vector<int> speeds;
    speeds.clear();
    for (int i=0; i<numOfSegments; i++)
    {
        speeds.push_back(0);
    }

    for (int i=0; i<m_particles.size(); i++)
    {
        double vPart = sqrt(pow(m_particles[i][2],2)+pow(m_particles[i][3],2));
        int segment = int(vPart/vMax*numOfSegments);
        if (segment<numOfSegments)
        {
            speeds[segment] += 1;
        }
    }

    return speeds;
}

void Gas::randPlaceParticles(int num, int radius, double speed)
{
    std::time_t timer;
    srand(time(&timer));

    for (int i=0; i<num; i++)
    {
        double x = double(rand()%m_boxSize[0]);
        double y = double(rand()%m_boxSize[1]);
        double angle = double(rand()%1000)/1000*atan(1)*8;
        double vx = speed*cos(angle);
        double vy = speed*sin(angle);

        m_particles.push_back({x,y,vx,vy,1,radius});
    }
}
