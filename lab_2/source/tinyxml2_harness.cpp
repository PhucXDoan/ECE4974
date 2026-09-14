#include <stdio.h>
#include "tinyxml2/tinyxml2.cpp"

extern int
main(int argc, char** argv)
{

    if (argc != 2)
    {
        return -1;
    }

    auto input_file_path = argv[1];

    auto document = tinyxml2::XMLDocument();

    document.LoadFile(input_file_path);

    auto error_id = document.ErrorID();

    printf("error_id: %d\n", error_id);

    return error_id;

}
