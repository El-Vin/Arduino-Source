/*  Scalar Button Framework
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <sstream>
#include "ClientSource/Libraries/MessageConverter.h"
#include "NintendoSwitch_Commands_ScalarButtons.h"
#include "NintendoSwitch_Messages_ScalarButtons.h"

namespace PokemonAutomation{
namespace NintendoSwitch{


void ssf_flush_pipeline(BotBaseContext& context){
    context.issue_request(
        DeviceRequest_ssf_flush_pipeline()
    );
}
void ssf_do_nothing(BotBaseContext& context, uint16_t ticks){
    context.issue_request(
        DeviceRequest_ssf_do_nothing(ticks)
    );
}



void ssf_press_button(
    BotBaseContext& context,
    Button button,
    uint16_t delay, uint16_t hold, uint8_t cool
){
    context.issue_request(
        DeviceRequest_ssf_press_button(button, delay, hold, cool)
    );
}
void ssf_press_dpad(
    BotBaseContext& context,
    DpadPosition position,
    uint16_t delay, uint16_t hold, uint8_t cool
){
    context.issue_request(
        DeviceRequest_ssf_press_dpad(position, delay, hold, cool)
    );
}
void ssf_press_joystick(
    BotBaseContext& context,
    bool left,
    uint8_t x, uint8_t y,
    uint16_t delay, uint16_t hold, uint8_t cool
){
    context.issue_request(
        DeviceRequest_ssf_press_joystick(left, x, y, delay, hold, cool)
    );
}



void ssf_mash1_button(BotBaseContext& context, Button button, uint16_t ticks){
    context.issue_request(
        DeviceRequest_ssf_mash1_button(button, ticks)
    );
}
void ssf_mash2_button(BotBaseContext& context, Button button0, Button button1, uint16_t ticks){
    context.issue_request(
        DeviceRequest_ssf_mash2_button(button0, button1, ticks)
    );
}
void ssf_mash_AZs(BotBaseContext& context, uint16_t ticks){
    context.issue_request(
        DeviceRequest_ssf_mash_AZs(ticks)
    );
}
void ssf_issue_scroll(
    BotBaseContext& context,
    ssf_ScrollDirection direction,
    uint16_t delay, uint16_t hold, uint8_t cool
){
    context.issue_request(
        DeviceRequest_ssf_issue_scroll(direction, delay, hold, cool)
    );
}






int register_message_converters_ssf(){
    register_message_converter(
        PABB_MSG_COMMAND_SSF_FLUSH_PIPELINE,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_flush_pipeline() - ";
            if (body.size() != sizeof(pabb_ssf_flush_pipeline)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_flush_pipeline*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_DO_NOTHING,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_press_button() - ";
            if (body.size() != sizeof(pabb_ssf_do_nothing)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_do_nothing*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", ticks = " << params->ticks;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_PRESS_BUTTON,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_press_button() - ";
            if (body.size() != sizeof(pabb_ssf_press_button)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_press_button*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", button = " << params->button;
            ss << ", delay = " << params->delay;
            ss << ", hold = " << params->hold;
            ss << ", cool = " << (int)params->cool;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_PRESS_DPAD,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_press_dpad() - ";
            if (body.size() != sizeof(pabb_ssf_press_dpad)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_press_dpad*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", position = " << (int)params->position;
            ss << ", delay = " << params->delay;
            ss << ", hold = " << params->hold;
            ss << ", cool = " << (int)params->cool;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_PRESS_JOYSTICK_L,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_press_joystick(left) - ";
            if (body.size() != sizeof(pabb_ssf_press_joystick)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_press_joystick*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", x = " << (int)params->x;
            ss << ", y = " << (int)params->y;
            ss << ", delay = " << params->delay;
            ss << ", hold = " << params->hold;
            ss << ", cool = " << (int)params->cool;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_PRESS_JOYSTICK_R,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_press_joystick(right) - ";
            if (body.size() != sizeof(pabb_ssf_press_joystick)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_press_joystick*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", x = " << (int)params->x;
            ss << ", y = " << (int)params->y;
            ss << ", delay = " << params->delay;
            ss << ", hold = " << params->hold;
            ss << ", cool = " << (int)params->cool;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_MASH1_BUTTON,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_mash1_button() - ";
            if (body.size() != sizeof(pabb_ssf_mash1_button)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_mash1_button*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", button = " << (int)params->button;
            ss << ", ticks = " << params->ticks;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_MASH2_BUTTON,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_mash2_button() - ";
            if (body.size() != sizeof(pabb_ssf_mash2_button)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_mash2_button*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", button0 = " << (int)params->button0;
            ss << ", button1 = " << (int)params->button1;
            ss << ", ticks = " << params->ticks;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_MASH_AZS,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_mash_AZs() - ";
            if (body.size() != sizeof(pabb_ssf_mash_AZs)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_mash_AZs*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", ticks = " << params->ticks;
            return ss.str();
        }
    );
    register_message_converter(
        PABB_MSG_COMMAND_SSF_SCROLL,
        [](const std::string& body){
            std::ostringstream ss;
            ss << "ssf_issue_scroll() - ";
            if (body.size() != sizeof(pabb_ssf_issue_scroll)){ ss << "(invalid size)" << std::endl; return ss.str(); }
            const auto* params = (const pabb_ssf_issue_scroll*)body.c_str();
            ss << "seqnum = " << (uint64_t)params->seqnum;
            ss << ", delay = " << params->delay;
            ss << ", hold = " << params->hold;
            ss << ", cool = " << (int)params->cool;
            return ss.str();
        }
    );
    return 0;
}
int init_ssf = register_message_converters_ssf();




}
}
