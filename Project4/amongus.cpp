// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <time.h>

#include "amgus.h"

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers
    /*cerr << fixed << showpoint << setprecision(2);
    cin >> std::boolalpha;
    cout << std::boolalpha;*/

    AmongUs amgus;
    try
    {
        amgus.get_mode(argc, argv);
    }
    catch(const NeedHelp & help)
    {
        help.print();
        return 0;
    }
    catch(const ErrorCheck & e)
    {
        e.print();
        return 1;
    }
    
    
    amgus.read_graph();

    try
    {
        amgus.solve();
    }
    catch(const NoMST & no_mst)
    {
        no_mst.print();
        return 1;
    }

    return 0;
}