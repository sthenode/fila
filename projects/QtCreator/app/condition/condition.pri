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
#   File: condition.pri
#
# Author: $author$
#   Date: 8/19/2019
#
# QtCreator .pri file for fila executable condition
########################################################################

########################################################################
# condition

# condition TARGET
#
condition_TARGET = condition

# condition INCLUDEPATH
#
condition_INCLUDEPATH += \
$${fila_INCLUDEPATH} \

# condition DEFINES
#
condition_DEFINES += \
$${fila_DEFINES} \

########################################################################
# condition OBJECTIVE_HEADERS
#
#condition_OBJECTIVE_HEADERS += \
#$${FILA_SRC}/fila/base/Base.hh \

# condition OBJECTIVE_SOURCES
#
#condition_OBJECTIVE_SOURCES += \
#$${FILA_SRC}/fila/base/Base.mm \

########################################################################
# condition HEADERS
#
condition_HEADERS += \
$${FILA_SRC}/xos/mt/condition.hpp \
$${FILA_SRC}/xos/mt/posix/condition.hpp \
\
$${FILA_SRC}/xos/app/console/mt/condition/main_opt.hpp \
$${FILA_SRC}/xos/app/console/mt/condition/main.hpp \

# condition SOURCES
#
condition_SOURCES += \
$${FILA_SRC}/xos/mt/posix/condition.cpp \
\
$${FILA_SRC}/xos/app/console/mt/condition/main_opt.cpp \
$${FILA_SRC}/xos/app/console/mt/condition/main.cpp \

########################################################################
# condition FRAMEWORKS
#
condition_FRAMEWORKS += \
$${fila_FRAMEWORKS} \

# condition LIBS
#
condition_LIBS += \
$${fila_LIBS} \


