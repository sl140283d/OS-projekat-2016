/*
 * blockLis.h
 *
 *  Created on: Aug 17, 2016
 *      Author: OS1
 */

#ifndef _blockLis_h_
#define _blockLis_h_

#include "pcb.h"
#include "semaphor.h"

class Elem{
private:
	Time time;
	ID id;
	Elem *next;
public:
	Elem(ID id, Time time = 0);
	~Elem();
protected:
	friend class BlockList;
};

class BlockList{
private:
	Elem *head;
	Elem *tail;
public:
	BlockList();
	~BlockList();
	void add(ID id, Time time = 0);
	void remove();
	void decAndRemove();
	void removeAll();
protected:
	friend class KernelSem;
};

#endif
