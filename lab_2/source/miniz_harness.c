#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"

extern int
main(int argc, char** argv)
{

    // Load file content.

    long           file_size = {};
    unsigned char* file_data = {};
    {

        char* file_path   = argv[1];
        FILE* file_handle = fopen(file_path, "rb");

        fseek(file_handle, 0, SEEK_END);
        file_size = ftell(file_handle);
        fseek(file_handle, 0, SEEK_SET);

        file_data = malloc(file_size);

        fread(file_data, file_size, 1, file_handle);

        fclose(file_handle);

    }



    // Run some `miniz` procedures.

    mz_zip_archive zip = {};
    mz_zip_zero_struct(&zip);
    mz_zip_reader_init_mem(&zip, file_data, file_size, 0);
    mz_zip_reader_end(&zip);

    free(file_data);

}
