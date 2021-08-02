#ifndef SCP_COPY_H
#define SCP_COPY_H

void scpCopy_size_t(const void* data, void* new_data);

void scpCopy_size_t(const void* data, void* new_data) {
	*(size_t*)new_data = *(const size_t*)data;
}

#endif // SCP_COPY_H