// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <string>
#include <getopt.h>
#include <algorithm>
#include <iterator>

#include "P2random.h"
#include "Zombie.h"

using namespace std;

class SortByETA
{
    const vector<Zombie*> & zbs;
    public:
        SortByETA(const vector<Zombie*> & z) : zbs(z)
        {}

        bool operator()(const size_t & i, const size_t & j) const
        {
            if (zbs[i]->getETA() > zbs[j]->getETA())
                return true;
            else if (zbs[i]->getETA() < zbs[j]->getETA())
                return false;
            else
            {
                if (zbs[i]->getHealth() > zbs[j]->getHealth())
                    return true;
                else if (zbs[i]->getHealth() < zbs[j]->getHealth())
                    return false;
                else
                    return zbs[i]->getName() > zbs[j]->getName();
            }
        }
};

// helper for round comparing
bool cmpRoundGreater(const Zombie* zb1, const Zombie* zb2)
{
    if (zb1->getRound() > zb2->getRound())
        return true;
    else if (zb1->getRound() < zb2->getRound())
        return false;
    else
        return zb1->getName() < zb2->getName();  
}

bool cmpRoundLess(const Zombie* zb1, const Zombie* zb2)
{
    if (zb1->getRound() < zb2->getRound())
        return true;
    else if (zb1->getRound() > zb2->getRound())
        return false;
    else
        return zb1->getName() < zb2->getName(); 
}

class SortByActiveRoundSmall
{
    const vector<Zombie*> & zbs;
    public:
        SortByActiveRoundSmall(const vector<Zombie*> & z) : zbs(z)
        {}

        bool operator()(const size_t & i, const size_t & j) const
        {
            return cmpRoundGreater(zbs[i], zbs[j]);
        }
};

class SortByActiveRoundBig
{
    const vector<Zombie*> & zbs;
    public:
        SortByActiveRoundBig(const vector<Zombie*> & z) : zbs(z)
        {}

        bool operator()(const size_t & i, const size_t & j) const
        {
            return cmpRoundLess(zbs[i], zbs[j]);
        }
};

class NeedHelp
{
    public:
        void printHelp()
        {
            cout << "Try [--verbose/-v] [--statistics/-s] [--median/-m]" << 
                "and give an input file about zombies.\n";
        }
};
// Read the command line
size_t getMode(int argc, char* argv[], bool & verbose, bool & statistics, bool & median)
{
    int choice;
    int option_index = 0;
    option long_options[] = 
    {
        { "verbose", no_argument, nullptr, 'v' },
        { "median", no_argument, nullptr, 'm' },
        { "statistics", required_argument, nullptr, 's'},
        { "help",  no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, '\0' }
    };

    size_t N = 0;
    while ((choice = getopt_long(argc, argv, "vms:h", long_options, &option_index)) != -1)
    {
        switch (choice)
        {
            case 'v':
                verbose = true;
                break;

            case 'm':
                median = true;
                break;

            case 's':
                statistics = true;
                N = atoi(optarg);
                break;

            case 'h':
                throw NeedHelp();
        } 
    }
    return N;
}

void printVAndUpdateQueue(vector<Zombie*> & zombies, const string & name, const uint32_t & distance, 
                    const uint32_t & speed, const uint32_t & health, const bool & verbose,
                    priority_queue<size_t, vector<size_t>, SortByETA> & ranked_zombies)
{
    if (verbose)
    {
        cout << "Created: " << name << " (distance: " << distance << ", speed: "
            << speed << ", health: " << health << ")\n";
    }
    zombies.emplace_back(new Zombie(name,distance,speed,health));
    ranked_zombies.push(zombies.size()-1); // add new "zombies" to priority queue
}

