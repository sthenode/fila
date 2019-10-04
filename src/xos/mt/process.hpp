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
#ifndef _XOS_MT_PROCESS_HPP
#define _XOS_MT_PROCESS_HPP

#include "xos/base/created.hpp"
#include "xos/base/joined.hpp"
#include "xos/base/waited.hpp"

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: processt
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char, typename TEnd = TChar, TEnd VEnd = 0,
 class TString = char_stringt<TChar, TEnd, VEnd>,
 class TImplements = creatort< joinedt<waited> > >

class _EXPORT_CLASS processt: virtual public TImplements {
public:
    typedef TImplements implements;

    typedef TString string_t;
    typedef TChar char_t;
    typedef TEnd end_t;
    enum { end = VEnd };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool create(const char_t* path, bool is_detached = false) {
        return false;
    }
    virtual bool create
    (const char_t* path, char_t** argv, bool is_detached = false) {
        return false;
    }
    virtual bool create
    (const char_t* path, char_t** argv, char_t** env, bool is_detached = false) {
        return false;
    }
    virtual bool create
    (const char_t* path, char_t** argv, char_t** env,
     int* fdup, int** pdup, bool is_detached = false) {
        return false;
    }
    virtual bool createl(const char_t* path, const char_t* arg, ...) {
        return false;
    }
    virtual bool createv(const char_t* path, const char_t* arg, va_list va) {
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool joined() {
        if ((this->is_forked())) {
            return this->join();
        }
        return true;
    }
    virtual bool join() {
        this->set_is_forked(false);
        return this->wait();
    }
    virtual wait_status timed_join(mseconds_t milliseconds) {
        wait_status status = this->timed_wait(milliseconds);
        if (wait_busy != (status)) {
            this->set_is_forked(false);
        }
    }
    virtual wait_status untimed_join() {
        this->set_is_forked(false);
        return this->untimed_wait();
    }
    virtual wait_status try_join() {
        wait_status status = this->try_wait();
        if (wait_busy != (status)) {
            this->set_is_forked(false);
        }
        return status;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS processt

typedef processt<> process;

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_PROCESS_HPP 
