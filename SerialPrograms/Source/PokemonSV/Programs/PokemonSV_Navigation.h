/*  Navigation
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 *  Move between different game elements: overworld, save, internet, map, etc.
 *
 */

#ifndef PokemonAutomation_PokemonSV_Navigation_H
#define PokemonAutomation_PokemonSV_Navigation_H

//#include <stdint.h>
#include <string>

namespace PokemonAutomation{
    struct ProgramInfo;
    class ConsoleHandle;
    class BotBaseContext;
//    class ProgramEnvironment;
//    class EventNotificationOption;
namespace NintendoSwitch{
namespace PokemonSV{


void set_time_to_12am_from_home(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  Perform a No-op day skip that rolls over all the outbreaks and raids.
void day_skip_from_overworld(ConsoleHandle& console, BotBaseContext& context);

//  From overworld, open map. Will change map view from rotated to fixed if not already fixed.
void open_map_from_overworld(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  From map, press A to fly to a travel spot.
void fly_to_overworld_from_map(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  Assume the user can set up picnic at current location, start picnic from overworld.
void picnic_from_overworld(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  While in picnic, stop picnic and back to overworld.
void leave_picnic(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  Enter box system from overworld.
void enter_box_system_from_overworld(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  From box system go to overworld.
void leave_box_system_to_overworld(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);


//  After just entering the Zero Gate, go to the specified station.
void inside_zero_gate_to_station(
    const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context,
    int station //  1 - 4
);

//  You are inside Area Zero having traveled there via the Zero Gate. Return to
//  the zero gate fly spot.
void return_to_outside_zero_gate(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);

//  You are inside Area Zero having traveled there via the Zero Gate. Return to
//  inside the zero gate to setup for a subsequent call to "inside_zero_gate_to_station()".
void return_to_inside_zero_gate(const ProgramInfo& info, ConsoleHandle& console, BotBaseContext& context);




}
}
}
#endif
