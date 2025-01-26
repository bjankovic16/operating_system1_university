/*
 * Event.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef IVTE_H_
#define IVTE_H_

typedef void interrupt (*pInterrupt)(...);
typedef int IVTNo;


#define PREPAREENTRY(numEntry, callOld)\
		void interrupt inter##numEntry(...); \
		IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
		void interrupt inter##numEntry(...) {\
			newEntry##numEntry.signal();\
			if (callOld == 1)\
			newEntry##numEntry.callStaru();\
}

class IVTEntry{
public:
	IVTEntry(IVTNo ivt,pInterrupt newR);
	~IVTEntry();

	static IVTEntry* getIVT(IVTNo i);

	void signal();

	void callStaru();

private:
	void setNewRoutine();
	friend class KernelEv;
	static IVTEntry* tabela[256];
	pInterrupt newi;
	pInterrupt oldi;
	IVTNo ivtNo;
	KernelEv* event;
};



#endif /* IVTE_H_ */
