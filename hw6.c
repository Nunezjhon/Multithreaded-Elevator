#include "hw6.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//
// Name: Jhon Nunez
// netid: jnunez34
// Homework 6 CS361
//
//----------------------------------------------------------------------------------------------------------------------------------------
//#define CAPACITY 2
pthread_mutex_t lock;
pthread_barrier_t barr;


// 		----------------SINGLE ELEVATOR TESTING--------------
//int current_floor;
//int direction;
//int occupancy;
//enum {ELEVATOR_ARRIVED=1, ELEVATOR_OPEN=2, ELEVATOR_CLOSED=3} state;
//struct elev elevators[ELEVATORS];
//struct floor atFloor[FLOORS];
// 		----------------SINGLE ELEVATOR TESTING--------------

//----------------------------------------------------------------------------------------------------------------------------------------
//ELEVATOR STRUCT
struct elev //elevator struct
{
        int number;
        int current_floor;
        int direction;
        int occupancy;
	int targetFloor;
        enum {ELEVATOR_ARRIVED=1, ELEVATOR_OPEN=2, ELEVATOR_CLOSED=3} state


}elevators[ELEVATORS];
//----------------------------------------------------------------------------------------------------------------------------------------
//FLOOR STRUCT
struct floor 
{
	int callButton;
	int elevatorHere; //what elevator is here
//	pthread_mutex_lock(&lock);
//	pthread_barrier_wait(&barr);

}atFloor[FLOORS];
//----------------------------------------------------------------------------------------------------------------------------------------
//PASSENGER STRUCT
struct pass
{
	int elevNum;

}passengerN[PASSENGERS];
//----------------------------------------------------------------------------------------------------------------------------------------
void scheduler_init() {
// 		----------------SINGLE ELEVATOR TESTING--------------
//		current_floor=0;
//		direction=-1;
//		occupancy=0;
//		state=ELEVATOR_ARRIVED;
//		pthread_mutex_init(&lock,0);
//		pthread_barrier_init(&barr,NULL, 1);
// 		----------------SINGLE ELEVATOR TESTING--------------

        int i;
        for (i=0; i < ELEVATORS; i++){//initialize each elevator

                elevators[i].number = i; 
                elevators[i].current_floor = 0;
                elevators[i].direction = -1; 
                elevators[i].occupancy = 0; 
		elevators[i].state=ELEVATOR_ARRIVED; //elevator state
		elevators[i].targetFloor = -1; //add which floor your elevator is heading to 

        }
/*
	for (i=0; i < FLOORS; i++){//initialize each floor

		atFloor[i].callButton = -999;
		atFloor[i].elevatorHere = 0;// what floor the elevator is located

	}

	for (i=0; i < PASSENGERS; i++){//initialize each passenger
	
		passengerN[i].elevNum = ( rand() % ( ((ELEVATORS+1)-0) +  1) );//assign a random number to each passenger

	}
*/
	pthread_mutex_init(&lock,0);
        pthread_barrier_init(&barr,NULL, 1);

}

//----------------------------------------------------------------------------------------------------------------------------------------
void passenger_request(int passenger, int from_floor, int to_floor, void (*enter)(int, int), void(*exit)(int, int)) {

// 		----------------TESTING--------------
//	int x = rand() % ( ((ELEVATORS+1) - 0) +  1);//assign passenger a random elevator
// 		----------------TESTING--------------
	int assignedElevator;
	int i;
	int waiting = 1;
	int riding=1;

	while(waiting) {
		
//		pthread_barrier_wait(&barr);
		pthread_mutex_lock(&lock); //   call wait after lock will cause deadlock

		for (i = 0; i < ELEVATORS; i++){//loop through all the elevators

			if(elevators[i].current_floor == from_floor && elevators[i].state == ELEVATOR_OPEN && elevators[i].occupancy==0) {

				enter(passenger, i);// tell passegner to enter elevator
				elevators[i].occupancy++;
				waiting=0;
				elevators[i].targetFloor = to_floor;
				assignedElevator = i;
											
			  break; //break once a passenger is assigned an elevator
		  	}

		}

		pthread_mutex_unlock(&lock);
		pthread_barrier_wait(&barr); //call barrier after unlock will cause deadlock

	}
	
	while(riding) {

//		pthread_barrier_wait(&barr);
		pthread_mutex_lock(&lock); //lock

		if( elevators[assignedElevator].current_floor == to_floor && elevators[assignedElevator].state == ELEVATOR_OPEN) {
			
			elevators[assignedElevator].targetFloor = -1;
			exit(passenger, assignedElevator);//tell passenger to exit his elevator
			elevators[assignedElevator].occupancy--;
			riding=0;
			

		}

		pthread_mutex_unlock(&lock); //unlock
		pthread_barrier_wait(&barr); //call barrier after unlock
	}

	pthread_barrier_wait(&barr);//call barrier after while look again

}
//----------------------------------------------------------------------------------------------------------------------------------------
void elevator_ready(int elevator, int at_floor, void(*move_direction)(int, int), void(*door_open)(int), void(*door_close)(int)) {


// 		----------------SINGLE ELEVATOR TESTING--------------
//	pthread_barrier_wait(&barr);
//	if(elevator!=0) return;
//	pthread_mutex_lock(&lock);
// 		----------------SINGLE ELEVATOR TESTING--------------



	if( elevators[elevator].state == ELEVATOR_ARRIVED) {

		door_open(elevator);
		elevators[elevator].state = ELEVATOR_OPEN;

	}
	else if( elevators[elevator].state == ELEVATOR_OPEN ) {

		door_close( elevator );
		elevators[elevator].state = ELEVATOR_CLOSED;

	}
	else {

		if (elevators[elevator].targetFloor != -1){//MAKE EVELEVATOR SMART

//		while (elevators[elevator].targetFloor != -1){

			if (at_floor > elevators[elevator].targetFloor){//If the requested floor is under the current floor
				elevators[elevator].direction = -1 ; // go down -1
			}
			else if (at_floor < elevators[elevator].targetFloor) {//if the requested floor is above the current floor
				elevators[elevator].direction = 1 ;// go up 1
			}
		//	else if (at_floor == elevators[elevator].targetFloor){
		//		elevators[elevator].direction = 0 ;
		//	}

		}

//		||elevators[elevator].targetFloor > at_floor
//		||elevators[elevator].targetFloor < at_floor

		if (at_floor==0  ){//If the elevator is on the bottom floor, go up
			elevators[elevator].direction = 1 ;// 1
		}
		else if (at_floor == (FLOORS -1) ) {//If the elevator is on the top floor, go down
			elevators[elevator].direction = -1 ;//-1
		}

		move_direction( elevator , elevators[elevator].direction);
		elevators[elevator].current_floor = at_floor + elevators[elevator].direction;
		elevators[elevator].state = ELEVATOR_ARRIVED;

	}


//	pthread_mutex_unlock(&lock);
	pthread_barrier_wait(&barr);


}
//----------------------------------------------------------------------------------------------------------------------------------------
