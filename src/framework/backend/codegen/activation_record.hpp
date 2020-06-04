//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ACTIVATION_RECORD_H__
#define __ACTIVATION_RECORD_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)

class access; //??

class activation_record : private noncopyable
{
    // abstract class to implement activationrecord/frame interface
    public:
        virtual access add_local(bool escape) = 0; // true - escapes (goes into frame), false - goes into register
};

END_NAMESPACE

#endif // __ACTIVATION_RECORD_H__
