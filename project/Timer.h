/*
 * Timer.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

void interrupt timer();

class Timer{
private:

public:
	static volatile int zahtevana_promena_konteksta;
	 friend class PCB;
	 friend class Thread;
	 static void postaviPrekid();
	 static void povratiPrekid();
	 static int x;
	 static int count;
	 static int provera;
};



#endif /* TIMER_H_ */
