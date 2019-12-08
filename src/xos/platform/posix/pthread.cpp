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
///   File: pthread.cpp
///
/// Author: $author$
///   Date: 10/29/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/posix/pthread.hpp"

namespace xos {
namespace platform {
namespace posix {

} /// namespace posix
} /// namespace platform
} /// namespace xos

#if !defined(HAS_POSIX_TIMEOUTS)
///
/// pthread
/// ...
#if defined(PTHREAD_HAS_TRYJOIN)
int pthread_tryjoin_np(pthread_t thread, void **retval) {
    return EINVAL;
}
#endif /// defined(PTHREAD_HAS_TRYJOIN)

#if defined(PTHREAD_HAS_TIMEDJOIN)
int pthread_timedjoin_np
(pthread_t thread, void **retval, const struct timespec *abstime) {
    return EINVAL;
}
#endif /// defined(PTHREAD_HAS_TIMEDJOIN)
/// ...
/// pthread
/// 
#endif /// !defined(HAS_POSIX_TIMEOUTS)
