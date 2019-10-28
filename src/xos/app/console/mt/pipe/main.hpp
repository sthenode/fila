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
///   Date: 9/27/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_APP_CONSOLE_MT_PIPE_MAIN_HPP
#define XOS_APP_CONSOLE_MT_PIPE_MAIN_HPP

#include "xos/app/console/mt/pipe/main_opt.hpp"
#include "xos/base/argv.hpp"
#include "xos/mt/pipe.hpp"
#include "xos/mt/posix/pipe.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace pipe {

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
            xos::argv args(&arg, 1);

            if ((argv = args.elements()) && (arg = argv[0]) && (arg[0])) {
                xos::mt::posix::pipe ipipe, opipe, epipe;
                int result = 0, options = 0, status = 0;
                pid_t pid = 0;

                this->outlln("::fork()...", NULL);
                if (0 < (pid = ::fork())) {
                    char chars[2];
                    this->outlln("...pid = ", signed_to_string(pid)(), " on ::fork()", NULL);

                    ipipe.close_in();
                    opipe.close_out();
                    epipe.close_out();

                    for (xos::mt::posix::pipe* pipe = &epipe; pipe; pipe = ((pipe != &opipe)?(&opipe):(0))) {
                        this->outln(((pipe != &opipe)?("err..."):("out...")));
                        for (ssize_t count = 0, amount = 0; (0 < (amount = pipe->read(chars , 2))); count += amount) {
                            this->out(chars, amount);
                            this->out_flush();
                            if (2 > amount) {
                                break;
                            }
                        }
                        this->outln(((pipe != &opipe)?("...err"):("...out")));
                    }
                    this->outlln("::waitpid(pid, &status, options)...", NULL);
                    if (!(pid != (result = ::waitpid(pid, &status, options)))) {
                        this->outlln("...", signed_to_string(result)(), " = ::waitpid(pid, &status, options)", NULL);
                    } else {
                        this->outlln("...failed ", signed_to_string(result)(), " = on ::waitpid(pid, &status, options)", NULL);
                    }
                } else {
                    if (0 > (pid)) {
                        this->outlln("...failed errno = ", signed_to_string(errno)(), "on ::fork()", NULL);
                    } else {
                        int err = 0, *p = 0, *pdup[3];
                        
                        pdup[0] = ipipe.attached_to();
                        pdup[1] = opipe.attached_to();
                        pdup[2] = epipe.attached_to();
                        
                        for (int i = 0; (i < 3) && (p = pdup[i]); ++i) {
                            int fd = p[(i)?(1):(0)];
                            if (0 > (::dup2(fd, i))) {
                                err = 1;
                                break;
                            } else {
                                ::close(p[0]);
                                ::close(p[1]);
                            }
                        }
                        if (!err) {
                            err = 1;
                            ::execvp(arg, argv);
                        }
                        ::exit(err);
                    }
                }
            }
        } else {
        }
        return err;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
}; /// class _EXPORT_CLASS maint

typedef maint<> main;


} /// namespace pipe
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// ndef XOS_APP_CONSOLE_MT_PIPE_MAIN_HPP
