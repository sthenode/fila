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
///   File: Mutex.hpp
///
/// Author: $author$
///   Date: 8/20/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_MUTEX_HPP
#define _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_MUTEX_HPP

#include "xos/platform/os/microsoft/windows/Handle.hpp"
#include "xos/mt/os/mutex.hpp"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

typedef Handle::implements MutexImplements;
typedef Handle MutexExtends;
///////////////////////////////////////////////////////////////////////
///  Class: Mutex
///////////////////////////////////////////////////////////////////////
class _EXPORT_CLASS Mutex: virtual public MutexImplements, public MutexExtends {
public:
    typedef MutexImplements implements;
    typedef MutexExtends extends;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    Mutex(BOOL bInitialOwner): mutex_(((::xos::mt::os::mutex::attached_t)0), false, false) {
        if (!(mutex_.create(bInitialOwner))) {
            throw create_exception(create_failed);
        }
    }
    Mutex(const Mutex& copy): mutex_(copy.mutex_) {
    }
    Mutex(): mutex_(((::xos::mt::os::mutex::attached_t)0), false, false) {
        if (!(mutex_.create())) {
            throw create_exception(create_failed);
        }
    }
    virtual ~Mutex() {
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual BOOL CloseHandle() {
        BOOL success = (mutex_.destroy())?(TRUE):(FALSE);
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::lock_status status = ::xos::lock_failed;
        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = mutex_.timed_lock(dwMilliseconds);
            } else {
                status = mutex_.try_lock();
            }
        } else {
            status = mutex_.untimed_lock();
        }
        switch (status) {
        case ::xos::lock_success:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::lock_busy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::lock_interrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    virtual DWORD GetLastError(void) {
        DWORD dwLastError = 1;
        return dwLastError;
    }
    virtual BOOL ReleaseMutex() {
        BOOL success = FALSE;
        if ((mutex_.unlock())) {
            success = TRUE;
        }
        return success;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Mutex* ToMutex() const {
        return ((Mutex*)this);
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    ::xos::mt::os::mutex mutex_;
}; /// class _EXPORT_CLASS Mutex

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#endif /// _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_MUTEX_HPP 
