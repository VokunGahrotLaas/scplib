#ifndef SCP_STREAM_H
#define SCP_STREAM_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "scp/exceptions.h"

typedef enum scpStreamSubstreamType {
	scpStreamSubstreamType_scpDStream,
	scpStreamSubstreamType_FILE,
	scpStreamSubstreamType_scpString
} scpStreamSubstreamType;

struct scpDStream;
struct scpString;

typedef union scpStreamSubstream {
	struct scpDStream* dstream;
	FILE* file;
	struct scpString* string;
} scpStreamSubstream;

typedef enum scpStreamWay {
	scpStreamWay_IN = 1,	// 0b01
	scpStreamWay_OUT = 2,	// 0b10
	scpStreamWay_BOTH = 3	// 0b11
} scpStreamWay;

struct scpStreamType;

struct scpStream {
	const struct scpStreamType* type;
	scpStreamSubstreamType substream_type;
	scpStreamSubstream substream;
	scpStreamWay way;
};

struct scpDStreamType;

struct scpDStream {
	const struct scpDStreamType* type;
	struct scpStream* in;
	struct scpStream* out;
};

struct scpDStream* scpDStream_new(scpStreamSubstreamType stream_type_in, void* stream_in, scpStreamSubstreamType stream_type_out, void* stream_out);
void scpDStream_delete(struct scpDStream* dstream);

static struct scpDStreamType {
	struct scpDStream* (*new)(scpStreamSubstreamType stream_type_in, void* stream_in, scpStreamSubstreamType stream_type_out, void* stream_out);
	void (*delete)(struct scpDStream* dstream);
} const scpDStream = {
	.new = scpDStream_new,
	.delete = scpDStream_delete
};

scpAttribute_malloc
struct scpStream* scpStream_new(scpStreamSubstreamType substream_type, void* substream, scpStreamWay way);
void scpStream_delete(struct scpStream* stream);
int scpStream_putc(struct scpStream* stream, int c);
int scpStream_getc(struct scpStream* stream);
int scpStream_puts(struct scpStream* stream, char* string);
char* scpStream_gets(struct scpStream* stream, char* buffer, size_t buffer_size);
scpAttribute_format_printf(2, 3)
int scpStream_printf(struct scpStream* stream, char* format, ...);
int scpStream_vprintf(struct scpStream* stream, char* format, va_list args);
scpAttribute_format_scanf(2, 3)
int scpStream_scanf(struct scpStream* stream, char* format, ...);
int scpStream_vscanf(struct scpStream* stream, char* format, va_list args);
int scpStream_flush(struct scpStream* stream);

static struct scpStreamType {
	struct scpStream* (*new)(scpStreamSubstreamType substream_type, void* substream, scpStreamWay way);
	void (*delete)(struct scpStream* stream);
	int (*putc)(struct scpStream* stream, int c);
	int (*getc)(struct scpStream* stream);
	int (*puts)(struct scpStream* stream, char* string);
	char* (*gets)(struct scpStream* stream, char* buffer, size_t buffer_size);
	scpAttribute_format_printf(2, 3)
	int (*printf)(struct scpStream* stream, char* format, ...);
	int (*vprintf)(struct scpStream* stream, char* format, va_list args);
	scpAttribute_format_scanf(2, 3)
	int (*scanf)(struct scpStream* stream, char* format, ...);
	int (*vscanf)(struct scpStream* stream, char* format, va_list args);
	int (*flush)(struct scpStream* stream);
} const scpStream = {
	.new = scpStream_new,
	.delete = scpStream_delete,
	.putc = scpStream_putc,
	.getc = scpStream_getc,
	.puts = scpStream_puts,
	.gets = scpStream_gets,
	.printf = scpStream_printf,
	.vprintf = scpStream_vprintf,
	.scanf = scpStream_scanf,
	.vscanf = scpStream_vscanf,
	.flush = scpStream_flush
};

#ifdef SCP_IMPLEMENTATION

struct scpDStream* scpDStream_new(scpStreamSubstreamType stream_type_in, void* stream_in, scpStreamSubstreamType stream_type_out, void* stream_out) {
	struct scpDStream* dstream = (struct scpDStream*)malloc(sizeof(struct scpDStream));
	dstream->type = &scpDStream;
	dstream->in = scpStream_new(stream_type_in, stream_in, scpStreamWay_IN);
	dstream->out = scpStream_new(stream_type_out, stream_out, scpStreamWay_OUT);
	return dstream;
}

void scpDStream_delete(struct scpDStream* dstream) {
	scpStream_delete(dstream->in);
	scpStream_delete(dstream->out);
	free(dstream);
}

