#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"elevator.h"
#include"event_list.h"
#include"circular_buffer.h"

extern unsigned int time;
extern struct circularBuffer pushedButtonsQueue;

extern struct passengerList waitingUp[];
extern struct passengerList waitingDown[];

extern struct eventList _eventList;

void initElevator(struct elevator *_elevator, int numLevels) {
  _elevator->currentPosition = 0;
  _elevator->destination = 0;
  _elevator->stateFuncPtr = elevatorIdle;
  _elevator->numLevels = numLevels;
  _elevator->numPassengers = 0;
  _elevator->capacity = 10;
  initPassengerList( &(_elevator->passengers) );
}

/* 
 * Example of state function for idle state. This shows how things work, and 
 * you are expected to modify this function.
 */
void elevatorIdle(struct elevator *_elevator, struct event* curEvent) {
	if(curEvent->type == EVENT_BUTTON_PUSH || curEvent->type == EVENT_BUTTON_DELAYED) {
		//printf("Button was pushed @ floor %d.  \n", curEvent->source);
		//If elevator at position of button push:
		//   Open doors and create departure event
		if (_elevator->currentPosition == curEvent->source) { 
			//printf("Elevator already there Loading passengers\n");
			addEvent(&_eventList, EVENT_DEPARTURE, time + 10 ,_elevator->currentPosition, curEvent->source);
			_elevator->stateFuncPtr = elevatorDoorsClose;
		}
		//    
		// If button push at some other position than elevator:
		//   Move elevator to floor of button push.
		//   Create arrival event.
		else {
			//printf("Elevator moving to passenger @ floor:%d from floor %d\n", curEvent->source, _elevator->currentPosition);
			addEvent(&_eventList, EVENT_ARRIVE,	time + abs(curEvent->source - _elevator->currentPosition)*5, _elevator->currentPosition, curEvent->source);
			_elevator->stateFuncPtr = elevatorMoving;
		}
	}

}

void elevatorMoving(struct elevator* _elevator, struct event* curEvent) {
	if (curEvent->type == EVENT_ARRIVE){
		_elevator->currentPosition = curEvent->destination;
		//printf("Elevator has arived @ floor %d\n", _elevator->currentPosition);
		addEvent(&_eventList, EVENT_DEPARTURE, time + 10, _elevator->currentPosition, curEvent->destination);
		
		_elevator->stateFuncPtr = elevatorDoorsClose;

	//All buttonpresses will be delayed by 20 time units
	}else if(curEvent->type == EVENT_BUTTON_PUSH || curEvent->type == EVENT_BUTTON_DELAYED){
		addEvent(&_eventList, EVENT_BUTTON_DELAYED, time + 40, curEvent->source, curEvent->destination);
	}
}

