#ifndef DC_MOT_DRV_H_
#define DC_MOT_DRV_H_

#include <stdint.h>

class dcMotDrv
{

public:
    dcMotDrv ( void );
    dcMotDrv ( void ( *digitalWritePrm )( uint32_t port, uint32_t pin, bool value ) );
    dcMotDrv ( uint32_t dutyMaxPrm, void ( *digitalWritePrm )( uint32_t port, uint32_t pin, bool value ) );
    dcMotDrv ( uint32_t dutyMaxPrm, void ( *digitalWritePrm )( uint32_t port, uint32_t pin, bool value ), uint32_t* portPrm, uint32_t* pinPrm );
    ~dcMotDrv ( void );

    void resetAllPinsToLow ( void );

    bool setDriveMode ( uint32_t value );
    bool setDriveDirection ( uint32_t value );

    bool setDuty ( uint32_t value );
    uint32_t getDuty ( );

    void drive ( void );

private:
    bool isAllPinsDefined;

    uint32_t driveState;
    uint32_t driveDirection;

    uint32_t duty;
    uint32_t dutyMax;
    uint32_t dutyMin;
    uint32_t dutyErrorCounter;

    uint32_t signalCounter;

    uint32_t port[ 4 ]; // H1H H1L H2H H2L
    uint32_t pin[ 4 ];  // H1H H1L H2H H2L

    void ( *digitalWrite )( uint32_t port, uint32_t pin, bool value );
};

/* FUNCTION DEFINITIONS */

/* DEFINITIONS */

/* STRUCTURES */

/* ENUMS */

enum motorId
{
    hah = 0,
    hal = 1,
    hbh = 2,
    hbl = 3,
};

enum driveMode
{
    driveNot = 0,
    driveRun = 1,
    driveLock = 2,
};

enum driveDirection
{
    driveForward = 0,
    driveBackward = 1,
};

/* TYPEDEFS */

/* CONSTS */

/* EXTERNS */

/* FUNCTION PROTOTYPES */

#endif /* DC_MOT_DRV_H_ */
