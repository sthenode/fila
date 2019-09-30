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
#   File: pipe.pri
#
# Author: $author$
#   Date: 9/27/2019
#
# QtCreator .pri file for fila executable pipe
########################################################################

########################################################################
# pipe

# pipe TARGET
#
pipe_TARGET = pipe

# pipe INCLUDEPATH
#
pipe_INCLUDEPATH += \
$${fila_INCLUDEPATH} \

# pipe DEFINES
#
pipe_DEFINES += \
$${fila_DEFINES} \

########################################################################
# pipe OBJECTIVE_HEADERS
#
#pipe_OBJECTIVE_HEADERS += \
#$${FILA_SRC}/fila/base/Base.hh \

# pipe OBJECTIVE_SOURCES
#
#pipe_OBJECTIVE_SOURCES += \
#$${FILA_SRC}/fila/base/Base.mm \

########################################################################
# pipe HEADERS
#
pipe_HEADERS += \
$${FILA_SRC}/xos/mt/pipe.hpp \
$${FILA_SRC}/xos/mt/posix/pipe.hpp \
\
$${FILA_SRC}/xos/app/console/mt/pipe/main_opt.hpp \
$${FILA_SRC}/xos/app/console/mt/pipe/main.hpp \

# pipe SOURCES
#
pipe_SOURCES += \
$${FILA_SRC}/xos/mt/posix/pipe.cpp \
\
$${FILA_SRC}/xos/app/console/mt/pipe/main_opt.cpp \
$${FILA_SRC}/xos/app/console/mt/pipe/main.cpp \

########################################################################
# pipe FRAMEWORKS
#
pipe_FRAMEWORKS += \
$${fila_FRAMEWORKS} \

# pipe LIBS
#
pipe_LIBS += \
$${fila_LIBS} \


