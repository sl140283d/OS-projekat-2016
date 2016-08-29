/*
 * ivtEntry.h
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#ifndef _ivtEntry_h_
#define _ivtEntry_h_

#include "ivt.h"
#include "myEvent.h"
#include "event.h"

typedef void interrupt (*pInterrupt)(...);

class IVT;
class KernelEv;

class IVTEntry{
private:
	pInterrupt oldRoutine;
	int callOld;
	int ivtno;
	KernelEv *event;
public:
	static IVT ivt;
	void interr();
	IVTEntry(int ivtno, int old, pInterrupt newRoutine);
	~IVTEntry();
protected:
	friend class KernelEv;
};

#define PREPAREENTRY(ivtno,callOld)\
	void interrupt f##ivtno(...){\
		IVTEntry::ivt.get( ivtno )->interr();\
	}\
	IVTEntry o##ivtno( ivtno , callOld , f##ivtno );

#endif
