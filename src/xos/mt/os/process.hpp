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
///   Date: 10/3/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_OS_PROCESS_HPP
#define _XOS_MT_OS_PROCESS_HPP

#include "xos/mt/os/os.hpp"

#if defined(WINDOWS)
#include "xos/mt/microsoft/windows/process.hpp"
#elif defined(MACOSX)
#include "xos/mt/apple/osx/process.hpp"
#elif defined(APPIOS)
#include "xos/mt/apple/ios/process.hpp"
#elif defined(LINUX)
#include "xos/mt/linux/process.hpp"
#else /// defined(WINDOWS)
#include "xos/mt/posix/process.hpp"
#endif /// defined(WINDOWS)

namespace xos {
namespace mt {
namespace os {

typedef os::process process;

} /// namespace os
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_OS_PROCESS_HPP 
