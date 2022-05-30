/*  Shiny Encounter Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonBDSP_ShinyEncounterDetector_H
#define PokemonAutomation_PokemonBDSP_ShinyEncounterDetector_H

#include "CommonFramework/Notifications/ProgramInfo.h"
#include "CommonFramework/Notifications/EventNotificationOption.h"
#include "Pokemon/Pokemon_DataTypes.h"
#include "Pokemon/Options/Pokemon_EncounterBotOptions.h"
#include "PokemonBDSP/Inference/PokemonBDSP_DialogDetector.h"
#include "PokemonSwSh/Inference/Battles/PokemonSwSh_BattleDialogTracker.h"
#include "PokemonBDSP/Inference/Battles/PokemonBDSP_BattleMenuDetector.h"
#include "PokemonBDSP_ShinySparkleSet.h"

namespace PokemonAutomation{
    class BotBaseContext;
    class ConsoleHandle;
namespace NintendoSwitch{
namespace PokemonBDSP{
using namespace Pokemon;



struct DoublesShinyDetection : public ShinyDetectionResult{
    bool left_is_shiny = false;
    bool right_is_shiny = false;
};


struct DetectionType{
    ImageFloatBox box;
    PokemonSwSh::EncounterState required_state;
    std::chrono::milliseconds state_duration;
    bool full_battle_menu;
};
extern const DetectionType WILD_POKEMON;
extern const DetectionType YOUR_POKEMON;




class ShinyEncounterTracker : public VisualInferenceCallback{
    using EncounterDialogTracker = PokemonSwSh::EncounterDialogTracker;

public:
    ShinyEncounterTracker(
        LoggerQt& logger, VideoOverlay& overlay,
        BattleType battle_type
    );

    const EncounterDialogTracker& dialog_tracker() const{ return m_dialog_tracker; }
    const ShinySparkleAggregator& sparkles_wild_overall() const{ return m_best_wild_overall; }
    const ShinySparkleAggregator& sparkles_wild_left() const{ return m_best_wild_left; }
    const ShinySparkleAggregator& sparkles_wild_right() const{ return m_best_wild_right; }
    const ShinySparkleAggregator& sparkles_own() const{ return m_best_own; }

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool process_frame(const QImage& frame, WallClock timestamp) override;

    ShinyType get_results() const;


private:
    LoggerQt& m_logger;
//    VideoOverlay& m_overlay;

    BattleMenuWatcher m_battle_menu;

    BattleDialogDetector m_dialog_detector;
    EncounterDialogTracker m_dialog_tracker;

    ImageFloatBox m_box_wild_left;
    ImageFloatBox m_box_wild_right;

    ShinySparkleSetBDSP m_sparkles_wild;
    ShinySparkleSetBDSP m_sparkles_own;
    ShinySparkleTracker m_sparkle_tracker_wild;
    ShinySparkleTracker m_sparkle_tracker_own;

    ShinySparkleAggregator m_best_wild_overall;
    ShinySparkleAggregator m_best_wild_left;
    ShinySparkleAggregator m_best_wild_right;
    ShinySparkleAggregator m_best_own;
};



void detect_shiny_battle(
    ConsoleHandle& console, BotBaseContext& context,
    DoublesShinyDetection& wild_result,
    ShinyDetectionResult& your_result,
    const ProgramInfo& info, EventNotificationOption& settings,
    const DetectionType& type,
    std::chrono::seconds timeout
);








}
}
}
#endif
