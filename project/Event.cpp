/*
 * IVTE.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#include "IVTE.h"
#include "Timer.h"
#include "Thread.h"
#include "Kernele.h"
#include "Event.h"

Event::Event(IVTNo ivtNo){
	asm{
		cli
	}
	myImpl = new KernelEv(ivtNo);
	asm{
		sti
	}
}

Event::~Event(){
	asm{
		cli
	}
	delete myImpl;
	asm{
		sti
	}
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}



