#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <c_core/c_core.h>

int main(int argc, char **argv)
{
	CC_Log_Trace("Hello world! %ld", rand());
	CC_Log_Info("Hello world! %ld", rand());
	CC_Log_Warn("Hello world! %ld", rand());
	CC_Log_Error("Hello world! %ld", rand());
}
