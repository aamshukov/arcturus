//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __OPTIMIZATION_PASS_H__
#define __OPTIMIZATION_PASS_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)
USING_NAMESPACE(frontend)

class pass : private noncopyable
{
    public:
        using context_type = std::shared_ptr<context>;

    protected:
        virtual void    execute(const context_type& context) = 0;

    public:
                        pass() = default;
        virtual        ~pass() = 0;
};

END_NAMESPACE

#endif // __OPTIMIZATION_PASS_H__
