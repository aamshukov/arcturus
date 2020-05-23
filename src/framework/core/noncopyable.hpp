//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// from boost
#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

#pragma once

BEGIN_NAMESPACE(core)

class noncopyable
{
      protected:
          constexpr noncopyable()                                   = default;

                    noncopyable(const noncopyable&)                 = delete;
                    noncopyable(const noncopyable&&)                = delete;

                   ~noncopyable()                                   = default;

                    noncopyable& operator = (const noncopyable&)    = delete;
                    noncopyable& operator = (const noncopyable&&)   = delete;
};

END_NAMESPACE

using noncopyable = COMPANY::core::noncopyable;

#endif // __NONCOPYABLE_H__
