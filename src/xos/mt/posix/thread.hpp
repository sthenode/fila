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
#ifndef _XOS_MT_POSIX_THREAD_HPP
#define _XOS_MT_POSIX_THREAD_HPP

#include "xos/mt/posix/timed.hpp"
#include "xos/mt/thread.hpp"
#include "xos/base/to_string.hpp"
#include "xos/io/logger.hpp"

#include <pthread.h>
#include <time.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_HAS_TRYJOIN)
#define PTHREAD_HAS_TRYJOIN
#endif /// !defined(PTHREAD_HAS_TRYJOIN)
#if !defined(PTHREAD_HAS_TIMEDJOIN)
#define PTHREAD_HAS_TIMEDJOIN
#endif /// !defined(PTHREAD_HAS_TIMEDJOIN)
#else /// defined(HAS_POSIX_TIMEOUTS)
///
/// pthread_tryjoin_np
///
#if !defined(PTHREAD_HAS_TRYJOIN)
#define PTHREAD_HAS_TRYJOIN
inline int pthread_tryjoin_np(pthread_t thread, void **retval) {
    return EINVAL;
}
#endif /// !defined(PTHREAD_HAS_TRYJOIN)
///
/// pthread_timedjoin_np
///
#if !defined(PTHREAD_HAS_TIMEDJOIN)
#define PTHREAD_HAS_TIMEDJOIN
inline int pthread_timedjoin_np(pthread_t thread, void **retval, const struct timespec *abstime) {
    return EINVAL;
}
#endif /// !defined(PTHREAD_HAS_TIMEDJOIN)
///
/// pthread_timedjoin_relative_np
///
#if !defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE)
#define PTHREAD_HAS_TIMEDJOIN_RELATIVE
inline int pthread_timedjoin_relative_np(pthread_t thread, void **retval, const struct timespec *reltime) {
#if defined(PTHREAD_HAS_TIMEDJOIN)
    if ((reltime)) {
        int err = 0; struct timespec until_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
            until_time.tv_sec +=  reltime->tv_sec;
            until_time.tv_nsec +=  reltime->tv_nsec;
            err = ::pthread_timedjoin_np(thread, retval, &until_time);
        }
        return err;
    }
#endif //// defined(PTHREAD_HAS_TIMEDJOIN)
    return EINVAL;
}
#endif /// !defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE)
#endif /// defined(HAS_POSIX_TIMEOUTS)

namespace xos {
namespace mt {
namespace posix {

typedef pthread_t* thread_attached_t;
///////////////////////////////////////////////////////////////////////
///  Class: threadt
///////////////////////////////////////////////////////////////////////
template 
<class TExtends = extended::threadt<thread_attached_t>, class TImplements = typename TExtends::implements>
class _EXPORT_CLASS threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;
    
