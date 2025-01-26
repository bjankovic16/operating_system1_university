/*
 * PCB.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"

enum Stanje { BLOKIRANO,POCETNO, POKRENUTO, ZAVRSIO };
class PCB {
public :
	struct listaPCB {
		PCB* info;
		listaPCB* next;
	}list;
	friend class Thread;
	friend class TestThread;
public:
	Time TimeSem;
	int TimeFlag;
	int id;
	unsigned sp;
	unsigned ss;
	unsigned* stack;
	Stanje stanje;
	int kvant;
	static int sizeZavrsenih;
	static int sizeListaPCB;
	StackSize stackS;
	Time timeS;
	static int flag;
	Thread* nit;
	static listaPCB* head;
	static listaPCB* tail;
	static PCB* runningPCB;//mora se nadograditi za running PCB
	static PCB* mainPCB;
	static PCB* idle;
	static int brojRadnika;
	static int pokrenute;
	void destroy();
	void static pokreniNiti();
	void static pokreniIdle();
	void static wrapper();

public:
	friend class Thread;
	friend class Timer;
	PCB(StackSize stackSize, Time timeSlice,Thread* thread);
	PCB();
	~PCB();
	void initStack();

};


#endif /* PCB_H_ */
