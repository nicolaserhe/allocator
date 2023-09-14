#include "allocator.h"

static char *memory_arrays[MEMORY_ARRAYS_SIZE] = {0};
static char *memory_pool_start = NULL;
static char *memory_pool_end = NULL;

void *allocator(size_t bytes_size)
{
	size_t memory_arrays_index = get_memory_arrays_index(bytes_size);
	void *memory = NULL;

	if(0 >= bytes_size)
	{
		memory = NULL;
	}
	else if((MEMORY_ARRAYS_SIZE * MEMORY_MIN_TYPES_SIZE) < bytes_size)
	{
		memory =  malloc(bytes_size);
	}
	else
	{
		if(NULL == memory_arrays[memory_arrays_index])
		{
			add_memory_to_memory_arrays(memory_arrays_index);
		}

		memory = memory_arrays[memory_arrays_index];
		memory_arrays[memory_arrays_index] = ((memory_link *)(memory_arrays[memory_arrays_index]))->next;
	}

	return memory;
}

void deallocator(void *memory, size_t memory_size)
{
	size_t index = get_memory_arrays_index(memory_size);
	((memory_link *)memory)->next = memory_arrays[index];
	memory_arrays[index] = (char*)memory;
}

static void add_memory_to_memory_arrays(size_t memory_arrays_index)
{
	size_t i = 0;
	size_t bytes_size = MEMORY_MIN_TYPES_SIZE * (memory_arrays_index + 1);
	char *memory_loop_point = NULL;

	if(get_memory_pool_size() < bytes_size)
	{
		add_memory_to_memory_pool(bytes_size);
	}

	if(get_memory_pool_size() >= bytes_size * MEMORY_POOL_MIN_SIZE)
	{
		memory_loop_point = memory_pool_start;
		for(i = 0; i < MEMORY_POOL_MIN_SIZE; i++)
		{
			((memory_link *)memory_loop_point)->next = memory_loop_point + bytes_size;
			memory_loop_point = memory_loop_point + bytes_size;
		}
		((memory_link *)(memory_pool_start + bytes_size * (MEMORY_POOL_MIN_SIZE - 1)))->next = NULL;
		memory_arrays[memory_arrays_index] = memory_pool_start;
		memory_pool_start = memory_pool_start + bytes_size * MEMORY_POOL_MIN_SIZE;
	}

}

static void add_memory_to_memory_pool(size_t bytes_size)
{
	add_residue_memory_pool_to_memory_arrays();
	memory_pool_start = (char *)malloc(sizeof(bytes_size * MEMORY_POOL_MIN_SIZE * 2));
	memory_pool_end = memory_pool_start + bytes_size * MEMORY_POOL_MIN_SIZE * 2;
}

static void add_residue_memory_pool_to_memory_arrays()
{
	if(0 == get_memory_pool_size())
	{
		return;
	}

	size_t index = get_memory_arrays_index(get_memory_pool_size());
	((memory_link *)memory_pool_start)->next = memory_arrays[index];
	memory_arrays[index] = memory_pool_start;
}

static size_t get_memory_pool_size()
{
	return memory_pool_end - memory_pool_start;
}

static size_t get_memory_arrays_index(size_t bytes_size)
{
	if(0 == (bytes_size % MEMORY_MIN_TYPES_SIZE))
	{
		return bytes_size / MEMORY_MIN_TYPES_SIZE - 1;
	}
	else
	{
		return bytes_size / MEMORY_MIN_TYPES_SIZE;
	}
}

