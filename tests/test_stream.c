#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/stream.h"

#define BUFFER_SIZE 1023

int main(void) {
	struct scpDStream* dstream = scpDStream.new(scpStreamSubstreamType_FILE, stdin, scpStreamSubstreamType_FILE, stdout);
	struct scpStream* stream = scpStream.new(scpStreamSubstreamType_scpDStream, dstream, scpStreamWay_BOTH);
	char buffer[BUFFER_SIZE + 1];

	scpStream.puts(stream, "Write something: ");
	scpStream.flush(stream);
	scpStream.scanf(stream, "%"SCP_TO_STRING(BUFFER_SIZE)"s", buffer);
	scpStream.printf(stream, "You have written: '%s' (%zu)\n", buffer, strlen(buffer));

	scpStream.delete(stream);
	scpDStream.delete(dstream);
	return EXIT_SUCCESS;
}
