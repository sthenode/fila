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
///   File: condition.hpp
///
/// Author: $author$
///   Date: 8/19/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_CONDITION_HPP
#define _XOS_MT_POSIX_CONDITION_HPP

#include "xos/mt/posix/timed.hpp"
#include "xos/mt/posix/mutex.hpp"
#include "xos/mt/condition.hpp"

#include <pthread.h>
#include <time.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#define PTHREAD_COND_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if defined(MACOSX)
#if !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#define PTHREAD_COND_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#endif /// defined(MACOSX)

#if !defined(PTHREAD_COND_HAS_TIMEDWAIT)
inline int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abs_timeout) {
    if ((cond) && (mutex) && (abs_timeout)) {
        int err = 0; 
        struct timespec abs_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &abs_time))) {
            useconds_t time_useconds = ::xos::seconds_useconds(abs_time.tv_sec) + ::xos::nseconds_useconds(abs_time.tv_nsec),
                       timeout_useconds = ::xos::seconds_useconds(abs_timeout->tv_sec) + ::xos::nseconds_useconds(abs_timeout->tv_nsec);
            if (timeout_useconds > time_useconds) {
                int err = 0;            
                for (useconds_t useconds_sleep = ::xos::mseconds_useconds(10);
                     timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
#if defined(PTHREAD_COND_HAS_TRYWAIT)
                    err = ::pthread_cond_trywait(cond, mutex);
#else /// defined(PTHREAD_COND_HAS_TRYWAIT)
                    err = EAGAIN;
#endif /// defined(PTHREAD_COND_HAS_TRYWAIT)
                    if (EAGAIN != (err)) {
                        if (EBUSY != (err)) {
                            if (ETIMEDOUT != (err)) {
                                return err;
                            }
                        }
                    }
                    ::usleep(useconds_sleep);
                }
            }
            return ETIMEDOUT;
        }
    }
    return EINVAL; 
}
#define PTHREAD_COND_HAS_TIMEDWAIT
#if defined(PTHREAD_COND_HAS_TRYWAIT)
#define PTHREAD_COND_TIMEDWAIT_USES_TRYWAIT
#endif /// defined(PTHREAD_COND_HAS_TRYWAIT)
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT)

#if !defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
inline int pthread_cond_timedwait_relative_np(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* reltime) {
#if defined(PTHREAD_COND_HAS_TIMEDWAIT)
    if ((cond) && (mutex) && (reltime)) {
        int err = 0; 
        struct timespec until_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
            until_time.tv_sec +=  reltime->tv_sec;
            until_time.tv_nsec +=  reltime->tv_nsec;
            err = ::pthread_cond_timedwait(cond, mutex, &until_time);
        }
        return err;
    }
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT)
    return EINVAL; 
}
#define PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP
#if defined(PTHREAD_COND_TIMEDWAIT_USES_TRYWAIT)
#define PTHREAD_COND_TIMEDWAIT_RELATIVE_NP_USES_TRYWAIT
#endif /// defined(PTHREAD_COND_TIMEDWAIT_USES_TRYWAIT)
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)

#if !defined(PTHREAD_COND_HAS_TRYWAIT)
inline int pthread_cond_trywait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
#if !defined(PTHREAD_COND_TIMEDWAIT_USES_TRYWAIT) && !defined(PTHREAD_COND_TIMEDWAIT_RELATIVE_NP_USES_TRYWAIT)
#if defined(PTHREAD_COND_HAS_TIMEDWAIT) || defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
    if ((cond) && (mutex)) {
        int err = 0; 
        struct timespec until_time;
#if defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        until_time.tv_sec = 0;
        until_time.tv_nsec = 0;
        err = ::pthread_cond_timedwait_relative_np(cond, mutex, &until_time);
#else /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
            err = ::pthread_cond_timedwait(cond, mutex, &until_time);
        }
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        return err;
    }
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT) || defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
#endif /// !defined(PTHREAD_COND_TIMEDWAIT_USES_TRYWAIT) && !defined(PTHREAD_COND_TIMEDWAIT_RELATIVE_NP_USES_TRYWAIT)
    return EINVAL;
}
#define PTHREAD_COND_HAS_TRYWAIT
#endif /// !defined(PTHREAD_COND_HAS_TRYWAIT)

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
///  Class: conditiont
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pthread_cond_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TExtends = extended::conditiont<TAttached, TUnattached, VUnattached>, 
 class TImplements = typename TExtends::implements>

