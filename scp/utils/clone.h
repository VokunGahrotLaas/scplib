#ifndef SCP_CLONE_H
#define SCP_CLONE_H

void* scpClone_size_t(const void* data) {
	void* new_data = malloc(sizeof(size_t));
	*(size_t*)new_data = *(size_t*)data;
	return new_data;
}

#endif // SCP_CLONE_H