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
///   File: Semaphore.hpp
///
/// Author: $author$
///   Date: 8/24/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_SEMAPHORE_HPP
#define _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_SEMAPHORE_HPP

#include "xos/mt/os/semaphore.hpp"
#include "xos/platform/os/microsoft/windows/Handle.hpp"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

typedef Handle::implements Semaphore_implements;
typedef Handle Semaphore_extends;
///////////////////////////////////////////////////////////////////////
///  Class: Semaphore
///////////////////////////////////////////////////////////////////////
class _EXPORT_CLASS Semaphore: virtual public Semaphore_implements, public Semaphore_extends {
public:
    typedef Semaphore_implements implements;
    typedef Semaphore_extends extends;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    Semaphore(LONG lInitialCount, LONG lMaximumCount)
    : semaphore_(((::xos::mt::os::semaphore::attached_t)0), false, false) {
        if (!(semaphore_.create(lInitialCount))) {
            throw create_exception(create_failed);
        }
    }
    Semaphore(const Semaphore &copy): semaphore_(copy.semaphore_) {
    }
    Semaphore()
    : semaphore_(((::xos::mt::os::semaphore::attached_t)0), false, false) {
        if (!(semaphore_.create())) {
            throw create_exception(create_failed);
        }
    }
    virtual ~Semaphore() {
        if (!(semaphore_.destroyed())) {
            throw create_exception(destroy_failed);
        }
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual BOOL ReleaseSemaphore(LONG lReleaseCount, LPLONG lpPreviousCount) {
        BOOL success = FALSE;
        if ((0 < lReleaseCount) && (2 > lReleaseCount)) {
            if ((semaphore_.release())) {
                success = TRUE;
            }
        }
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::acquire_status status = ::xos::acquire_failed;

        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = semaphore_.timed_acquire(dwMilliseconds);
            } else {
                status = semaphore_.try_acquire();
            }
        } else {
            status = semaphore_.untimed_acquire();
        }
        switch (status) {
        case ::xos::acquire_success:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::acquire_busy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::acquire_interrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    virtual BOOL CloseHandle() {
        BOOL success = (semaphore_.destroy())?(TRUE):(FALSE);
        return success;
    }
    virtual DWORD GetLastError(void) {
        DWORD dwLastError = 1;
        return dwLastError;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Semaphore* ToSemaphore() const {
        return ((Semaphore*)this);
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    ::xos::mt::os::semaphore semaphore_;
}; /// class _EXPORT_CLASS Semaphore

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#endif /// _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_SEMAPHORE_HPP 
