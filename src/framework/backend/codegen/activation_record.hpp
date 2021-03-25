//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ACTIVATION_RECORD_H__
#define __ACTIVATION_RECORD_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(symtable)
USING_NAMESPACE(frontend)

class access; //??

class activation_record : private noncopyable
{
    // abstract class to implement activationrecord/frame interface
    private:
        // saved data (regs, etc.)

        // actual params (arguments)

        // local variables
        // temporary variables

        // dynamic/control link (old BP)
        // static/access link

        // return address

        // result

    public:
        virtual access  add_local(bool escape) = 0; // true - escapes (goes into frame), false - goes into register
};

END_NAMESPACE

#endif // __ACTIVATION_RECORD_H__
