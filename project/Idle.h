/*
 * Idle.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_
#include "Kernels.h"
#include "PCB.h"
#include "Timer.h"
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

class Idle : public Thread
{
public:
	Idle(): Thread(defaultStackSize,0) {};
	~Idle()
	{
		waitToComplete();
	}
protected:

	void run(){
		while(1){
				//cout<<"XXX"<<endl;
				//cout<<"PCB::sizeZavrsenih "<<PCB::sizeZavrsenih<<"a   PCB::sizeListaPCB "<<PCB::sizeListaPCB<<"size blok"<<KernelSem::sizeBLokirani;
				for(int j=0;j<100;j++);
				//exit(-1);
				if(PCB::sizeZavrsenih==PCB::sizeListaPCB-2 && !KernelSem::sizeBLokirani){
					PCB::idle->stanje=ZAVRSIO;
				}


				dispatch();
			}
	}

};




#endif /* IDLE_H_ */
