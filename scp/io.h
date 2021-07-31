#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "scp/exceptions.h"

typedef enum scpIOStreamType {
	scpIOStreamType_scpIOS,
	scpIOStreamType_FILE,
	scpIOStreamType_scpString
} scpIOStreamType;

struct scpIOS;
struct scpString;

typedef union scpIOStream {
	struct scpIOS* ios;
	FILE* file;
	struct scpString* string;
} scpIOStream;

typedef enum scpIOWay {
	scpIOWay_IN = 1,	// 0b01
	scpIOWay_OUT = 2,	// 0b10
	scpIOWay_BOTH = 3	// 0b11
} scpIOWay;

struct scpIOType;

struct scpIO {
	struct scpIOType* type;
	scpIOStreamType stream_type;
	scpIOStream stream;
	scpIOWay way;
};

struct scpIOSType;

struct scpIOS {
	struct scpIOSType* type;
	struct scpIO* in;
	struct scpIO* out;
};

struct scpIOS* scpIOS_new(scpIOStreamType stream_type_in, void* stream_in, scpIOStreamType stream_type_out, void* stream_out);
void scpIOS_delete(struct scpIOS* ios);

struct scpIOSType {
	struct scpIOS* (*new)(scpIOStreamType stream_type_in, void* stream_in, scpIOStreamType stream_type_out, void* stream_out);
	void (*delete)(struct scpIOS* ios);
} scpIOS = {
	.new = scpIOS_new,
	.delete = scpIOS_delete
};

struct scpIOS* scpIOS_new(scpIOStreamType stream_type_in, void* stream_in, scpIOStreamType stream_type_out, void* stream_out) {
	struct scpIOS* ios = (struct scpIOS*)malloc(sizeof(struct scpIOS));
	ios->type = &scpIOS;
	ios->in = scpIO_new(stream_type_in, stream_in, scpIOWay_IN);
	ios->out = scpIO_new(stream_type_out, stream_out, scpIOWay_OUT);
	return ios;
}

void scpIOS_delete(struct scpIOS* ios) {
	scpIO_delete(ios->in);
	scpIO_delete(ios->out);
	free(ios);
}

struct scpIO* scpIO_new(scpIOStreamType stream_type, void* stream, scpIOWay way);
void scpIO_delete(struct scpIO* io);
int scpIO_putc(struct scpIO* io, int c);
int scpIO_getc(struct scpIO* io);
int scpIO_puts(struct scpIO* io, char* string);
char* scpIO_gets(struct scpIO* io, char* buffer, size_t buffer_size);
int scpIO_printf(struct scpIO* io, char* format, ...);
int scpIO_vprintf(struct scpIO* io, char* format, va_list args);
int scpIO_scanf(struct scpIO* io, char* format, ...);
int scpIO_vscanf(struct scpIO* io, char* format, va_list args);
int scpIO_flush(struct scpIO* io);

struct scpIOType {
	struct scpIO* (*new)(scpIOStreamType stream_type, void* stream, scpIOWay way);
	void (*delete)(struct scpIO* io);
	int (*putc)(struct scpIO* io, int c);
	int (*getc)(struct scpIO* io);
	int (*puts)(struct scpIO* io, char* string);
	char* (*gets)(struct scpIO* io, char* buffer, size_t buffer_size);
	int (*printf)(struct scpIO* io, char* format, ...);
	int (*vprintf)(struct scpIO* io, char* format, va_list args);
	int (*scanf)(struct scpIO* io, char* format, ...);
	int (*vscanf)(struct scpIO* io, char* format, va_list args);
	int (*flush)(struct scpIO* io);
} scpIO = {
	.new = scpIO_new,
	.delete = scpIO_delete,
	.putc = scpIO_putc,
	.getc = scpIO_getc,
	.puts = scpIO_puts,
	.gets = scpIO_gets,
	.printf = scpIO_printf,
	.vprintf = scpIO_vprintf,
	.scanf = scpIO_scanf,
	.vscanf = scpIO_vscanf,
	.flush = scpIO_flush
};

struct scpIO* scpIO_new(scpIOStreamType stream_type, void* stream, scpIOWay way) {
	struct scpIO* io = (struct scpIO*)malloc(sizeof(struct scpIO));
	io->type = &scpIO;
	io->stream_type = stream_type;
	switch (stream_type) {
	case scpIOStreamType_scpIOS:
		io->stream.ios = (struct scpIOS*)stream;
		break;
	case scpIOStreamType_FILE:
		io->stream.file = (FILE*)stream;
		break;
	case scpIOStreamType_scpString:
		io->stream.string = (struct scpString*)stream;
		break;
	default:
		scpException_InvalidArgument("scpIO_new: invalid scpIOStreamType\n");
	}
	io->way = way;
	return io;
}

