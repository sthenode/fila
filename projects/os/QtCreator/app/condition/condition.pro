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
#   File: condition.pro
#
# Author: $author$
#   Date: 8/19/2019
#
# QtCreator .pro file for fila executable condition
########################################################################
include(../../../../../build/QtCreator/fila.pri)
include(../../../../QtCreator/fila.pri)
include(../../fila.pri)
include(../../../../QtCreator/app/condition/condition.pri)

TARGET = $${condition_TARGET}

########################################################################
# INCLUDEPATH
#
INCLUDEPATH += \
$${condition_INCLUDEPATH} \

# DEFINES
# 
DEFINES += \
$${condition_DEFINES} \

########################################################################
# OBJECTIVE_HEADERS
#
OBJECTIVE_HEADERS += \
$${condition_OBJECTIVE_HEADERS} \

# OBJECTIVE_SOURCES
#
OBJECTIVE_SOURCES += \
$${condition_OBJECTIVE_SOURCES} \

########################################################################
# HEADERS
#
HEADERS += \
$${condition_HEADERS} \
$${OBJECTIVE_HEADERS} \

# SOURCES
#
SOURCES += \
$${condition_SOURCES} \

########################################################################
# FRAMEWORKS
#
FRAMEWORKS += \
$${condition_FRAMEWORKS} \

# LIBS
#
LIBS += \
$${condition_LIBS} \
$${FRAMEWORKS} \


