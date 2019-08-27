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
///   File: main.hpp
///
/// Author: $author$
///   Date: 8/19/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_CONDITION_MAIN_HPP
#define _XOS_APP_CONSOLE_MT_CONDITION_MAIN_HPP

#include "xos/app/console/mt/condition/main_opt.hpp"
#include "xos/mt/condition.hpp"
#include "xos/mt/posix/condition.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace condition {

///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = main_opt::implements, class TExtends = main_opt>

class _EXPORT_CLASS maint: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef maint derives;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    maint() {
    }
    virtual ~maint() {
    }
private:
    maint(const maint &copy) {
    }
    
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    int run(::xos::signaled& signaled) {
        int err = 0;
        mseconds_t timeout = 0;
        bool untimed = this->infinite_timeout(timeout);
        for (unsigned tries = 1, tried = 0; tried < tries; ++tried) {
            this->outlln(__LOCATION__, "try {...", NULL);
            try {
                if ((untimed)) {
                    this->outlln(__LOCATION__, "signaled.signal()...", NULL);
                    signaled.signal();
                    this->outlln(__LOCATION__, "wait wait(signaled)...", NULL);
                    ::xos::wait wait(signaled);
                    this->outlln(__LOCATION__, "...wait wait(signaled)", NULL);
                } else {
                    if ((timeout)) {
                        this->outlln(__LOCATION__, "timed_wait wait(signaled, timeout)...", NULL);
                        ::xos::wait wait(signaled, timeout);
                        this->outlln(__LOCATION__, "...timed_wait wait(signaled, timeout)", NULL);
                    } else {
                        this->outlln(__LOCATION__, "try_wait wait(signaled)...", NULL);
                        ::xos::try_wait wait(signaled);
                        this->outlln(__LOCATION__, "...try_wait wait(signaled)", NULL);
                    }
                }
                this->outlln(__LOCATION__, "...} try", NULL);
            } catch(const wait_exception& e) {
                this->outlln(__LOCATION__, "...catch (const wait_exception& e status = ", e.status_to_string().chars(), ")", NULL);
                if (wait_busy == (e.status())) {
                    this->outlln(__LOCATION__, "signaled.signal()...", NULL);
                    signaled.signal();
                    tries = 2;
                } else {
                    err = 1;
                }
            } catch(const exception& e) {
                this->outlln(__LOCATION__, "...catch (const exception& e status = ", e.status_to_string().chars(), ")", NULL);
                err = 1;
            } catch(...) {
                this->outlln(__LOCATION__, "...catch (...)", NULL);
                err = 1;
            }
        }
        return err;
    }
    template <class TCondition, class TMutex>
    int run() {
        int err = 0;
        this->outlln(__LOCATION__, "try {...", NULL);
        try {
            bool signaled = false;
            TMutex mutex;
            TCondition condition(mutex, signaled);
            err = run(condition);
            this->outlln(__LOCATION__, "...} try", NULL);
        } catch(const exception& e) {
            this->outlln(__LOCATION__, "...catch (const exception& e status = ", e.status_to_string().chars(), ")", NULL);
            err = 1;
        } catch (...) {
            this->outlln(__LOCATION__, "...catch (...)", NULL);
            err = 1;
        }
        return err;
    }

    virtual int posix_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::posix::condition, ::xos::mt::posix::mutex >();
    }
    virtual int derived_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::derived::condition, ::xos::mt::derived::mutex >();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS maint
typedef maint<> main;

} /// namespace condition
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_CONDITION_MAIN_HPP 
