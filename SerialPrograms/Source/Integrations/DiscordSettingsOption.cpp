/*  Discord Settings
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "DiscordSettingsOption.h"

//#include <iostream>
//using std::cout;
//using std::endl;

namespace PokemonAutomation{
namespace Integration{


DiscordMessageSettingsOption::DiscordMessageSettingsOption()
    : BatchOption(LockWhileRunning::LOCKED)
    , instance_name(
        false,
        "<b>Instance Name:</b><br>If you are running multiple instances of this program, give it a name to distinguish them in notifications.",
        LockWhileRunning::LOCKED,
        "",
        "(e.g. Living Room Switch)"
    )
    , user_id(
        false,
        "<b>Discord User ID:</b><br>Set this to your discord user ID to receive pings. Your ID is a number.",
        LockWhileRunning::LOCKED,
        "",
        "123456789012345678"
    )
    , message(
        false,
        "<b>Discord Message:</b><br>Message to put on every discord notification.",
        LockWhileRunning::LOCKED,
        "",
        "(e.g. Kim's Shiny Hunt)"
    )
{
    PA_ADD_OPTION(instance_name);
    PA_ADD_OPTION(user_id);
    PA_ADD_OPTION(message);
}
DiscordMessageSettingsOptionUI::DiscordMessageSettingsOptionUI(QWidget& parent, DiscordMessageSettingsOption& value)
    : BatchWidget(parent, value)
{}






DiscordSettingsOption::DiscordSettingsOption()
    : BatchOption(LockWhileRunning::LOCKED)
{
    PA_ADD_OPTION(message);
    PA_ADD_OPTION(webhooks);
#if defined PA_SLEEPY || defined PA_DPP
    PA_ADD_OPTION(integration);
#endif
}





}
}

