/*
 * IVTE.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

typedef int IVTNo;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};

#endif /* EVENT_H_ */
