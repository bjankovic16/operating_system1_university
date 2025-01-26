/*
 * Timer.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#include "Timer.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include <dos.h>
#include <stdlib.h>
#include "Kernels.h"
#include "Idle.h"


// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti
int c=0;
unsigned tsp;
unsigned tss;
volatile int brojac = 20;
volatile int Timer::zahtevana_promena_konteksta = 0;
int Timer::x=0;
int Timer::provera=0;
int Timer::count=0;

void interrupt timer(){
	if (!Timer::zahtevana_promena_konteksta) {
		brojac--;
		//cout<<"ovde sam"<<endl;
		//exit(-1);
		for (KernelSem::headB = KernelSem::headG; KernelSem::headB != NULL; KernelSem::headB = KernelSem::headB->next) {//provera za semafor
			//cout<<KernelSem::headB->info->TimeSem<<"za novu"<<"init za novu.."<<KernelSem::headB->info->id<<endl;
			if (KernelSem::headB->info->TimeFlag==1) {
				KernelSem::headB->info->TimeSem -= 1;
				if (KernelSem::headB->info->TimeSem == 0) {
					//cout<<"init niti: "<<KernelSem::headB->info->TimeSem<<"id niti: "<<KernelSem::headB->info->id<<endl;
					//cout<<"Odblokirana nit.. "<<KernelSem::headB->info->id<<endl;
					KernelSem::headB->semafor->init = 0;
					KernelSem::headB->info->stanje = POKRENUTO;
					KernelSem::sem1--;
					PCB::brojRadnika++;
					Scheduler::put(KernelSem::headB->info);
					KernelSem::izbaciZahtev(KernelSem::headB->info, NULL);
				}
			}
		}
	}
	if (brojac == 0 || Timer::zahtevana_promena_konteksta) {
				asm {
					mov tsp, sp
					mov tss, ss
				}

				PCB::runningPCB->sp = tsp;
				PCB::runningPCB->ss = tss;

				if((PCB::runningPCB->stanje!=ZAVRSIO) && (PCB::runningPCB->stanje!=BLOKIRANO) &&(PCB::runningPCB->id!=PCB::idle->id)){
						Scheduler::put(PCB::runningPCB);
				}

				PCB::runningPCB= Scheduler::get();// Scheduler
				if(PCB::runningPCB==NULL){
					PCB::runningPCB=PCB::idle;
					//cout<<KernelSem::headG->info->id;
					//exit(-1);
				}

				tsp = PCB::runningPCB->sp;
				tss = PCB::runningPCB->ss;

				brojac = PCB::runningPCB->timeS;

				asm {
					mov sp, tsp   // restore sp
					mov ss, tss
				}}
	if(!Timer::zahtevana_promena_konteksta){
		asm int 60h;
	}
		Timer::zahtevana_promena_konteksta = 0;
}

unsigned oldTimerOFF=0;
unsigned oldTimerSEG=0;
void Timer::postaviPrekid(){
	if(PCB::mainPCB==NULL && PCB::idle==NULL){
		PCB::pokreniNiti();
	}
	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0x0022 //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0x0020
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0x0022, seg timer	 //postavlja
			mov word ptr es:0x0020, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0x0182, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0x0180, ax

			pop ax
			pop es
			sti
		}
}

void Timer::povratiPrekid(){
	PCB::idle->stanje=ZAVRSIO;
	delete PCB::idle->nit;
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}


