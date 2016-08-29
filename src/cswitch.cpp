/*
 * cswitch.cpp
 *
 *  Created on: Aug 8, 2016
 *      Author: OS1
 */

#include "cswitch.h"
#include "pcb.h"

int cs_req = 0;
int cs_reqH = 0;
int cs_reqTimer = 0;
Time counter = 0;
REG tsp, tss, tbp;
REG oldTimerOFF, oldTimerSEG;
int lockFlag = 1;
int deleteFlag = 0;
class SemList;
REG mainSP,mainSS,mainBP;

void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
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
#endif
	PCB::IDLE = 0;
}

void interrupt timer(){
	cs_reqH = cs_req;

	if ( !cs_req ){
		if ( lockFlag ){
		semList *curr = KernelSem::head;
		while ( curr != 0 ){
			int count = KernelSem::blCounter;
			curr->sem->blockList->decAndRemove();
			curr->sem->value += ( count - KernelSem::blCounter );
			curr = curr->next;
		}
		}
	}

	if ( ( cs_req == 0 ) && ( counter != 0 ) ) counter--;
	if ( ( cs_req == 1 ) || ( ( counter == 0 ) && ( PCB::running->timeSlice != 0 ))){
			if ( lockFlag == 1 ){
				cs_req = 0;
				cs_reqTimer = 0;
				#ifndef BCC_BLOCK_IGNORE
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
				#endif
				if ( PCB::running == 0 ){
					if ( ( KernelSem::blCounter == 0 ) && ( !deleteFlag ) ){
						mainSP = tsp;
						mainSS = tss;
						mainBP = tbp;
					}
				}
				else {
					PCB::running->setSpBp(tsp,tss,tbp);
				}
				deleteFlag = 0;
				if ( ( PCB::running != 0 ) && ( PCB::running->state == ready ) ){
					Scheduler::put(PCB::running);
				}
				PCB *newPCB = Scheduler::get();

				if ( newPCB == 0 ){
					if ( ( KernelSem::blCounter > 0 ) ) {
						PCB::running = PCB::IDLE;
						tsp = PCB::running->sp;
						tss = PCB::running->ss;
						tbp = PCB::running->bp;
						counter = PCB::running->timeSlice;
					}
					else{
						tsp = mainSP;
						tss = mainSS;
						tbp = mainBP;
						PCB::running = 0;
						lockFlag = 0;
					}
				}
				else {
					PCB::running = newPCB;
					tsp = PCB::running->sp;
					tss = PCB::running->ss;
					tbp = PCB::running->bp;
					counter = PCB::running->timeSlice;
				}

				#ifndef BCC_BLOCK_IGNORE
				asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
				}
				#endif
			}
			else cs_req = 1;
	}

	if ( !cs_reqH ){
		#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
		#endif
		tick();
	}
}