struct scpStream* scpStream_new(scpStreamSubstreamType substream_type, void* substream, scpStreamWay way) {
	struct scpStream* stream = (struct scpStream*)malloc(sizeof(struct scpStream));
	stream->type = &scpStream;
	stream->substream_type = substream_type;
	switch (substream_type) {
	case scpStreamSubstreamType_scpDStream:
		stream->substream.dstream = (struct scpDStream*)substream;
		break;
	case scpStreamSubstreamType_FILE:
		stream->substream.file = (FILE*)substream;
		break;
	case scpStreamSubstreamType_scpString:
		stream->substream.string = (struct scpString*)substream;
		break;
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpIO_new: invalid scpIOStreamType\n");
	}
	stream->way = way;
	return stream;
}

void scpStream_delete(struct scpStream* stream) {
	stream->type->flush(stream);
	free(stream);
}

int scpStream_putc(struct scpStream* stream, int c) {
	if (!(stream->way & scpStreamWay_OUT))
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_putc: invalid scpStreamWay\n");
	int r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->out->type->putc(stream->substream.dstream->out, c);
		break;
	case scpStreamSubstreamType_FILE:
		r = fputc(c, stream->substream.file);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_putc: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_putc: invalid scpStreamType\n");
	}
	return r;
}

int scpStream_getc(struct scpStream* stream) {
	if (!(stream->way & scpStreamWay_IN))
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_getc: invalid scpStreamWay\n");
	int r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->in->type->getc(stream->substream.dstream->in);
		break;
	case scpStreamSubstreamType_FILE:
		r = fgetc(stream->substream.file);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_getc: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_getc: invalid scpStreamType\n");
	}
	return r;
}

int scpStream_puts(struct scpStream* stream, char* string) {
	if (!(stream->way & scpStreamWay_OUT))
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_puts: invalid scpStreamWay\n");
	int r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->out->type->puts(stream->substream.dstream->out, string);
		break;
	case scpStreamSubstreamType_FILE:
		r = fputs(string, stream->substream.file);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_puts: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_puts: invalid scpStreamType\n");
	}
	return r;
}

char* scpStream_gets(struct scpStream* stream, char* buffer, size_t buffer_size) {
	if (!(stream->way & scpStreamWay_IN))
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_gets: invalid scpStreamWay\n");
	char* r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->in->type->gets(stream->substream.dstream->in, buffer, buffer_size);
		break;
	case scpStreamSubstreamType_FILE:
		if (buffer_size & ~(size_t)__INT_MAX__)
			SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_gets: invalid buffer_size for scpStreamSubstreamType_FILE\n");
		r = fgets(buffer, (int)buffer_size, stream->substream.file);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_gets: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_gets: invalid scpStreamType\n");
	}
	return r;
}

int scpStream_printf(struct scpStream* stream, char* format, ...) {
	va_list args;
	va_start(args, format);
	int r = scpStream_vprintf(stream, format, args);
	va_end(args);
	return r;
}

int scpStream_vprintf(struct scpStream* stream, char* format, va_list args) {
	if (!(stream->way & scpStreamWay_OUT))
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_vprintf: invalid scpStreamWay\n");
	int r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->out->type->vprintf(stream->substream.dstream->out, format, args);
		break;
	case scpStreamSubstreamType_FILE:
		r = vfprintf(stream->substream.file, format, args);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_vprintf: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_vprintf: invalid scpStreamType\n");
	}
	return r;
}

int scpStream_scanf(struct scpStream* stream, char* format, ...) {
	va_list args;
	va_start(args, format);
	int r = scpStream_vscanf(stream, format, args);
	va_end(args);
	return r;
}

int scpStream_vscanf(struct scpStream* stream, char* format, va_list args) {
	if (!(stream->way & scpStreamWay_IN))
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_vscanf: invalid scpStreamWay\n");
	int r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->in->type->vscanf(stream->substream.dstream->in, format, args);
		break;
	case scpStreamSubstreamType_FILE:
		r = vfscanf(stream->substream.file, format, args);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_vscanf: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_vscanf: invalid scpStreamType\n");
	}
	return r;
}

int scpStream_flush(struct scpStream* stream) {
	int r;
	switch (stream->substream_type) {
	case scpStreamSubstreamType_scpDStream:
		r = stream->substream.dstream->in->type->flush(stream->substream.dstream->in);
		r = stream->substream.dstream->out->type->flush(stream->substream.dstream->out);
		break;
	case scpStreamSubstreamType_FILE:
		r = fflush(stream->substream.file);
		break;
	case scpStreamSubstreamType_scpString:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_flush: unsupported scpStreamSubstreamType_scpString\n");
	default:
		SCP_EXCEPTION(scpException_InvalidArgument, "scpStream_flush: invalid scpStreamType\n");
	}
	return r;
}

#endif // SCP_IMPLEMENTATATION

#endif // SCP_STREAM_H
