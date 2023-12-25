// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include "Zombie.h"

Zombie::Zombie(string name, uint32_t dist, uint32_t speed, uint32_t health, uint32_t rounds):
            name(name), dist(dist), speed(speed), health(health), rounds(rounds)
{}

void Zombie::advance()
{
    if (dist >= speed)
        dist -= speed;
    else
        dist = 0;
}

void Zombie::updateRound()
{
    rounds++;
}

string Zombie::getName() const
{
    return name;
}

uint32_t Zombie::getDist() const
{
    return dist;
}

uint32_t Zombie::getSpeed() const
{
    return speed;
}

uint32_t Zombie::getHealth() const
{
    return health;
}

uint32_t & Zombie::getHealth()
{
    return health;
}

uint32_t Zombie::getRound() const
{
    return rounds;
}

uint32_t Zombie::getETA() const
{
    return dist / speed;
}