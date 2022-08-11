/*  Multi-Switch Program Template
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_NintendoSwitch_MultiSwitchProgram_H
#define PokemonAutomation_NintendoSwitch_MultiSwitchProgram_H

#include <functional>
#include "Common/Cpp/FixedLimitVector.h"
#include "CommonFramework/Tools/ProgramEnvironment.h"
#include "CommonFramework/Tools/ConsoleHandle.h"
#include "NintendoSwitch_MultiSwitchSystemOption.h"
#include "NintendoSwitch_RunnableProgram.h"

namespace PokemonAutomation{
    class BotBaseContext;
namespace NintendoSwitch{


class MultiSwitchProgramInstance2;


class MultiSwitchProgramEnvironment : public ProgramEnvironment{
public:
    ~MultiSwitchProgramEnvironment();
    MultiSwitchProgramEnvironment(
        const ProgramInfo& program_info,
        CancellableScope& scope,
        Logger& logger,
        StatsTracker* current_stats,
        const StatsTracker* historical_stats,
        FixedLimitVector<ConsoleHandle> p_switches
    );

    FixedLimitVector<ConsoleHandle> consoles;

    //  Run the specified lambda for all switches in parallel.
    void run_in_parallel(
        CancellableScope& scope,
        const std::function<void(ConsoleHandle& console, BotBaseContext& context)>& func
    );

    //  Run the specified lambda for switch indices [s, e) in parallel.
    void run_in_parallel(
        CancellableScope& scope, size_t s, size_t e,
        const std::function<void(ConsoleHandle& console, BotBaseContext& context)>& func
    );

};



class MultiSwitchProgramDescriptor : public RunnableSwitchProgramDescriptor{
public:
    MultiSwitchProgramDescriptor(
        std::string identifier,
        std::string category, std::string display_name,
        std::string doc_link,
        std::string description,
        FeedbackType feedback, bool allow_commands_while_running,
        PABotBaseLevel min_pabotbase_level,
        size_t min_switches,
        size_t max_switches,
        size_t default_switches
    );

    size_t min_switches() const{ return m_min_switches; }
    size_t max_switches() const{ return m_max_switches; }
    size_t default_switches() const{ return m_default_switches; }

    virtual std::unique_ptr<PanelInstance> make_panel() const override;
    virtual std::unique_ptr<MultiSwitchProgramInstance2> make_instance() const{ return nullptr; }

private:
    const size_t m_min_switches;
    const size_t m_max_switches;
    const size_t m_default_switches;
};



class MultiSwitchProgramInstance : public RunnableSwitchProgramInstance{
public:
    MultiSwitchProgramInstance(const MultiSwitchProgramDescriptor& descriptor);

    //  Called when the user changes the # of Switches.
    virtual void update_active_consoles(){}

    size_t system_count() const{ return m_switches.count(); }

    virtual QWidget* make_widget(QWidget& parent, PanelHolder& holder) override;
    virtual void program(MultiSwitchProgramEnvironment& env, CancellableScope& scope) = 0;

private:
    friend class MultiSwitchProgramWidget;

    MultiSwitchSystemOption m_switches;
};






class MultiSwitchProgramInstance2{
public:
    virtual ~MultiSwitchProgramInstance2() = default;
    MultiSwitchProgramInstance2(const MultiSwitchProgramInstance2&) = delete;
    void operator=(const MultiSwitchProgramInstance2&) = delete;

    MultiSwitchProgramInstance2();

    virtual void program(MultiSwitchProgramEnvironment& env, CancellableScope& scope) = 0;


public:
    //  Settings

    virtual void from_json(const JsonValue& json);
    virtual JsonValue to_json() const;

    virtual std::string check_validity() const;
    virtual void restore_defaults();


protected:
    friend class MultiSwitchProgramOption;

    BatchOption m_options;
    void add_option(ConfigOption& option, std::string serialization_string);


public:
    EventNotificationOption NOTIFICATION_PROGRAM_FINISH;
    EventNotificationOption NOTIFICATION_ERROR_RECOVERABLE;
    EventNotificationOption NOTIFICATION_ERROR_FATAL;
};




template <typename Descriptor, typename Instance>
class MultiSwitchProgramWrapper : public Descriptor{
public:
    virtual std::unique_ptr<MultiSwitchProgramInstance2> make_instance() const override{
        return std::unique_ptr<MultiSwitchProgramInstance2>(new Instance());
    }
};

template <typename Descriptor, typename Instance>
std::unique_ptr<PanelDescriptor> make_multi_switch_program(){
    return std::make_unique<MultiSwitchProgramWrapper<Descriptor, Instance>>();
}









}
}
#endif

