#include <stdio.h>
#include "miniz.c"
#include "miniz_zip.c"
#include "miniz_tinfl.c"
#include "miniz_tdef.c"

extern int
main(int argc, char** argv)
{

    if (argc <= 1)
    {
        printf("No arguments.\n");
        return -1;
    }

    char* file_path   = argv[1];
    FILE* file_handle = fopen(file_path, "rb");

    if (!file_handle)
    {
        printf("Couldn't open `%s`.\n", file_path);
        return -1;
    }

    fseek(file_handle, 0, SEEK_END);
    long file_size = ftell(file_handle);
    fseek(file_handle, 0, SEEK_SET);



    unsigned char* file_content = malloc(file_size);

    if (!file_content)
    {
        printf("Couldn't allocate.\n");
        return -1;
    }

    size_t fread_count = fread(file_content, file_size, 1, file_handle);

    if (fread_count != 1)
    {
        printf("Couldn't read.\n");
        return -1;
    }

    fclose(file_handle);

    mz_zip_archive zip = {};
    mz_zip_zero_struct(&zip);
    mz_zip_reader_init_mem(&zip, file_content, file_size, 0);
    mz_zip_reader_end(&zip);

    free(file_content);

    printf("Done.\n");

}