void scpIO_delete(struct scpIO* io) {
	io->type->flush(io);
	free(io);
}

int scpIO_putc(struct scpIO* io, int c) {
	if (!(io->way & scpIOWay_OUT))
		scpException_InvalidArgument("scpIO_putc: invalid scpIOWay\n");
	int r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->out->type->putc(io->stream.ios->out, c);
		break;
	case scpIOStreamType_FILE:
		r = fputc(c, io->stream.file);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_putc: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_putc: invalid scpIOStreamType\n");
	}
	return r;
}

int scpIO_getc(struct scpIO* io) {
	if (!(io->way & scpIOWay_IN))
		scpException_InvalidArgument("scpIO_getc: invalid scpIOWay\n");
	int r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->in->type->getc(io->stream.ios->in);
		break;
	case scpIOStreamType_FILE:
		r = fgetc(io->stream.file);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_getc: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_getc: invalid scpIOStreamType\n");
	}
	return r;
}

int scpIO_puts(struct scpIO* io, char* string) {
	if (!(io->way & scpIOWay_OUT))
		scpException_InvalidArgument("scpIO_puts: invalid scpIOWay\n");
	int r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->out->type->puts(io->stream.ios->out, string);
		break;
	case scpIOStreamType_FILE:
		r = fputs(string, io->stream.file);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_puts: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_puts: invalid scpIOStreamType\n");
	}
	return r;
}

char* scpIO_gets(struct scpIO* io, char* buffer, size_t buffer_size) {
	if (!(io->way & scpIOWay_IN))
		scpException_InvalidArgument("scpIO_gets: invalid scpIOWay\n");
	char* r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->in->type->gets(io->stream.ios->in, buffer, buffer_size);
		break;
	case scpIOStreamType_FILE:
		if (buffer_size & ~(size_t)__INT_MAX__)
			scpException_InvalidArgument("scpIO_gets: invalid buffer_size for scpIOStreamType_FILE\n");
		r = fgets(buffer, (int)buffer_size, io->stream.file);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_gets: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_gets: invalid scpIOStreamType\n");
	}
	return r;
}

int scpIO_printf(struct scpIO* io, char* format, ...) {
	va_list args;
	va_start(args, format);
	int r = scpIO_vprintf(io, format, args);
	va_end(args);
	return r;
}

int scpIO_vprintf(struct scpIO* io, char* format, va_list args) {
	if (!(io->way & scpIOWay_OUT))
		scpException_InvalidArgument("scpIO_vprintf: invalid scpIOWay\n");
	int r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->out->type->vprintf(io->stream.ios->out, format, args);
		break;
	case scpIOStreamType_FILE:
		r = vfprintf(io->stream.file, format, args);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_vprintf: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_vprintf: invalid scpIOStreamType\n");
	}
	return r;
}

int scpIO_scanf(struct scpIO* io, char* format, ...) {
	va_list args;
	va_start(args, format);
	int r = scpIO_vscanf(io, format, args);
	va_end(args);
	return r;
}

int scpIO_vscanf(struct scpIO* io, char* format, va_list args) {
	if (!(io->way & scpIOWay_IN))
		scpException_InvalidArgument("scpIO_vscanf: invalid scpIOWay\n");
	int r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->in->type->vscanf(io->stream.ios->in, format, args);
		break;
	case scpIOStreamType_FILE:
		r = vfscanf(io->stream.file, format, args);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_vscanf: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_vscanf: invalid scpIOStreamType\n");
	}
	return r;
}

int scpIO_flush(struct scpIO* io) {
	int r;
	switch (io->stream_type) {
	case scpIOStreamType_scpIOS:
		r = io->stream.ios->in->type->flush(io->stream.ios->in);
		r = io->stream.ios->out->type->flush(io->stream.ios->out);
		break;
	case scpIOStreamType_FILE:
		r = fflush(io->stream.file);
		break;
	case scpIOStreamType_scpString:
		scpException_InvalidArgument("scpIO_flush: unsupported scpIOStreamType_scpString\n");
	default:
		scpException_InvalidArgument("scpIO_flush: invalid scpIOStreamType\n");
	}
	return r;
}
