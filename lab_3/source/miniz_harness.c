#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    mz_zip_archive zip;
    mz_zip_zero_struct(&zip);

    if (!mz_zip_reader_init_mem(&zip, data, size, 0))
        return 0;

    mz_uint n = mz_zip_reader_get_num_files(&zip);




    for (mz_uint i = 0; i < n; i++) {
        mz_zip_archive_file_stat st;

        if (mz_zip_reader_file_stat(&zip, i, &st)) {
            size_t out_size = 0;
            void *out = mz_zip_reader_extract_to_heap(&zip, i, &out_size, 0);
            free(out);  // safe when out == NULL

            (void)mz_zip_reader_locate_file(&zip, st.m_filename, NULL, 0);
        }

        int idx = mz_zip_reader_locate_file(
            &zip, st.m_filename, NULL, 0);
        (void)idx;
    }

    mz_zip_reader_end(&zip);
    return 0;
}
