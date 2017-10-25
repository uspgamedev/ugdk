#include <stdio.h>
#include <stdlib.h>

#include "wuff.h"


wuff_sint32 read_callback(void * userdata, wuff_uint8 * buffer, size_t * size)
{
	FILE * input = userdata;
	*size = fread(buffer, 1, *size, input);
	return WUFF_SUCCESS;
}

wuff_sint32 seek_callback(void * userdata, wuff_uint64 offset)
{
	FILE * input = userdata;
	if (fseek(input, (long)offset, SEEK_SET) != 0)
		return WUFF_ERROR;
	return WUFF_SUCCESS;
}

wuff_sint32 tell_callback(void * userdata, wuff_uint64 * offset)
{
	FILE * input = userdata;
	long int cur_offset = ftell(input);
	if (cur_offset < 0)
		return WUFF_ERROR;
	*offset = cur_offset;
	return WUFF_SUCCESS;
}


int main(int argc, char * argv[])
{
	int wuff_status = 0;
	struct wuff_handle * handle;
	struct wuff_callback callback = {
		read_callback,
		seek_callback,
		tell_callback
	};

	FILE * input = NULL;
	const size_t buffer_size = 4096;
	wuff_uint8 * buffer;

	/* Opens a file from the first argument. */
	if (argc > 1)
	{
		input = fopen(argv[1], "rb");
	}
	if (input == NULL)
	{
		printf("Could not open file.\n");
		return EXIT_FAILURE;
	}

	buffer = malloc(buffer_size);

	wuff_status = wuff_open(&handle, &callback, input);
	if (wuff_status < 0)
	{
		printf("Wuff could not open the file\n");
		free(buffer);
		fclose(input);
		return EXIT_FAILURE;
	}

	while (wuff_status >= 0)
	{
		size_t count = 4096;
		wuff_status = wuff_read(handle, buffer, &count);

		/* Process 'count' bytes however your application needs them. */

		/* 'count' will be 0 on end-of-file. */
		if (count == 0)
			break;
	}

	wuff_close(handle);

	free(buffer);
	fclose(input);

	return EXIT_SUCCESS;
}
