/*
	This test creates a 30 second sine wave and converts it	to another format
	multiple times using different buffer sizes. If one of the outputs doesn't
	match the others, it may indicate a problem with the code where truncated
	samples are handled or the conversion itself.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

const int sample_rate = 44100;
const int seconds = 30;
#define num_test_buffers 11

struct file_data
{
	wuff_uint8 * data;
	size_t size;
	size_t offset;
};

wuff_sint32 read_cb(void * userdata, wuff_uint8 * buffer, size_t * size)
{
	size_t new_size;
	struct file_data * input = userdata;
	new_size = input->offset + *size > input->size ? input->size - input->offset : *size;
	memcpy(buffer, input->data + input->offset, new_size);
	input->offset += new_size;
	*size = new_size;
	return WUFF_SUCCESS;
}

wuff_sint32 seek_cb(void * userdata, wuff_uint64 offset)
{
	struct file_data * input = userdata;
	input->offset = offset;
	return WUFF_SUCCESS;
}

wuff_sint32 tell_cb(void * userdata, wuff_uint64 * offset)
{
	struct file_data * input = userdata;
	*offset = input->offset;
	return WUFF_SUCCESS;
}

const char * getFormat(wuff_uint16 format)
{
	switch (format)
	{
	case WUFF_FORMAT_PCM_U8:
		return "u8";
	case WUFF_FORMAT_PCM_S16:
		return "s16";
	case WUFF_FORMAT_PCM_S24:
		return "s24";
	case WUFF_FORMAT_PCM_S32:
		return "s32";
	case WUFF_FORMAT_IEEE_FLOAT_32:
		return "f32";
	case WUFF_FORMAT_IEEE_FLOAT_64:
		return "f64";
	}
	return "";
}

void generate_header(wuff_uint8 * header, wuff_uint32 data_size, wuff_uint8 format, wuff_uint8 bits)
{
	const wuff_uint8 * header_template =
		"RIFF"
		"\xff\xff\xff\x7f"   /* File size. */
		"WAVE"
		"fmt "
		"\x10\x00\x00\x00"   /* Header size. */
		"\x00\x00"           /* Format. */
		"\x01\x00"           /* Channels. */
		"\x44\xac\x00\x00"   /* Sample rate. */
		"\x00\x00\x00\x00"   /* Bytes per second. */
		"\x00\x00"           /* Block align. */
		"\x00\x00"           /* Bits per sample. */
		"data";

	memcpy(header, header_template, 40);
	memcpy(header + 40, &data_size, 4);

	header[20] = format;
	header[34] = bits;
}

void generate_data(wuff_uint8 * header, wuff_uint32 samples, wuff_uint16 format)
{
	int i;
	switch (format)
	{
	case WUFF_FORMAT_PCM_U8:
		for (i = 0; i < samples; ++i)
			header[i] = (wuff_uint8)(sin(i) * 127.5 + 128);
		break;
	case WUFF_FORMAT_PCM_S16:
		{
			for (i = 0; i < samples; ++i)
			{
				wuff_sint16 s = (wuff_sint16)(sin(i) * 32767.5);
				memcpy(header + i * 2, &s, 2);
			}
			break;
		}
	case WUFF_FORMAT_PCM_S24:
		{
			for (i = 0; i < samples; ++i)
			{
				wuff_sint32 s = (wuff_sint32)(sin(i) * 16777215.5);
				memcpy(header + i * 3, (wuff_uint8 *)&s + 1, 3);
			}
			break;
		}
	case WUFF_FORMAT_PCM_S32:
		{
			for (i = 0; i < samples; ++i)
			{
				wuff_sint32 s = (wuff_sint32)(sin(i) * 2147483647.5);
				memcpy(header + i * 4, &s, 4);
			}
			break;
		}
	case WUFF_FORMAT_IEEE_FLOAT_32:
		{
			for (i = 0; i < samples; ++i)
			{
				float s = sin(i);
				memcpy(header + i * 4, &s, 4);
			}
			break;
		}
	case WUFF_FORMAT_IEEE_FLOAT_64:
		{
			for (i = 0; i < samples; ++i)
			{
				double s = sin(i);
				memcpy(header + i * 8, &s, 8);
			}
			break;
		}
	}
}

