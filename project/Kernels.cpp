/*
 * Kernels.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */
#include "Kernels.h"
#include "PCB.h"
#include <iostream.h>
#include <SCHEDULE.h>
#include <dos.h>
#include <stdlib.h>
#include "PCB.h"
#include "Timer.h"

int KernelSem::ID=0;

KernelSem::listaBlokiranih* KernelSem::headB=NULL;
KernelSem::listaBlokiranih* KernelSem::tailB=NULL;
KernelSem::listaBlokiranih* KernelSem::headG=NULL;
int PCB::brojRadnika=0;
int KernelSem::sizeBLokirani=0;
int KernelSem::sem1=0;

KernelSem::KernelSem(int val){
	init=val;
	id=ID++;
}

KernelSem::~KernelSem(){
	listaBlokiranih* prev = NULL;
	listaBlokiranih* tek = headG;
	/*while (tek != NULL){
		prev=tek;
		tek=tek->next;
		delete prev;
	}*/
}

void KernelSem::add(PCB* p,KernelSem* sem){
	if (headG == NULL) {
				headG = new listaBlokiranih();
				headG->info = p;
				headG->semafor=sem;
				headG->next = NULL;
				tailB = headG;
				headB=headG;
			}
			else {
				listaBlokiranih* tek = new listaBlokiranih();
				tek->info = p;
				tek->semafor=sem;
				tek->next = NULL;
				tailB->next = tek;
				tailB = tek;
			}
	KernelSem::sizeBLokirani++;
	/*cout<<endl;
	cout<<"Lista blokiranih: "<<endl;
	for (KernelSem::headB = KernelSem::headG; KernelSem::headB != NULL; KernelSem::headB = KernelSem::headB->next)
				cout<<"Pre "<<KernelSem::headB->info->TimeSem<<"a id je: "<<KernelSem::headB->info->id<<endl;
	cout<<"krajjjjjj"<<endl;

	if(KernelSem::sizeBLokirani==3){
		for (KernelSem::headB = KernelSem::headG; KernelSem::headB != NULL; KernelSem::headB = KernelSem::headB->next){
			cout<<"nep ulaz"<<endl;
		}Timer::zahtevana_promena_konteksta = 0;
		timer();*/

	//}
}

void KernelSem::deblokiraj(KernelSem*sem){
	KernelSem::listaBlokiranih*tek=KernelSem::headG;
	while(tek->semafor!=sem){
		tek=tek->next;
	}
	tek->info->stanje=POKRENUTO;
	//cout<<"deblokirao sam: "<<tek->info->id<<"sa init "<<tek->semafor->init<<endl;
	Scheduler::put(tek->info);
	KernelSem::izbaciZahtev(tek->info,tek->semafor);
}

void KernelSem::izbaciZahtev(PCB*p, KernelSem*sem){
	KernelSem::listaBlokiranih*tek=KernelSem::headG;
	KernelSem::listaBlokiranih*prev=NULL;

	if(sem!=NULL){
	while(tek->info->id!=p->id && tek->semafor!=sem){
		prev=tek;
		tek=tek->next;
	}
	if(!prev){
		prev=tek;
		tek=tek->next;
		if(KernelSem::tailB==prev){
			KernelSem::tailB=tek;
		}
		KernelSem::headG=tek;
		delete prev;
	}else{
		prev->next=tek->next;
		if(tek==KernelSem::tailB){
			tailB=prev;
		}
		delete tek;
	}}
	else{
		while(tek->info->id!=p->id && tek->info->TimeFlag){
				prev=tek;
				tek=tek->next;
			}
			if(!prev){
				prev=tek;
				tek=tek->next;
				if(KernelSem::tailB==prev){
							KernelSem::tailB=tek;
						}
				KernelSem::headG=tek;
				delete prev;
			}else{
				prev->next=tek->next;
				if(tek==KernelSem::tailB){
							tailB=prev;
						}
				delete tek;
			}}
	KernelSem::sizeBLokirani--;
	}


int KernelSem::wait(Time maxTimeToWait){
	int x=0;
	if(maxTimeToWait != 0) { PCB::runningPCB->TimeFlag = 1; PCB::runningPCB->TimeSem=maxTimeToWait; KernelSem::sem1++;}
	//cout<<"u wait sam "<<PCB::runningPCB->id<<"a time sem je "<<PCB::runningPCB->TimeSem<<" a time flag je "<<PCB::runningPCB->TimeSem;
		asm{
			cli
		}
		if(--init<0){
			//cout<<"zabod se"<< init<<" Thread"<<PCB::runningPCB->id<<endl;
			PCB::runningPCB->stanje = BLOKIRANO;
			//PCB::brojRadnika--;
			add(PCB::runningPCB,this);
			asm{
				sti
			}
			dispatch();
			x=1;
		}
		asm{
			cli
		}
		int rez = 1;
		if(x==1 && init<=0) rez=0;
		if((PCB::runningPCB->TimeFlag == 1) && (PCB::runningPCB->TimeSem == 0)) { rez = 0; }
		asm{
			sti
		}
		//cout<<"rezultat je "<<rez<<endl;
		return rez;
}

void KernelSem::signal(){
	asm{
		cli
	}
		if(++init<=0){
			//cout<<"usao u signal a init je "<<init<<endl;
			//PCB::brojRadnika++;
			deblokiraj(this);
			asm{
				sti
			}
			return;
		}
	else{
		asm{
			sti
		}
		return;
	}
}

int KernelSem::val() const{
	asm{
		cli
	}
	int r = init;
	asm{
		sti
	}
	return r;
}









