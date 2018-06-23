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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/source/AdjacencyList.o \
	${OBJECTDIR}/source/AdjacencyListSchemaHashedTableManager.o \
	${OBJECTDIR}/source/AdjacencyListUniversalTableManager.o \
	${OBJECTDIR}/source/AdjacencyMatrix.o \
	${OBJECTDIR}/source/AdjacencyMatrixUniversalTableManager.o \
	${OBJECTDIR}/source/SchemaHashedTable.o \
	${OBJECTDIR}/source/Test.o \
	${OBJECTDIR}/source/UniversalTable.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++17
CXXFLAGS=-std=c++17

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/graphstorage.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/graphstorage.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/graphstorage ${OBJECTFILES} ${LDLIBSOPTIONS} -lstdc++fs

${OBJECTDIR}/source/AdjacencyList.o: source/AdjacencyList.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/AdjacencyList.o source/AdjacencyList.cpp

${OBJECTDIR}/source/AdjacencyListSchemaHashedTableManager.o: source/AdjacencyListSchemaHashedTableManager.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/AdjacencyListSchemaHashedTableManager.o source/AdjacencyListSchemaHashedTableManager.cpp

${OBJECTDIR}/source/AdjacencyListUniversalTableManager.o: source/AdjacencyListUniversalTableManager.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/AdjacencyListUniversalTableManager.o source/AdjacencyListUniversalTableManager.cpp

${OBJECTDIR}/source/AdjacencyMatrix.o: source/AdjacencyMatrix.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/AdjacencyMatrix.o source/AdjacencyMatrix.cpp

${OBJECTDIR}/source/AdjacencyMatrixUniversalTableManager.o: source/AdjacencyMatrixUniversalTableManager.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/AdjacencyMatrixUniversalTableManager.o source/AdjacencyMatrixUniversalTableManager.cpp

${OBJECTDIR}/source/SchemaHashedTable.o: source/SchemaHashedTable.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/SchemaHashedTable.o source/SchemaHashedTable.cpp

${OBJECTDIR}/source/Test.o: source/Test.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/Test.o source/Test.cpp

${OBJECTDIR}/source/UniversalTable.o: source/UniversalTable.cpp
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iheader -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/UniversalTable.o source/UniversalTable.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
