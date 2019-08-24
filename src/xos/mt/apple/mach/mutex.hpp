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
///   File: mutex.hpp
///
/// Author: $author$
///   Date: 8/22/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_APPLE_MACH_MUTEX_HPP
#define _XOS_MT_APPLE_MACH_MUTEX_HPP

#include <mach/task.h>
#include <mach/mach.h>
#include <mach/lock_set.h>

#include "xos/mt/mutex.hpp"

#if !defined(HAS_LOCK_TIMED)
inline kern_return_t lock_timed
(lock_set_t lock_set, int lock_id, mach_timespec_t timeout) {
    kern_return_t err = KERN_SUCCESS;
    for (useconds_t time_useconds = 0,
         timeout_useconds = ::xos::seconds_useconds(timeout.tv_sec) + ::xos::nseconds_useconds(timeout.tv_nsec),
         useconds_sleep = ::xos::mseconds_useconds(10);
         timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
        if (KERN_SUCCESS == (err = ::lock_try(lock_set, lock_id))) {
            return err;
        }
        ::usleep(useconds_sleep);
    }
    return KERN_OPERATION_TIMED_OUT;
}
#define HAS_LOCK_TIMED
#endif /// !defined(HAS_LOCK_TIMED)

namespace xos {
namespace mt {
namespace apple {
namespace mach {

typedef lock_set_t mutex_t;
typedef mutex_t* mutex_attached_t;

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
    mutext(attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created, is_logged, is_err_logged) {
    }
    mutext(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        XOS_MT_MUTEX_CREATED();
    }
    mutext(bool is_logged): extends(is_logged) {
        XOS_MT_MUTEX_CREATED();
    }
    mutext(const mutext &copy): extends(copy) {
    }
    mutext() {
        XOS_MT_MUTEX_CREATED();
    }
    virtual ~mutext() {
        XOS_MT_MUTEX_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_attached() {
        task_t task = mach_task_self();
        mutex_t& mutex = mutex_;
        attached_t detached = 0;
        if ((detached = create_detached(task, mutex))) {
            this->attach(detached);
            return detached;
        }
        return 0;
    }
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->detach())) {
            task_t task = mach_task_self();
            return destroy_detached(task, *detached);
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return lock_detached(*detached);
        }
        return false;
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return unlock_detached(*detached);
        }
        return false;
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_lock_detached(*detached, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_lock_detached(*detached);
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_lock_detached(*detached);
        }
        return lock_failed;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool lock_detached(mutex_t& mutex) {
        lock_status status = lock_success;
        if (lock_success != (status = untimed_lock_detached(mutex))) {
            return false;
        }
        return true;
    }
    virtual bool unlock_detached(mutex_t& mutex) {
        bool success = true;
        lock_status status = lock_success;
        int lock_id = 1;
        kern_return_t err = KERN_SUCCESS;
        IS_ERR_LOGGED_DEBUG("::lock_release(mutex, lock_id)...");
        if (KERN_SUCCESS == (err = ::lock_release(mutex, lock_id))) {
            IS_ERR_LOGGED_DEBUG("...::lock_release(mutex, lock_id)");
            status = lock_success;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::lock_release(mutex, lock_id)");
            status = lock_failed;
        }
        return success;
    }
    virtual lock_status timed_lock_detached(mutex_t& mutex, mseconds_t milliseconds) {
        lock_status status = lock_success;
        int lock_id = 1;
        kern_return_t err = KERN_SUCCESS;
        mach_timespec_t wait_time;
        wait_time.tv_sec = mseconds_seconds(milliseconds);
        wait_time.tv_nsec = mseconds_nseconds(mseconds_mseconds(milliseconds));
        IS_ERR_LOGGED_TRACE("::lock_timed(mutex, lock_id, wait_time)...");
        if (KERN_SUCCESS == (err = ::lock_timed(mutex, lock_id, wait_time))) {
            IS_ERR_LOGGED_TRACE("...::lock_timed(mutex, lock_id, wait_time)");
            status = lock_success;
        } else {
            if (KERN_OPERATION_TIMED_OUT == (err)) {
                IS_ERR_LOGGED_TRACE("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::lock_timed(mutex, lock_id, wait_time)");
                status = lock_busy;
            } else {
                if (KERN_ABORTED == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::lock_timed(mutex, lock_id, wait_time)");
                    status = lock_interrupted;
                } else {
                    IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::lock_timed(mutex, lock_id, wait_time)");
                    status = lock_failed;
                }
            }
        }
        return status;
    }
    virtual lock_status try_lock_detached(mutex_t& mutex) {
        lock_status status = lock_success;
        int lock_id = 1;
        kern_return_t err = KERN_SUCCESS;
        IS_ERR_LOGGED_TRACE("::lock_try(mutex, lock_id)...");
        if (KERN_SUCCESS == (err = ::lock_try(mutex, lock_id))) {
            IS_ERR_LOGGED_TRACE("...::lock_try(mutex, lock_id)");
            status = lock_success;
        } else {
            if (KERN_OPERATION_TIMED_OUT == (err)) {
                IS_ERR_LOGGED_TRACE("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::lock_try(mutex, lock_id)");
                status = lock_busy;
            } else {
                if (KERN_ABORTED == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::lock_try(mutex, lock_id)");
                    status = lock_interrupted;
                } else {
                    IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::lock_try(mutex, lock_id)");
                    status = lock_failed;
                }
            }
        }
        return status;
    }
    virtual lock_status untimed_lock_detached(mutex_t& mutex) {
        lock_status status = lock_success;
        int lock_id = 1;
        kern_return_t err = KERN_SUCCESS;
        IS_ERR_LOGGED_DEBUG("::lock_acquire(mutex, lock_id)...");
        if (KERN_SUCCESS == (err = ::lock_acquire(mutex, lock_id))) {
            IS_ERR_LOGGED_DEBUG("...::lock_acquire(mutex, lock_id)");
            status = lock_success;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::lock_acquire(mutex, lock_id)");
            status = lock_failed;
        }
        return status;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(task_t& task, mutex_t& mutex) const {
        attached_t detached = &mutex;
        int locks = 1, policy = SYNC_POLICY_FIFO;
        kern_return_t err = KERN_SUCCESS;
        IS_ERR_LOGGED_DEBUG("::lock_set_create(task, &mutex, locks, policy)...");
        if (KERN_SUCCESS == (err = ::lock_set_create(task, &mutex, locks, policy))) {
            IS_ERR_LOGGED_DEBUG("...::lock_set_create(task, &mutex, locks, policy)");
            detached = &mutex;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::lock_set_create(task, &mutex, locks, policy)");
            detached = 0;
        }
        return detached;
    }
    virtual bool destroy_detached(task_t& task, mutex_t& mutex) const {
        bool success = true;
        kern_return_t err = KERN_SUCCESS;
        IS_ERR_LOGGED_DEBUG("::lock_set_destroy(task, mutex)...");
        if (KERN_SUCCESS == (err = ::lock_set_destroy(task, mutex))) {
            IS_ERR_LOGGED_DEBUG("...::lock_set_destroy(task, mutex)");
            success = true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::lock_set_destroy(task, mutex)");
            success = false;
        }
        return success;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    mutex_t mutex_;
}; /// class _EXPORT_CLASS mutext
typedef mutext<> mutex;

} /// namespace mach
} /// namespace apple
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_APPLE_MACH_MUTEX_HPP 
