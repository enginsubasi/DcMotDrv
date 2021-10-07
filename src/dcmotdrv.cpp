/**
  ******************************************************************************
  *
  * @file:      dcmotdrv.cpp
  * @author:    Engin Subasi
  * @email:     enginsubasi@gmail.com
  * @address:   github.com/enginsubasi/DcMotDrv
  *
  * @version:   v 0.0.1
  * @cdate:     07/10/2021
  * @history:   07/10/2021 Created
  *
  * @about:     dcmotdrv cpp file.
  * @device:    Generic
  *
  * @content:
  *     FUNCTIONS:
  *
  *
  * @notes:
  *
  ******************************************************************************
  */

#include "dcmotdrv.h"

dcMotDrv::dcMotDrv ( void )
{
    isAllPinsDefined = false;
    driveState = driveNot;
    driveDirection = driveForward;

    duty = 0;
    dutyMax = 100;
    dutyMin = 0;
    dutyErrorCounter = 0;

    digitalWrite = NULL;

    signalCounter = 0;
}

dcMotDrv::dcMotDrv ( void ( *digitalWritePrm )( uint32_t port, uint32_t pin, bool value ) )
{
    isAllPinsDefined = false;
    driveState = driveNot;
    driveDirection = driveForward;

    duty = 0;
    dutyMax = 100;
    dutyMin = 0;
    dutyErrorCounter = 0;

    digitalWrite = digitalWritePrm;

    signalCounter = 0;
}

dcMotDrv::dcMotDrv ( uint32_t dutyMaxPrm, void ( *digitalWritePrm )( uint32_t port, uint32_t pin, bool value ) )
{
    isAllPinsDefined = false;
    driveState = driveNot;
    driveDirection = driveForward;

    duty = 0;
    dutyMax = dutyMaxPrm;
    dutyMin = 0;
    dutyErrorCounter = 0;

    digitalWrite = digitalWritePrm;

    signalCounter = 0;
}

dcMotDrv::dcMotDrv ( uint32_t dutyMaxPrm, void ( *digitalWritePrm )( uint32_t port, uint32_t pin, bool value ), uint32_t* portPrm, uint32_t* pinPrm )
{
    driveState = driveNot;
    driveDirection = driveForward;

    duty = 0;
    dutyMax = dutyMaxPrm;
    dutyMin = 0;
    dutyErrorCounter = 0;

    digitalWrite = digitalWritePrm;

    signalCounter = 0;

    port[ hah ] = portPrm[ hah ];
    port[ hal ] = portPrm[ hal ];
    port[ hbh ] = portPrm[ hbh ];
    port[ hbl ] = portPrm[ hbl ];

    pin[ hah ] = pinPrm[ hah ];
    pin[ hal ] = pinPrm[ hal ];
    pin[ hbh ] = pinPrm[ hbh ];
    pin[ hbl ] = pinPrm[ hbl ];

    isAllPinsDefined = true;
}

void dcMotDrv::resetAllPinsToLow ( void )
{
    if ( isAllPinsDefined == true )
    {
        digitalWrite ( port[ hah ], pin[ hah ], false );
        digitalWrite ( port[ hal ], pin[ hal ], false );
        digitalWrite ( port[ hbh ], pin[ hbh ], false );
        digitalWrite ( port[ hbl ], pin[ hbl ], false );
    }
    else
    {
        /* Intentionally blank */
    }
}

bool dcMotDrv::setDriveMode ( uint32_t value )
{
    bool retVal = false;

    if ( ( value == driveNot ) || ( value == driveRun ) || ( value == driveLock ) )
    {
        driveState = value;
        retVal = true;
    }
    else
    {
        driveState = driveNot;
        retVal = false;
    }

    if ( ( driveState == driveNot ) || ( driveState == driveLock ) )
    {
    	signalCounter = 0;
    }

    return ( retVal );
}

bool dcMotDrv::setDriveDirection ( uint32_t value )
{
    bool retVal = false;

    if ( ( value == driveForward ) || ( value == driveBackward ) )
    {
        driveDirection = value;
        retVal = true;
    }
    else
    {
        driveDirection = driveForward;
        retVal = false;
    }

    return ( retVal );
}

bool dcMotDrv::setDuty ( uint32_t value )
{
    bool retVal = false;

    if ( ( value >= dutyMin ) && ( value <= dutyMax ) )
    {
        duty = value;

        if ( dutyErrorCounter )
        {
            --dutyErrorCounter;
        }

        retVal = true;
    }
    else
    {
        if ( dutyErrorCounter > 5 ) // Arbitrary
        {
            duty = 0; // Safe state
        }
        else
        {
            ++dutyErrorCounter;
        }

        retVal = false;
    }

    return ( retVal );
}

uint32_t dcMotDrv::getDuty ( void )
{
    return ( duty );
}

void dcMotDrv::drive ( void )
{
    if ( isAllPinsDefined == true )
    {
        switch ( driveState )
        {
        case driveNot:

            digitalWrite ( port[ hah ], pin[ hah ], false );
            digitalWrite ( port[ hal ], pin[ hal ], false );

            digitalWrite ( port[ hbh ], pin[ hbh ], false );
            digitalWrite ( port[ hbl ], pin[ hbl ], false );

            break;

        case driveRun:

            ++signalCounter;

            switch ( driveDirection )
            {

            case driveForward:

                digitalWrite ( port[ hal ], pin[ hal ], false );
                digitalWrite ( port[ hbh ], pin[ hbh ], false );

                if ( signalCounter <= duty )
                {
                    digitalWrite ( port[ hah ], pin[ hah ], true );
                    digitalWrite ( port[ hbl ], pin[ hbl ], true );
                }
                else
                {
                    digitalWrite ( port[ hah ], pin[ hah ], false );
                    digitalWrite ( port[ hbl ], pin[ hbl ], false );

                    if ( signalCounter > dutyMax )
                    {
                        signalCounter = 0;
                    }
                }

                break;

            case driveBackward:

                digitalWrite ( port[ hah ], pin[ hah ], false );
                digitalWrite ( port[ hbl ], pin[ hbl ], false );

                if ( signalCounter <= duty )
				{
					digitalWrite ( port[ hbh ], pin[ hbh ], true );
					digitalWrite ( port[ hal ], pin[ hal ], true );
				}
				else
				{
					digitalWrite ( port[ hbh ], pin[ hbh ], false );
					digitalWrite ( port[ hal ], pin[ hal ], false );

					if ( signalCounter > dutyMax )
					{
						signalCounter = 0;
					}
				}
               break;

            default:

                break;
            }

            break;

        case driveLock:

            digitalWrite ( port[ hah ], pin[ hah ], true );
            digitalWrite ( port[ hal ], pin[ hal ], false );

            digitalWrite ( port[ hbh ], pin[ hbh ], true );
            digitalWrite ( port[ hbl ], pin[ hbl ], false );

        break;

        default:

            break;
        }
    }
    else
    {
        /* Intentionally blank */
    }
}
