/*
 * Event.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */
#include "IVTE.h"
#include "Kernele.h"
#include <dos.h>
#include <stdio.h>

IVTEntry* IVTEntry::tabela[256];
IVTEntry::IVTEntry(IVTNo ivt,pInterrupt newR){
	newi=newR;
	ivtNo=ivt;
	setNewRoutine();
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
	}
	setvect(ivtNo,oldi);
	tabela[ivtNo] = 0;
	asm{
		sti
	}
#endif
}

void IVTEntry::setNewRoutine(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
	}
	oldi = getvect(ivtNo);
	setvect(ivtNo,newi);
	tabela[ivtNo] = this;
	asm{
		sti
	}
#endif
}
IVTEntry* IVTEntry::getIVT(IVTNo i){
	return tabela[i];
}

void IVTEntry::signal(){
	if(event==NULL) return;
	event->signal();
}

void IVTEntry::callStaru(){
	if(oldi==NULL) return;
	oldi();
}
