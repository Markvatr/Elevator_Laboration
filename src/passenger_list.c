#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include"passenger_list.h"

/*
 * It is suggested to use a compare function to simplify the implementation of your
 * addPassenger function. 
 */
int comparePassengers(struct passenger *passenger1, struct passenger *passenger2, int sortOrder) {
  if (sortOrder == SORT_TIME) {
	if (passenger1->time == passenger2->time) {
	  return 0;
	}
	else if(passenger1->time < passenger2->time) {
	  return -1;
	}
	else {
	  return 1;
	}
  }


  else if(sortOrder == SORT_DESTINATION_ASCENDING) {
	if (passenger1->destinationLevel == passenger2->destinationLevel) {
	  return 0;
	}
	else if(passenger1->destinationLevel < passenger2->destinationLevel) {
	  return -1;
	}
	else {
	  return 1;
	}
  }

  else if(sortOrder == SORT_DESTINATION_DESCENDING) {
	if (passenger1->destinationLevel == passenger2->destinationLevel) {
	  return 0;
	}
	else if(passenger1->destinationLevel < passenger2->destinationLevel) {
	  return 1;
	}
	else {
	  return -1;
	}
  }

  return 0;
} /* End comparePassengers (...) */


void initPassengerList(struct passengerList* list) {
  list->firstElement = NULL;
}



unsigned int addPassenger(struct passengerList* list, unsigned int time, int sourceLevel, int destinationLevel, int sortOrder) {
	struct passenger temp;
	struct passengerListElement *tempEle;

	tempEle = malloc(sizeof(struct passengerListElement));

	temp.time = time;
	temp.sourceLevel = sourceLevel;
	temp.destinationLevel = destinationLevel;

	tempEle->data = temp;
	tempEle->nextElement = NULL;


	struct passengerListElement *comp = list->firstElement;

	if (comp == NULL){
		list->firstElement = tempEle;
		return time;
	}
	if (comparePassengers(&tempEle->data, &comp->data, sortOrder) == 0 || 
		comparePassengers(&tempEle->data, &comp->data, sortOrder) == -1){
		tempEle->nextElement = list->firstElement->nextElement;
		list->firstElement = tempEle;
		return time;
	}else{	
		while(comp->nextElement != NULL && comparePassengers(&tempEle->data, &comp->nextElement->data, sortOrder) == 1){
			comp = comp->nextElement;
		}
		tempEle->nextElement = comp->nextElement;
		comp->nextElement = tempEle;
	}
	return 1;
}


struct passenger *getNextPassenger(struct passengerList *list, struct passenger *nextPassager) {
	struct passenger *temp = malloc(sizeof(struct passenger));
	if(list->firstElement == NULL){
		return NULL;
	}else if(list->firstElement->nextElement == NULL){
		*temp = list->firstElement->data;
		list->firstElement = NULL;
		nextPassager = temp;
		return temp;
	}else{
		*temp = list->firstElement->data;
		list->firstElement = list->firstElement->nextElement;
		nextPassager = temp;
		return temp;
	}
	return NULL;
}


struct passenger *peekNextPassenger(struct passengerList* list, struct passenger *nextPassager) {
	struct passenger *temp = malloc(sizeof(struct passenger));
	if(list->firstElement == NULL){
		return NULL;
	}else{
		*temp = list->firstElement->data;
		nextPassager = temp;
		return temp;
	}
	return NULL;
}
