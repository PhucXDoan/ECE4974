#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"

#define UPPER_BOUND ((size_t) 16 * 1024 * 1024)



static size_t
dummy_callback
(
    void*       opaque,
    mz_uint64   offset,
    const void* buffer,
    size_t      size
)
{
    return size;
}



static void
exercise_zip_reader
(
    const uint8_t* data,
    size_t         size,
    mz_uint        init_flags
)
{

    mz_zip_archive zip = {};
    mz_zip_zero_struct(&zip);

    if (!mz_zip_reader_init_mem(&zip, data, size, init_flags))
    {
        mz_zip_get_error_string(mz_zip_get_last_error(&zip));
        return;
    }

    mz_zip_get_type(&zip);
    mz_zip_get_archive_size(&zip);
    mz_zip_get_archive_file_start_offset(&zip);
    mz_zip_validate_archive(&zip, 0);

    mz_uint file_count = mz_zip_reader_get_num_files(&zip);

    for (mz_uint file_index = 0; file_index < file_count; file_index += 1)
    {

        mz_zip_archive_file_stat stat = {};

        if (!mz_zip_reader_file_stat(&zip, file_index, &stat))
        {
            continue;
        }

        mz_zip_reader_is_file_a_directory(&zip, file_index);
        mz_zip_reader_is_file_encrypted  (&zip, file_index);
        mz_zip_reader_is_file_supported  (&zip, file_index);
        mz_zip_validate_file(&zip, file_index, 0);

        mz_uint name_length = mz_zip_reader_get_filename(&zip, file_index, nullptr, 0);
        char*   name        = malloc(name_length + 1);

        if (name)
        {

            mz_zip_reader_get_filename(&zip, file_index, name, name_length + 1);

            mz_uint found_index = 0;
            mz_zip_reader_locate_file(&zip, name, nullptr, 0);
            mz_zip_reader_locate_file(&zip, name, nullptr, MZ_ZIP_FLAG_CASE_SENSITIVE);
            mz_zip_reader_locate_file(&zip, name, nullptr, MZ_ZIP_FLAG_IGNORE_PATH);
            mz_zip_reader_locate_file_v2(&zip, name, nullptr, 0, &found_index);

            if (stat.m_uncomp_size <= UPPER_BOUND)
            {
                size_t by_name_size = 0;
                void*  by_name      = mz_zip_reader_extract_file_to_heap(&zip, name, &by_name_size, 0);
                if (by_name)
                {
                    mz_free(by_name);
                }
            }

            free(name);

        }

        if (stat.m_uncomp_size > UPPER_BOUND)
        {
            continue;
        }

        size_t heap_size = 0;
        void*  heap_data = mz_zip_reader_extract_to_heap(&zip, file_index, &heap_size, 0);
        if (heap_data)
        {
            mz_free(heap_data);
        }

        heap_data = mz_zip_reader_extract_to_heap(&zip, file_index, &heap_size, MZ_ZIP_FLAG_COMPRESSED_DATA);
        if (heap_data)
        {
            mz_free(heap_data);
        }

        size_t   mem_size = (size_t) stat.m_uncomp_size;
        uint8_t* mem      = malloc(mem_size ? mem_size : 1);
        if (mem)
        {
            mz_zip_reader_extract_to_mem(&zip, file_index, mem, mem_size, 0);
            mz_zip_reader_extract_to_mem(&zip, file_index, mem, mem_size, MZ_ZIP_FLAG_COMPRESSED_DATA);
            free(mem);
        }

        mz_zip_reader_extract_to_callback(&zip, file_index, dummy_callback, nullptr, 0);

        mz_zip_reader_extract_iter_state* iterator = mz_zip_reader_extract_iter_new(&zip, file_index, 0);

        if (iterator)
        {

            uint8_t chunk[4096];

            size_t total = 0;
            size_t read  = 0;

            while (true)
            {

                read = mz_zip_reader_extract_iter_read(iterator, chunk, sizeof(chunk));

                if (read <= 0)
                {
                    break;
                }

                total += read;
                if (total > UPPER_BOUND)
                {
                    break;
                }
            }

            mz_zip_reader_extract_iter_free(iterator);

        }

    }

    mz_zip_archive copy = {};
    mz_zip_zero_struct(&copy);

    if (mz_zip_writer_init_heap(&copy, 0, 0))
    {

        for (mz_uint file_index = 0; file_index < file_count; file_index += 1)
        {

            mz_zip_archive_file_stat stat = {};

            if (mz_zip_reader_file_stat(&zip, file_index, &stat) && stat.m_comp_size <= UPPER_BOUND)
            {
                mz_zip_writer_add_from_zip_reader(&copy, &zip, file_index);
            }

        }

        void*  copy_data = nullptr;
        size_t copy_size = 0;

        if (mz_zip_writer_finalize_heap_archive(&copy, &copy_data, &copy_size))
        {

            mz_zip_archive reread = {};
            mz_zip_zero_struct(&reread);

            if (mz_zip_reader_init_mem(&reread, copy_data, copy_size, 0))
            {
                mz_zip_validate_archive(&reread, 0);
                mz_zip_reader_end(&reread);
            }

            mz_free(copy_data);

        }

        mz_zip_writer_end(&copy);

    }

    mz_zip_reader_end(&zip);

}



extern int
LLVMFuzzerTestOneInput(const uint8_t* file_data, size_t file_size)
{

    exercise_zip_reader(file_data, file_size, 0                                                                            );
    exercise_zip_reader(file_data, file_size, MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY                                    );
    exercise_zip_reader(file_data, file_size, MZ_ZIP_FLAG_VALIDATE_HEADERS_ONLY                                            );
    exercise_zip_reader(file_data, file_size, MZ_ZIP_FLAG_CASE_SENSITIVE | MZ_ZIP_FLAG_IGNORE_PATH                         );
    exercise_zip_reader(file_data, file_size, MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY | MZ_ZIP_FLAG_VALIDATE_HEADERS_ONLY);

    return 0;

}