    typedef typename implements::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    threadt(attached_t detached, bool is_created, bool is_logged, bool is_err_logged)
    : extends(detached, is_created, is_logged, is_err_logged) {
    }
    threadt(attached_t detached, bool is_created, bool is_logged)
    : extends(detached, is_created, is_logged) {
    }
    threadt(attached_t detached, bool is_created)
    : extends(detached, is_created) {
    }
    threadt(attached_t detached)
    : extends(detached) {
    }
    threadt(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    threadt(bool is_logged): extends(is_logged) {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    threadt(ran_t& ran): extends(ran) {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    threadt(const threadt &copy): extends(copy) {
    }
    threadt() {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    virtual ~threadt() {
        if (!(this->joined())) {
            throw join_exception(join_failed);
        }
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual join_status try_join_detached(bool& forked, attached_t detached) const {
#if defined(PTHREAD_HAS_TRYJOIN)
        if ((detached)) {
            void* value = 0;
            int err = 0;
            IS_LOGGED_TRACE("::pthread_tryjoin_np(*detached, &value)...");
            if (!(err =  ::pthread_tryjoin_np(*detached, &value))) {
                IS_LOGGED_TRACE("...::pthread_tryjoin_np(*detached, &value)");
                forked = false;
                return join_success;
            } else {
                switch(err) {
                case EBUSY:
                    IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return join_busy;
                case ETIMEDOUT:
                    IS_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return join_busy;
                case EINTR:
                    IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return join_interrupted;
                default:
                    IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return join_failed;
                }
            }        
        }
        return join_failed;
#else // defined(PTHREAD_HAS_TRYJOIN)
        IS_LOGGED_ERROR("...return join_invalid");
        return join_invalid;
#endif // defined(PTHREAD_HAS_TRYJOIN)
    }
    virtual join_status untimed_join_detached(bool& forked, attached_t detached) const {
        if ((detached)) {
            void* value = 0;
            int err = 0;
            IS_LOGGED_DEBUG("::pthread_join(*detached, &value)...");
            if (!(err = ::pthread_join(*detached, &value))) {
                IS_LOGGED_DEBUG("...::pthread_join(*detached, &value)");
                forked = false;
                return join_success;
            } else {
                switch(err) {
                case EBUSY:
                    IS_LOGGED_ERROR("...EBUSY err = " << err << " on ::pthread_join(*detached, &value)");
                    return join_busy;
                case ETIMEDOUT:
                    IS_LOGGED_ERROR("...ETIMEDOUT err = " << err << " on ::pthread_join(*detached, &value)");
                    return join_busy;
                case EINTR:
                    IS_LOGGED_ERROR("...EINTR err = " << err << " on ::pthread_join(*detached, &value)");
                    return join_interrupted;
                default:
                    IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_join(*detached, &value)");
                    forked = false;
                }
            }
        }
        return join_failed;
    }
    virtual join_status timed_join_detached
    (bool& forked, attached_t detached, mseconds_t milliseconds) const {
        if (0 < (milliseconds)) {
            if ((detached)) {
#if defined(PTHREAD_HAS_TIMEDJOIN)
            void* value = 0;
            int err = 0;
            struct timespec until_time;
            IS_LOGGED_TRACE("::clock_gettime(CLOCK_REALTIME, &until_time)...");
            if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
                IS_LOGGED_TRACE("...::clock_gettime(CLOCK_REALTIME, &until_time)");
                until_time.tv_sec +=  mseconds_seconds(milliseconds);
                until_time.tv_nsec +=  mseconds_nseconds(mseconds_mseconds(milliseconds));
            } else {
                IS_LOGGED_ERROR("...failed errno = " << errno << " on ::clock_gettime(CLOCK_REALTIME, &until_time)");
                return join_failed;
            }
            IS_LOGGED_TRACE("::pthread_timedjoin_np(*detached, &value, &until_time)...");
            if (!(err = ::pthread_timedjoin_np(*detached, &value, &until_time))) {
                IS_LOGGED_TRACE("...::pthread_timedjoin_np(*detached, &value, &until_time)");
                forked = false;
                return join_success;
            } else {
                switch(err) {
                case EBUSY:
                    IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                    return join_busy;
                case ETIMEDOUT:
                    IS_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                    return join_busy;
                case EINTR:
                    IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                    return join_interrupted;
                default:
                    IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                    return join_failed;
                }
            }
#else // defined(PTHREAD_HAS_TIMEDJOIN)
            IS_LOGGED_ERROR("...return join_invalid");
            return join_invalid;
#endif // defined(PTHREAD_HAS_TIMEDJOIN)
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_join_detached(forked, detached);
            } else {
                return try_join_detached(forked, detached);
            }
        }
        return join_failed;
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(bool initially_suspended) const {
        return create_detached(threadattr(), thread(), initially_suspended);
    }
    virtual attached_t create_detached() const {
        return create_detached(threadattr(), thread());
    }
    virtual attached_t create_detached
    (pthread_attr_t& threadattr, pthread_t& thread, bool initially_suspended) const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual attached_t create_detached
    (pthread_attr_t& threadattr, pthread_t& thread) const {
        attached_t detached = (attached_t)(unattached);
        int err = 0;

        IS_LOGGED_DEBUG("::pthread_attr_init(&threadattr)...");
        if (!(err = ::pthread_attr_init(&threadattr))) {

            IS_LOGGED_DEBUG("::pthread_create(&thread, &threadattr, start_routine, ((void*)this))...");
            if (!(err = ::pthread_create(&thread, &threadattr, start_routine, ((void*)this)))) {

                IS_LOGGED_DEBUG("::pthread_attr_destroy(&threadattr)...");
                if ((err = ::pthread_attr_destroy(&threadattr))) {
                    bool forked = true;
                    untimed_join_detached(forked, &thread);
                    return detached;
                }
                detached = &thread;
                return detached;
            }
            IS_LOGGED_DEBUG("::pthread_attr_destroy(&threadattr)...");
            if ((err = ::pthread_attr_destroy(&threadattr))) {
            }
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((detached)) {
            return true;
        }
        return false;
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual void run() {
        IS_LOGGED_DEBUG("...");
    }
    static void* start_routine(void* param) {
        void* result = 0;
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

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual pthread_t& thread() const {
        return ((pthread_t&)thread_);
    }
    virtual pthread_attr_t& threadattr() const {
        return ((pthread_attr_t&)threadattr_);
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    pthread_t thread_;
    pthread_attr_t threadattr_;
}; /// class _EXPORT_CLASS threadt
typedef threadt<> thread;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_THREAD_HPP 
