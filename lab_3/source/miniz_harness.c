#include <stdbool.h>
#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"

static size_t
discard_output
(
    void*       opaque,
    mz_uint64   file_offset,
    const void* data,
    size_t      size
)
{
    return size;
}

extern void
run
(
    const uint8_t* input_data,
    size_t         input_size
)
{

    mz_zip_archive zip = {0};

    if (mz_zip_reader_init_mem(&zip, input_data, input_size, 0))
    {

        mz_uint file_count = mz_zip_reader_get_num_files(&zip);

        if (file_count > 1024)
        {
            file_count = 1024;
        }

        for (mz_uint i = 0; i < file_count; i += 1)
        {

            mz_zip_archive_file_stat stat = {0};

            if (!mz_zip_reader_file_stat(&zip, i, &stat))
            {
                continue;
            }

            mz_zip_reader_locate_file(&zip, stat.m_filename, NULL, 0);
            mz_zip_reader_locate_file(&zip, stat.m_filename, NULL, MZ_ZIP_FLAG_CASE_SENSITIVE);
            mz_zip_reader_locate_file(&zip, stat.m_filename, NULL, MZ_ZIP_FLAG_IGNORE_PATH);

            size_t heap_size = 0;
            void*  heap_data = mz_zip_reader_extract_to_heap(&zip, i, &heap_size, 0);
            free(heap_data);

            mz_zip_reader_extract_to_callback(&zip, i, discard_output, NULL, 0);

            if (stat.m_uncomp_size <= 1024 * 1024)
            {

                size_t capacity = stat.m_uncomp_size;
                void*  buffer   = malloc(capacity ? capacity : 1);

                if (buffer)
                {
                    mz_zip_reader_extract_to_mem(&zip, i, buffer, capacity, 0);
                }

                free(buffer);

            }

        }

        mz_zip_reader_locate_file(&zip, "", NULL, 0);
        mz_zip_reader_locate_file(&zip, "missing-fuzz-entry", NULL, MZ_ZIP_FLAG_IGNORE_PATH);
        mz_zip_reader_end(&zip);

    }

    if (input_size)
    {

        int       window_bits = (input_data[0] & 1) ? 15 : -15;
        mz_stream stream      = {0};

        if (mz_inflateInit2(&stream, window_bits) == MZ_OK)
        {

            uint8_t output[4096];

            size_t input_offset = 0;
            bool   done         = false;

            while (!done)
            {

                if (stream.avail_in == 0 && input_offset < input_size)
                {

                    size_t remaining = input_size - input_offset;
                    size_t amount    = remaining < 4096 ? remaining : 4096;

                    stream.next_in   = (mz_uint8*)(input_data + input_offset);
                    stream.avail_in  = (unsigned int)amount;
                    input_offset    += amount;

                }

                stream.next_out  = output;
                stream.avail_out = sizeof(output);

                unsigned int old_avail_in = stream.avail_in;
                int          status       = mz_inflate(&stream, MZ_NO_FLUSH);

                if (status == MZ_STREAM_END)
                {
                    done = true;
                }
                else if (status != MZ_OK)
                {
                    done = true;
                }
                else if (old_avail_in == stream.avail_in && stream.avail_out == sizeof(output) && input_offset == input_size)
                {
                    done = true;
                }

            }

            mz_inflateEnd(&stream);

        }

    }

}



#if MODE == 0

    extern int
    main(int argc, char** argv)
    {

        long           input_size = {};
        unsigned char* input_data = {};
        {

            char* input_path   = argv[1];
            FILE* input_handle = fopen(input_path, "rb");

            fseek(input_handle, 0, SEEK_END);
            input_size = ftell(input_handle);
            fseek(input_handle, 0, SEEK_SET);

            input_data = malloc(input_size);

            (void) fread(input_data, input_size, 1, input_handle);

            fclose(input_handle);

        }

        run(input_data, input_size);

        free(input_data);

        return 0;

    }

#elif MODE == 1

    #include <unistd.h>

    __AFL_FUZZ_INIT();

    extern int
    main(void)
    {

        __AFL_INIT();

        const uint8_t* input_data = __AFL_FUZZ_TESTCASE_BUF;

        while (__AFL_LOOP(10000))
        {

            size_t input_size = __AFL_FUZZ_TESTCASE_LEN;

            run(input_data, input_size);

        }

        return 0;

    }

#elif MODE == 2 || MODE == 3 || MODE == 4

    extern int
    LLVMFuzzerTestOneInput(const uint8_t* input_data, size_t input_size)
    {
        run(input_data, input_size);
        return 0;
    }

#endif
