#include "daemon.h"

int main(int argc, char** argv)
{
	return demonize([](){
		LOG("[TASK]");
	});

	return 0;
}
