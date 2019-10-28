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
///   File: kern.h
///
/// Author: $author$
///   Date: 10/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_MT_LINUX_MACH_KERN_H
#define _XOS_MT_LINUX_MACH_KERN_H

#include "xos/mt/linux/os.h"

typedef	int kern_return_t;

#define KERN_SUCCESS			0
#define KERN_INVALID_ADDRESS		1
#define KERN_PROTECTION_FAILURE		2
#define KERN_NO_SPACE			3
#define KERN_INVALID_ARGUMENT		4
#define KERN_FAILURE			5
#define KERN_RESOURCE_SHORTAGE		6
#define KERN_NOT_RECEIVER		7
#define KERN_NO_ACCESS			8
#define KERN_MEMORY_FAILURE		9
#define KERN_MEMORY_ERROR		10
#define	KERN_ALREADY_IN_SET		11
#define KERN_NOT_IN_SET			12
#define KERN_NAME_EXISTS		13
#define KERN_ABORTED			14
#define KERN_INVALID_NAME		15
#define	KERN_INVALID_TASK		16
#define KERN_INVALID_RIGHT		17
#define KERN_INVALID_VALUE		18
#define	KERN_UREFS_OVERFLOW		19
#define	KERN_INVALID_CAPABILITY		20
#define KERN_RIGHT_EXISTS		21
#define	KERN_INVALID_HOST		22
#define KERN_MEMORY_PRESENT		23
#define KERN_MEMORY_DATA_MOVED		24
#define KERN_MEMORY_RESTART_COPY	25
#define KERN_INVALID_PROCESSOR_SET	26
#define KERN_POLICY_LIMIT		27
#define KERN_INVALID_POLICY		28
#define KERN_INVALID_OBJECT		29
#define KERN_ALREADY_WAITING		30
#define KERN_DEFAULT_SET		31
#define KERN_EXCEPTION_PROTECTED	32
#define KERN_INVALID_LEDGER		33
#define KERN_INVALID_MEMORY_CONTROL	34
#define KERN_INVALID_SECURITY		35
#define KERN_NOT_DEPRESSED		36
#define KERN_TERMINATED			37
#define KERN_LOCK_SET_DESTROYED		38
#define KERN_LOCK_UNSTABLE		39
#define KERN_LOCK_OWNED			40
#define KERN_LOCK_OWNED_SELF		41
#define KERN_SEMAPHORE_DESTROYED	42
#define KERN_RPC_SERVER_TERMINATED	43
#define KERN_RPC_TERMINATE_ORPHAN	44
#define KERN_RPC_CONTINUE_ORPHAN	45
#define	KERN_NOT_SUPPORTED		46
#define	KERN_NODE_DOWN			47
#define KERN_NOT_WAITING		48
#define	KERN_OPERATION_TIMED_OUT        49
#define KERN_CODESIGN_ERROR		50
#define	KERN_RETURN_MAX			0x100

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

#endif /*/ _XOS_MT_LINUX_MACH_KERN_H /*/
