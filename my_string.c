#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_string.h"

/***********************************************************************************
 *
 * MY STRING OPERATION
 *
 ***********************************************************************************/
my_string_t* my_string_create(size_t capacity)
{
	my_string_t* my_str = (my_string_t*)calloc(1, sizeof(my_string_t));

	if(my_str == NULL)
	{
		fprintf(stderr, "Can't allocate mem for my_string_t\n");
		return NULL;
	}

	my_str->data = (char*)calloc(capacity, sizeof(char));

	if(my_str->data == NULL)
	{
		fprintf(stderr, "Can't allocate mem for my_string_t->data\n");
		free(my_str);
		return NULL;
	}

	my_str->capacity = capacity;

	return my_str;
}

void my_string_clean(my_string_t* my_str)
{
	if(my_str == NULL)
	{
		return;
	}

	my_str->size = 0;
}

my_string_t* my_string_destroy(my_string_t* my_str)
{
	if(my_str != NULL)
	{
		free(my_str->data);
 		free(my_str);
	}

	return NULL;
}

char* my_string_realloc(my_string_t* my_str, size_t new_capacity)
{
	if(my_str == NULL)
	{
		return NULL;
	}

	char* data = (char*)realloc(my_str->data, sizeof(char) * new_capacity);

	if(data == NULL)
	{
		fprintf(stderr, "Can't reallocate mem for my_string_t->data\n");
		return NULL;
	}

	my_str->capacity = new_capacity;
	my_str->data = data;

	return data;
}

void my_string_append(my_string_t* my_str, const char* str, size_t size)
{
	if(my_str == NULL || my_str->data == NULL || str == NULL)
	{
		return;
	}

	// 判断有没有位置存放，没有会扩容
	if((my_str->size + size) >= my_str->capacity)
	{
		if(my_string_realloc(my_str, my_str->size + size + 1) == NULL)
		{
			return;
		}
	}

	memcpy(my_str->data + my_str->size, str, size);
	my_str->size += size;
	my_str->data[my_str->size] = '\0';
}

void my_string_append_one_char(my_string_t* my_str, const char data)
{
	if(my_str == NULL)
	{
		return;
	}

	// 判断有没有位置存放，没有会扩容
	if((my_str->size + 1) >= my_str->capacity)
	{
		if(my_string_realloc(my_str, my_str->size + 1 + 1) == NULL)
		{
			return;
		}
	}

	my_str->data[my_str->size] = data;
	my_str->size += 1;
	my_str->data[my_str->size] = '\0';
}

size_t my_string_get_size(my_string_t* my_str)
{
	return (my_str == NULL ? 0 : my_str->size);
}

char* my_string_get_raw_str(my_string_t* my_str)
{
	return (my_str == NULL ? NULL : my_str->data);
}

char* my_string_detach_raw_str(my_string_t* my_str)
{
	if(my_str == NULL)
	{
		return NULL;
	}

	char* data = my_str->data;

	my_str->data = NULL;
	my_str->size = 0;
	my_str->capacity = 0;

	return data;
}

my_string_t* my_string_duplicate_by_raw_str(const char* str, size_t size)
{
	if(str == NULL)
	{
		return NULL;
	}

	my_string_t* new_str = my_string_create(size + 1);
	my_string_append(new_str, str, size);

	return new_str;
}

my_string_t* my_string_duplicate(my_string_t* my_str)
{
	if(my_str == NULL)
	{
		return NULL;
	}

	return my_string_duplicate_by_raw_str(my_str->data, my_str->size);
}

void my_string_print(my_string_t* my_str)
{
	if(my_str == NULL || my_str->data == NULL)
	{
		return;
	}

#ifdef _WIN32
	printf("size=%Iu capacity=%Iu str=", my_str->size, my_str->capacity);
#else
	printf("size=%zu capacity=%zu str=", my_str->size, my_str->capacity);
#endif

	size_t idx = 0;

	for(idx = 0; idx < my_str->size; ++idx)
	{
		char ch = my_str->data[idx];

		if(isprint(ch))
		{
			printf("%c", ch);
		}
		else
		{
			printf("\\x%02x", (uint8_t)ch);
		}
	}

	printf("\n");
}
/***********************************************************************************/



/***********************************************************************************
 *
 * MY STRING ARRAY OPERATION
 *
 ***********************************************************************************/
my_string_array_t* my_string_array_create(size_t capacity)
{
	my_string_array_t* my_str_array = (my_string_array_t*)calloc(1, sizeof(my_string_array_t));

	if(my_str_array == NULL)
	{
		fprintf(stderr, "Can't allocate mem for my_string_array_t\n");
		return NULL;
	}

	my_str_array->data = (my_string_t**)calloc(capacity, sizeof(my_string_t*));

	if(my_str_array->data == NULL)
	{
		fprintf(stderr, "Can't allocate mem for my_string_array_t->data\n");
		free(my_str_array);
		return NULL;
	}

	my_str_array->capacity = capacity;

	return my_str_array;
}

