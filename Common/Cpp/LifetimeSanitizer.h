/*  Lifetime Sanitizer
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_LifetimeSanitizer_H
#define PokemonAutomation_LifetimeSanitizer_H

//#define PA_SANITIZER_ENABLE

namespace PokemonAutomation{



class LifetimeSanitizer{
public:
#ifdef PA_SANITIZER_ENABLE
    LifetimeSanitizer();
    ~LifetimeSanitizer();

    LifetimeSanitizer(LifetimeSanitizer&& x);
    void operator=(LifetimeSanitizer&& x);

    LifetimeSanitizer(const LifetimeSanitizer& x);
    void operator=(const LifetimeSanitizer& x);

    void check_usage() const;
    static void set_enabled(bool enabled);

#else
    void check_usage() const{}
    static void set_enabled(bool){}
#endif

};




}
#endif
