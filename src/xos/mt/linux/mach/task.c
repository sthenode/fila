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
///   File: task.c
///
/// Author: $author$
///   Date: 10/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#include "xos/mt/linux/mach/task.h"

#if defined(__cplusplus)
namespace xos {
namespace mt {
namespace linux {
namespace mach {
#endif /*/ defined(__cplusplus) /*/

#if defined(__cplusplus)
} /*/ namespace mach /*/
} /*/ namespace linux /*/
} /*/ namespace mt /*/
} /*/ namespace xos /*/
#endif /*/ defined(__cplusplus) /*/

mach_port_t mach_task_self(void) { return 0; }
mach_port_t mach_thread_self(void) { return 0; }

