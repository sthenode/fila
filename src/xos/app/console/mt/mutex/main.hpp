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
///   Date: 8/20/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_MUTEX_MAIN_HPP
#define _XOS_APP_CONSOLE_MT_MUTEX_MAIN_HPP

#include "xos/app/console/mt/mutex/main_opt.hpp"
#include "xos/mt/mutex.hpp"
#include "xos/mt/posix/mutex.hpp"
#include "xos/mt/apple/mach/mutex.hpp"
#include "xos/mt/apple/osx/mutex.hpp"
#include "xos/mt/microsoft/windows/mutex.hpp"
#include "xos/mt/os/mutex.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace mutex {

typedef main_opt::implements maint_implements;
typedef main_opt maint_extends;
///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template <class TImplements = maint_implements, class TExtends = maint_extends>
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
    int run(::xos::locked& locked) {
        int err = 0;
        mseconds_t timeout = 0;
        bool untimed = this->infinite_timeout(timeout);
        this->outlln(__LOCATION__, "try {...", NULL);
        try {
            if (untimed) {
                this->outlln(__LOCATION__, "::xos::lock lock(locked)...", NULL);
                ::xos::lock lock(locked);
            } else {
                if (0 < timeout) {
                    this->outlln(__LOCATION__, "::xos::lock lock(locked, timeout = ", unsigned_to_string(timeout).chars(), ")...", NULL);
                    ::xos::lock lock(locked, timeout);
                } else {
                    this->outlln(__LOCATION__, "::xos::try_lock try_lock(locked)...", NULL);
                    ::xos::try_lock try_lock(locked);
                }
            }
            this->outlln(__LOCATION__, "...} try", NULL);
        } catch (const exception& e) {
            this->outlln(__LOCATION__, "...catch (const exception& e.status = \"", e.status_to_string().chars(), "\")", NULL);
            err = 1;
        } catch (...) {
            this->outlln(__LOCATION__, "...catch (...)", NULL);
            err = 1;
        }
        return err;
    }
    template <class TMutex>
    int run() {
        int err = 0;
        mseconds_t timeout = 0;
        bool untimed = this->infinite_timeout(timeout);
        this->outlln(__LOCATION__, "try {...", NULL);
        try {
            this->outlln(__LOCATION__, "TMutex mutex...", NULL);
            TMutex mutex;
            if (untimed) {
                err = run(mutex);
            } else {
                try {
                    this->outlln(__LOCATION__, "::xos::lock lock(mutex)...", NULL);
                    ::xos::lock lock(mutex);
                    err = run(mutex);
                    this->outlln(__LOCATION__, "...} try", NULL);
                } catch (const exception& e) {
                    this->outlln(__LOCATION__, "...catch (const exception& e.status = \"", e.status_to_string().chars(), "\")", NULL);
                    err = 1;
                } catch (...) {
                    this->outlln(__LOCATION__, "...catch (...)", NULL);
                    err = 1;
                }
            }
            this->outlln(__LOCATION__, "...} try", NULL);
        } catch (const exception& e) {
            this->outlln(__LOCATION__, "...catch (const exception& e.status = \"", e.status_to_string().chars(), "\")", NULL);
            err = 1;
        } catch (...) {
            this->outlln(__LOCATION__, "...catch (...)", NULL);
            err = 1;
        }
        return err;
    }

    virtual int windows_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::microsoft::windows::mutex >();
    }
    virtual int osx_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::apple::osx::mutex >();
    }
    virtual int posix_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::posix::mutex >();
    }
    virtual int os_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::os::mutex >();
    }
    virtual int derived_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::derived::mutex >();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS maint
typedef maint<> main;

} /// namespace mutex
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_MUTEX_MAIN_HPP 
