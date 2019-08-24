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
#   File: semaphore.pro
#
# Author: $author$
#   Date: 8/23/2019
#
# QtCreator .pro file for fila executable semaphore
########################################################################
include(../../../../../build/QtCreator/fila.pri)
include(../../../../QtCreator/fila.pri)
include(../../fila.pri)
include(../../../../QtCreator/app/semaphore/semaphore.pri)

TARGET = $${semaphore_TARGET}

########################################################################
# INCLUDEPATH
#
INCLUDEPATH += \
$${semaphore_INCLUDEPATH} \

# DEFINES
# 
DEFINES += \
$${semaphore_DEFINES} \

########################################################################
# OBJECTIVE_HEADERS
#
OBJECTIVE_HEADERS += \
$${semaphore_OBJECTIVE_HEADERS} \

# OBJECTIVE_SOURCES
#
OBJECTIVE_SOURCES += \
$${semaphore_OBJECTIVE_SOURCES} \

########################################################################
# HEADERS
#
HEADERS += \
$${semaphore_HEADERS} \
$${OBJECTIVE_HEADERS} \

# SOURCES
#
SOURCES += \
$${semaphore_SOURCES} \

########################################################################
# FRAMEWORKS
#
FRAMEWORKS += \
$${semaphore_FRAMEWORKS} \

# LIBS
#
LIBS += \
$${semaphore_LIBS} \
$${FRAMEWORKS} \


