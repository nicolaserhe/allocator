#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <malloc.h>

#define MEMORY_MIN_TYPES_SIZE					8
#define MEMORY_ARRAYS_SIZE						16
#define MEMORY_POOL_MIN_SIZE					20

typedef struct
{
	char *next;
}memory_link;

void *allocator(size_t bytes_size);
void deallocator(void *memory, size_t memory_size);

static void		add_memory_to_memory_arrays(size_t memory_arrays_index);
static void		add_memory_to_memory_pool(size_t bytes_size);
static void		add_residue_memory_pool_to_memory_arrays();
static size_t	get_memory_pool_size();
static size_t	get_memory_arrays_index(size_t bytes_size);

#endif

