#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/democlient-callbacks.o \
	${OBJECTDIR}/democlient-backend.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/democlient-interface.o \
	${OBJECTDIR}/democlient-support.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/democlientv2

dist/Debug/GNU-Linux-x86/democlientv2: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.c} `pkg-config --libs gstreamer-0.10 gtk+-2.0 dbus-glib-1` -lgstinterfaces-0.10 -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/democlientv2  ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/democlient-callbacks.o: nbproject/Makefile-${CND_CONF}.mk democlient-callbacks.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/lib/glib-2.0/include -I/usr/include/glib-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/pango-1.0 -I/usr/include/libpng -I/usr/include/gstreamer-0.10 -I/usr/lib/gstreamer-0.10 -I/usr/include/libxml2 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/democlient-callbacks.o democlient-callbacks.c

${OBJECTDIR}/democlient-backend.o: nbproject/Makefile-${CND_CONF}.mk democlient-backend.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/lib/glib-2.0/include -I/usr/include/glib-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/pango-1.0 -I/usr/include/libpng -I/usr/include/gstreamer-0.10 -I/usr/lib/gstreamer-0.10 -I/usr/include/libxml2 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/democlient-backend.o democlient-backend.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/lib/glib-2.0/include -I/usr/include/glib-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/pango-1.0 -I/usr/include/libpng -I/usr/include/gstreamer-0.10 -I/usr/lib/gstreamer-0.10 -I/usr/include/libxml2 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/democlient-interface.o: nbproject/Makefile-${CND_CONF}.mk democlient-interface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/lib/glib-2.0/include -I/usr/include/glib-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/pango-1.0 -I/usr/include/libpng -I/usr/include/gstreamer-0.10 -I/usr/lib/gstreamer-0.10 -I/usr/include/libxml2 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/democlient-interface.o democlient-interface.c

${OBJECTDIR}/democlient-support.o: nbproject/Makefile-${CND_CONF}.mk democlient-support.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/lib/glib-2.0/include -I/usr/include/glib-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/freetype2 -I/usr/include/pango-1.0 -I/usr/include/libpng -I/usr/include/gstreamer-0.10 -I/usr/lib/gstreamer-0.10 -I/usr/include/libxml2 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/democlient-support.o democlient-support.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/democlientv2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
