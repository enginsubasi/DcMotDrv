#include <iostream>
#include <thread>
#include <ctime>

#include "dcmotdrv.h"

using namespace std;

typedef std::chrono::system_clock Clock;

typedef std::numeric_limits< double > dbl;

// Motor pin definitions
uint32_t m1port[ 4 ] = { 0, 0, 0, 0 };
uint32_t m1pin[ 4 ] = { 0, 1, 2, 3 };
bool driveOut[ 4 ] = { false, false, false, false };

void digitalWriteAdapter ( uint32_t port, uint32_t pin, bool value )
{
    driveOut[ pin ] = value;
}

void driveLoop ( void )
{
    uint32_t i = 0;
    const long double sysTime = time(0);
    const long double sysTimeMS = sysTime*1000;
cout.precision(17);

    for ( i = 0; i < 1000; ++i )
    {
        const long double sysTime = time(0);
        const long double sysTimeMS = sysTime*1000;
        cout <<  sysTime << endl;
        // cout << i << endl;
    }
}

// Motor instances
dcMotDrv testMot ( 100, digitalWriteAdapter, m1port, m1pin );

int main ( void )
{
    thread drv ( driveLoop );
    testMot.resetAllPinsToLow ( );

    while ( true )
    {
        testMot.drive ( );
    }
}


