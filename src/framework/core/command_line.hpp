//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__

#pragma once

BEGIN_NAMESPACE(core)

class command_line : public noncopyable
{
    private:
        // list of warnings
        // list of options


    public:
        command_line() = default;

        void parse(const char* command_line);
};

END_NAMESPACE

#endif // __COMMAND_LINE_H__
