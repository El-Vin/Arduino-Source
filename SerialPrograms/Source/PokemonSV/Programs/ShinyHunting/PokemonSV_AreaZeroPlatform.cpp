/*  Area Zero Platform
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Kernels/Waterfill/Kernels_Waterfill_Session.h"
#include "CommonFramework/Exceptions/OperationFailedException.h"
//#include "CommonFramework/ImageTools/ImageFilter.h"
#include "CommonFramework/ImageTools/BinaryImage_FilterRgb32.h"
#include "CommonFramework/VideoPipeline/VideoFeed.h"
#include "CommonFramework/Tools/ConsoleHandle.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_PushButtons.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_ScalarButtons.h"
#include "PokemonSV/Programs/PokemonSV_Navigation.h"
#include "PokemonSV_AreaZeroPlatform.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{



NavigatePlatformSettings::NavigatePlatformSettings()
    : GroupOption("Navigate to Platform Settings", LockWhileRunning::UNLOCKED)
    , STATION_ARRIVE_PAUSE_SECONDS(
        "<b>Station Arrive Pause</b><br>Pause for this many seconds after leaving the station. "
        "This allows stuff to load to reduce the chance of lag affecting the fly to platform.",
        LockWhileRunning::UNLOCKED,
        1
    )
    , MIDAIR_PAUSE_TIME(
        "<b>Mid-Air Pause Time:</b><br>Pause for this long before final approach to the platform. "
        "Too small and you may crash into the wall or have reduced spawns. "
        "Too large and you may undershoot the platform.",
        LockWhileRunning::UNLOCKED,
        TICKS_PER_SECOND,
        "50"
    )
{
    PA_ADD_OPTION(STATION_ARRIVE_PAUSE_SECONDS);
    PA_ADD_OPTION(MIDAIR_PAUSE_TIME);
}


void inside_zero_gate_to_platform(
    const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context,
    NavigatePlatformSettings& settings
){
    inside_zero_gate_to_station(info, console, context, 2);

    context.wait_for(std::chrono::seconds(settings.STATION_ARRIVE_PAUSE_SECONDS));

    //  Navigate to platform.
#if 0
    //  Don't jump to avoid spawn.
    pbf_press_button(context, BUTTON_PLUS, 20, 105);
    pbf_move_left_joystick(context, 128, 0, 625, 0);
    ssf_press_button(context, BUTTON_B, 0, 50);
    pbf_move_left_joystick(context, 128, 0, 250, 0);
    pbf_move_left_joystick(context, 160, 0, 600, 0);
    pbf_move_left_joystick(context, 128, 0, 1875, 0);
#endif

#if 0
    //  Jump late.
    pbf_press_button(context, BUTTON_PLUS, 20, 105);

    ssf_press_joystick(context, true, 128, 0, 315, 500);

    //  Jump
    ssf_press_button(context, BUTTON_B, 125, 100);

    //  Fly
    ssf_press_button(context, BUTTON_B, 0, 50);

    pbf_move_left_joystick(context, 144, 0, 1150, 0);
    pbf_move_left_joystick(context, 128, 0, 125, NAVIGATE_TO_PLATFORM.MIDAIR_PAUSE_TIME);

    pbf_move_left_joystick(context, 128, 0, 1375, 250);
#endif

#if 0
    //  Jump earlier.
    pbf_press_button(context, BUTTON_PLUS, 20, 105);

    ssf_press_joystick(context, true, 128, 0, 280, 500);

    //  Jump
    ssf_press_button(context, BUTTON_B, 125, 100);

    //  Fly
    ssf_press_button(context, BUTTON_B, 0, 50);

    pbf_move_left_joystick(context, 144, 0, 1150, 0);
    pbf_move_left_joystick(context, 128, 0, 125, NAVIGATE_TO_PLATFORM.MIDAIR_PAUSE_TIME);

    pbf_move_left_joystick(context, 128, 0, 1375, 250);
#endif

#if 1
    //  Jump on the downhill to improve chance of clearing things.
    pbf_move_left_joystick(context, 192, 0, 20, 105);
    pbf_press_button(context, BUTTON_L | BUTTON_PLUS, 20, 105);

    ssf_press_button(context, BUTTON_LCLICK, 0, 500);
    ssf_press_left_joystick(context, 128, 0, 125, 1250);

    //  Jump
    ssf_press_button(context, BUTTON_B, 125, 100);

    //  Fly
    ssf_press_button(context, BUTTON_B, 0, 50);

    pbf_move_left_joystick(context, 144, 0, 700, 0);
    pbf_move_left_joystick(context, 128, 0, 125, settings.MIDAIR_PAUSE_TIME);
    pbf_move_left_joystick(context, 128, 0, 875, 250);
#endif

//    context.wait_for_all_requests();
    pbf_press_button(context, BUTTON_PLUS, 20, 105);
    ssf_press_left_joystick(context, 128, 0, 1 * TICKS_PER_SECOND, 4 * TICKS_PER_SECOND);
    pbf_press_button(context, BUTTON_R, 20, 355);
    pbf_press_button(context, BUTTON_R, 20, 105);
}


bool read_platform_center(
    double& x, double& y,
    const ProgramInfo& info, ConsoleHandle& console
){
    using namespace Kernels::Waterfill;

    VideoSnapshot screen = console.video().snapshot();
    if (!screen){
        return false;
    }

    PackedBinaryMatrix matrix = compress_rgb32_to_binary_range(screen, 0xff000040, 0xff8080ff);

//    size_t min_width = screen.width() / 4;
//    size_t min_height = screen.height() / 4;

    WaterfillObject biggest;
    WaterfillObject object;

    std::unique_ptr<WaterfillSession> session = make_WaterfillSession(matrix);
    size_t min_area = screen->width() * screen->height() / 10;
    auto iter = session->make_iterator(min_area);
    while (iter->find_next(object, false)){
//        if (object.min_y != 0){
//            continue;
//        }
        if (biggest.area < object.area){
            biggest = std::move(object);
        }
    }

    if (biggest.area == 0){
        return false;
    }

    x = biggest.center_of_gravity_x() / screen->width();
    y = biggest.center_of_gravity_y() / screen->height();

    return true;
}






}
}
}
