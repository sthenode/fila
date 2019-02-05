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
///   File: mutex.hpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP
#define _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP

#include "xos/mt/mutex.hpp"
#include "xos/base/to_string.hpp"
#include "xos/base/created.hpp"
#include "xos/io/logger.hpp"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

typedef HANDLE mutex_attached_t;

typedef mt::mutex mutext_implements;
typedef mt::extended::mutext<mutex_attached_t> mutext_extends;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template <class TImplements = mutext_implements, class TExtends = mutext_extends>
class _EXPORT_CLASS mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    mutext(bool is_logged): extends(is_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    mutext(const mutext &copy): extends(copy) {
    }
    mutext() {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    virtual ~mutext() {
        IS_ERR_LOGGED_DEBUG("this->destroyed()...");
        if (!(this->destroyed())) {
            const create_exception e(destroy_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->destroyed() throw (const create_exception e(destroy_failed))...");
            throw (e);
        }
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool create() {
        if ((this->destroyed())) {
            attached_t detached = 0;
            if ((detached = this->create_detached())) {
                this->attach_created(detached);
                return true;
            }
        }
        return false;
    }
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->detach())) {
            if ((this->destroy_detached(detached))) {
                return true;
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool lock() {
        return lock_success == untimed_lock();
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            IS_LOGGING_DEBUG("::ReleaseMutex(detached)...")
            if ((::ReleaseMutex(detached))) {
                return true;
            } else {
                int error = ::GetLastError();
                IS_LOGGING_ERROR("...failed error = " << error << " on ::ReleaseMutex(detached)");
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual lock_status try_lock() {
        return timed_lock(0);
    }
    virtual lock_status untimed_lock() {
        return timed_lock(-1);
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            DWORD dwMilliseconds = (0 > milliseconds)?(INFINITE):((DWORD)milliseconds);
            DWORD dwResult = 0;
            if ((0 < milliseconds) && ((this->trace_threashold_mseconds()) > milliseconds)) {
                IS_LOGGING_TRACE("::WaitForSingleObject(detached, dwMilliseconds)...");
            } else {
                IS_LOGGING_DEBUG("::WaitForSingleObject(detached, dwMilliseconds)...");
            }
            if ((WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds)))) {
                if ((0 < milliseconds) && ((this->trace_threashold_mseconds()) > milliseconds)) {
                    IS_LOGGING_TRACE("...::WaitForSingleObject(detached, dwMilliseconds)");
                } else {
                    IS_LOGGING_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                }
                return lock_success;
            } else {
                switch (dwResult) {
                case WAIT_TIMEOUT:
                    if ((0 < milliseconds) && ((this->trace_threashold_mseconds()) > milliseconds)) {
                        IS_LOGGING_TRACE("...failed WAIT_TIMEOUT on ::WaitForSingleObject(detached, dwMilliseconds)");
                    } else {
                        IS_LOGGING_DEBUG("...failed WAIT_TIMEOUT on ::WaitForSingleObject(detached, dwMilliseconds)");
                    }
                    return lock_busy;
                case WAIT_ABANDONED:
                    IS_LOGGING_ERROR("...failed WAIT_ABANDONED on ::WaitForSingleObject(detached, dwMilliseconds)");
                    return lock_interrupted;
                default:
                    IS_LOGGING_ERROR("...failed on ::WaitForSingleObject(detached, dwMilliseconds)");
                }
            }
        }
        return lock_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached() const {
        LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL;
        BOOL bInitialOwner = FALSE;
        LPCTSTR lpName = NULL;
        attached_t detached = 0;
        IS_LOGGING_DEBUG("::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)...");
        if ((detached = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName))) {
            return detached;
        } else {
            int error = GetLastError();
            IS_LOGGING_ERROR("...failed error = " << error << " on ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)");
        }
        return 0;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((detached)) {
            bool success = true;
            IS_LOGGING_DEBUG("::CloseHandle(detached = " << pointer_to_string(detached) << ")...");
            if (!(::CloseHandle(detached))) {
                int error = ::GetLastError();
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
typedef mutext<> mutex;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP 
