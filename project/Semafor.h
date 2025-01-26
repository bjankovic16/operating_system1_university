/*
 * Semaphore.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef SEMAFOR_H_
#define SEMAFOR_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const; // Returns the current value of the semaphore
public:
	KernelSem* myImpl;
};




#endif /* SEMAFOR_H_ */
