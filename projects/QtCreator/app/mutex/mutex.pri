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
#   File: mutex.pri
#
# Author: $author$
#   Date: 8/20/2019
#
# QtCreator .pri file for fila executable mutex
########################################################################

########################################################################
# mutex

# mutex TARGET
#
mutex_TARGET = mutex

# mutex INCLUDEPATH
#
mutex_INCLUDEPATH += \
$${fila_INCLUDEPATH} \

# mutex DEFINES
#
mutex_DEFINES += \
$${fila_DEFINES} \

########################################################################
# mutex OBJECTIVE_HEADERS
#
#mutex_OBJECTIVE_HEADERS += \
#$${FILA_SRC}/fila/base/Base.hh \

# mutex OBJECTIVE_SOURCES
#
#mutex_OBJECTIVE_SOURCES += \
#$${FILA_SRC}/fila/base/Base.mm \

########################################################################
# mutex HEADERS
#
mutex_HEADERS += \
$${FILA_SRC}/xos/platform/os/microsoft/windows/Mutex.h \
$${FILA_SRC}/xos/platform/os/microsoft/windows/Mutex.hpp \
\
$${FILA_SRC}/xos/mt/mutex.hpp \
$${FILA_SRC}/xos/mt/posix/mutex.hpp \
$${FILA_SRC}/xos/mt/apple/mach/mutex.hpp \
$${FILA_SRC}/xos/mt/apple/osx/mutex.hpp \
$${FILA_SRC}/xos/mt/microsoft/windows/mutex.hpp \
$${FILA_SRC}/xos/mt/os/mutex.hpp \
\
$${FILA_SRC}/xos/app/console/mt/mutex/main_opt.hpp \
$${FILA_SRC}/xos/app/console/mt/mutex/main.hpp \

# mutex SOURCES
#
mutex_SOURCES += \
$${FILA_SRC}/xos/platform/os/microsoft/windows/Mutex.cpp \
\
$${FILA_SRC}/xos/app/console/mt/mutex/main_opt.cpp \
$${FILA_SRC}/xos/app/console/mt/mutex/main.cpp \

########################################################################
# mutex FRAMEWORKS
#
mutex_FRAMEWORKS += \
$${fila_FRAMEWORKS} \

# mutex LIBS
#
mutex_LIBS += \
$${fila_LIBS} \


