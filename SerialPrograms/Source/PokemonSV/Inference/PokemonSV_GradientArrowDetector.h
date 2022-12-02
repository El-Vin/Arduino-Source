/*  Gradient Arrow Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_GradientArrowDetector_H
#define PokemonAutomation_PokemonSV_GradientArrowDetector_H

#include <vector>
#include "Common/Cpp/Color.h"
#include "Common/Cpp/Containers/FixedLimitVector.h"
#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/VideoPipeline/VideoOverlayScopes.h"
#include "CommonFramework/InferenceInfra/VisualInferenceCallback.h"
#include "CommonFramework/Inference/VisualDetector.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

enum class GradientArrowType{
    RIGHT,
    DOWN,
};


//  This only works for horizontal arrows. If we need the vertical arrow, we can
//  add that later.
class GradientArrowDetector : public StaticScreenDetector{
public:
    GradientArrowDetector(
        Color color,
        GradientArrowType type,
        const ImageFloatBox& box
    );

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool detect(const ImageViewRGB32& screen) const override;

    //  If arrow is found, returns true and "box" contains the box for the arrow.
    //  Otherwise, returns false and "box" is undefined.
    bool detect(ImageFloatBox& box, const ImageViewRGB32& screen) const;

protected:
    Color m_color;
    GradientArrowType m_type;
    ImageFloatBox m_box;
};



class GradientArrowWatcher : public VisualInferenceCallback{
public:
    ~GradientArrowWatcher();
    GradientArrowWatcher(
        Color color,
        VideoOverlay& overlay,
        GradientArrowType type,
        const ImageFloatBox& box
    );

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool process_frame(const ImageViewRGB32& frame, WallClock timestamp) override;


protected:
    VideoOverlay& m_overlay;
    GradientArrowDetector m_detector;
    FixedLimitVector<OverlayBoxScope> m_arrows;
};



}
}
}
#endif
