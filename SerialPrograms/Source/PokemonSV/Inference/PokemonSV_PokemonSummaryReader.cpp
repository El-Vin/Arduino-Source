/*  Pokemon Summary Reader
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/ImageTools/SolidColorTest.h"
#include "CommonFramework/VideoPipeline/VideoOverlayScopes.h"
#include "PokemonSV_PokemonSummaryReader.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{



PokemonSummaryDetector::PokemonSummaryDetector(Color color)
    : m_color(color)
    , m_top_blue_left(0.30, 0.09, 0.10, 0.05)
    , m_top_blue_right(0.60, 0.09, 0.35, 0.05)
    , m_bottom_orange(0.03, 0.94, 0.80, 0.04)
    , m_shiny_symbol(0.575, 0.865, 0.017, 0.030)
{}
void PokemonSummaryDetector::make_overlays(VideoOverlaySet& items) const{
    items.add(m_color, m_top_blue_left);
    items.add(m_color, m_top_blue_right);
    items.add(m_color, m_bottom_orange);
    items.add(m_color, m_shiny_symbol);
}
bool PokemonSummaryDetector::detect(const ImageViewRGB32& screen) const{
    ImageStats top_blue_left = image_stats(extract_box_reference(screen, m_top_blue_left));
//    cout << top_blue_left.average << top_blue_left.stddev << endl;
    if (!is_solid(top_blue_left, {0.0745162, 0.311321, 0.614163}, 0.20, 10)){
        return false;
    }

    ImageStats top_blue_right = image_stats(extract_box_reference(screen, m_top_blue_right));
//    cout << top_blue_right.average << top_blue_right.stddev << endl;
    if (!is_solid(top_blue_right, {0.0745162, 0.311321, 0.614163}, 0.20, 10)){
        return false;
    }

    ImageStats bottom_orange = image_stats(extract_box_reference(screen, m_bottom_orange));
//    cout << bottom_orange.average << bottom_orange.stddev << endl;
    if (!is_solid(bottom_orange, {0.648549, 0.286158, 0.0652928}, 0.15, 20)){
        return false;
    }

//    ImageStats shiny_symbol = image_stats(extract_box_reference(screen, m_shiny_symbol));
//    cout << shiny_symbol.average << shiny_symbol.stddev << endl;

    return true;
}
bool PokemonSummaryDetector::is_shiny(const ImageViewRGB32& screen) const{
    ImageStats shiny = image_stats(extract_box_reference(screen, m_shiny_symbol));
    return shiny.stddev.sum() > 100;
}






}
}
}