void my_string_array_clean(my_string_array_t* my_str_array)
{
	if(my_str_array == NULL)
	{
		return;
	}

	size_t idx = 0;

	for(idx = 0; idx < my_str_array->size; ++idx)
	{
		my_str_array->data[idx] = my_string_destroy(my_str_array->data[idx]);
	}

	my_str_array->size = 0;
}

my_string_array_t* my_string_array_destroy(my_string_array_t* my_str_array)
{
	if(my_str_array != NULL)
	{
		size_t idx = 0;

		for(idx = 0; idx < my_str_array->size; ++idx)
		{
			my_str_array->data[idx] = my_string_destroy(my_str_array->data[idx]);
		}

		free(my_str_array->data);
		free(my_str_array);
	}

	return NULL;
}

my_string_t** my_string_array_realloc(my_string_array_t* my_str_array, size_t new_capacity)
{
	if(my_str_array == NULL)
	{
		return NULL;
	}

	my_string_t** data = (my_string_t**)realloc(my_str_array->data, sizeof(my_string_t*) * new_capacity);

	if(data == NULL)
	{
		fprintf(stderr, "Can't reallocate mem for my_string_array_t->data\n");
		return NULL;
	}

	my_str_array->capacity = new_capacity;
	my_str_array->data = data;

	return data;
}

void my_string_array_append(my_string_array_t* my_str_array, const char* str, size_t size)
{
	if(my_str_array == NULL || my_str_array->data == NULL || str == NULL)
	{
		return;
	}

	// 判断有没有位置存放，没有会扩容为原来的2倍
	if(my_str_array->size >= my_str_array->capacity)
	{
		if(my_string_array_realloc(my_str_array, my_str_array->capacity * 2) == NULL)
		{
			return;
		}
	}

	my_string_t* my_str = my_string_create(size + 1);
	my_string_append(my_str, str, size);

	if(my_str != NULL)
	{
		my_str_array->data[my_str_array->size] = my_str;
		++(my_str_array->size);
	}
}

size_t my_string_array_get_size(my_string_array_t* my_str_array)
{
	return (my_str_array == NULL ? 0 : my_str_array->size);
}

size_t my_string_array_get_str_size_by_index(my_string_array_t* my_str_array, size_t idx)
{
	if(my_str_array == NULL || my_str_array->data == NULL || idx >= my_str_array->size)
	{
		return 0;
	}

	return my_string_get_size(my_str_array->data[idx]);
}

char* my_string_array_get_raw_str_by_index(my_string_array_t* my_str_array, size_t idx)
{
	if(my_str_array == NULL || my_str_array->data == NULL || idx >= my_str_array->size)
	{
		return NULL;
	}

	return my_string_get_raw_str(my_str_array->data[idx]);
}

void my_string_array_print(my_string_array_t* my_str_array)
{
	if(my_str_array == NULL || my_str_array->data == NULL)
	{
		return;
	}

#ifdef _WIN32
	printf("size=%Iu capacity=%Iu\n", my_str_array->size, my_str_array->capacity);
#else
	printf("size=%zu capacity=%zu\n", my_str_array->size, my_str_array->capacity);
#endif

	size_t idx = 0;

	for(idx = 0; idx < my_str_array->size; ++idx)
	{
	    printf("\t");
		my_string_print(my_str_array->data[idx]);
	}
}
/***********************************************************************************/


/***********************************************************************************
 *
 * OTHER STRING OPERATION
 *
 ***********************************************************************************/
my_string_array_t* my_raw_str_split(const char* src, const char* sep)
{
	if(src == NULL || sep == NULL || src[0] == '\0' || sep[0] == '\0')
	{
		return NULL;
	}

	char* prev = (char*)src;
	size_t src_len = strlen(src);
	size_t sep_len = strlen(sep);
	my_string_array_t* result = my_string_array_create(16);

	while(*prev != '\0')
	{
		char* next = strstr(prev, sep);

		// 没有搜索到分隔符就直接将指针移到结束字符位置
		if(next == NULL)
		{
			next = (char*)src + src_len;
		}

		my_string_array_append(result, prev, next - prev);

		if(*next == '\0')
		{
			break;
		}

		prev = (next + sep_len);

		if(*prev == '\0')
		{
			// 加入空字符串""
			my_string_array_append(result, prev, 0);
			//break; // 此句可加可不加
		}
	}

	return result;
}
