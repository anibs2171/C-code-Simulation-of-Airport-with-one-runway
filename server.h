
#define ARRIVE 0
#define DEPART 1
int mx; //mx is the number of planes that can be in queue

struct plane   //The Plane data members plane ID, plane_arrival_time are initialized.
{
    int id ;    //Plane number
    int tm ;    //time when the plane is added to queue
} ;

struct queue
{
    int count ; //Number of airplanes in the queue
    int front ; //front of the queue
    int rear ;  //rear of queue
       struct plane p[1000] ;
} ;




struct airport
{
    struct queue landing ;  //structure variable
    struct queue takeoff ;  //structure variable  
    struct queue *pland ;   //Pointer of Structure type
    struct queue *ptakeoff ;    //Pointer of Structure type
    int idletime ;  //total time runway is idle
    int landwait, takeoffwait ; //total time planes waited to land and total time planes waited to take off

    
    int nland,  //Number of planes landed
    nplanes,    //Total number of planes processed
    nrefuse,    //Number of planes refused use
    ntakeoff ;  //Number of planes taken off
    struct plane pln ;
} ;



//Function declarations



void initqueue ( struct queue * ) ;
void addqueue ( struct queue *, struct plane ) ;
struct plane delqueue ( struct queue * ) ;
int size ( struct queue ) ;
int isempty ( struct queue ) ;
int isfull ( struct queue ) ;

void initairport ( struct airport * ) ;
void start ( int *, double *, double * ) ;
void newplane ( struct airport *, int, int ) ;
void refuse ( struct airport *, int ) ;
void land ( struct airport *, struct plane, int ) ;
void fly ( struct airport *, struct plane, int ) ;
void idle ( struct airport *, int ) ;
void conclude ( struct airport *, int ) ;
int randomnumber ( double ) ;
void apaddqueue ( struct airport *, char ) ;
struct plane apdelqueue ( struct airport *, char ) ;

int apsize ( struct airport, char ) ;
int apfull ( struct airport, char ) ;
int apempty ( struct airport, char ) ;
void myrandomize ( ) ;






















