///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2018 $organization$
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
///   File: timed.hpp
///
/// Author: $author$
///   Date: 4/12/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_TIMED_HPP
#define _XOS_MT_POSIX_TIMED_HPP

#include "xos/base/base.hpp"

#include <sys/time.h>
#include <time.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

///
/// clock_gettime
///
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#if !defined(CLOCK_HAS_GETTIME)
#define CLOCK_HAS_GETTIME
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res) && (CLOCK_REALTIME == clk_id)) {
        int err = 0;
        struct timeval tv;
        if ((err = gettimeofday(&tv, NULL))) {
            res->tv_sec = 0;
            res->tv_nsec = 0;
            return errno;
        } else {
            res->tv_sec = tv.tv_sec;
            res->tv_nsec = ::xos::useconds_nseconds(tv.tv_usec);
            return 0;
        }
    }
    return EINVAL;
}
#endif /// !defined(CLOCK_HAS_GETTIME)
#endif /// !defined(CLOCK_REALTIME)

namespace xos {
namespace mt {
namespace posix {

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_TIMED_HPP 
