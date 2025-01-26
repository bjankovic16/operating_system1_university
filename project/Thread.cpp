/*
 * Thread.cpp
 *
 *  Created on: Jun 19, 2021
 *      Author: OS1
 */

//#include <iostream.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
#include "Thread.h"
#include "Timer.h"
#include "SCHEDULER.H"
#include "PCB.h"
#include "Kernels.h"

int Thread::poslez=0;
void dispatch(){
	asm{
		cli
	}
	Timer::zahtevana_promena_konteksta = 1;
	timer();
	asm{
		sti
	}
}

void Thread::start(){
	if (myPCB) {
		asm{
			sti
		}
		PCB::sizeListaPCB++;
		myPCB->stanje=POKRENUTO;
 		Scheduler::put(myPCB);
		asm {
			cli
		}
	}
}
void Thread::waitToComplete(){
	int res=0;
	asm{
		cli
	}
	while(1){
	if(myPCB->stanje==POCETNO) res=1;
	if(myPCB->stanje==ZAVRSIO){
		//cout<<"zavrsio idll "<<myPCB->id<<endl;
		Thread::poslez=1;
		//cout<<"broj radnika ... "<<PCB::brojRadnika<<endl;
		res=1;
	}
	asm{
		sti
	}
	if(!res)
	dispatch();
	else{
		return;
	}
	}
}
Thread::~Thread(){
	//waitToComplete();
	delete myPCB;
}
ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	if(PCB::runningPCB!=NULL)
	return PCB::runningPCB->id;
	else{
	return -1;}
}

Thread* Thread::getThreadById(ID id){
	for (listaPCB* tek = PCB::head; tek != NULL; tek = tek->next) {
		if (tek->info->id == id) {
			return tek->info->nit;
		}
	}
	return NULL;
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = new PCB(stackSize, timeSlice, this);
}
