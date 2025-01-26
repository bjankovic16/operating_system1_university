/*
 * PCB.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */
#include "PCB.h"
#include <iostream.h>
#include <dos.h>
#include <stdlib.h>
#include "Idle.h"
#include "Timer.h"

// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

static int I = 1;
listaPCB* PCB::head=NULL;
listaPCB* PCB::tail=NULL;
int PCB::sizeListaPCB=2;
int PCB::sizeZavrsenih=0;
PCB* PCB::runningPCB=NULL;
PCB* PCB::idle=NULL;
PCB* PCB::mainPCB=NULL;
int PCB::flag=1;

PCB::~PCB(){
	destroy();
	if(stack!=NULL){
		delete []stack;
	}
}
void PCB::wrapper(){
	runningPCB->nit->run();
		asm{
		cli
	}
		//cout<<"zavrsena nit ta i ta..."<<runningPCB->id<<endl;
		PCB::sizeZavrsenih++;
		runningPCB->stanje = ZAVRSIO;

		asm{
			sti
		}
		dispatch();
}
void PCB::pokreniIdle(){
	Thread* t=new Idle();
	PCB::idle=t->myPCB;
	PCB::idle->stanje = POKRENUTO;
}
void PCB::pokreniNiti(){
	PCB::mainPCB=new PCB();
	PCB::pokreniIdle();
}
void PCB::destroy(){
	listaPCB* prev = NULL;
	listaPCB* tek = NULL;
	for (tek = head; tek->info != this;prev=tek, tek = tek->next);
	if (tek == head) {
		prev=tek->next;
		if(!prev){
		delete head;
		head = 0;
		tail = 0;}
		else{
			delete head;
			head=prev;
		}
	}
	else {
		prev->next = tek->next;
		delete tek;
	}
}
void PCB::initStack(){
	unsigned* st1 = new unsigned[stackS];
	stack=st1;
		st1[stackS-1] =0x200;//setovan I fleg u
		st1[stackS-2] = FP_SEG(PCB::wrapper);
		st1[stackS-3] = FP_OFF(PCB::wrapper);
		sp = FP_OFF(st1+stackS-12);
		ss = FP_SEG(st1+stackS-12);//dodati dealokaciju za ovaj stek
}
PCB::PCB(){
	id=I++;
	nit=NULL;
	timeS=20;
	stanje=POKRENUTO;
	if (head == NULL) {
			head = new listaPCB();
			head->info = this;
			head->next = NULL;
			tail = head;
		}
		else {
			listaPCB* tek = new listaPCB();
			tek->info = this;
			tek->next = NULL;
			tail->next = tek;
			tail = tek;
		}
	runningPCB=this;
}
PCB::PCB(StackSize stackSize, Time timeSlice,Thread* thread){
	id = I++;
	stanje = POCETNO;
	stackS = stackSize;
	timeS = timeSlice*55;
	nit = thread;
	initStack();
	if (head == NULL) {
		head = new listaPCB();
		head->info = this;
		head->next = NULL;
		tail = head;
	}
	else {
		listaPCB* tek = new listaPCB();
		tek->info = this;
		tek->next = NULL;
		tail->next = tek;
		tail = tek;
	}
}



