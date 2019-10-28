########################################################################
# Copyright (c) 1988-2019 $organization$
#
# This software is provided by the author and contributors ``as is''
# and any express or implied warranties, including, but not limited to,
# the implied warranties of merchantability and fitness for a particular
# purpose are disclaimed. In no event shall the author or contributors
# be liable for any direct, indirect, incidental, special, exemplary,
# or consequential damages (including, but not limited to, procurement
# of substitute goods or services; loss of use, data, or profits; or
# business interruption) however caused and on any theory of liability,
# whether in contract, strict liability, or tort (including negligence
# or otherwise) arising in any way out of the use of this software,
# even if advised of the possibility of such damage.
#
#   File: thread.pri
#
# Author: $author$
#   Date: 10/28/2019
#
# QtCreator .pri file for fila executable thread
########################################################################

########################################################################
# thread

# thread TARGET
#
thread_TARGET = thread

# thread INCLUDEPATH
#
thread_INCLUDEPATH += \
$${fila_INCLUDEPATH} \

# thread DEFINES
#
thread_DEFINES += \
$${fila_DEFINES} \

########################################################################
# thread OBJECTIVE_HEADERS
#
#thread_OBJECTIVE_HEADERS += \
#$${FILA_SRC}/fila/base/Base.hh \

# thread OBJECTIVE_SOURCES
#
#thread_OBJECTIVE_SOURCES += \
#$${FILA_SRC}/fila/base/Base.mm \

########################################################################
# thread HEADERS
#
thread_HEADERS += \
$${FILA_SRC}/xos/mt/thread.hpp \
$${FILA_SRC}/xos/mt/posix/thread.hpp \
\
$${FILA_SRC}/xos/app/console/mt/thread/main_opt.hpp \
$${FILA_SRC}/xos/app/console/mt/thread/main.hpp \

# thread SOURCES
#
thread_SOURCES += \
$${FILA_SRC}/xos/mt/posix/thread.cpp \
\
$${FILA_SRC}/xos/app/console/mt/thread/main_opt.cpp \
$${FILA_SRC}/xos/app/console/mt/thread/main.cpp \

########################################################################
# thread FRAMEWORKS
#
thread_FRAMEWORKS += \
$${fila_FRAMEWORKS} \

# thread LIBS
#
thread_LIBS += \
$${fila_LIBS} \