void elevatorDoorsClose(struct elevator *_elevator, struct event* curEvent) {
	if(curEvent->type == EVENT_DEPARTURE){
		/**
		 *Start unloading passengers 
		 */
		struct passenger *p;
		//We have no passengers
		if (peekNextPassenger(&_elevator->passengers,p) == NULL){
		//We have passengers
		}else {
			while(1){
				struct passenger *pass = peekNextPassenger(&_elevator->passengers, p);
				//While there are passengers that want to get off 
				if(pass != NULL && pass->destinationLevel == _elevator->currentPosition){
					//Remove them from the elevator
					_elevator->numPassengers--;
					getNextPassenger(&_elevator->passengers, p);
					printf("Passenger unloaded @ floor %d\n", _elevator->currentPosition);
				}else{
					break;
				}
			}
		}
		/**
		 * Start loading new passengers
		 */
		switch (elevatorDirection(_elevator)){	
		//Ascending
		case 1:
			while (_elevator->capacity - _elevator->numPassengers > 0 && peekNextPassenger(&waitingUp[_elevator->currentPosition],p) != NULL){
				struct passenger *pass = getNextPassenger(&waitingUp[_elevator->currentPosition], p);
				addPassenger(&_elevator->passengers, pass->time, pass->sourceLevel, pass->destinationLevel, SORT_DESTINATION_ASCENDING);
				free(pass);
				_elevator->numPassengers++;
				printf("Passenger loaded @ floor %d @ time %u\n", _elevator->currentPosition, curEvent->time);				
			}
			break;
		//Descending
		case -1:
			while (_elevator->capacity - _elevator->numPassengers > 0 && peekNextPassenger(&waitingDown[_elevator->currentPosition - 1],p) != NULL){
				struct passenger *pass = getNextPassenger(&waitingDown[_elevator->currentPosition - 1], p);
				addPassenger(&_elevator->passengers, pass->time, pass->sourceLevel, pass->destinationLevel, SORT_DESTINATION_DESCENDING);
				free(pass);
				_elevator->numPassengers++;
				printf("Passenger loaded @ floor %d @ time %u\n", _elevator->currentPosition, curEvent->time);
			}
		break;
		case 0:
			if(peekNextPassenger(&(waitingUp[_elevator->currentPosition]),p) != NULL){
				while (_elevator->capacity - _elevator->numPassengers > 0 && peekNextPassenger(&(waitingUp[_elevator->currentPosition]),p) != NULL){
					struct passenger *pass = getNextPassenger(&(waitingUp[_elevator->currentPosition]), p);
					if(pass->sourceLevel == _elevator->currentPosition){
						addPassenger(&_elevator->passengers, pass->time, pass->sourceLevel, pass->destinationLevel, SORT_DESTINATION_ASCENDING);
						_elevator->numPassengers++;
						
						printf("Passenger loaded @ floor %d @ time %u\n", _elevator->currentPosition, curEvent->time);				
					}
				}
				break;
			}else if(peekNextPassenger(&waitingDown[_elevator->currentPosition - 1],p) != NULL){
				while (_elevator->capacity - _elevator->numPassengers > 0 && waitingDown[_elevator->currentPosition - 1].firstElement != NULL){
					struct passenger *pass = getNextPassenger(&waitingDown[_elevator->currentPosition - 1], p);
					if(pass->sourceLevel == _elevator->currentPosition){
						addPassenger(&_elevator->passengers, pass->time, pass->sourceLevel, pass->destinationLevel, SORT_DESTINATION_ASCENDING);
						_elevator->numPassengers++;
						
						printf("Passenger loaded @ floor %d @ time %u\n", _elevator->currentPosition, curEvent->time);				
					}
				}
			}
			break;
		}
		/**
		 * Get a new destination 
		 */
		if (peekNextPassenger(&_elevator->passengers, p) == NULL || _elevator->numPassengers == 0){
			_elevator->stateFuncPtr = elevatorIdle;
			//printf("Elevator empty. Returning to idle\n");
		}else{
			_elevator->destination = peekNextPassenger(&_elevator->passengers, p)->destinationLevel;
			addEvent(&_eventList, EVENT_ARRIVE, time + abs(_elevator->destination - _elevator->currentPosition)*5, _elevator->currentPosition, _elevator->destination);
			_elevator->stateFuncPtr = elevatorMoving;
			//printf("Elevator loaded. moving to floor @ %d\n",_elevator->destination);
		}	
	//All buttonpresses will be delayed by 20 time units
	}else if(curEvent->type == EVENT_BUTTON_PUSH || curEvent->type == EVENT_BUTTON_DELAYED){
		addEvent(&_eventList, EVENT_BUTTON_DELAYED, time + 40, curEvent->source, curEvent->destination);
	}
}

int elevatorDirection(struct elevator *elevator){
	struct passenger *nextPassenger;
	if (peekNextPassenger(&elevator->passengers,nextPassenger) != NULL){
		if(elevator->currentPosition - nextPassenger->destinationLevel > 0){
			return -1;
		}else{
			return 1;
		}	
		return 0;
	}
	return 0;
}
