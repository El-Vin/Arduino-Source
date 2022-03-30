/*  Image Diff
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <cmath>
#include <smmintrin.h>
#include "Common/Compiler.h"
#include "Common/Cpp/Exceptions.h"
#include "Common/Cpp/SIMDDebuggers.h"
#include "Kernels/ImageStats/Kernels_ImagePixelSumSqr.h"
#include "Kernels/ImageStats/Kernels_ImagePixelSumSqrDev.h"
#include "Kernels/ImageScaleBrightness/Kernels_ImageScaleBrightness.h"
#include "ImageDiff.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace ImageMatch{


FloatPixel pixel_average(const ConstImageRef& image, const ConstImageRef& alpha_mask){
    if (!image){
        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Invalid Dimensions");
    }
    if (image.width() != alpha_mask.width() || image.height() != alpha_mask.height()){
        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Mismatching Dimensions");
    }
    Kernels::PixelSums sums;
    Kernels::pixel_sum_sqr(
        sums, image.width(), image.height(),
        image.data(), image.bytes_per_row(),
        alpha_mask.data(), alpha_mask.bytes_per_row()
    );

    FloatPixel sum(sums.sumR, sums.sumG, sums.sumB);
    sum /= sums.count;
    return sum;
}



void scale_brightness(const ImageRef& image, const FloatPixel& multiplier){
    Kernels::scale_brightness(
        image.width(), image.height(),
        image.data(), image.bytes_per_row(),
        (float)multiplier.r, (float)multiplier.g, (float)multiplier.b
    );
}



double pixel_RMSD(const ConstImageRef& reference, const ConstImageRef& image){
    if (!image){
//        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Invalid Dimensions");
        return 765; //  Max possible deviation.
    }
    if (reference.width() != image.width() || reference.height() != image.height()){
        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Mismatching Dimensions");
    }
    uint64_t count = 0;
    uint64_t sumsqrs = 0;
    Kernels::sum_sqr_deviation(
        count, sumsqrs,
        reference.width(), reference.height(),
        reference.data(), reference.bytes_per_row(),
        image.data(), image.bytes_per_row()
    );
    return std::sqrt((double)sumsqrs / (double)count);
}
double pixel_RMSD(const ConstImageRef& reference, const ConstImageRef& image, QRgb background){
    if (!image){
//        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Invalid Dimensions");
        return 765; //  Max possible deviation.
    }
    if (reference.width() != image.width() || reference.height() != image.height()){
        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Mismatching Dimensions");
    }
    uint64_t count = 0;
    uint64_t sumsqrs = 0;
    Kernels::sum_sqr_deviation(
        count, sumsqrs,
        reference.width(), reference.height(),
        reference.data(), reference.bytes_per_row(),
        image.data(), image.bytes_per_row(),
        background
    );
    return std::sqrt((double)sumsqrs / (double)count);
}
double pixel_RMSD_masked(const ConstImageRef& reference, const ConstImageRef& image){
    if (!image){
//        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Invalid Dimensions");
        return 765; //  Max possible deviation.
    }
    if (reference.width() != image.width() || reference.height() != image.height()){
        throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Mismatching Dimensions");
    }
    uint64_t count = 0;
    uint64_t sumsqrs = 0;
    Kernels::sum_sqr_deviation_masked(
        count, sumsqrs,
        reference.width(), reference.height(),
        reference.data(), reference.bytes_per_row(),
        image.data(), image.bytes_per_row()
    );
    return std::sqrt((double)sumsqrs / (double)count);
}



}
}
