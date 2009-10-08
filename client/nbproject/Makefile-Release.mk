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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/gst-ipcam-client-interface.o \
	${OBJECTDIR}/gst-ipcam-client-backend.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/gst-ipcam-client-callbacks.o \
	${OBJECTDIR}/gst-ipcam-client-windowid.o \
	${OBJECTDIR}/gst-ipcam-client-support.o

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
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/client

dist/Release/GNU-Linux-x86/client: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/client ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/gst-ipcam-client-interface.o: nbproject/Makefile-${CND_CONF}.mk gst-ipcam-client-interface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/gst-ipcam-client-interface.o gst-ipcam-client-interface.c

${OBJECTDIR}/gst-ipcam-client-backend.o: nbproject/Makefile-${CND_CONF}.mk gst-ipcam-client-backend.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/gst-ipcam-client-backend.o gst-ipcam-client-backend.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/gst-ipcam-client-callbacks.o: nbproject/Makefile-${CND_CONF}.mk gst-ipcam-client-callbacks.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/gst-ipcam-client-callbacks.o gst-ipcam-client-callbacks.c

${OBJECTDIR}/gst-ipcam-client-windowid.o: nbproject/Makefile-${CND_CONF}.mk gst-ipcam-client-windowid.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/gst-ipcam-client-windowid.o gst-ipcam-client-windowid.c

${OBJECTDIR}/gst-ipcam-client-support.o: nbproject/Makefile-${CND_CONF}.mk gst-ipcam-client-support.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/gst-ipcam-client-support.o gst-ipcam-client-support.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/client

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
