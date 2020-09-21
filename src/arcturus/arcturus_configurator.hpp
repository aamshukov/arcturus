//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __ARCTURUS_CONFIGURATOR_H__
#define __ARCTURUS_CONFIGURATOR_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)

class arcturus_configurator : public singleton<arcturus_configurator>, public configurator
{
    private:
        void    populate_options() override;
        void    populate_flags() override;

    public:
                arcturus_configurator() = default;
};

END_NAMESPACE

#endif // __ARCTURUS_CONFIGURATOR_H__
