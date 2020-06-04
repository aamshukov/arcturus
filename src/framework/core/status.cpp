//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/status.hpp>

BEGIN_NAMESPACE(core)

status::status()
      : my_custom_code(status::custom_code::success),
        my_system_code(SYSTEM_SUCCESS_CODE),
        my_library_code(0),
        my_contributor(contributor::core)
{
}
    
status::~status()
{
}

status::status(const status& other)
{
    if(this != &other)
    {
        my_custom_code = other.my_custom_code;
        my_system_code = other.my_system_code;
        my_library_code = other.my_library_code;

        my_contributor = other.my_contributor;

        my_text = other.my_text;
    }
}

status::status(status&& other)
{
    if(this != &other)
    {
        my_custom_code = other.my_custom_code;
        my_system_code = other.my_system_code;
        my_library_code = other.my_library_code;

        my_contributor = other.my_contributor;

        my_text = std::move(other.my_text);
    }
}

const status& status::operator = (const status& other)
{
    if(this != &other)
    {
        my_custom_code = other.my_custom_code;
        my_system_code = other.my_system_code;
        my_library_code = other.my_library_code;

        my_contributor = other.my_contributor;

        my_text = other.my_text;
    }

    return *this;
}

status& status::operator = (status&& other)
{
    if(this != &other)
    {
        my_custom_code = other.my_custom_code;
        my_system_code = other.my_system_code;
        my_library_code = other.my_library_code;

        my_contributor = other.my_contributor;

        my_text = std::move(other.my_text);
    }

    return *this;
}

#define XL(x)    L ## x
#define XL2(x)    XL(x)

string_type status::get_system_error_message()
{
#ifdef WIN32
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
   
    DWORD lastError = GetLastError(); 

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  lastError,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR)&lpMsgBuf,
                  0,
                  NULL);

    DWORD bufferSize = lstrlen((LPCTSTR)lpMsgBuf) + lstrlen(XL2(__FUNCTION__)) + lstrlen(XL2(__FILE__)) + 64 * sizeof(TCHAR);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, bufferSize);

    StringCchPrintf((LPTSTR)lpDisplayBuf,
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("Operation completed with system code '%d': %s"),
                    lastError,
                    lpMsgBuf);

    string_type result((char_type*)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
#else
    string_type result;
#endif
    return result;
}

END_NAMESPACE
