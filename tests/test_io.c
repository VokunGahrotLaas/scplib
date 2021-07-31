#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/io.h"

int main(void) {
	struct scpIOS* ios = scpIOS.new(scpIOStreamType_FILE, stdin, scpIOStreamType_FILE, stdout);
	struct scpIO* io = scpIO.new(scpIOStreamType_scpIOS, ios, scpIOWay_BOTH);
	char buffer[1024];

	scpIO.puts(io, "Write something: ");
	scpIO.flush(io);
	scpIO.scanf(io, "%1023s", buffer);
	scpIO.printf(io, "You have written: '%s' (%i)\n", buffer, strlen(buffer));

	scpIO.delete(io);
	scpIOS.delete(ios);
	return EXIT_SUCCESS;
}
