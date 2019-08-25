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
#ifndef _XOS_MT_POSIX_SEMAPHORE_HPP
#define _XOS_MT_POSIX_SEMAPHORE_HPP

#include "xos/mt/semaphore.hpp"

#include <semaphore.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#if defined(SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// defined(SEM_HAS_TIMEDWAIT)
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#if defined(SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// defined(SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#if !defined(CLOCK_HAS_GETTIME)
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res)) {
        res->tv_sec = 0;
        res->tv_nsec = 0;
        return 0; 
    }
    return EINVAL; 
}
#define CLOCK_HAS_GETTIME
#endif /// !defined(CLOCK_HAS_GETTIME)
#endif /// !defined(CLOCK_REALTIME)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
inline int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    if ((sem) && (abs_timeout)) {
        int err = 0; 
        struct timespec abs_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &abs_time))) {
            useconds_t time_useconds = ::xos::seconds_useconds(abs_time.tv_sec) + ::xos::nseconds_useconds(abs_time.tv_nsec),
                       timeout_useconds = ::xos::seconds_useconds(abs_timeout->tv_sec) + ::xos::nseconds_useconds(abs_timeout->tv_nsec);
            if (timeout_useconds > time_useconds) {
                int err = 0;            
                for (useconds_t useconds_sleep = ::xos::mseconds_useconds(10);
                     timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
                    err = ::sem_trywait(sem);
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
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
inline int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout) {
#if defined(POSIX_SEM_HAS_TIMEDWAIT)
    if ((sem) && (timeout)) {
        int err = 0; struct timespec until_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
            until_time.tv_sec +=  timeout->tv_sec;
            until_time.tv_nsec +=  timeout->tv_nsec;
            err = ::_sem_timedwait(sem, &until_time);
        }
        return err;
    }
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT)
    return EINVAL;
}
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
/// Enum: sem_pshared_t
///////////////////////////////////////////////////////////////////////
typedef int sem_pshared_t;
enum {
    SEMAPHORE_PROCESS_PRIVATE = 0,
    SEMAPHORE_PROCESS_SHARED  = 1
};

///////////////////////////////////////////////////////////////////////
///  Class: semaphoret
///////////////////////////////////////////////////////////////////////
template 
<class TAttached = sem_t*,
 class TExtends = mt::extended::semaphoret<TAttached>, 
 class TImplements = typename TExtends::implements>

