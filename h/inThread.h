/*
 * inicThread.h
 *
 *  Created on: Aug 12, 2016
 *      Author: OS1
 */

#ifndef _inicThread_h_
#define _inicThread_h_
#include "Thread.h"

class inicThread:public Thread{
private:
	int argc;
	char **argv;
public:
	inicThread(int argc, char* argv[]);
	void run();
	~inicThread();
};

#endif /* H_INICTHREAD_H_ */
