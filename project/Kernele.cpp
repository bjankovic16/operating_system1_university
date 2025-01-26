/*
 * Kernele.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */
#include "Kernele.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include <IVTE.h>
#include <iostream.h>
#include <dos.h>
#include <stdlib.h>

KernelEv::KernelEv(IVTNo no){
	value=1;
	ivtNo=no;
	owner=PCB::runningPCB;
	IVTEntry::getIVT(ivtNo)->event = this;
}

KernelEv::~KernelEv(){

}

void KernelEv::wait(){
	asm{
		cli
	}
	if(PCB::runningPCB!=owner || value==0){
		asm{
			sti
		}
		return;
	}else{
		value=0;
		PCB::runningPCB->stanje=BLOKIRANO;

	asm{
		sti
	}
	dispatch();
}}

void KernelEv::signal(){
	asm{
		cli
	}
	if(value){
		asm{
			sti
		}
		return;
	}else{
		value=1;
		owner->stanje=POKRENUTO;
		Scheduler::put(owner);
	}
	asm{
		sti
	}
}


