#include <stdio.h>
#include "./lodepng/lodepng.cpp"

extern int
main(int argc, char** argv)
{

    if (argc <= 1)
    {
        printf("No arguments.\n");
        return -1;
    }



    auto buffer          = std::vector<unsigned char> {};
    auto load_file_error = lodepng::load_file(buffer, argv[1]);

    if (load_file_error)
    {
        return -1;
    }



    auto state = LodePNGState {};
    lodepng_state_init(&state);



    unsigned width_a      = {};
    unsigned height_a     = {};
    auto    inspect_error =
        lodepng_inspect
        (
            &width_a,
            &height_a,
            &state,
            buffer.data(),
            buffer.size()
        );

    if (inspect_error)
    {
        return -1;
    }



    lodepng_state_cleanup(&state);



    lodepng_state_init(&state);


    unsigned char* image_a        = {};
    unsigned       decode_error_a =
        lodepng_decode
        (
            &image_a,
            &width_a,
            &height_a,
            &state,
            buffer.data(),
            buffer.size()
        );

    if (!decode_error_a && image_a)
    {
        auto encoded = std::vector<unsigned char> {};

        auto encode_error =
            lodepng::encode
            (
                encoded,
                image_a,
                width_a,
                height_a,
                state.info_png.color.colortype,
                state.info_png.color.bitdepth
            );

        if (encode_error)
        {
            return -1;
        }

        free(image_a);
    }



    lodepng_state_cleanup(&state);



    auto     image_b  = std::vector<unsigned char> {};
    unsigned width_b  = {};
    unsigned height_b = {};

    unsigned decode_error_b =
        lodepng::decode
        (
            image_b,
            width_b,
            height_b,
            buffer,
            LCT_RGBA,
            8
        );

    if (decode_error_b)
    {
        return -1;
    }

}
