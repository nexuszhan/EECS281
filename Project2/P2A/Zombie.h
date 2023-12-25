// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <string>
using namespace std;

class Zombie
{
    private:
        string name;
        uint32_t dist;
        uint32_t speed;
        uint32_t health;
        uint32_t rounds;
    public:
        Zombie(string name, uint32_t dist, uint32_t speed, uint32_t health, uint32_t rounds=1);
        void advance();
        void updateRound();
        string getName() const;
        uint32_t getDist() const;
        uint32_t getSpeed() const;
        uint32_t getHealth() const;
        uint32_t & getHealth();
        uint32_t getRound() const;
        uint32_t getETA() const;
};

#endif