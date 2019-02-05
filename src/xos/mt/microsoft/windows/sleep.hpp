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
///   File: sleep.hpp
///
/// Author: $author$
///   Date: 2/4/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_SLEEP_HPP
#define _XOS_MT_MICROSOFT_WINDOWS_SLEEP_HPP

#include "xos/mt/sleep.hpp"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

typedef mt::sleep::implements sleept_implements;
typedef mt::sleep sleept_extends;
///////////////////////////////////////////////////////////////////////
///  Class: sleept
///////////////////////////////////////////////////////////////////////
template <class TImplements = sleept_implements, class TExtends = sleept_extends>
class _EXPORT_CLASS sleept: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    sleept(mseconds_t milliseconds): extends(milliseconds) {
        run();
    }
    sleept() {
        run();
    }
    virtual ~sleept() {
    }
private:
    sleept(const sleept &copy) {
    }
public:

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual void run() {
        DWORD dwMilliseconds = (this->milliseconds_<0)?(INFINITE):(this->milliseconds_);
        ::Sleep(dwMilliseconds);
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS sleept
typedef sleept<> sleep;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_SLEEP_HPP 
