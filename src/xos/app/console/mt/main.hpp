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
///   Date: 8/21/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_MAIN_HPP
#define _XOS_APP_CONSOLE_MT_MAIN_HPP

#include "xos/app/console/mt/main_opt.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {

typedef main_opt maint_extends;
typedef maint_extends::implements maint_implements;
///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template <class TImplements = maint_implements, class TExtends = maint_extends>
class _EXPORT_CLASS maint: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename implements::string_t string_t;
    typedef typename implements::char_t char_t;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    maint(): sleep_((mseconds_t)-1), timeout_((mseconds_t)-1) {
    }
    virtual ~maint() {
    }
private:
    maint(const maint &copy) {
    }
    
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual int on_sleep_option
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        if ((optarg) && (optarg[0])) {
            string_t opt(optarg);
            unsigned to = opt.to_unsigned();
            set_sleep(to);
        }
        return err;
    }
    virtual int on_timeout_option
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        if ((optarg) && (optarg[0])) {
            string_t opt(optarg);
            unsigned to = opt.to_unsigned();
            set_timeout(to);
        }
        return err;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual mseconds_t set_sleep(mseconds_t to) {
        sleep_ = to;
        return sleep_;
    }
    virtual mseconds_t sleep() const {
        return sleep_;
    }
    virtual bool infinite_sleep(mseconds_t& timeout) const {
        return ((timeout = sleep_) == ((mseconds_t)-1));
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual mseconds_t set_timeout(mseconds_t to) {
        timeout_ = to;
        return timeout_;
    }
    virtual mseconds_t timeout() const {
        return timeout_;
    }
    virtual bool infinite_timeout(mseconds_t& timeout) const {
        return ((timeout = timeout_) == ((mseconds_t)-1));
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    mseconds_t sleep_, timeout_;
}; /// class _EXPORT_CLASS maint
typedef maint<> main;

} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_MAIN_HPP 
