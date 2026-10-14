#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"



extern void
run
(
    const uint8_t* input_data,
    size_t         input_size
)
{

    mz_zip_archive zip = {};
    mz_zip_zero_struct(&zip);

    if (mz_zip_reader_init_mem(&zip, input_data, input_size, 0))
    {

        for
        (
            mz_uint file_index = 0;
            file_index < mz_zip_reader_get_num_files(&zip);
            file_index += 1
        )
        {

            mz_zip_archive_file_stat file_stat = {};

            if (mz_zip_reader_file_stat(&zip, file_index, &file_stat))
            {

                size_t out_size = {};
                void*  out      = mz_zip_reader_extract_to_heap(&zip, file_index, &out_size, 0);
                free(out);

                mz_zip_reader_locate_file(&zip, file_stat.m_filename, NULL, 0);

            }

            mz_zip_reader_locate_file(&zip, file_stat.m_filename, NULL, 0);

        }

        mz_zip_reader_end(&zip);

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
