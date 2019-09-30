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
///   File: process.hpp
///
/// Author: $author$
///   Date: 9/24/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_PROCESS_HPP
#define _XOS_MT_POSIX_PROCESS_HPP

#include "xos/mt/process.hpp"
#include "xos/base/argv.hpp"
#include "thirdparty/gnu/glibc/posix/execvpe.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

namespace xos {
namespace mt {
namespace posix {

typedef attachert<pid_t, int, -1, mt::process> processt_attacher;
typedef attachedt<pid_t, int, -1, processt_attacher> processt_attached;
typedef createdt<pid_t, int, -1, processt_attacher, processt_attached> processt_created;
///////////////////////////////////////////////////////////////////////
///  Class: processt
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = processt_attacher, class TExtends = processt_created>

class _EXPORT_CLASS processt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename implements::attached_t attached_t;
    typedef typename implements::unattached_t unattached_t;
    enum { unattached = implements::unattached};

    typedef typename implements::char_t char_t;
    typedef typename implements::end_t end_t;
    enum { end = implements::end };
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    processt
    (const char_t* path, char_t** argv, char_t** env,
     int* fdup, int** pdup, bool is_detached = false): is_forked_(false) {
        if (!(this->create(path, argv, env, fdup, pdup, is_detached))) {
            LOG_ERROR("...failed on this->create(path, argv, env, fdup, pdup, is_detached) throw create_exception(create_failed)...")
            throw create_exception(create_failed);
        }
    }
    processt
    (const char_t* path, char_t** argv, char_t** env, bool is_detached = false): is_forked_(false) {
        if (!(this->create(path, argv, env, is_detached))) {
            LOG_ERROR("...failed on this->create(path, argv, env, is_detached) throw create_exception(create_failed)...")
            throw create_exception(create_failed);
        }
    }
    processt
    (const char_t* path, char_t** argv, bool is_detached = false): is_forked_(false) {
        if (!(this->create(path, argv, is_detached))) {
            LOG_ERROR("...failed on this->create(path, argv, is_detached) throw create_exception(create_failed)...")
            throw create_exception(create_failed);
        }
    }
    processt
    (const char_t* path, const char_t* arg, ...): is_forked_(false) {
        if ((path) && (path[0]) && (arg) && (arg[0])) {
            bool success = false;
            va_list va;
            va_start(va, arg);
            success = this->createv(path, arg, va);
            va_end(va);
            if (!(success)) {
                LOG_ERROR("...failed on this->creatv(path, arg, va) throw create_exception(create_failed)...")
                throw create_exception(create_failed);
            }
        }
    }
    processt
    (const char_t* path, bool is_detached = false): is_forked_(false) {
        if (!(this->create(path, is_detached))) {
            LOG_ERROR("...failed on this->create(path, is_detached) throw create_exception(create_failed)...")
            throw create_exception(create_failed);
        }
    }
    processt(): is_forked_(false) {
    }
    virtual ~processt() {
        if (!(this->destroyed())) {
            LOG_ERROR("...failed on this->destroyed() throw create_exception(destroy_failed)...")
            throw create_exception(destroy_failed);
        }
    }
private:
    processt(const processt &copy) {
        LOG_ERROR("...unexpected throw exception(exception_unexpected)...");
        throw exception(exception_unexpected);
    }
    
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool exec_detached
    (const char_t* path, char_t** argv, char_t** env,
     int* fdup, int** pdup, bool is_detached = false) const {
        if ((path) && (path[0])) {
            if ((argv) && (argv[0])) {
                if ((env) && (env[0])) {
                    int err = 0;

                    if ((fdup)) {
                        int fd = 0;
                        for (int i = 0; (i < 3) && (fd = fdup[i]); ++i) {
                            if (0 > (err = ::dup2(fd, i))) {
                                return false;
                            } else {
                                ::close(fd);
                            }
                        }
                    }
                    if ((pdup)) {
                        int* p = 0;
                        for (int i = 0; (i < 3) && (p = pdup[i]); ++i) {
                            int fd = p[(i)?(1):(0)];
                            if (0 > (err = ::dup2(fd, i))) {
                                return false;
                            } else {
                                ::close(p[0]);
                                ::close(p[1]);
                            }
                        }
                    }
                    if (!(err = ::execvpe(path, argv, env))) {
                        return true;
                    }
                } else {
                    return exec_detached(path, argv, is_detached);
                }
            } else {
                return exec_detached(path, is_detached);
            }
        }
        return false;
    }
    virtual bool exec_detached
    (const char_t* path, char_t** argv, char_t** env, bool is_detached = false) const {
        if ((path) && (path[0])) {
            if ((argv) && (argv[0])) {
                if ((env) && (env[0])) {
                    int err = 0;
                    if (!(err = ::execvpe(path, argv, env))) {
                        return true;
                    }
                } else {
                    return exec_detached(path, argv, is_detached);
                }
            } else {
                return exec_detached(path, is_detached);
            }
        }
        return false;
    }
    virtual bool exec_detached
    (const char_t* path, char_t** argv, bool is_detached = false) const {
        if ((path) && (path[0])) {
            if ((argv) && (argv[0])) {
                int err = 0;
                if (!(err = ::execvp(path, argv))) {
                    return true;
                }
            } else {
                return exec_detached(path, is_detached);
            }
        }
        return false;
    }
    virtual bool exec_detached(const char_t* path, bool is_detached = false) const {
        if ((path) && (path[0])) {
            ::xos::argvt<char_t> args(&path, 1);
            char_t **argv = args.elements();
            int err = 0;
            if ((argv) && !(err = ::execvp(path, argv))) {
                return true;
            }
        }
        return false;
    }

public:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool create
    (const char_t* path, char_t** argv, char_t** env,
     int* fdup, int** pdup, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_attached(path, argv, env, fdup, pdup, is_detached))) {
            this->set_is_created();
            return true;
        }
        return false;
    }
    virtual bool create
    (const char_t* path, char_t** argv, char_t** env, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_attached(path, argv, env, is_detached))) {
            this->set_is_created();
            return true;
        }
        return false;
    }
    virtual bool create
    (const char_t* path, char_t** argv, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_attached(path, argv, is_detached))) {
            this->set_is_created();
            return true;
        }
        return false;
    }
    virtual bool createv(const char_t* path, const char_t* arg, va_list va, bool is_detached = false) {
        if ((path) && (path[0]) && (arg) && (arg[0])) {
            xos::argvt<char_t> argv(arg, va);
            attached_t detached = ((attached_t)unattached);
            if (0 <= (detached = create_attached(path, argv, is_detached))) {
                this->set_is_created();
                return true;
            }
        }
        return false;
    }
    virtual bool create(const char_t* path, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_attached(path, is_detached))) {
            this->set_is_created();
            return true;
        }
        return false;
    }
    virtual attached_t create_attached
    (const char_t* path, char_t** argv, char_t** env,
     int* fdup, int** pdup, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_detached(path, argv, env, fdup, pdup, is_detached))) {
            this->attach(detached);
        }
        return detached;
    }
    virtual attached_t create_attached
    (const char_t* path, char_t** argv, char_t** env, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_detached(path, argv, env, is_detached))) {
            this->attach(detached);
        }
        return detached;
    }
    virtual attached_t create_attached
    (const char_t* path, char_t** argv, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_detached(path, argv, is_detached))) {
            this->attach(detached);
        }
        return detached;
    }
    virtual attached_t create_attached(const char_t* path, bool is_detached = false) {
        attached_t detached = ((attached_t)unattached);
        if (0 <= (detached = create_detached(path, is_detached))) {
            this->attach(detached);
        }
        return detached;
    }
    virtual bool destroy() {
        if ((this->joined())) {
            return true;
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual wait_status timed_wait(mseconds_t milliseconds) {
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            return timed_wait_detached(detached, milliseconds);
        }
        return wait_failed;
    }
    virtual wait_status untimed_wait() {
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            return untimed_wait_detached(detached);
        }
        return wait_failed;
    }
    virtual wait_status try_wait() {
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            return try_wait_detached(detached);
        }
        return wait_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual wait_status timed_wait_detached(attached_t detached, mseconds_t milliseconds) const {
        if (((attached_t)unattached) != detached) {
        }
        return wait_failed;
    }
    virtual wait_status untimed_wait_detached(attached_t pid) const {
        if (((attached_t)unattached) != pid) {
            int result = 0, options = 0, status = 0;

            LOG_DEBUG("::waitpid(" << pid << ", &status = " << status << ", options)...");
            if (!(pid != (result = ::waitpid(pid, &status, options)))) {
                LOG_DEBUG("...result = " << result << " on ::waitpid(" << pid << ", &status = " << status << ", options)");
                return wait_success;
            } else {
                XOS_LOG_ERROR("...failed result = " << result << ", errno = " << errno << " on ::waitpid(" << pid << ", &status = " << status << ", options)");
            }
        }
        return wait_failed;
    }
    virtual wait_status try_wait_detached(attached_t detached) const {
        if (((attached_t)unattached) != detached) {
        }
        return wait_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached
    (const char_t* path, char_t** argv, char_t** env,
     int* fdup, int** pdup, bool is_detached = false) const {
        attached_t pid = ((attached_t)unattached);

        LOG_DEBUG("::fork()...");
        if (0 < (pid = ::fork())) {
            LOG_DEBUG("... pid = " << pid << " on ::fork()");
        } else {
            if (0 > (pid)) {
                LOG_ERROR("failed errno = " << errno << "on ::fork()");
            } else {
                int err = 1;
                exec_detached(path, argv, env, fdup, pdup, is_detached);
                ::exit(err);
            }
        }
        return pid;
    }
    virtual attached_t create_detached
    (const char_t* path, char_t** argv, char_t** env, bool is_detached = false) const {
        attached_t pid = ((attached_t)unattached);

        LOG_DEBUG("::fork()...");
        if (0 < (pid = ::fork())) {
            LOG_DEBUG("... pid = " << pid << " on ::fork()");
        } else {
            if (0 > (pid)) {
                LOG_ERROR("failed errno = " << errno << "on ::fork()");
            } else {
                int err = 1;
                exec_detached(path, argv, env, is_detached);
                ::exit(err);
            }
        }
        return pid;
    }
    virtual attached_t create_detached
    (const char_t* path, char_t** argv, bool is_detached = false) const {
        attached_t pid = ((attached_t)unattached);

        LOG_DEBUG("::fork()...");
        if (0 < (pid = ::fork())) {
            LOG_DEBUG("... pid = " << pid << " on ::fork()");
        } else {
            if (0 > (pid)) {
                LOG_ERROR("failed errno = " << errno << "on ::fork()");
            } else {
                int err = 1;
                exec_detached(path, argv, is_detached);
                ::exit(err);
            }
        }
        return pid;
    }
    virtual attached_t create_detached(const char_t* path, bool is_detached = false) const {
        attached_t pid = ((attached_t)unattached);

        LOG_DEBUG("::fork()...");
        if (0 < (pid = ::fork())) {
            LOG_DEBUG("... pid = " << pid << " on ::fork()");
        } else {
            if (0 > (pid)) {
                LOG_ERROR("failed errno = " << errno << "on ::fork()");
            } else {
                int err = 1;
                exec_detached(path, is_detached);
                ::exit(err);
            }
        }
        return pid;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool set_is_forked(bool to = true) {
        is_forked_ = to;
        return is_forked_;
    }
    virtual bool is_forked() const {
        return is_forked_;
    }
    virtual bool set_is_created(bool to = true) {
        extends::set_is_created(to);
        is_forked_ = to;
        return this->is_created();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    bool is_forked_;
}; /// class _EXPORT_CLASS processt

typedef processt<> process;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_PROCESS_HPP 