class _EXPORT_CLASS semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    semaphoret(attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created, is_logged, is_err_logged) {
    }
    semaphoret(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
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
    virtual attached_t create_attached() {
        return create_attached(0);
    }
    virtual attached_t create_attached(size_t initially_released) {
        attached_t detached = ((attached_t)unattached);
        if ((detached = create_detached(sem_, initially_released))) {
            this->attach(detached);
        }
        return detached;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool acquire() { 
        if (acquire_success == (untimed_acquire())) {
            return true;
        }
        return false; 
    }
    virtual bool release() { 
        return release_detached(this->attached_to()); 
    }
    virtual acquire_status timed_acquire(mseconds_t milliseconds) { 
        return timed_acquire_detached(this->attached_to(), milliseconds); 
    }
    virtual acquire_status try_acquire() { 
        return try_acquire_detached(this->attached_to()); 
    }
    virtual acquire_status untimed_acquire() { 
        return untimed_acquire_detached(this->attached_to()); 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual acquire_status timed_acquire_detached(attached_t detached, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
#if defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
            if (((attached_t)unattached) != detached) {
                int err = 0;
                bool debug = this->is_logged_debug(milliseconds), 
                     logged = this->is_logged(), err_logged = this->is_err_logged();
                struct timespec until_time;

                until_time.tv_sec =  mseconds_seconds(milliseconds);
                until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));

                IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "::sem_timedwait_relative_np(detached, &until_time)...");
                if (!(err = ::sem_timedwait_relative_np(detached, &until_time))) {
                    IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...::sem_timedwait_relative_np(detached, &until_time)...");
                    return acquire_success;
                } else {
                    err = errno;
                    if (EAGAIN == (err)) {
                        IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...failed EAGAIN err = " << err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                        return acquire_busy;
                    } else {
                        if (EBUSY == (err)) {
                            IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...failed EBUSY err = " << err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                            return acquire_busy;
                        } else {
                            if (ETIMEDOUT == (err)) {
                                IF_ERR_LOGGED_DEBUG_TRACE(debug, logged, err_logged, "...failed ETIMEDOUT err = " << err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                                return acquire_busy;
                            } else {
                                if (EINTR == (err)) {
                                    IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                                    return acquire_interrupted;
                                } else {
                                    IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                                }
                            }
                        }
                    }
                }
            }
#else /// defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
            IS_ERR_LOGGED_ERROR("...return acquire_invalid");
            return acquire_invalid;
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
        } else {
            if (0 > (milliseconds)) {
                return untimed_acquire_detached(detached);
            } else {
                return try_acquire_detached(detached);
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status try_acquire_detached(attached_t detached) const { 
        if (((attached_t)unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_TRACE("::sem_trywait(detached)...");
            if (!(err = ::sem_trywait(detached))) {
                IS_ERR_LOGGED_TRACE("...::sem_trywait(detached)");
                return acquire_success;
            } else {
                err = errno;
                if (EAGAIN == (err)) {
                    IS_ERR_LOGGED_TRACE("...failed EAGAIN err = " << err << " on ::sem_trywait(detached)");
                    return acquire_busy;
                } else {
                    if (EBUSY == (err)) {
                        IS_ERR_LOGGED_TRACE("...failed EBUSY err = " << err << " on ::sem_trywait(detached)");
                        return acquire_busy;
                    } else {
                        if (ETIMEDOUT == (err)) {
                            IS_ERR_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on ::sem_trywait(detached)");
                            return acquire_busy;
                        } else {
                            if (EINTR == (err)) {
                                IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_trywait(detached)");
                                return acquire_interrupted;
                            } else {
                                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_trywait(detached)");
                            }
                        }
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status untimed_acquire_detached(attached_t detached) const { 
        if (((attached_t)unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sem_wait(detached)...");
            if (!(err = ::sem_wait(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sem_wait(detached)");
                return acquire_success;
            } else {
                err = errno;
                if (EAGAIN == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed EAGAIN err = " << err << " on ::sem_wait(detached)");
                    return acquire_failed;
                } else {
                    if (EBUSY == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed EBUSY err = " << err << " on ::sem_wait(detached)");
                        return acquire_failed;
                    } else {
                        if (ETIMEDOUT == (err)) {
                            IS_ERR_LOGGED_ERROR("...failed ETIMEDOUT err = " << err << " on ::sem_wait(detached)");
                            return acquire_failed;
                        } else {
                            if (EINTR == (err)) {
                                IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_wait(detached)");
                                return acquire_interrupted;
                            } else {
                                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_wait(detached)");
                            }
                        }
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual bool release_detached(attached_t detached) const { 
        if (((attached_t)unattached) != detached) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::sem_post(detached)...");
            if (!(err = ::sem_post(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sem_post(detached)");
                return true;
            } else {
                err = errno;
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_signal(detached)");
            }
        }
        return false; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(sem_t& sem) const {
        size_t initially_released = 0;
        return create_detached(sem, initially_released);
    }
    virtual attached_t create_detached(sem_t& sem, size_t initially_released) const {
        sem_pshared_t pshared = SEMAPHORE_PROCESS_PRIVATE;
        return create_detached(sem, pshared, initially_released);
    }
    virtual attached_t create_detached(sem_t& sem, sem_pshared_t pshared, size_t initially_released) const {
        attached_t detached = ((attached_t)unattached);
        int err = 0;
        IS_ERR_LOGGED_DEBUG("::sem_init(&sem, pshared, initially_released)...");
        if ((!(err = ::sem_init(&sem, pshared, initially_released)))) {
            IS_ERR_LOGGED_DEBUG("...::sem_init(&sem, pshared, initially_released)");
            return &sem;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_init(&sem, pshared, initially_released)");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if (((attached_t)unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sem_destroy(detached)...");
            if (!(err = ::sem_destroy(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sem_destroy(detached)");
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_destroy(detached)");
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    sem_t sem_;
};
typedef semaphoret<> semaphore;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_SEMAPHORE_HPP 
