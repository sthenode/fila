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
///   File: main.hpp
///
/// Author: $author$
///   Date: 7/14/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_FILA_MAIN_HPP
#define _XOS_APP_CONSOLE_FILA_MAIN_HPP

#include "xos/mt/std/queue.hpp"
#include "xos/mt/os/sleep.hpp"
#include "xos/mt/os/mutex.hpp"
#include "xos/mt/os/semaphore.hpp"
#include "xos/mt/os/thread.hpp"
#include "xos/console/getopt/main.hpp"
#include "xos/console/lib/version/main.hpp"
#include "xos/lib/fila/version.hpp"

namespace xos {
namespace app {
namespace console {
namespace fila {

///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = xos::console::lib::version::maint_implements, 
 class TExtends = xos::console::lib::version::maint<xos::lib::fila::version> >
class _EXPORT_CLASS maint: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef maint derives;

    typedef typename extends::char_t char_t;
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    maint(): run_(0) {
    }
    virtual ~maint() {
    }
private:
    maint(const maint &copy) {
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    friend class thread_t;
    typedef xos::lock lock_t;
    typedef mt::std::queuet<unsigned> queue_t;
    typedef mt::os::mutex mutex_t;
    typedef mt::os::sleep sleep_t;
    class thread_t: public mt::os::thread {
    public:
        typedef mt::os::thread extends;
        thread_t(maint& main, mutex_t& mutex, queue_t& queue, unsigned& enqueue, mseconds_t sleep = 5000)
        : extends(false, false), main_(main), mutex_(mutex), queue_(queue), enqueue_(enqueue), sleep_(sleep) {
        }
        virtual void run() {
            try {
                lock_t lock(mutex_);
                main_.outlln("sleep_t sleep(sleep_ = ", signed_to_string(sleep_).chars(), ")...", NULL);
                main_.out_flush();
            } catch (...) {}
            sleep_t sleep(sleep_);
            try {
                lock_t lock(mutex_);
                main_.outlln("queue_.enqueue(enqueue_ = ", unsigned_to_string(enqueue_).chars(), ")...", NULL);
                main_.out_flush();
            } catch (...) {}
            if (queue_.enqueue(enqueue_)) {
            try {
                lock_t lock(mutex_);
                main_.outlln("...queue_.enqueue(enqueue_ = ", unsigned_to_string(enqueue_).chars(), ")", NULL);
                main_.out_flush();
            } catch (...) {}
            }
        }
    protected:
        maint& main_;
        mutex_t& mutex_;
        queue_t& queue_;
        unsigned enqueue_;
        mseconds_t sleep_;
    };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    int (derives::*run_)(int argc, char_t** argv, char_t** env);
    virtual int run(int argc, char_t** argv, char_t** env) {
        if ((run_)) {
            return (this->*run_)(argc, argv, env);
        }
        return default_run(argc, argv, env);
    }
    virtual int default_run(int argc, char_t** argv, char_t** env) {
        unsigned enqueue = 123, dequeue = 0;
        queue_t queue(false, false);
        mutex_t mutex(false, false);
        thread_t thread(*this, mutex, queue, enqueue);
        int err = 0;

        try {
            lock_t lock(mutex);
            err = extends::run(argc, argv, env);
        } catch (...) {}
        try {
            lock_t lock(mutex);
            this->outlln("queue.dequeue(dequeue = ", unsigned_to_string(dequeue).chars(), ")...", NULL);
            this->out_flush();
        } catch (...) {}
        if ((queue.dequeue(dequeue))) {
            try {
                lock_t lock(mutex);
                this->outlln("...queue.dequeue(dequeue = ", unsigned_to_string(dequeue).chars(), ")", NULL);
                this->out_flush();
            } catch (...) {}
        }
        return err;
    }
    virtual int this_version_run(int argc, char_t** argv, char_t** env) {
        int err = 0;
        err = this->version_run(argc, argv, env);
        return err;
    }
    virtual int set_version_run(int argc, char_t**argv, char_t**env) {
        int err = 0;
        run_ = &derives::this_version_run;
        return err;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
};
typedef maint<> main;

} /// namespace fila
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_FILA_MAIN_HPP 
