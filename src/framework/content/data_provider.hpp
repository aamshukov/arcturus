//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DATA_PROVIDER_H__
#define __DATA_PROVIDER_H__

#pragma once

BEGIN_NAMESPACE(core)

interface data_provider
{
    using datum_type = text::datum_type;

    virtual ~data_provider() = 0
    {
    }

    virtual bool load(std::shared_ptr<datum_type[]>& buffer, size_type& count) = 0;
};

END_NAMESPACE

#endif // __DATA_PROVIDER_H__
