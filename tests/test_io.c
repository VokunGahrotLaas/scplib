#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/io.h"

#define BUFFER_SIZE 1023

int main(void) {
	struct scpIOS* ios = scpIOS.new(scpIOStreamType_FILE, stdin, scpIOStreamType_FILE, stdout);
	struct scpIO* io = scpIO.new(scpIOStreamType_scpIOS, ios, scpIOWay_BOTH);
	char buffer[BUFFER_SIZE + 1];

	scpIO.puts(io, "Write something: ");
	scpIO.flush(io);
	scpIO.scanf(io, "%"SCP_TO_STRING(BUFFER_SIZE)"s", buffer);
	scpIO.printf(io, "You have written: '%s' (%zu)\n", buffer, strlen(buffer));

	scpIO.delete(io);
	scpIOS.delete(ios);
	return EXIT_SUCCESS;
}
