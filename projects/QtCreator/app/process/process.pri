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
#   File: process.pri
#
# Author: $author$
#   Date: 9/24/2019
#
# QtCreator .pri file for fila executable process
########################################################################

########################################################################
# process

# process TARGET
#
process_TARGET = process

# process INCLUDEPATH
#
process_INCLUDEPATH += \
$${fila_INCLUDEPATH} \

# process DEFINES
#
process_DEFINES += \
$${fila_DEFINES} \

########################################################################
# process OBJECTIVE_HEADERS
#
#process_OBJECTIVE_HEADERS += \
#$${FILA_SRC}/fila/base/Base.hh \

# process OBJECTIVE_SOURCES
#
#process_OBJECTIVE_SOURCES += \
#$${FILA_SRC}/fila/base/Base.mm \

########################################################################
# process HEADERS
#
process_HEADERS += \
$${FILA_SRC}/xos/mt/process.hpp \
$${FILA_SRC}/xos/mt/posix/process.hpp \
\
$${FILA_SRC}/xos/app/console/mt/process/main_opt.hpp \
$${FILA_SRC}/xos/app/console/mt/process/main.hpp \

# process SOURCES
#
process_SOURCES += \
$${FILA_SRC}/xos/mt/posix/process.cpp \
\
$${FILA_SRC}/xos/app/console/mt/process/main_opt.cpp \
$${FILA_SRC}/xos/app/console/mt/process/main.cpp \

########################################################################
# process FRAMEWORKS
#
process_FRAMEWORKS += \
$${fila_FRAMEWORKS} \

# process LIBS
#
process_LIBS += \
$${fila_LIBS} \