wuff_uint8 * generate_wave(wuff_uint16 format, size_t * size)
{
	unsigned short bits, wave_format;
	size_t buffer_size;
	wuff_uint8 * buffer;

	switch (format)
	{
	case WUFF_FORMAT_PCM_U8:
		bits = 8;
		wave_format = 1;
		break;
	case WUFF_FORMAT_PCM_S16:
		bits = 16;
		wave_format = 1;
		break;
	case WUFF_FORMAT_PCM_S24:
		bits = 24;
		wave_format = 1;
		break;
	case WUFF_FORMAT_PCM_S32:
		bits = 32;
		wave_format = 1;
		break;
	case WUFF_FORMAT_IEEE_FLOAT_32:
		bits = 32;
		wave_format = 3;
		break;
	case WUFF_FORMAT_IEEE_FLOAT_64:
		bits = 64;
		wave_format = 3;
		break;
	}

	buffer_size = 44 + sample_rate * seconds * (bits / 8);
	buffer = malloc(buffer_size);
	if (buffer == NULL)
		return NULL;

	generate_header(buffer, buffer_size - 44, wave_format, bits);
	generate_data(buffer + 44, sample_rate * seconds, format);

	*size = buffer_size;

	return buffer;
}


size_t test_buffers(struct wuff_handle * handle, wuff_uint8 ** buffers)
{
	size_t i, offset, size;

	/*  Sane buffer size. */
	wuff_seek(handle, 0);
	offset = 0;
	do
	{
		size = 3072;
		wuff_read(handle, buffers[0] + offset, &size);
		offset += size;
	}
	while (size > 0);

	/*  Very small buffer sizes. */
	for (i = 1; i < 10; i++)
	{
		wuff_seek(handle, 0);
		offset = 0;
		do
		{
			size = i;
			wuff_read(handle, buffers[i] + offset, &size);
			offset += size;
		}
		while (size > 0);
	}

	/* Random buffer sizes. */
	wuff_seek(handle, 0);
	offset = 0;
	do
	{
		size = rand() % 100 + 1;
		wuff_read(handle, buffers[10] + offset, &size);
		offset += size;
	}
	while (size > 0);

	return offset;
}

wuff_sint32 test_format(struct file_data * input, wuff_uint8 ** buffers, wuff_uint16 format)
{
	struct wuff_handle * handle;
	struct wuff_callback callback = {read_cb, seek_cb, tell_cb};
	wuff_uint16 out_format, i, j;

	TRY(wuff_open)(&handle, &callback, input);
	CATCH

	for (out_format = 0; out_format < WUFF_FORMAT_MAX; out_format++)
	{
		size_t size;
		TRY(wuff_format)(handle, out_format);
		CATCH

		size = test_buffers(handle, buffers);

		printf("In: %s Out: %s\n", getFormat(format), getFormat(out_format));

		for (i = 0; i < num_test_buffers; i++)
		{
			printf("   % 3d: ", i);
			for (j = 0; j < num_test_buffers; j++)
			{
				int cmp;
				if (j == i)
				{
					printf("    ");
					continue;
				}

				cmp = memcmp(buffers[i], buffers[j], size);

				printf("% 2d", j);
				if (cmp)
					printf("! ");
				else
					printf("  ");
			}
			printf("\n");
		}
	}

	TRY(wuff_close)(handle);
	CATCH

	return 0;
}


int main(int argc, char * argv[])
{
	struct wuff_version version;
	int i;
	wuff_uint64 offset;
	wuff_uint16 input_format;
	struct file_data input;
	wuff_uint8 * buffers[num_test_buffers];

	wuff_version(&version);
	printf("Wuff %d.%d.%d.%d - A very basic WAVE reader.\n", version.major, version.minor, version.build, version.revision);

	for (i = 0; i < num_test_buffers; i++)
	{
		buffers[i] = malloc(sample_rate * seconds * 8);
		if (buffers[i] == NULL)
		{
			printf("Memory allocation failed.\n");
			return -1;
		}
	}

	for (input_format = 0; input_format < WUFF_FORMAT_MAX; input_format++)
	{
		input.data = generate_wave(input_format, &input.size);
		input.offset = 0;

		TRY(test_format)(&input, buffers, input_format);
		CATCH

		free(input.data);
	}

	for (i = 0; i < num_test_buffers; i++)
		free(buffers[i]);

	return 0;
}
