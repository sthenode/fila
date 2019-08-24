///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2019 $organization$
///
/// This software is provided by the author and contributors ``as is'' 
/// and any express or implied warranties, including, but not limited to, 
/// the implied warranties of merchantability and fitness for a particular 
/// purpose are disclaimed. In no event shall the author or contributors 
/// be liable for any direct, indirect, incidental, special, exemplary, 
/// or consequential damages (including, but not limited to, procurement 
/// of substitute goods or services; loss of use, data, or profits; or 
/// business interruption) however caused and on any theory of liability, 
/// whether in contract, strict liability, or tort (including negligence 
/// or otherwise) arising in any way out of the use of this software, 
/// even if advised of the possibility of such damage.
///
///   File: Mutex.cpp
///
/// Author: $author$
///   Date: 8/20/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/microsoft/windows/Mutex.hpp"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
///
/// windows mutexes
/// ...
HANDLE WINAPI CreateMutex(
  _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
  _In_     BOOL                  bInitialOwner,
  _In_opt_ LPCTSTR               lpName
) {
    ERR_LOG_DEBUG("try {...");
    try {
        bool initiallyLocked = (bInitialOwner != FALSE);
        ::xos::platform::os::microsoft::windows::Mutex* mutex = 0;

        ERR_LOG_DEBUG("new ::xos::platform::os::microsoft::windows::Mutex(initiallyLocked)...");
        if ((mutex = new ::xos::platform::os::microsoft::windows::Mutex(initiallyLocked))) {
            ERR_LOG_DEBUG("...new ::xos::platform::os::microsoft::windows::Mutex(initiallyLocked)");
            return mutex;
        } else {
            ERR_LOG_ERROR("...failed on new ::xos::platform::os::microsoft::windows::Mutex(initiallyLocked)");
        }
        ERR_LOG_DEBUG("...} try");
    } catch (...) {
        ERR_LOG_ERROR("...catch (...)");
    }
    ERR_LOG_ERROR("...return NULL_HANDLE");
    return NULL_HANDLE;
}
BOOL WINAPI ReleaseMutex(
  _In_ HANDLE hMutex
) {
    ::xos::platform::os::microsoft::windows::Handle* handle = 0;

    if ((handle = ((::xos::platform::os::microsoft::windows::Handle*)hMutex))) {
        ::xos::platform::os::microsoft::windows::Mutex* mutex = 0;

        ERR_LOG_DEBUG("handle->ToMutex()...");
        if ((mutex = handle->ToMutex())) {
            BOOL success = FALSE;

            ERR_LOG_DEBUG("mutex->ReleaseMutex()...");
            if ((success = mutex->ReleaseMutex())) {
                ERR_LOG_DEBUG("...mutex->ReleaseMutex()");
            } else {
                ERR_LOG_ERROR("...failed on mutex->ReleaseMutex()");
            }
            return success;
        } else {
            ERR_LOG_ERROR("...failed 0 = handle->ToMutex()");            
        }
    } else {
        ERR_LOG_ERROR("...failed 0 = ((::xos::platform::os::microsoft::windows::Handle*)hMutex)");
    }
    ERR_LOG_ERROR("...return FALSE");
    return FALSE;
}
/// ...
/// windows mutexes
///
#endif /// !defined(WINDOWS)
