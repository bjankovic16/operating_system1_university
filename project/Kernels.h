/*
 * KernelSem.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_

typedef unsigned int Time;
class PCB;
class Semaphore;
class Idle;
class KernelSem {
public:
	KernelSem (int init=1);
	~KernelSem ();
	int wait (Time maxTimeToWait);
	void signal();
	int val () const;
	struct listaBlokiranih {
			PCB* info;
			KernelSem* semafor;
			listaBlokiranih* next;
		}lista;
public:
	friend class Timer;
	friend class Idle;
	int init;
	int id;
	static int ID;
	void add(PCB* p,KernelSem* sem);
	void deblokiraj(KernelSem* sem);
	static void izbaciZahtev(PCB* p,KernelSem* sem);
	static int sizeBLokirani;
	static int sem1;
	static listaBlokiranih*headG;
	static listaBlokiranih*headB;
	static listaBlokiranih*tailB;
};



#endif /* KERNELS_H_ */
