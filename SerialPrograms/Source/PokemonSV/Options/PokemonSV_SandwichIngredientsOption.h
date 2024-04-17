/*  Sandwich Ingredients Option
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_SandwichIngredientsOption_H
#define PokemonAutomation_PokemonSV_SandwichIngredientsOption_H

#include "CommonFramework/Options/StringSelectOption.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{


class SandwichIngredientsTableCell : public StringSelectCell{
public:
    SandwichIngredientsTableCell(const std::string& default_slug);
};



}
}
}
#endif
