//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONFIGURATOR_H__
#define __CONFIGURATOR_H__

#pragma once

BEGIN_NAMESPACE(core)

class configurator : public singleton<configurator>
{
    private:
        // list of warnings
        // list of options
        // list of features

    public:
        configurator() = default;

        // generate_fsa_as_case...
};

END_NAMESPACE

#endif // __CONFIGURATOR_H__
