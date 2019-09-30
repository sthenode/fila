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
///   File: pipe.hpp
///
/// Author: $author$
///   Date: 9/24/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_PIPE_HPP
#define _XOS_MT_PIPE_HPP

#include "xos/io/stream.hpp"

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: pipet
///////////////////////////////////////////////////////////////////////
template 
<typename TWhat = char, typename TSized = TWhat,
 typename TEndof = TSized, TEndof VEndof = 0,
 class TSequence = io::sequencet<TWhat, TSized, TEndof, VEndof>,
 class TSeeker = io::seekert<TSequence>,
 class TReader = io::readert<TSeeker>,
 class TWriter = io::writert<TSequence>,
 class TStream = io::streamt<TReader, TWriter>,
 class TImplements = implement_base>

class _EXPORT_CLASS pipet: virtual public TImplements {
public:
    typedef TImplements implements;

    typedef TWhat what_t;
    typedef TSized sized_t;
    typedef TEndof endof_t;
    enum { endof = VEndof };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS pipet

typedef pipet
<void, char, char, 0, 
 io::sequence, io::seeker, io::reader, io::writer, io::stream> pipe;

typedef pipet
<char, char, char, 0, 
 io::char_sequence, io::char_seeker, 
 io::char_reader, io::char_writer, io::char_stream> char_pipe;

typedef pipet
<tchar_t, tchar_t, tchar_t, 0, 
 io::tchar_sequence, io::tchar_seeker, 
 io::tchar_reader, io::tchar_writer, io::tchar_stream> tchar_pipe;

typedef pipet
<wchar_t, wchar_t, wchar_t, 0, 
 io::wchar_sequence, io::wchar_seeker, 
 io::wchar_reader, io::wchar_writer, io::wchar_stream> wchar_pipe;

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_PIPE_HPP 
