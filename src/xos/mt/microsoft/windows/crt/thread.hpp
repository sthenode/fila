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
///   File: thread.hpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HPP
#define _XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HPP

#include "xos/mt/thread.hpp"
#include "xos/base/to_string.hpp"
#include "xos/io/logger.hpp"

#include <process.h>
#include <errno.h>

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {
namespace crt {

typedef HANDLE thread_attached_t;

typedef mt::thread threadt_implements;
typedef mt::extended::threadt<thread_attached_t> threadt_extends;
///////////////////////////////////////////////////////////////////////
///  Class: threadt
///////////////////////////////////////////////////////////////////////
template <class TImplements = threadt_implements, class TExtends = threadt_extends>
class _EXPORT_CLASS threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    threadt(attached_t attached, bool forked = false)
    : extends(attached, forked), thread_id_(0) {
    }
    threadt(const threadt& copy)
    : extends(copy), thread_id_(copy.thread_id_) {
    }
    threadt(bool is_logged = true, bool is_err_logged = true)
    : extends(is_logged, is_err_logged), thread_id_(0) {
        IS_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    virtual ~threadt() {
        IS_LOGGED_DEBUG("this->destroyed()...");
        if (!(this->destroyed())) {
            const create_exception e(destroy_failed);
            IS_LOGGED_ERROR("...failed on this->destroyed() throw (const create_exception e(destroy_failed))...");
            throw (e);
        }
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t detach() {
        thread_id_ = 0;
        return extends::detach();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(unsigned& thread_id) const {
        return create_detached(thread_id, false);
    }
    virtual attached_t create_detached(unsigned& thread_id, bool initially_suspended) const {
        attached_t detached = (attached_t)(unattached);
        unsigned (__stdcall*startAddress)(void*) = thread_proc;
        void* parameter = ((void*)this);
        unsigned initFlags = (initially_suspended)?(CREATE_SUSPENDED):(0);
        unsigned stackSize = 0;
        SECURITY_ATTRIBUTES securityAttributes;

        ::memset(&securityAttributes, 0, sizeof(securityAttributes));
        securityAttributes.nLength = sizeof(securityAttributes);

        IS_LOGGED_DEBUG("::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)...");
        if (((HANDLE)-1) != (detached = ((HANDLE)::_beginthreadex
            (&securityAttributes, stackSize, startAddress, 
             parameter, initFlags, &thread_id)))) {
            return detached;
        } else {
            int err = errno;
            switch(err) {
            case EACCES:
                IS_LOGGED_ERROR("...EACCES err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            case EAGAIN:
                IS_LOGGED_ERROR("...EAGAIN err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            case EINVAL:
                IS_LOGGED_ERROR("...EINVAL err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            default:
                IS_LOGGED_ERROR("...failed err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
            }
            detached = (attached_t)(unattached);
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((detached)) {
            IS_LOGGED_DEBUG("::CloseHandle(detached = " << pointer_to_string(detached) << ")...");
            if ((::CloseHandle(detached))) {
                return true;
            } else {
                DWORD dwError = ::GetLastError();
                IS_LOGGED_ERROR("...failed dwError = " << dwError << " on ::CloseHandle(detached = " << pointer_to_string(detached) << ")");
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool join_detached(bool& forked, attached_t detached) const {
        if (join_success == (untimed_join_detached(forked, detached))) {
            return true;
        }
        return false;
    }
    virtual join_status try_join_detached(bool& forked, attached_t detached) const {
        return timed_join_detached(forked, detached, 0);
    }
    virtual join_status untimed_join_detached(bool& forked, attached_t detached) const {
        if ((detached)) {
            DWORD dwMilliseconds = ((DWORD)INFINITE);
            DWORD dwResult = 0;

            IS_LOGGED_DEBUG("::WaitForSingleObject(detached, dwMilliseconds)...");
            if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                switch(dwResult) {
                case WAIT_TIMEOUT:
                    IS_LOGGED_ERROR("...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    return join_busy;
                case WAIT_ABANDONED:
                    IS_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    return join_interrupted;
                default:
                    IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    forked = false;
                    return join_failed;
                }
            } else {
                IS_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                forked = false;
                return join_success;
            }
        }
        return join_failed;
    }
    virtual join_status timed_join_detached
    (bool& forked, attached_t detached, mseconds_t milliseconds) const {
        if (0 > (milliseconds)) {
            return untimed_join_detached(forked, detached);
        } else {
            if ((detached)) {
                DWORD dwMilliseconds = ((DWORD)milliseconds);
                DWORD dwResult = 0;

                IS_LOGGED_TRACE("::WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                    switch(dwResult) {
                    case WAIT_TIMEOUT:
                        IS_LOGGED_TRACE("...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        return join_busy;
                    case WAIT_ABANDONED:
                        IS_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        return join_interrupted;
                    default:
                        IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        forked = false;
                        return join_failed;
                    }
                } else {
                    IS_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                    forked = false;
                    return join_success;
                }
            }
        }
        return join_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    virtual void run() {
        IS_LOGGED_DEBUG("...");
    }
    static unsigned __stdcall thread_proc(void* param) {
        unsigned result = 0;
        threadt* t = 0;
        if ((t = ((threadt*)param))) {
            IF_LOGGED_DEBUG(t->is_logged(), "t->ran_.run()...");
            t->ran_.run();
            IF_LOGGED_DEBUG(t->is_logged(), "...t->ran_.run()");
        } else {
            LOG_ERROR("...unexpected 0 = ((threadt*)param)");
        }
        return result;
    }
    virtual attached_t create_detached() const {
        return create_detached(thread_id());
    }
    virtual attached_t create_detached(bool initially_suspended) const {
        return create_detached(thread_id(), initially_suspended);
    }
    virtual unsigned& thread_id() const {
        return (unsigned&)thread_id_;
    }
    virtual mseconds_t trace_threashold_mseconds() const {
        return 500;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    unsigned thread_id_;
};
typedef threadt<> thread;

} /// namespace crt
} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HPP 
