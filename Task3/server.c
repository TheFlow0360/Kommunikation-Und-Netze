#include "robot.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(int argc,char* argv[])
{
	signal(SIGPIPE, SIG_IGN);
	
	if(-1 == initializeRob()) {
		printf("initializeRob()\n");
		exit(1);
	}
	
	// TODO: code goes here ...
	
	// shutdown
	sleep(1);	// required to perform last robot command
	printf("Simulation finished\n");
	shutdownRob();

	return(0);
}

