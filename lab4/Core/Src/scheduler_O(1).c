/*
 * scheduler_O(1).c
 *
 *  Created on: Oct 1, 2024
 *      Author: ASUS
 */


#include "scheduler_O(1).h"

Container* container = NULL;

S_Task* addNode(void(*pFunction)(), int delay, int period){
	S_Task* newNode = (S_Task*)malloc(sizeof(S_Task));
	newNode->Delay = delay;
	newNode->Period = period;
	newNode->pTask = pFunction;
	newNode->next = NULL;
	return newNode;
}

void SCH_Init(void){
	container = (Container*)malloc(sizeof(Container));
	container->tail = NULL;
	container->numTask = 0;
	container->emptySlot = 0;
}

void SCH_Add_Task(void(*pFunction)(), int period, int delay){
	 S_Task* newNode;
	int newDelay = period;
	if(period == 0){
		newDelay = delay;
	}
	if(container->tail == NULL){
		// add first node
		newNode = addNode(pFunction, newDelay, period);
		container->tail = newNode;
		container->tail->next =  newNode;
		container->numTask++;
	}
	else{
		S_Task* prevTail = container->tail->next;
		S_Task* temp = container->tail->next;
		int Total = 0;

		for(int i = 0; i < container->numTask; i++){
			Total += temp->Delay;
			if((Total + temp->next->Delay > newDelay) && temp != container->tail){
				// add head
				if(Total > newDelay){
					if(container->emptySlot == 0){
						newNode = addNode(pFunction ,newDelay, period);
						// update next node delay
						temp->Delay -= newDelay;
						// update newNode position
						newNode->next = temp;
						container->tail->next = newNode;
						//update numTask
						container->numTask++;
						return;
					}
					else{
						// update new head
						container->tail->Delay = period;
						container->tail->Period = period;
						container->tail->pTask = pFunction;
						//update next to node delay
						temp->Delay -= period;
						// update tail
						while(prevTail->next != container->tail){
							prevTail = prevTail->next;
						}
						container->tail = prevTail;
						// update emptySlot
						container->emptySlot--;
						return;
					}
				}
				// add middle
				if(container->emptySlot == 0){
					// update newNode delay
					newNode = addNode(pFunction ,newDelay - Total, period);
					// update position
					newNode->next = temp->next;
					temp->next = newNode;
					// update next to delay
					newNode->next->Delay -=  newNode->Delay;

					container->numTask++;
					return;
				}
				else{
					// update newNode delay
					newNode = container->tail;
					newNode->Delay = period - Total;
					newNode->Period = period;
					newNode->pTask = pFunction;
					// update new tail
					while(prevTail->next != container->tail){
						prevTail = prevTail->next;
					}
					prevTail->next = newNode->next;
					container->tail = prevTail;
					// update new node position
					newNode->next = temp->next;
					temp->next = newNode;
					// update next to delay
					newNode->next->Delay -= newNode->Delay;

					container->emptySlot--;
					return;
				}
			}

			// add tail
			if(temp == container->tail){
				if(container->emptySlot == 0){
					// update delay
					newNode = addNode(pFunction ,newDelay - Total, period);
					// update position
					newNode->next = temp->next;
					temp->next = newNode;
					container->tail = newNode;
					// update num Task
					container->numTask++;
					return;
				}
				else{
					// update delay
					container->tail->Delay = period - Total;
					container->tail->Period = period;
					container->tail->pTask = pFunction;
					// update empty slot
					container->emptySlot--;
					return;
				}
			}
			temp = temp->next;
		}
	}
}

void SCH_Delete_Task(void){
	container->tail->next->Delay = 0;
	container->tail->next->Period = 0;
	container->tail->next->pTask = NULL;
	container->tail = container->tail->next;
	container->emptySlot++;
}

void SCH_Update_Task(void){
	if(container->tail->next->Delay > 0){
		container->tail->next->Delay--;
	}
}

void SCH_Dispatch_Task(void){
	if(container->tail->next->Delay == 0){
		(*(container -> tail -> next -> pTask))();
		struct S_Task temp = *(container -> tail -> next);
		SCH_Delete_Task();
		if(temp.Period != 0){
			SCH_Add_Task(temp.pTask, temp.Period, 0);
		}
	}
}

