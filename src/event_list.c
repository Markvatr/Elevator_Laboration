#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include"event_list.h"

void initEventList(struct eventList* list) {
	list->firstElement = NULL;
}

unsigned int addEvent(struct eventList *list, int type, unsigned int time, int source, int destination) {
	struct event temp;
	struct eventListElement *tempEle;

	tempEle = malloc(sizeof(struct eventListElement));

	temp.time = time;
	temp.destination = destination;
	temp.source = source;
	temp.type = type;

	tempEle->data = temp;
	tempEle->nextElement = NULL;

	

	//The list is empty?
	if(list->firstElement == NULL){
		list->firstElement = tempEle;
		return time;
	}
	struct eventListElement *comp = list->firstElement;
	if(tempEle->data.time < comp->data.time) {
		tempEle->nextElement = 	list->firstElement;
		list->firstElement = tempEle;
		return time;
	}else{
		while (comp->nextElement != NULL && comp->nextElement->data.time < tempEle->data.time){
			comp = comp->nextElement;
		}
		tempEle->nextElement = comp->nextElement;
		comp->nextElement = tempEle;
	}
	return time;
}

struct event *getNextEvent(struct eventList* list, struct event *nextEvent){
	struct event *temp = malloc(sizeof(struct event));
	if(list->firstElement == NULL){
		return NULL;
	}else if(list->firstElement->nextElement == NULL){
		*temp = list->firstElement->data;
		free(list->firstElement);
		list->firstElement = NULL;
	}else{
		*temp = list->firstElement->data;
		list->firstElement = list->firstElement->nextElement;
	}
	*nextEvent = *temp;
	return temp;
}
