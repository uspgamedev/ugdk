Wuff - A very basic WAVE reader
===============================

Wuff is a simple WAVE reader that supports the most used uncompressed PCM
formats. When reading the samples from the stream, it can convert the
bit depth to one of the other supported formats, though no dithering or other
algorithms to retain the quality are applied.


#### Wuff's features

* Read RIFF WAVEs containing PCM samples with a bit depth of 8, 16, 24, 32
  and also IEEE float samples with a bit depth of 32 and 64.
* Report the length of the audio in samples. (Currently implemented by trusting
  the chunk size information.)
* Report the current and seek to any sample position.


#### Wuff's limits

* No support for RIFX.
* No support for WAVE chunks other than fmt  and data.
* The C types float and double are expected to be in the 32-bit and 64-bit IEEE
  floating-point number format.
* The code currently targets only little-endian platforms.
* Wuff expects the chunks to be padded to the WORD boundary.


#### Not planned for Wuff

* A sample rate resampler is not planned.
* Support for compressed formats. There are better lossy alternatives.
* Support for many of the other WAVE chunks.



Overview
--------

Wuff's C API provides functions to open and close a stream, select the output
format, read the audio data into the application buffer, and seeking. These
functions all take a pointer to a `struct wuff_handle` that is initialized
with `wuff_open`. Their return values indicate success or failure where errors
have a negative value.

The data of the WAVE is passed to Wuff with callback functions. All callback
functions have a void pointer as their first argument. This userdata is set
with the value passed to `wuff_open` and can be used to make the callbacks work
on application specific data. There are currently no other options to deliver
the WAVE data. The return value of the callbacks indicates success if it's zero
or positive. A negative return value aborts whatever Wuff is doing and the value
from the callback will be returned by the function the application called.

The stream sample data gets written into an application-owned buffer with
`wuff_read`. The buffer can be of any size, but it is recommended to make it
not too small and it should be a multiple of the block size (channels * bytes
per sample) to get the best performance.


Example
-------

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
