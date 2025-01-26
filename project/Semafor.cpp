/*
 * Semafor.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */
#include <dos.h>
#include "Semafor.h"
#include "Kernels.h"
#include "Timer.h"

Semaphore::Semaphore(int value){
	asm{
		cli
	}
	myImpl = new KernelSem(value);
	asm{
		sti
	}
}

Semaphore::~Semaphore(){
	asm{
		cli
	}
	delete myImpl;
	asm{
		sti
	}
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal(){
	 myImpl->signal();
}

int Semaphore::val() const{
	return myImpl->val();
}



