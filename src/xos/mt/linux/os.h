/*/
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
///   File: os.h
///
/// Author: $author$
///   Date: 10/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_MT_LINUX_OS_H
#define _XOS_MT_LINUX_OS_H

#include "xos/platform/platform.h"

#if defined(linux) && !defined(platform_defined_linux)
#define platform_defined_linux linux
#undef linux
#endif /// defined(linux) && !defined(platform_defined_linux)

#if defined(__cplusplus)
namespace xos {
namespace mt {
namespace linux {
#endif /*/ defined(__cplusplus) /*/

#if defined(__cplusplus)
} /*/ namespace linux /*/
} /*/ namespace mt /*/
} /*/ namespace xos /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ _XOS_MT_LINUX_OS_H /*/
