/*  Program Notifications
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QFile>
#include "Common/Cpp/PrettyPrint.h"
#include "Common/Cpp/Json/JsonValue.h"
#include "Common/Cpp/Json/JsonArray.h"
#include "Common/Cpp/Json/JsonObject.h"
#include "CommonFramework/Globals.h"
#include "CommonFramework/GlobalSettingsPanel.h"
#include "CommonFramework/Tools/ProgramEnvironment.h"
#include "CommonFramework/Tools/StatsTracking.h"
#include "Integrations/DiscordWebhook.h"
#include "Integrations/SleepyDiscordRunner.h"
#include "ProgramNotifications.h"

#ifdef PA_OFFICIAL
#include "../Internal/SerialPrograms/TelemetryURLs.h"
#endif

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{


void send_program_notification(
    Logger& logger,
    Color color, bool should_ping, const std::vector<std::string>& tags,
    const ProgramInfo& info,
    const std::string& title,
    const std::vector<std::pair<std::string, std::string>>& messages,
    const ImageAttachment& image
){
    std::shared_ptr<PendingFileSend> file(new PendingFileSend(logger, image));
    bool hasFile = !file->filepath().empty();

    JsonObject embed_sleepy;
    JsonArray embeds;
    {
        JsonObject embed;
        embed["title"] = title;

        if (color){
            embed["color"] = (int)((uint32_t)color & 0xffffff);
        }

        JsonArray fields;
        {
            JsonObject field;
//            field["name"] = PreloadSettings::instance().DEVELOPER_MODE
//                ? PROGRAM_NAME + " (" + PROGRAM_VERSION + "-dev)"
//                : PROGRAM_NAME + " (" + PROGRAM_VERSION + ")";
            field["name"] = info.program_name;
            std::string text;

            const std::string& instance_name = GlobalSettings::instance().DISCORD.message.instance_name;
            if (!instance_name.empty()){
                text += "Instance Name: " + instance_name;
            }

            text += "\nUp Time: ";
            if (info.start_time != WallClock::min()){
                text += duration_to_string(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        current_time() - info.start_time
                    )
                );
            }else{
                text += "(test message)";
            }
            field["value"] = std::move(text);
            fields.push_back(std::move(field));
        }

        for (const auto& item : messages){
            JsonObject field;
            field["name"] = item.first;
            field["value"] = item.second;
            if (!item.first.empty() && !item.second.empty()){
                fields.push_back(std::move(field));
            }
        }
        {
            JsonObject field;
            field["name"] = "Powered By:";
            std::string text = PreloadSettings::instance().DEVELOPER_MODE
                ? PROGRAM_NAME + " CC " + PROGRAM_VERSION + "-dev"
                : PROGRAM_NAME + " CC " + PROGRAM_VERSION + "";
            if (GlobalSettings::instance().HIDE_NOTIF_DISCORD_LINK){
                text += " ([GitHub](" + PROJECT_GITHUB_URL + "About/))";
            }else{
                text += " ([GitHub](" + PROJECT_GITHUB_URL + "About/)/[Discord](" + DISCORD_LINK_URL + "))";
            }
            field["value"] = std::move(text);
            fields.push_back(std::move(field));
        }
        embed["fields"] = std::move(fields);

        if (hasFile){
            JsonObject field;
            field["url"] = "attachment://" + file->filename();
            embed["image"] = std::move(field);
        }
        embeds.push_back(embed.clone());
        embed_sleepy = std::move(embed);
    }

    Integration::DiscordWebhook::send_message(
        logger, should_ping, tags, "",
        std::move(embeds),
        hasFile ? file : nullptr
    );
#ifdef PA_SLEEPY
    Integration::SleepyDiscordRunner::send_message_sleepy(
        should_ping, tags, "", embed_sleepy,
        hasFile ? file : nullptr
    );
#endif
}
void send_program_notification(
    Logger& logger, EventNotificationOption& settings,
    Color color,
    const ProgramInfo& info,
    const std::string& title,
    const std::vector<std::pair<std::string, std::string>>& messages,
    const ImageViewRGB32& image, bool keep_file
){
    if (!settings.ok_to_send_now(logger)){
        return;
    }
    send_program_notification(
        logger,
        color,
        settings.ping(), settings.tags(),
        info, title,
        messages,
        ImageAttachment(image, settings.screenshot(), keep_file)
    );
}

void send_program_telemetry(
    Logger& logger, bool is_error, Color color,
    const ProgramInfo& info,
    const std::string& title,
    const std::vector<std::pair<std::string, std::string>>& messages,
    const std::string& file
){
#ifdef PA_OFFICIAL
    if (!GlobalSettings::instance().SEND_ERROR_REPORTS){
        return;
    }

    bool hasFile = !file.empty();
    std::shared_ptr<PendingFileSend> pending = !hasFile
            ? nullptr
            : std::shared_ptr<PendingFileSend>(new PendingFileSend(file, GlobalSettings::instance().SAVE_DEBUG_IMAGES));

    JsonArray embeds;
    {
        JsonObject embed;
        embed["title"] = title;
        if (color){
            embed["color"] = (uint32_t)color & 0xffffff;
        }

        JsonArray fields;
        {
            JsonObject field;
            field["name"] = PreloadSettings::instance().DEVELOPER_MODE
                ? PROGRAM_NAME + " (" + PROGRAM_VERSION + "-dev)"
                : PROGRAM_NAME + " (" + PROGRAM_VERSION + ")";
            field["value"] = info.program_name.empty() ? "(unknown)" : info.program_name;
            fields.push_back(std::move(field));
        }
        for (const auto& item : messages){
            JsonObject field;
            field["name"] = item.first;
            field["value"] = item.second;
            if (!item.first.empty() && !item.second.empty()){
                fields.push_back(std::move(field));
            }
        }
        embed["fields"] = std::move(fields);

        if (hasFile){
            JsonObject image;
            image["url"] = "attachment://" + pending->filename();
            embed["image"] = std::move(image);
        }
        embeds.push_back(std::move(embed));
    }

    JsonObject jsonContent;
//    jsonContent["content"] = "asdf";
    jsonContent["embeds"] = std::move(embeds);

    std::string url = is_error
            ? flip(ERROR_REPORTING_URL, sizeof(ERROR_REPORTING_URL))
            : flip(TELEMETRY_URL, sizeof(TELEMETRY_URL));

    using namespace Integration::DiscordWebhook;

    DiscordWebhookSender& sender = DiscordWebhookSender::instance();
    if (hasFile){
        sender.send_json(logger, QString::fromStdString(url), jsonContent, pending);
    }else{
        sender.send_json(logger, QString::fromStdString(url), jsonContent, nullptr);
    }
#endif
}




#if 0
void send_program_status_notification(
    Logger& logger, EventNotificationOption& settings,
    const ProgramInfo& info,
    const std::string& message,
    const StatsTracker* current_stats,
    const StatsTracker* historical_stats,
    const ImageViewRGB32& image, bool keep_file
){
    std::vector<std::pair<std::string, std::string>> messages{
        {"Message", message},
    };
    if (current_stats){
        messages.emplace_back("Session Stats", current_stats->to_str());
    }
    if (GlobalSettings::instance().ALL_STATS && historical_stats){
        messages.emplace_back("Historical Stats", historical_stats->to_str());
    }
    send_program_notification(
        logger, settings,
        Color(), info,
        "Program Status",
        messages,
        image, keep_file
    );
}
#endif
void send_program_status_notification(
    ProgramEnvironment& env, EventNotificationOption& settings,
    const std::string& message,
    const ImageViewRGB32& image, bool keep_file
){
    const StatsTracker* current_stats = env.current_stats();
    const StatsTracker* historical_stats = env.historical_stats();
    std::vector<std::pair<std::string, std::string>> messages{
        {"Message", message},
    };
    if (current_stats){
        messages.emplace_back("Session Stats", env.current_stats()->to_str());
    }
    if (GlobalSettings::instance().ALL_STATS && historical_stats){
        messages.emplace_back("Historical Stats", env.historical_stats()->to_str());
    }
    send_program_notification(
        env.logger(), settings,
        Color(), env.program_info(),
        "Program Status",
        messages,
        image, keep_file
    );
}
void send_program_finished_notification(
    Logger& logger, EventNotificationOption& settings,
    const ProgramInfo& info,
    const std::string& message,
    std::string current_stats,
    std::string historical_stats,
    const ImageViewRGB32& image, bool keep_file
){
    std::vector<std::pair<std::string, std::string>> messages{
        {"Message", message},
    };
    if (!current_stats.empty()){
        messages.emplace_back("Session Stats", std::move(current_stats));
    }
    if (GlobalSettings::instance().ALL_STATS && !historical_stats.empty()){
        messages.emplace_back("Historical Stats", std::move(historical_stats));
    }
    send_program_notification(
        logger, settings,
        COLOR_GREEN, info,
        "Program Finished",
        messages,
        image, keep_file
    );
}
void send_program_finished_notification(
    ProgramEnvironment& env, EventNotificationOption& settings,
    const std::string& message,
    const ImageViewRGB32& image, bool keep_file
){
    const StatsTracker* current_stats = env.current_stats();
    const StatsTracker* historical_stats = env.historical_stats();
    std::vector<std::pair<std::string, std::string>> messages{
        {"Message", message},
    };
    if (current_stats){
        messages.emplace_back("Session Stats", env.current_stats()->to_str());
    }
    if (GlobalSettings::instance().ALL_STATS && historical_stats){
        messages.emplace_back("Historical Stats", env.historical_stats()->to_str());
    }
    send_program_notification(
        env.logger(), settings,
        COLOR_GREEN, env.program_info(),
        "Program Finished",
        messages,
        image, keep_file
    );
}
void send_program_recoverable_error_notification(
    ProgramEnvironment& env, EventNotificationOption& settings,
    const std::string& message,
    const ImageViewRGB32& image, bool keep_file
){
    const StatsTracker* current_stats = env.current_stats();
    const StatsTracker* historical_stats = env.historical_stats();
    std::vector<std::pair<std::string, std::string>> messages{
        {"Message", message},
    };
    if (current_stats){
        messages.emplace_back("Session Stats", env.current_stats()->to_str());
    }
    if (GlobalSettings::instance().ALL_STATS && historical_stats){
        messages.emplace_back("Historical Stats", env.historical_stats()->to_str());
    }
    send_program_notification(
        env.logger(), settings,
        COLOR_RED, env.program_info(),
        "Program Error (Recoverable)",
        messages,
        image, keep_file
    );
}
void send_program_fatal_error_notification(
    Logger& logger, EventNotificationOption& settings,
    const ProgramInfo& info,
    const std::string& message,
    std::string current_stats,
    std::string historical_stats,
    const ImageViewRGB32& image, bool keep_file
){
    std::vector<std::pair<std::string, std::string>> messages{
        {"Message", message},
    };
    if (!current_stats.empty()){
        messages.emplace_back("Session Stats", std::move(current_stats));
    }
    if (GlobalSettings::instance().ALL_STATS && !historical_stats.empty()){
        messages.emplace_back("Historical Stats", std::move(historical_stats));
    }
    send_program_notification(
        logger, settings,
        COLOR_RED, info,
        "Program Stopped (Fatal Error)",
        messages,
        image, keep_file
    );
}




}
