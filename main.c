/*
Anirudh BS
Section M
Roll No. : 03
SRN : PES1UG19CS065
*/

//Airport Simulation with one runway.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "server.h"


#define ARRIVE 0
#define DEPART 1

int mx; //mx is the number of planes that can be in queue

void main( )
{
    struct airport a ;
    int i,	//Initializing loop control variable i 
    ri,	//ri as a random integer
    curtime,	//current time as curtime
    endtime ;	//time to run simulation as endtime 
    double expectarrive, expectdepart ;	//The user specified expected number of arrivals and departures per unit time or the arrival and departure rate  
    struct plane temp ;



    initairport ( &a );

    start ( &endtime, &expectarrive, &expectdepart ) ;

    for ( curtime = 1 ; curtime <= endtime ; curtime++ )
    {
        ri = randomnumber ( expectarrive ) ;	

        for ( i = 1 ; i <= ri ; i++ )
        {
            newplane ( &a, curtime, ARRIVE ) ;
            if ( apfull ( a, 'l' ) )	//If landing queue is full
                 refuse ( &a, ARRIVE ) ;	//the arriving plane is refused to land
            else
                apaddqueue( &a, 'l' ) ;	//else it is added to landing queue
        }

        ri = randomnumber ( expectdepart ) ;	
        for ( i = 1 ; i <= ri ; i++ )
        {
            newplane ( &a, curtime, DEPART ) ;
            if ( apfull ( a, 't' ) )	//If takeoff queue is full 
               refuse ( &a, DEPART ) ;	//the plane waiting to depart is refused takeoff
            else
               apaddqueue ( &a, 't' ) ;	//else it is added to the takeoff queue
        }

        if (  ! ( apempty ( a, 'l' ) ) )	//If there are planes in the landing queue(landing queue is not empty)
        {
            temp = apdelqueue ( &a, 'l' ) ;	//deleting the front element of the queue 
            land ( &a, temp, curtime ) ;	//and allow it to land
        }
        else	//If the landing queue is empty
        {
            if ( ! ( apempty ( a, 't' ) ) )	//checking whether the takeoff queue is empty, if not empty
            {
                temp = apdelqueue ( &a, 't' ) ;	//deleting the front element of the queue
                fly ( &a, temp, curtime ) ;	//and allow it to takeoff
            }
            else	//If both the queues(landing and takeoff) are empty, the runway is idle
                idle ( &a, curtime ) ;
        }
    }

    conclude ( &a, endtime ) ;	//summarizing statistics

}
