/*  Runnable Computer Program
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_RunnableComputerProgram_H
#define PokemonAutomation_RunnableComputerProgram_H

#include "RunnablePanel.h"

namespace PokemonAutomation{

class CancellableScope;
class ProgramEnvironment;


class RunnableComputerProgramDescriptor : public RunnablePanelDescriptor{
public:
    RunnableComputerProgramDescriptor(
        std::string identifier,
        std::string category, std::string display_name,
        std::string doc_link,
        std::string description
    );
};



class RunnableComputerProgramInstance : public RunnablePanelInstance{
public:
    using RunnablePanelInstance::RunnablePanelInstance;

    const RunnableComputerProgramDescriptor& descriptor() const{
        return static_cast<const RunnableComputerProgramDescriptor&>(m_descriptor);
    }

    virtual QWidget* make_widget(QWidget& parent, PanelHolder& holder) override;
    virtual void program(ProgramEnvironment& env, CancellableScope& scope) = 0;
};




}
#endif