void printStatistics(vector<Zombie*> & zombies, const vector<size_t> & killed, const size_t & N)
{
    // remaining active zombies
    cout << "Zombies still active: " << zombies.size()-killed.size() << "\n";

    cout << "First zombies killed:\n";
    size_t tmp = min(N, killed.size());
    for (size_t i=1; i<=tmp; i++)
        cout << zombies[killed[i-1]]->getName() << " " << i << "\n";

    cout << "Last zombies killed:\n";
    for (size_t i=0; i<tmp; i++)
        cout << zombies[killed[killed.size()-i-1]]->getName() << " " << tmp-i << "\n";
    
    tmp = min(N, zombies.size());
    partial_sort(zombies.begin(), zombies.begin()+tmp, zombies.end(), cmpRoundGreater);
    cout << "Most active zombies:\n";
    vector<Zombie*>::iterator it;
    for (it=zombies.begin(); it!=zombies.begin()+tmp; it++)
        cout << (*it)->getName() << " " << (*it)->getRound() << "\n";
    
    partial_sort(zombies.begin(), zombies.begin()+tmp, zombies.end(), cmpRoundLess);
    cout << "Least active zombies:\n";
    for (it=zombies.begin(); it!=zombies.begin()+tmp; it++)
        cout << (*it)->getName() << " " << (*it)->getRound() << "\n";
}   

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    bool verbose = false, statistics = false, median = false;
    size_t N = 0;
    try
    {
        N = getMode(argc,argv,verbose,statistics,median);
    }
    catch(NeedHelp & e)
    {
        e.printHelp();
        return 0;
    }
    
    // Read header info
    uint32_t quiver_capacity, random_seed, max_rand_distance, max_rand_speed, max_rand_health;
    string str;
    getline(cin,str); // get the comment line
    cin >> str >> quiver_capacity >> str >> random_seed >> str >> max_rand_distance >> str 
        >> max_rand_speed >> str >> max_rand_health;

    uint32_t round, random_zombies, named_zombies, cur_round = 1, cur_quiver = quiver_capacity;
    cin >> str; // get ---
    // Read the first round info
    cin >> str >> round >> str >> random_zombies >> str >> named_zombies;

    vector<Zombie*> zombies;
    vector<size_t> killed; // killed zombies
    priority_queue<size_t, vector<size_t>, SortByETA> ranked_zombies(zombies);
    // Two queues for active time (median mode)
    priority_queue<size_t, vector<size_t>, SortByActiveRoundBig> active_time_L(zombies); // Biggest on top
    priority_queue<size_t, vector<size_t>, SortByActiveRoundSmall> active_time_R(zombies); // Smallest on top
    Zombie* eater = nullptr; // record the zombie eating brain if there is
    bool no_more_zombie = false;
    P2random::initialize(random_seed, max_rand_distance, max_rand_speed, max_rand_health);
    while (1)
    {
        if (verbose)
            cout << "Round: " << cur_round << "\n";
        // refill quiver
        cur_quiver = quiver_capacity;
        // update zombies
        for (Zombie* pz :zombies)
        {
            if (pz->getHealth())
            {
                pz->advance();
                pz->updateRound();
                if (verbose)
                    cout << "Moved: " << pz->getName() << " (distance: " << pz->getDist()
                        << ", speed: " << pz->getSpeed() << ", health: " << pz->getHealth() << ")\n";
                if (!pz->getDist() && !eater)
                    eater = pz;
            }
        }
        // check player died or not
        if (eater)
        {
            cout << "DEFEAT IN ROUND " << cur_round << "! " << eater->getName()
                << " ate your brains!\n";
            if (statistics)
                printStatistics(zombies, killed, N);
                
            break;
        }
        // Create new zombies if current round is in the input
        if (cur_round == round)
        {
            string name;
            uint32_t distance, speed, health;
            while (random_zombies--)
            {   // generate random zombies
                name  = P2random::getNextZombieName();
	            distance = P2random::getNextZombieDistance();
	            speed = P2random::getNextZombieSpeed();
	            health = P2random::getNextZombieHealth();
                printVAndUpdateQueue(zombies, name, distance, speed, health, verbose, ranked_zombies);
            }
            while (named_zombies--)
            {
                cin >> name >> str >> distance >> str >> speed >> str >> health;
                printVAndUpdateQueue(zombies, name, distance, speed, health, verbose, ranked_zombies);
            }

            if (cin >> str) // get ---
            {   // Read new round info
                cin >> str >> round;
                cin >> str >> random_zombies;
                cin >> str >> named_zombies;
            }
            else
                no_more_zombie = true;
        }
        // Player shoots zombies
        while (cur_quiver && !ranked_zombies.empty())
        {
            size_t index = ranked_zombies.top();
            if (zombies[index]->getHealth() <= cur_quiver)
            {
                cur_quiver -= zombies[index]->getHealth();
                zombies[index]->getHealth() = 0;

                if (verbose)
                {    
                    cout << "Destroyed: " << zombies[index]->getName() 
                        << " (distance: " << zombies[index]->getDist()
                        << ", speed: " << zombies[index]->getSpeed()
                        << ", health: " << zombies[index]->getHealth() << ")\n";
                }

                killed.emplace_back(index);
                if (median)
                {
                    if (active_time_L.empty())
                        active_time_L.push(index);
                    else
                    {
                        if (active_time_L.size() > active_time_R.size())
                        {
                            if (cmpRoundGreater(zombies[index], zombies[active_time_L.top()]))
                                active_time_R.push(index);
                            else
                            {
                                active_time_R.push(active_time_L.top());
                                active_time_L.pop();
                                active_time_L.push(index);
                            }
                        }
                        else if (active_time_L.size() == active_time_R.size())
                        {
                            if (cmpRoundLess(zombies[index], zombies[active_time_L.top()]))
                                active_time_L.push(index);
                            else
                                active_time_R.push(index);
                        }
                        else
                        {
                            if (cmpRoundLess(zombies[index], zombies[active_time_R.top()]))
                                active_time_L.push(index);
                            else
                            {
                                active_time_L.push(active_time_R.top());
                                active_time_R.pop();
                                active_time_R.push(index);
                            }       
                        }
                    }
                }
                ranked_zombies.pop();
            }
            else
            {
                zombies[index]->getHealth() -= cur_quiver;
                cur_quiver = 0;
            }
        }

        if (median && !killed.empty())
        {
            /*while (active_time_L.size() > killed.size()-killed.size()/2)
            {
                size_t tmp = active_time_L.top();
                active_time_R.push(tmp);
                active_time_L.pop();
            }
            while (active_time_R.size() > killed.size()/2)
            {
                size_t tmp = active_time_R.top();
                active_time_L.push(tmp);
                active_time_R.pop();
            }*/
            
            uint32_t med = 0;
            if (active_time_L.size() == active_time_R.size())
                med = (zombies[active_time_L.top()]->getRound() + zombies[active_time_R.top()]->getRound()) / 2;
            else if (active_time_L.size() > active_time_R.size())
                med = zombies[active_time_L.top()]->getRound();
            else
                med = zombies[active_time_R.top()]->getRound();
            
            cout << "At the end of round " << cur_round << ", the median zombie lifetime is "
                <<  med << "\n";
        }

        if (no_more_zombie && ranked_zombies.empty())
        {
            cout << "VICTORY IN ROUND " << cur_round << "! " << zombies[killed.back()]->getName()
                << " was the last zombie.\n";
            if (statistics)
                printStatistics(zombies, killed, N);
                
            break;
        }
        
        cur_round++;
    }

    for (Zombie* pz : zombies)
        delete pz;

    return 0;
}