class _EXPORT_CLASS conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef conditiont derives;

    typedef posix::mutex mutex_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    typedef signal_status (derives::*wait_detached_t)(pthread_cond_t&, pthread_mutex_t&, mseconds_t) const;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created, is_logged, is_err_logged), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(detached, is_created), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(detached), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged), signaled_(&signaled), mutex_(mutex.attached_to()) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged): extends(is_logged), signaled_(&signaled), mutex_(mutex.attached_to()) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(mutex_t& mutex, bool& signaled): signaled_(&signaled), mutex_(mutex.attached_to()) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created, is_logged, is_err_logged), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached): extends(detached), signaled_(0), mutex_(0) {
    }
    conditiont(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged), signaled_(0), mutex_(0) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(bool is_logged): extends(is_logged), signaled_(0), mutex_(0) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(const conditiont &copy): extends(copy), signaled_(0), mutex_(0) {
    }
    conditiont(): signaled_(0), mutex_(0) {
        XOS_MT_CONDITION_CREATED();
    }
    virtual ~conditiont() {
        XOS_MT_CONDITION_DESTROYED();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_attached() {
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = create_detached(cond_))) {
            this->attach(detached);
        }
        return detached;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool broadcast() { 
        bool success = false;
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            bool* signaled = 0;
            if ((0 != (signaled = signaled_))) {
                pthread_mutex_t* mutex = 0;
                if ((0 != (mutex = mutex_))) {
                    success = broadcast_detached(*detached, *mutex, *signaled);
                } else {
                    IS_ERR_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
        }
        return success; 
    }
    virtual bool signal() { 
        bool success = false;
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            bool* signaled = 0;
            if ((0 != (signaled = signaled_))) {
                pthread_mutex_t* mutex = 0;
                if ((0 != (mutex = mutex_))) {
                    success = signal_detached(*detached, *mutex, *signaled);
                } else {
                    IS_ERR_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
        }
        return success; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool wait() {
        return (wait_success == (untimed_wait()));
    }
    virtual signal_status timed_wait(mseconds_t milliseconds) { 
        signal_status status = try_timed_untimed_wait(&derives::timed_wait_detached, milliseconds);
        return status; 
    }
    virtual signal_status try_wait() { 
        signal_status status = try_timed_untimed_wait(&derives::try_wait_detached, 0);
        return status; 
    }
    virtual signal_status untimed_wait() { 
        signal_status status = try_timed_untimed_wait(&derives::untimed_wait_detached, -1);
        return status; 
    }
    virtual signal_status try_timed_untimed_wait(wait_detached_t wait_detached, mseconds_t milliseconds) { 
        signal_status status = wait_failed;
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            bool* signaled = 0;
            if (((signaled = signaled_))) {
                pthread_mutex_t* mutex = 0;
                if (((mutex = mutex_))) {
                    status = try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, *signaled, milliseconds);
                } else {
                    IS_ERR_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
        }
        return status; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool wait(pthread_mutex_t& mutex, bool& signaled) {
        return (wait_success == (untimed_wait(mutex, signaled)));
    }
    virtual signal_status timed_wait(pthread_mutex_t& mutex, bool& signaled, mseconds_t milliseconds) { 
        signal_status status = try_timed_untimed_wait(&derives::timed_wait_detached, mutex, signaled, milliseconds);
        return status; 
    }
    virtual signal_status try_wait(pthread_mutex_t& mutex, bool& signaled) { 
        signal_status status = try_timed_untimed_wait(&derives::try_wait_detached, mutex, signaled, 0);
        return status; 
    }
    virtual signal_status untimed_wait(pthread_mutex_t& mutex, bool& signaled) { 
        signal_status status = try_timed_untimed_wait(&derives::untimed_wait_detached, mutex, signaled, -1);
        return status; 
    }
    virtual signal_status try_timed_untimed_wait(wait_detached_t wait_detached, pthread_mutex_t& mutex, bool& signaled, mseconds_t milliseconds) { 
        signal_status status = wait_failed;
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            status = try_timed_untimed_wait_detached(wait_detached, *detached, mutex, signaled, milliseconds);
        }
        return status; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual signal_status try_timed_untimed_wait_detached(wait_detached_t wait_detached, pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled, mseconds_t milliseconds) const { 
        signal_status status = wait_failed;
        if ((0 != (wait_detached))) {
            for (bool unsignaled = true; unsignaled; ) {
                try {
                    mutex_t locked(&mutex);
                    ::xos::lock lock(locked);
                    if (!(unsignaled = !signaled)) {
                        status = wait_success;
                    } else {
                        lock.set_unlocked();
                        if (wait_success != (status = (this->*wait_detached)(cond, mutex, milliseconds))) {
                            lock.set_unlocked(false);
                            unsignaled = false;
                        }
                    }
                } catch (...) {
                    IS_ERR_LOGGED_ERROR("...catch(...)");
                    unsignaled = false;
                }
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed 0 = (wait_Detached)");
        }
        return status; 
    }
    virtual signal_status timed_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const {
#if defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        int err = 0;
        bool debug = this->is_logged_debug(milliseconds), 
             logged = this->is_logged(), err_logged = this->is_err_logged();
        struct timespec until_time;

        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));

        IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)...");
        if ((err = ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time))) {
            switch(err) {
            case EBUSY:
                IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...EBUSY err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_busy;
            case ETIMEDOUT:
                IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...ETIMEDOUT err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_busy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_interrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_failed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
            return wait_success;
        }
        return wait_failed; 
#else /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        IS_ERR_LOGGED_ERROR("...return invalid...");
        return wait_invalid; 
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
    }
    virtual signal_status try_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const {
#if defined(PTHREAD_COND_HAS_TRYWAIT)
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_trywait(&cond, &mutex)...");
        if ((err = ::pthread_cond_trywait(&cond, &mutex))) {
            switch(err) {
            case EBUSY:
                IS_ERR_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_busy;
            case ETIMEDOUT:
                IS_ERR_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_busy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_interrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_failed;
            }
        } else {
            return wait_success;
        }
        return wait_failed; 
#else /// defined(PTHREAD_COND_HAS_TRYWAIT)
        IS_ERR_LOGGED_ERROR("...return invalid...");
        return wait_invalid; 
#endif /// defined(PTHREAD_COND_HAS_TRYWAIT)
    }
    virtual signal_status untimed_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const {
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_wait(&cond, &mutex)...");
        if ((err = ::pthread_cond_wait(&cond, &mutex))) {
            switch(err) {
            case EBUSY:
                IS_ERR_LOGGED_ERROR("...EBUSY err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_busy;
            case ETIMEDOUT:
                IS_ERR_LOGGED_ERROR("...ETIMEDOUT err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_busy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_interrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_failed;
            }
        } else {
            return wait_success;
        }
        return wait_failed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool broadcast_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled) const { 
        bool success = false;
        try {
            mutex_t locked(&mutex);
            ::xos::lock lock(locked);
            signaled = true;
        } catch (...) {
            IS_ERR_LOGGED_ERROR("...catch(...)");
            return false;
        }
        success = broadcast_detached(cond);
        return success;
    }
    virtual bool broadcast_detached(pthread_cond_t& cond) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_broadcast(&cond)...");
        if (!(err = ::pthread_cond_broadcast(&cond))) {
            IS_LOGGED_DEBUG("...::pthread_cond_broadcast(&cond)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_broadcast(&cond)");
        }
        return false; 
    }
    virtual bool signal_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled) const { 
        bool success = false;
        try {
            mutex_t locked(&mutex);
            ::xos::lock lock(locked);
            signaled = true;
        } catch (...) {
            IS_ERR_LOGGED_ERROR("...catch(...)");
            return false;
        }
        success = signal_detached(cond);
        return success;
    }
    virtual bool signal_detached(pthread_cond_t& cond) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_signal(&cond)...");
        if (!(err = ::pthread_cond_signal(&cond))) {
            IS_LOGGED_DEBUG("...::pthread_cond_signal(&cond)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_signal(&cond)");
        }
        return false; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(pthread_cond_t& cond) const {
        attached_t detached = (attached_t)(unattached);
        int err = 0;
        pthread_condattr_t condattr;

        IS_ERR_LOGGED_DEBUG("::pthread_condattr_init(&condattr)...");
        if (!(err = ::pthread_condattr_init(&condattr))) {
            IS_ERR_LOGGED_DEBUG("...::pthread_condattr_init(&condattr)");
            IS_ERR_LOGGED_DEBUG("::pthread_cond_init(&cond, &condattr)...");
            if (!(err = ::pthread_cond_init(&cond, &condattr))) {
                IS_ERR_LOGGED_DEBUG("...::pthread_cond_init(&cond, &condattr)");
                detached = &cond;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_init(&cond, &condattr)");
            }
            IS_ERR_LOGGED_DEBUG("::pthread_condattr_destroy(&condattr)...");
            if ((err = ::pthread_condattr_destroy(&condattr))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_condattr_destroy(&condattr)");
                detached = ((attached_t)unattached);
                IS_ERR_LOGGED_DEBUG("::pthread_cond_destroy(&cond)...");
                if ((err = ::pthread_cond_destroy(&cond))) {
                    IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_destroy(&cond)");
                } else {
                    IS_ERR_LOGGED_DEBUG("...::pthread_cond_destroy(&cond)");
                }
            } else {
                IS_ERR_LOGGED_DEBUG("...::pthread_condattr_destroy(&condattr)");
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_condattr_init(&condattr)");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) {
        if ((attached_t)(unattached) != (detached)) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::pthread_cond_destroy(detached)...");
            if (((err = ::pthread_cond_destroy(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " ::pthread_cond_destroy(detached)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::pthread_cond_destroy(detached)");
                return true;
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    bool* signaled_;
    pthread_mutex_t* mutex_;
    pthread_cond_t cond_;
}; /// class _EXPORT_CLASS conditiont
typedef conditiont<> condition;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_CONDITION_HPP 
