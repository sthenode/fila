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
///   Date: 9/24/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_APP_CONSOLE_MT_PROCESS_MAIN_HPP
#define XOS_APP_CONSOLE_MT_PROCESS_MAIN_HPP

#include "xos/app/console/mt/process/main_opt.hpp"
#include "xos/mt/posix/process.hpp"
#include "xos/base/argv.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace process {

///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template 
<class TVersion = xos::lib::fila::version, 
 class TExtends = main_optt<TVersion>, 
 class TImplements = typename TExtends::implements>

class _EXPORT_CLASS maint: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef maint derives;

    typedef TVersion lib_version_t;
    typedef typename implements::file_t file_t;
    typedef typename implements::string_t string_t;
    typedef typename implements::char_t char_t;

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
        int err = 1;
        if ((run_)) {
            err = (this->*run_)(argc, argv, env);
        } else {
            err = default_run(argc, argv, env);
        }
        return err;
    }
    virtual int default_run(int argc, char_t** argv, char_t** env) {
        int err = this_version_run(argc, argv, env);
        return err;
    }
    virtual int this_version_run(int argc, char_t** argv, char_t** env) {
        const xos::lib::version& which_version = lib_version_t::which();
        int err = this->version_run(which_version, argc, argv, env);
        if (!(err)) {
            this->outln();
            this->usage(argc, argv, env);
        }
        return err;
    }
    virtual int set_version_run(int argc, char_t**argv, char_t**env) {
        int err = 0;
        run_ = &derives::this_version_run;
        return err;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual int set_posix_run(int argc, char_t**argv, char_t**env) {
        int err = 0;
        run_ = &derives::this_posix_run;
        return err;
    }
    virtual int this_posix_run(int argc, char_t** argv, char_t** env) {
        return this->posix_run(argc, argv, env);
    }
    virtual int posix_run(int argc, char_t** argv, char_t** env) {
        int err = 0;
        const char_t *arg = 0;
        if ((argc > optind) && (arg = argv[optind]) && (arg[0])) {
            xos::mt::posix::process p(arg, arg, NULL);
        }
        return err;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
}; /// class _EXPORT_CLASS maint

typedef maint<> main;


} /// namespace process
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// ndef XOS_APP_CONSOLE_MT_PROCESS_MAIN_HPP
