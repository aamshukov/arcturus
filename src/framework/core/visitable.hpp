//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VISITABLE_H__
#define __VISITABLE_H__

#pragma once

BEGIN_NAMESPACE(core)

class visitable : private noncopyable
{
};

END_NAMESPACE

#define ACCEPT_METHOD                                                                                       \
    template <typename TVisitor> /* deficiency of C++ ... template methods cannot be virtual ...*/          \
    typename TVisitor::return_type accept(TVisitor& visitor, const typename TVisitor::param_type& param)    \
    {                                                                                                       \
        if constexpr (std::is_void_v<typename TVisitor::return_type>)                                       \
        {                                                                                                   \
            if constexpr (std::is_void_v<typename TVisitor::param_type>)                                    \
            {                                                                                               \
                visitor.visit(*this);                                                                       \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                visitor.visit(*this, param);                                                                \
            }                                                                                               \
        }                                                                                                   \
        else                                                                                                \
        {                                                                                                   \
            if constexpr (std::is_void_v<typename TVisitor::param_type>)                                    \
            {                                                                                               \
                return visitor.visit(*this);                                                                \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                return visitor.visit(*this, param);                                                         \
            }                                                                                               \
        }                                                                                                   \
    }

#endif // __VISITABLE_H__
