#ifndef GAS_H
#define GAS_H

#include <vector>


class Gas
{
    public:
        Gas(int boxSizeX, int boxSizeY, bool moduloSpace, int numPart, int radiusPart, double speedPart, bool brownianMotion, double brownianMass, int brownianRadius);
        ~Gas();

        std::vector<std::vector<double>> Getparticles(){return m_particles;}
        bool GetbrownianMotion(){return m_brownianMotion;}

        void moveParticles(double dt);
        void performCollisions();
        void resizeBox(int sizeX, int sizeY);
        std::vector<int> speeds(int numOfSegments);
        void randPlaceParticles(int num, int radius, double speed);

    protected:

    private:
        bool m_moduloSpace;
        bool m_brownianMotion;
        double m_initSpeed;
        std::vector<int> m_boxSize;
        std::vector<std::vector<double>> m_particles;   // posX, posY, velX, velY, mass, radius

        std::vector<double> vecRotation(std::vector<double> vec, double angle);
};

#endif // GAS_H
