#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include"server.h"
 
#define ARRIVE 0
#define DEPART 1
int mx; //mx is the number of planes that can be in queue


void initqueue ( struct queue *pq ) //the members of the structure queue are set to default values
{
    pq -> count = 0 ;
    pq -> front = 0 ;
    pq -> rear = -1 ;
}

void addqueue ( struct queue *pq, struct plane item )   //If possible, the Plane is added to the Queue; 

{
    if ( pq -> count >= mx )
    {
        printf ( "\nQueue is full.\n" ) ;   //otherwise, a statement indicationg overflow is printed.
        return ;
    }
    ( pq -> count )++ ;

    pq -> rear = ( pq -> rear + 1 ) % mx ; //rear position is incremented, when a new plane is added to the queue(enqueue operation )
    pq -> p[pq -> rear] = item ;
}

struct plane delqueue ( struct queue *pq )  //called when a plane lands or takes off, to remove it from the queue
{
    struct plane p1 ;

    if ( pq -> count <= 0 )
    {
        printf ( "\nQueue is empty.\n" ) ;
        p1.id = 0 ;
        p1.tm = 0 ;
    }
    else    //when the queue is not empty
    {
        ( pq -> count )-- ;
        p1 = pq -> p[pq -> front] ;
        pq -> front = ( pq -> front + 1 ) % mx ;   //next position becomes the front of the queue (dequeue operation)
    }
    return p1 ;
}


int size ( struct queue q ) //returns the number of airplanes in the queue
{
    return q.count ;
}

int isempty ( struct queue q )    //returns true if the queue is empty, otherwise returns false
{
    return ( q.count <= 0 ) ;
}

int isfull ( struct queue q ) //returs true if the queue is full, otherwise returns false
{
    return ( q.count >= mx ) ;
}   


void initairport ( struct airport *ap )
{   //initializing landing and takeoff queues
    initqueue ( &( ap-> landing ) ) ;   
    initqueue ( &( ap -> takeoff ) ) ;

    ap -> pland = &( ap -> landing ) ;  //dereferencing
    ap -> ptakeoff = &( ap -> takeoff ) ;   //dereferencing
    ap -> nplanes = ap -> nland = ap -> ntakeoff = ap -> nrefuse = 0 ;  //initialization to default values
    ap -> landwait = ap -> takeoffwait = ap -> idletime = 0 ;
}

void start ( int *endtime, double *expectarrive, double *expectdepart ) 
{
    int flag = 0 ;
    char newnums ;

    printf ( "\nProgram that simulates an airport with only one runway.\n" ) ;
    printf ( "One plane can land or depart in each unit of time.\n" ) ;
    printf ( "Enter max number of planes allowed in the queue:\n" ) ;
    scanf ( "%d", &mx );
    printf ( "Up to %d planes can be waiting to land or take off at any time.\n", mx ) ;
    printf ( "How many units of time will the simulation run?" ) ;
    scanf ( "%d", endtime ) ;   //The user specifies the number of time units in the simulation
    myrandomize( ) ;
    do
    {
        printf ( "\nExpected number of arrivals per unit time? " ) ;
        scanf ( "%lf", expectarrive ) ; //The user specifies the expected number of arrivals per unit time or the arrival rate
        printf ( "\nExpected number of departures per unit time? " ) ;
        scanf ( "%lf", expectdepart ) ; //The user specifies the expected number of departures per unit time or the departure rate

        if ( *expectarrive < 0.0 || *expectdepart < 0.0 )
        {
            printf ( "These numbers must be nonnegative.\n" ) ;
            flag = 0 ;
        }
        else
        {
            if ( *expectarrive + *expectdepart > 1.0 )
            {
                printf ( "The airport will become saturated. Press 'y' and enter to read new numbers  \n or Press other character key and enter to continue     :" ) ;   
                //If the sum of the arrival rate and departure rate is greater than 1, then the airport becomes saturated
                scanf ( " %c", &newnums ) ;
                if ( tolower ( newnums ) == 'y' )
                    flag = 0 ;
                else
                    flag = 1 ;
            }
            else
                flag = 1 ;
        }
    } while ( flag == 0 ) ;
}

void newplane ( struct airport *ap, int curtime, int action )   //Makes a new record for a plane. 
//Makes a new structure for a plane and updates nplanes
{
    ( ap -> nplanes )++ ;   //value of total planes processed is incremented 
    ap -> pln.id = ap -> nplanes ;  // and that number is assigned to the plane ID
    ap -> pln.tm = curtime ;    //the current time is assigned as the time when the plane has been added to the queue

    switch ( action )
    {
        case ARRIVE :   //statement is printed, indicating the action of the new plane(whether it is landing or taking off)
            printf ( "\n" ) ;
            printf ( "Plane %d ready to land.\n", ap -> nplanes ) ;
            break ;

        case DEPART :
            printf ( "\nPlane %d ready to take off.\n", ap -> nplanes ) ;
            break ;
    }
}

void refuse ( struct airport *ap, int action )  //Processes a Plane wanting to use Runway, when the Queue is full
{
    switch ( action )   //appropriate statement is printed based on whether the plane is about to arrive or depart
    {
        case ARRIVE :

             printf ( "\tplane  %d directed to another airport.\n", ap -> pln.id ) ;
             break ;

        case DEPART :

             printf ( "\tplane %d told to try later.\n", ap -> pln.id ) ;
             break ;
    }
    ( ap -> nrefuse )++ ;
}

