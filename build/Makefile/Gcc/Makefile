########################################################################
# Copyright (c) 1988-2018 $organization$
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
#   File: Makefile
#
# Author: $author$
#   Date: 8/8/2018
#
# build Gcc Makefile for fila
########################################################################


########################################################################
# rostra
#
# pkg-config --cflags --libs rostra
#

ifndef USE_HOME_BUILD_rostra
USE_HOME_BUILD_rostra = no
endif

ifeq ($(USE_HOME_BUILD_rostra),yes)
#
# home build rostra
#
build_rostra_USRDEFINES += \

build_rostra_USRINCLUDES += \
-I$(HOME)/build/rostra/include \

build_rostra_USRCXXFLAGS += \

build_rostra_USRLIBDIRS += \
-L$(HOME)/build/rostra/lib \

build_rostra_LIBS += \
-lrostra \

build_rostra_FRAMEWORKS += \

else
#
# build rostra
#
build_rostra_USRDEFINES += \

build_rostra_USRINCLUDES += \

build_rostra_USRCXXFLAGS += \

build_rostra_USRLIBDIRS += \

build_rostra_LIBS += \

build_rostra_FRAMEWORKS += \

endif

########################################################################
# nadir
#
# pkg-config --cflags --libs nadir
#

ifndef USE_HOME_BUILD_nadir
USE_HOME_BUILD_nadir = no
endif

ifeq ($(USE_HOME_BUILD_nadir),yes)
#
# home build nadir
#
build_nadir_USRDEFINES += \

build_nadir_USRINCLUDES += \
-I$(HOME)/build/nadir/include \

build_nadir_USRCXXFLAGS += \

build_nadir_USRLIBDIRS += \
-L$(HOME)/build/nadir/lib \

build_nadir_LIBS += \
-lnadir \

build_nadir_FRAMEWORKS += \

else
#
# build nadir
#
build_nadir_USRDEFINES += \

build_nadir_USRINCLUDES += \

build_nadir_USRCXXFLAGS += \

build_nadir_USRLIBDIRS += \

build_nadir_LIBS += \

build_nadir_FRAMEWORKS += \

endif

########################################################################
# crono
#
# pkg-config --cflags --libs crono
#

ifndef USE_HOME_BUILD_crono
USE_HOME_BUILD_crono = no
endif

ifeq ($(USE_HOME_BUILD_crono),yes)
#
# home build crono
#
build_crono_USRDEFINES += \

build_crono_USRINCLUDES += \
-I$(HOME)/build/crono/include \

build_crono_USRCXXFLAGS += \

build_crono_USRLIBDIRS += \
-L$(HOME)/build/crono/lib \

build_crono_LIBS += \
-lcrono \

build_crono_FRAMEWORKS += \

else
#
# build crono
#
build_crono_USRDEFINES += \

build_crono_USRINCLUDES += \

build_crono_USRCXXFLAGS += \

build_crono_USRLIBDIRS += \

build_crono_LIBS += \

build_crono_FRAMEWORKS += \

endif

########################################################################
# fila

# build fila USRDEFINES
#
build_fila_USRDEFINES += \
${build_rostra_USRDEFINES} \
${build_nadir_USRDEFINES} \
${build_crono_USRDEFINES} \


# build fila USRINCLUDES
#
build_fila_USRINCLUDES += \
${build_crono_USRINCLUDES} \
${build_nadir_USRINCLUDES} \
${build_rostra_USRINCLUDES} \


# build fila USRCXXFLAGS
#
build_fila_USRCXXFLAGS += \
${build_rostra_USRCXXFLAGS} \
${build_nadir_USRCXXFLAGS} \
${build_crono_USRCXXFLAGS} \


# build fila USRLIBDIRS
#
build_fila_USRLIBDIRS += \
${build_crono_USRLIBDIRS} \
${build_nadir_USRLIBDIRS} \
${build_rostra_USRLIBDIRS} \


# build fila FRAMEWORKS
#
build_fila_FRAMEWORKS += \
${build_crono_FRAMEWORKS} \
${build_nadir_FRAMEWORKS} \
${build_rostra_FRAMEWORKS} \


# build fila LIBS
#
build_fila_LIBS += \
${build_crono_LIBS} \
${build_nadir_LIBS} \
${build_rostra_LIBS} \
${build_fila_FRAMEWORKS}


