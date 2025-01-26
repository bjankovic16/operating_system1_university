/*
 * Thread.h
 *
 *  Created on: Jun 19, 2021
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	static int poslez;
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);
protected:
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice =
					defaultTimeSlice);
	friend class PCB;
	virtual void run() {}
public:
	PCB* myPCB;
};
void dispatch ();

#endif /* THREAD_H_ */
