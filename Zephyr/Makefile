# Makefile

SHELL = /bin/sh

prefix = .
exec_prefix = ${prefix}
srcdir = ${prefix}/src
bindir = ${exec_prefix}/bin

vpath %.cpp ${srcdir} 

SLICE = slice2cpp

# Insert your program definition(s) here
# Example:
#    PROGRAMS = foo
#    foo_SRCS = fooer.c error_msg.c
#    all_LDFLAGS = -pthread

PROGRAMS = zephyr

zephyr_SRCS = Zephyr.cpp

all_LDFLAGS =

CXXFLAGS= -O2 -g -Wall

# End of program definitions 

SRCS = ${foreach prog,${PROGRAMS},${${prog}_SRCS}} 

define name_to_OBJS
${1}_OBJS = $${${1}_SRCS:%.cpp=$${bindir}/%.o}
endef

${foreach prog,${PROGRAMS},${eval ${call name_to_OBJS,${prog}}}}
OBJS = ${sort ${foreach prog,${PROGRAMS},${${prog}_OBJS}}}

EXECS = ${addprefix ${bindir}/,${PROGRAMS}}


.PHONY: all
all: ${EXECS}
	echo ${CXX}

.SECONDEXPANSION:
${EXECS}: ${bindir}/%: $${$$*_OBJS}
	${CXX} -o $@ $^ ${all_LDFLAGS} ${$*_LDFLAGS}

${OBJS}: ${bindir}/%.o: %.cpp | ${bindir} 
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $^

${bindir}:
	mkdir ${bindir}
	
${gendir}:
	mkdir ${gendir}
  
.PHONY: clean
clean:
	${RM} ${EXECS} ${OBJS}