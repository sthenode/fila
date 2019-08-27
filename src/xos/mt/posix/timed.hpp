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

#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#if !defined(CLOCK_HAS_GETTIME)
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
#define CLOCK_HAS_GETTIME
#endif /// !defined(CLOCK_HAS_GETTIME)
#endif /// !defined(CLOCK_REALTIME)

namespace xos {
namespace mt {
namespace posix {

/*///////////////////////////////////////////////////////////////////////
/// Function: timed_until_time
///////////////////////////////////////////////////////////////////////
inline struct timespec timed_until_time(mseconds_t milliseconds) {
    struct timespec until_time;
#if defined(CLOCK_HAS_GETTIME)
    int err = 0;
    if ((err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
        until_time.tv_sec = 0;
        until_time.tv_nsec = 0;
        return until_time;
    }
#else /// defined(CLOCK_HAS_GETTIME)  
    until_time.tv_sec = 0;
    until_time.tv_nsec = 0;
#endif /// defined(CLOCK_HAS_GETTIME)  
    until_time.tv_sec +=  mseconds_seconds(milliseconds);
    until_time.tv_nsec +=  mseconds_nseconds(mseconds_mseconds(milliseconds));
    return until_time;
}*/

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_TIMED_HPP 
