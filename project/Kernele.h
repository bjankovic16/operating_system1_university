/*
 * Kernele.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef KERNELE_H_
#define KERNELE_H_
#include "Event.h"
#include "PCB.h"

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	int value;
	IVTNo ivtNo;//
	PCB* owner;
};



#endif /* KERNELE_H_ */
