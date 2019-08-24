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
#if !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#define PTHREAD_CONDITION_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res)) {
        res->tv_sec = 0;
        res->tv_nsec = 0;
        return 0; 
    }
    return EINVAL; 
}
#endif /// !defined(CLOCK_REALTIME)
inline int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime) {
    return EINVAL; 
}
#define PTHREAD_CONDITION_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)

#if !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
inline int pthread_cond_timedwait_relative_np(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* reltime) {
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
    if ((cond) && (mutex) && (reltime)) {
        int err = 0; struct timespec untilTime;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            untilTime.tv_sec +=  reltime->tv_sec;
            untilTime.tv_nsec +=  reltime->tv_nsec;
            err = ::pthread_cond_timedwait(cond, mutex, &untilTime);
        }
        return err;
    }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
    return EINVAL; 
}
#define PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)

#if !defined(PTHREAD_CONDITION_HAS_TRYWAIT)
inline int pthread_cond_trywait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT) || defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
    if ((cond) && (mutex)) {
        int err = 0; struct timespec untilTime;
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        untilTime.tv_sec = 0;
        untilTime.tv_nsec = 0;
        err = ::pthread_cond_timedwait_relative_np(cond, mutex, &untilTime);
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            err = ::pthread_cond_timedwait(cond, mutex, &untilTime);
        }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        return err;
    }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT) || defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
    return EINVAL;
}
#define PTHREAD_CONDITION_HAS_TRYWAIT
#endif /// !defined(PTHREAD_CONDITION_HAS_TRYWAIT)

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

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
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