void land ( struct airport *ap, struct plane pl, int curtime )  //Processes a Plane that is landing at the specified time.
{
    int wait ;

    wait = curtime - pl.tm ;    //waiting time is the difference between the current time and the time at which the plane was added to the queue
    printf ( "%d: Plane %d landed;", curtime, pl.id ) ;
    printf ( " in queue  for %d time units \n", wait ) ;
    ( ap -> nland ) ++ ;
    ( ap -> landwait ) += wait ;    //the waiting time for landing is updated
}

void fly ( struct airport *ap, struct plane pl, int curtime )   //Processes a Plane that is taking off at the specified time
{
    int wait ;

    wait = curtime - pl.tm ;    //waiting time is the difference between the current time and the time at which the plane was added to the queue
    printf ( "%d: Plane %d took off;", curtime, pl.id ) ;
    printf ( " in queue for %d time units \n", wait ) ;
    ( ap -> ntakeoff )++ ;
    ( ap -> takeoffwait ) += wait ; //the waiting time for takeoff is updated
}

void idle ( struct airport *ap, int curtime )   //Updates the variables during the time when the runway is idle
{
    printf ( "%d: Runway is idle.\n", curtime ) ;
    ap -> idletime++ ;  //idletime is updated
}

void conclude ( struct airport *ap, int endtime )   //Runway usage statistics are summarized and printed.
{
    printf ( "\tSimulation has concluded after %d units.\n", endtime ) ;
    printf ( "\tTotal number of planes processed: %d\n", ap -> nplanes ) ;
    printf ( "\tNumber of planes landed: %d\n", ap -> nland ) ;
    printf ( "\tNumber of planes taken off: %d\n", ap -> ntakeoff ) ;
    printf ( "\tNumber of planes refused use: %d\n", ap -> nrefuse ) ;
    printf ( "\tNumber left ready to land: %d\n", apsize ( *ap, 'l' ) ) ;
    printf ( "\tNumber left ready to take off: %d\n", apsize ( *ap, 't' ) ) ;

    if ( endtime > 0 )
        printf ( "\tPercentage of time runway idle: %lf \n", ( ( double ) ap -> idletime / endtime ) * 100.0 ) ;

    if ( ap -> nland > 0 )
        printf ( "\tAverage wait time to land: %lf \n", ( ( double ) ap -> landwait / ap -> nland ) ) ;

    if ( ap -> ntakeoff > 0 )
        printf ( "\tAverage wait time to take off: %lf \n", ( ( double ) ap -> takeoffwait / ap -> ntakeoff ) ) ;
}

int randomnumber ( double expectedvalue )   // // Returns a Poisson random variable according to Poisson Distributiom
// Poisson random variable counts the total number of occurrences during a given time period 
//when an event has a small probability of occurring and a large number of independent trials take place.
{
    int n = 0 ; //counter of iteration
    double limit ;
    double x ;  //pseudo random number

    limit = exp ( -expectedvalue ) ;
    x = rand( ) / ( double ) INT_MAX ;

    while ( x > limit )
    {
        n++ ;
        x *= rand( ) / ( double ) INT_MAX ;
    }

    return n ;
}

void myrandomize( ) //sets starting point for pseudo random integers
{
    srand ( ( unsigned int ) ( time ( NULL ) % 10000 ) ) ;
}

void apaddqueue ( struct airport *ap, char type )
{
    switch ( tolower( type ) )
    {
        case'l' :   //If the plane is about to land
              addqueue ( ap -> pland, ap -> pln ) ; //add it to the landing queue
              break ;

        case't' :   //If the plane is about to takeoff 
              addqueue ( ap -> ptakeoff, ap -> pln ) ;  //add it to the takeoff queue
              break ;
    }
}

struct plane apdelqueue ( struct airport *ap, char type )
{
    struct plane p1 ;

    switch ( tolower ( type ) )
    {
        case'l' :
              p1 = delqueue ( ap -> pland ) ;   //delqueue function is used to delete the front element of the landing queue
              break ;

        case't' :
              p1 = delqueue ( ap -> ptakeoff ) ;    //delqueue function is used to delete the front element of the takeoff queue
              break ;
    }

    return p1 ;
}

int apsize ( struct airport ap, char type )  
{
    switch ( tolower ( type ) )
    {
        case'l' :
              return ( size ( *( ap.pland ) ) ) ;   //Returns the size of landing queue

        case't' :
              return ( size ( *( ap.ptakeoff ) ) ) ;    //Returns the size of takeoff queue
    }

    return 0 ;
}

int apfull ( struct airport ap, char type )
{
    switch ( tolower ( type ) )
    {
        case'l' :
              return (isfull ( *( ap.pland ) ) ) ;  //if the landing queue is full, returns true

        case't' :
              return ( isfull ( *( ap.ptakeoff ) ) ) ;  //if the takeoff queue is full, returns true

    }

    return 0 ;
}

int apempty ( struct airport ap, char type )
{
    switch ( tolower ( type ) )
    {
        case'l' :
              return ( isempty ( *( ap.pland ) ) ) ;    //if the landing queue is empty, returns true

        case't' :
              return ( isempty ( *( ap.ptakeoff ) ) ) ; //if the takeoff queue is empty, returns true
    }

    return 0 ;
}


