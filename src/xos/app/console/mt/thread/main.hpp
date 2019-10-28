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
///   Date: 10/28/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_APP_CONSOLE_MT_THREAD_MAIN_HPP
#define XOS_APP_CONSOLE_MT_THREAD_MAIN_HPP

#include "xos/app/console/mt/thread/main_opt.hpp"
#include "xos/mt/os/sleep.hpp"
#include "xos/mt/thread.hpp"
#include "xos/mt/posix/thread.hpp"
#include "xos/mt/linux/thread.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace thread {

///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template 
<class TRan = xos::ran,
 class TVersion = xos::lib::fila::version, 
 class TExtends = main_optt<TVersion>, 
 class TImplements = typename TExtends::implements>

class _EXPORT_CLASS maint: virtual public TRan, virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef maint derives;

    typedef typename implements::file_t file_t;
    typedef typename implements::string_t string_t;
    typedef typename implements::char_t char_t;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    maint() {
    }
    virtual ~maint() {
    }
private:
    maint(const maint &copy) {
        throw exception(exception_unexpected);
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual void run() {
        mseconds_t sleep_timeout = this->sleep();
        if ((!sleep_timeout)) {
            this->outlln(__LOCATION__, "...", NULL);
        } else {
            this->outlln(__LOCATION__, "sleep(timeout = ", unsigned_to_string(sleep_timeout).chars(), ")...", NULL);
            ::xos::mt::os::sleep sleep(sleep_timeout);
            this->outlln(__LOCATION__, "...sleep(timeout = ", unsigned_to_string(sleep_timeout).chars(), ")", NULL);
        }
    }
    int run(::xos::joined& joined) {
        int err = 0;
        mseconds_t timeout = 0;
        bool untimed = this->infinite_timeout(timeout);
        this->outlln(__LOCATION__, "try {...", NULL);
        try {
            if ((untimed)) {
                this->outlln(__LOCATION__, "::xos::join join(joined)...", NULL);
                ::xos::join join(joined);
                this->outlln(__LOCATION__, "...::xos::join join(joined)", NULL);
            } else {
                if ((timeout)) {
                    this->outlln(__LOCATION__, "::xos::join join(joined, timeout = ", unsigned_to_string(timeout).chars(), ")...", NULL);
                    ::xos::join join(joined, timeout);
                    this->outlln(__LOCATION__, "...::xos::join join(joined = ", unsigned_to_string(timeout).chars(), ")", NULL);
                } else {
                    this->outlln(__LOCATION__, "::xos::try_join join(joined)...", NULL);
                    ::xos::try_join try_join(joined);
                    this->outlln(__LOCATION__, "...::xos::try_join join(joined)", NULL);
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
    template <class TThread>
    int run() {
        int err = 0;
        mseconds_t timeout = 0;
        bool untimed = this->infinite_timeout(timeout);
        this->outlln(__LOCATION__, "try {...", NULL);
        try {
            TThread thread(*this);
            run(thread);
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
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual int linux_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::linux::thread >();
    }
    virtual int posix_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::posix::thread >();
    }
    virtual int derived_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::derived::thread >();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
}; /// class _EXPORT_CLASS maint

typedef maint<> main;


} /// namespace thread
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// ndef XOS_APP_CONSOLE_MT_THREAD_MAIN_HPP
