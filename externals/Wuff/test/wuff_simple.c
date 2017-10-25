#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wuff.h>

#define TRY(func) { \
wuff_uint8 * func_name = #func; \
wuff_sint32 wuff_status; \
wuff_status = func

#define CATCH if (wuff_status < 0) \
	{ \
		printf("%s failed with %d.\n", func_name, wuff_status); \
		return wuff_status; \
	} \
}

static const char usage[] = "Usage: wuff_simple [-v] -i input-file -o output-file";

wuff_sint32 read_cb(void * userdata, wuff_uint8 * buffer, size_t * size)
{
	FILE * input = userdata;
	*size = fread(buffer, 1, *size, input);
	return WUFF_SUCCESS;
}

wuff_sint32 seek_cb(void * userdata, wuff_uint64 offset)
{
	FILE * input = userdata;
	if (fseek(input, offset, SEEK_SET) != 0)
		return -1000;
	return WUFF_SUCCESS;
}

wuff_sint32 tell_cb(void * userdata, wuff_uint64 * offset)
{
	FILE * input = userdata;
	long int cur_offset = ftell(input);
	if (cur_offset < 0)
		return -1001;
	*offset = cur_offset;
	return WUFF_SUCCESS;
}


int main(int argc, char * argv[])
{
	struct wuff_handle * handle;
	struct wuff_info info;
	struct wuff_version version;
	struct wuff_callback callback = {read_cb, seek_cb, tell_cb};
	int i, verbose = 0;
	char * input_name = 0, * output_name = 0;
	size_t size, buffer_size = 3072;
	wuff_uint64 offset;
	wuff_uint8 * buffer;
	FILE * input, * output;

	wuff_version(&version);
	printf("Wuff %d.%d.%d.%d - A bery basic WAVE reader.\n", version.major, version.minor, version.build, version.revision);

	for (i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-v") == 0)
		{
			verbose = 1;
		}
		else if(strcmp(argv[i], "-i") == 0 && i < (argc - 1))
		{
			++i;
			input_name = argv[i];
		}
		else if(strcmp(argv[i], "-o") == 0 && i < (argc - 1))
		{
			++i;
			output_name = argv[i];
		}
		else
		{
			printf("%s", usage);
			return 0;
		}
	}

	if (input_name == 0)
	{
		printf("No input file. %s\n", usage);
		return 1;
	}
	else if (output_name == 0)
	{
		printf("No output file. %s\n", usage);
		return 1;
	}

	input = fopen(input_name, "rb");
	if (input == 0)
	{
		printf("Could not open input file.");
		return 1;
	}

	output = fopen(output_name, "wb");
	if (output == 0)
	{
		printf("Could not open output file.");
		return 1;
	}


	TRY(wuff_open)(&handle, &callback, input);
	CATCH

	TRY(wuff_stream_info)(handle, &info);
	CATCH

	printf(
		"\n%s\n"
		"---------\n"
		"Stream format:   % 10d\n"
		"Channels:        % 10d\n"
		"Sample rate:     % 10d\n"
		"Bits per sample: % 10d\n"
		"Length:          % 10d\n",
		input_name,
		info.format,
		info.channels,
		info.sample_rate,
		info.bits_per_sample,
		info.length
	);

	TRY(wuff_format)(handle, WUFF_FORMAT_PCM_S32);
	CATCH

	buffer_size = 4096;
	size = buffer_size;
	buffer = malloc(buffer_size);

	while (size > 0)
	{

		TRY(wuff_tell)(handle, &offset);
		CATCH
		if (verbose > 0)
			printf("Reading from sample position %u.\n", (int)offset);

		size = buffer_size;
		TRY(wuff_read)(handle, buffer, &size);
		CATCH

		if (verbose > 0)
			printf("Writing %u bytes to the output file\n", (int)size);
		fwrite(buffer, 1, size, output);
	}

	TRY(wuff_close)(handle);
	CATCH

	return 0;
}
