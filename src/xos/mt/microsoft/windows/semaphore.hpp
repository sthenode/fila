///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2018 $organization$
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
///   File: semaphore.hpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HPP
#define _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HPP

#include "xos/mt/semaphore.hpp"
#include "xos/base/created.hpp"
#include "xos/base/to_string.hpp"
#include "xos/io/logger.hpp"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

typedef HANDLE semaphore_attached_t;

typedef mt::semaphore semaphoret_implements;
typedef mt::extended::semaphoret<semaphore_attached_t> semaphoret_extends;
///////////////////////////////////////////////////////////////////////
///  Class: semaphoret
///////////////////////////////////////////////////////////////////////
template <class TImplements = semaphoret_implements, class TExtends = semaphoret_extends>
class _EXPORT_CLASS semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    semaphoret(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    semaphoret(attached_t detached): extends(detached) {
    }
    semaphoret(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        XOS_MT_SEMAPHORE_CREATED();
    }
    semaphoret(bool is_logged): extends(is_logged) {
        XOS_MT_SEMAPHORE_CREATED();
    }
    semaphoret(const semaphoret &copy): extends(copy) {
    }
    semaphoret() {
        XOS_MT_SEMAPHORE_CREATED();
    }
    virtual ~semaphoret() {
        XOS_MT_SEMAPHORE_DESTROYED();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool create(size_t initially_released) {
        attached_t detached = 0;
        if ((detached = create_attached(initially_released))) {
            this->set_is_created();
            return detached;
        }
        return false;
    }
    virtual bool create() {
        attached_t detached = 0;
        if ((detached = create_attached())) {
            this->set_is_created();
            return detached;
        }
        return false;
    }
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->detach())) {
            if ((destroy_detached(detached))) {
                return true;
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool acquire() {
        return acquire_success == untimed_acquire();
    }
    virtual bool release() {
        return release(1);
    }
    virtual bool release(size_t amount) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            LONG lReleaseCount = ((LONG)amount);
            LONG lPreviousCount = 0;
            IS_LOGGING_DEBUG("::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)...");
            if ((::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount))) {
                return true;
            } else {
                int error = GetLastError();
                IS_LOGGING_ERROR("...failed error = " << error << " on ::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)");
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual acquire_status try_acquire() {
        return timed_acquire(0);
    }
    virtual acquire_status untimed_acquire() {
        return timed_acquire(-1);
    }
    virtual acquire_status timed_acquire(mseconds_t milliseconds) {
        attached_t detached = 0;

        if ((detached = this->attached_to())) {
            bool debug = this->is_logged_debug(milliseconds), logged = this->is_logged(), err_logged = this->is_err_logged();
            DWORD dwMilliseconds = (0 > milliseconds)?(INFINITE):((DWORD)milliseconds);
            DWORD dwResult = 0;

            IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")...");
            if ((WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds)))) {
                IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                return acquire_success;
            } else {
                switch (dwResult) {
                case WAIT_TIMEOUT:
                    IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...failed WAIT_TIMEOUT on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_busy;
                case WAIT_ABANDONED:
                    IS_LOGGING_ERROR("...failed WAIT_ABANDONED on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_interrupted;
                default:
                    IS_LOGGING_ERROR("...failed on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                }
            }
        }
        return acquire_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_attached(size_t initially_released) {
        attached_t detached = 0;
        if ((this->destroyed())) {
            if ((detached = create_detached(initially_released))) {
                this->attach(detached);
                return detached;
            }
        }
        return 0;
    }
    virtual attached_t create_attached() {
        attached_t detached = 0;
        if ((this->destroyed())) {
            if ((detached = create_detached())) {
                this->attach(detached);
                return detached;
            }
        }
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached() const {
        return create_detached(0, -1);
    }
    virtual attached_t create_detached(size_t initial_count) const {
        return create_detached(initial_count, -1);
    }
    virtual attached_t create_detached(size_t initial_count, ssize_t maximum_count) const {
        LONG lInitialCount = ((LONG)initial_count);
        LONG lMaximumCount = ((LONG)(0 > maximum_count)?(((ULONG)(-1))>>1):(maximum_count));
        LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0;
        LPCTSTR lpName = 0;
        attached_t detached = 0;

        IS_LOGGING_DEBUG("::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)...");
        if ((detached = ::CreateSemaphore
            (lpSecurityAttributes, lInitialCount, lMaximumCount, lpName))) {
            return detached;
        } else {
            int error = GetLastError();
            IS_LOGGING_ERROR("...failed error = " << error << " on ::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)");
        }
        return 0;
    }
    virtual bool destroy_detached(HANDLE detached) const {
        if ((detached)) {
            bool success = true;
            IS_LOGGING_DEBUG("::CloseHandle(detached = " << pointer_to_string(detached) << ")...");
            if (!(CloseHandle(detached))) {
                int error = GetLastError();
                IS_LOGGING_ERROR("...failed error = " << error << " on ::CloseHandle(detached = " << pointer_to_string(detached) << ")");
                success = false;
            }
            return success;
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual mseconds_t trace_threashold_mseconds() const {
        return 500;
    }
};
typedef semaphoret<> semaphore;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HPP 
        

