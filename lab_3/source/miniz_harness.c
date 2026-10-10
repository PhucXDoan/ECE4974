#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"

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

        fread(input_data, input_size, 1, input_handle);

        fclose(input_handle);

    }

    mz_zip_archive zip = {};
    mz_zip_zero_struct(&zip);

    if (!mz_zip_reader_init_mem(&zip, input_data, input_size, 0))
    {
        return 0;
    }

    mz_uint file_count = mz_zip_reader_get_num_files(&zip);

    for (mz_uint file_index = 0; file_index < file_count; file_index += 1)
    {

        mz_zip_archive_file_stat file_stat = {};

        if (mz_zip_reader_file_stat(&zip, file_index, &file_stat))
        {

            size_t out_size = {};
            void*  out      = mz_zip_reader_extract_to_heap(&zip, file_index, &out_size, 0);
            free(out);

            mz_zip_reader_locate_file(&zip, file_stat.m_filename, NULL, 0);

        }

        int file_location = mz_zip_reader_locate_file(&zip, file_stat.m_filename, NULL, 0);

    }

    mz_zip_reader_end(&zip);

    return 0;

}
