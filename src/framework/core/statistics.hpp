//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#pragma once

BEGIN_NAMESPACE(core)

class statistics : public singleton<statistics>
{
    public:
        statistics() = default;
};

END_NAMESPACE

#endif // __STATISTICS_H__
