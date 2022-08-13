/*  Clone Items (Box Copy Method)
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonBDSP_CloneItemsBoxCopy2_H
#define PokemonAutomation_PokemonBDSP_CloneItemsBoxCopy2_H

#include "Common/Qt/Options/SimpleIntegerOption.h"
#include "CommonFramework/Options/StaticTextOption.h"
#include "CommonFramework/Notifications/EventNotificationsTable.h"
#include "NintendoSwitch/Options/GoHomeWhenDoneOption.h"
//#include "NintendoSwitch/Options/TimeExpressionOption.h"
#include "NintendoSwitch/NintendoSwitch_SingleSwitchProgram.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonBDSP{


class CloneItemsBoxCopy2_Descriptor : public SingleSwitchProgramDescriptor{
public:
    CloneItemsBoxCopy2_Descriptor();
    struct Stats;
    virtual std::unique_ptr<StatsTracker> make_stats() const override;
};



class CloneItemsBoxCopy2 : public SingleSwitchProgramInstance{
public:
    CloneItemsBoxCopy2();

    virtual void program(SingleSwitchProgramEnvironment& env, BotBaseContext& context) override;

private:
    GoHomeWhenDoneOption GO_HOME_WHEN_DONE;
    SimpleIntegerOption<uint16_t> BOXES;
    BooleanCheckBoxOption RELEASE;

    EventNotificationOption NOTIFICATION_STATUS_UPDATE;
    EventNotificationsOption NOTIFICATIONS;
};




}
}
}
#endif
