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
    maint(): run_(0) {
    }
    virtual ~maint() {
    }
private:
    maint(const maint &copy) {
    }
    
protected:
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
        return this->posix_run(argc, argv, env);
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    template <class TCondition>
    int run() {
        int err = 0;
        this->outlln("try {...", NULL);
        try {
            TCondition condition;
            this->outlln("...} try", NULL);
        } catch (...) {
            this->outlln("...catch (...)", NULL);
            err = 1;
        }
        return err;
    }

    virtual int posix_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::posix::condition >();
    }
    virtual int derived_run(int argc, char_t** argv, char_t** env) {
        return this->run< ::xos::mt::derived::condition >();
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
