/*  Image Filter
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/ImageTypes/ImageViewRGB32.h"
#include "CommonFramework/ImageTypes/ImageRGB32.h"
#include "ImageGradient.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{


size_t count_horizontal_translucent_border_pixels(const ImageViewRGB32& image, const Color& threshold, bool dark_top){
    std::vector<int16_t> gradients(image.width() * 3, 0);
    std::vector<bool> is_black(image.width() * 3, false);

    if (image.height() == 0 || image.width() == 0){
        return 0;
    }

    size_t num_rows = image.height() - 1;
    for(size_t row_index = 0; row_index < num_rows; row_index++){
        for(size_t x = 0; x < image.width(); x++){
            uint32_t p_above = image.pixel(x, row_index);
            uint32_t p_below = image.pixel(x, row_index+1);

            // c1 may be darker, while c2 is not yet covered by the dark translucent region
            Color c1(dark_top ? p_above : p_below);
            Color c2(dark_top ? p_below : p_above);
            if (c1.r() == 0 && c2.r() > 0){
                is_black[3*x] = true;
            }
            if (c1.g() == 0 && c2.g() > 0){
                is_black[3*x+1] = true;
            }
            if (c1.b() == 0 && c2.b() > 0){
                is_black[3*x+2] = true;
            }
            gradients[3*x] = std::max(gradients[3*x], int16_t((int)c2.r() - (int) c1.r()));
            gradients[3*x+1] = std::max(gradients[3*x+1], int16_t((int)c2.g() - (int) c1.g()));
            gradients[3*x+2] = std::max(gradients[3*x+2], int16_t((int)c2.b() - (int) c1.b()));
        }
    }

    int16_t thres_c[3] = {threshold.r(), threshold.g(), threshold.b()};

    size_t num_border_pixels = 0;
    for(size_t x = 0; x < image.width(); x++){
        bool has_non_border_channel = false;
        for(int c = 0; c < 3; c++){
            if (is_black[3*x+c] == false && gradients[3*x+c] < thres_c[c]){
                has_non_border_channel = true;
                break;
            }
        }

        num_border_pixels += !has_non_border_channel;
    }
    
    int16_t min_color[3] = {255, 255, 255}; 
    for(size_t i = 0; i < image.width(); i++){
        if (is_black[3*i] == false || is_black[3*i+1] == false || is_black[3*i+2] == false){
            // cout << i << ": (" << gradients[3*i] << (is_black[3*i] ? "*" : "");
            // cout << ", " << gradients[3*i+1] << (is_black[3*i+1] ? "*" : "");
            // cout << ", " << gradients[3*i+2] << (is_black[3*i+2] ? "*" : "") << ")\n";

            for(int j = 0; j < 3; j++){
                if (is_black[3*i+j] == false){
                    min_color[j] = std::min(min_color[j], gradients[3*i + j]);
                }
            }
        }
    }
    // cout << "min non-0 color " << min_color[0] << " " << min_color[1] << " " << min_color[2] << endl;
    cout << "num border pixels proportion: " << num_border_pixels / (float)image.width() << endl;

    return num_border_pixels;
}


size_t count_vertical_translucent_border_pixels(const ImageViewRGB32& image, const Color& threshold, bool dark_left){
    if (image.height() == 0 || image.width() == 0){
        return 0;
    }

    size_t num_border_pixels = 0;
    int16_t thres_c[3] = {threshold.r(), threshold.g(), threshold.b()};

    size_t num_cols = image.width() - 1;
    // Go through each row in the image
    for(size_t y = 0; y < image.height(); y++){
        // Record the border gradient of this row:
        bool is_black[3] = {false};
        int16_t gradients[3] = {0};
        for(size_t col_index = 0; col_index < num_cols; col_index++){
            uint32_t p_left = image.pixel(col_index, y);
            uint32_t p_right = image.pixel(col_index+1, y);

            // c1 may be darker, while c2 is not yet covered by the dark translucent region
            Color c1(dark_left ? p_left : p_right);
            Color c2(dark_left ? p_right : p_left);

            if (c1.r() == 0 && c2.r() > 0){
                is_black[0] = true;
            }
            if (c1.g() == 0 && c2.g() > 0){
                is_black[1] = true;
            }
            if (c1.b() == 0 && c2.b() > 0){
                is_black[2] = true;
            }

            gradients[0] = std::max(gradients[0], int16_t((int)c2.r() - (int) c1.r()));
            gradients[1] = std::max(gradients[1], int16_t((int)c2.g() - (int) c1.g()));
            gradients[2] = std::max(gradients[2], int16_t((int)c2.b() - (int) c1.b()));
        }

        bool has_non_border_channel = false;
        for(int c = 0; c < 3; c++){
            if (is_black[c] == false && gradients[c] < thres_c[c]){
                has_non_border_channel = true;
                break;
            }
        }
        num_border_pixels += !has_non_border_channel;

        // cout << y << ": (" << gradients[0] << (is_black[0] ? "*" : "");
        // cout << ", " << gradients[1] << (is_black[1] ? "*" : "");
        // cout << ", " << gradients[2] << (is_black[2] ? "*" : "") << ")\n";
        // cout << "has_non_border_channel " << has_non_border_channel << endl;
    }
    
    cout << "num border pixels proportion: " << num_border_pixels / (float)image.height() << endl;

    return num_border_pixels;
}





}
