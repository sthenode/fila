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
///   File: main_opt.hpp
///
/// Author: $author$
///   Date: 8/20/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_MUTEX_MAIN_OPT_HPP
#define _XOS_APP_CONSOLE_MT_MUTEX_MAIN_OPT_HPP

#include "xos/app/console/mt/main.hpp"
#include "xos/console/lib/version/main.hpp"
#include "xos/lib/fila/version.hpp"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace mutex {

typedef mt::main main_optt_extends;
typedef main_optt_extends::implements main_optt_implements;
///////////////////////////////////////////////////////////////////////
///  Class: main_optt
///////////////////////////////////////////////////////////////////////
template <class TImplements = main_optt_implements, class TExtends = main_optt_extends>
class _EXPORT_CLASS main_optt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    main_optt() {
    }
    virtual ~main_optt() {
    }
private:
    main_optt(const main_optt &copy) {
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS main_optt
typedef main_optt<> main_opt;

} /// namespace mutex
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_MUTEX_MAIN_OPT_HPP 
