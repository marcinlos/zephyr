# Makefile

SHELL = /bin/sh

prefix = .
exec_prefix = ${prefix}
srcdir = ${prefix}/src
bindir = ${exec_prefix}/bin
example = example
exampledir = ${exec_prefix}/${example}

zephyr = zephyr
zephyrbindir = ${bindir}/${zephyr}


vpath %.cpp ${srcdir} 
vpath %.cpp ${prefix}/${example}

# Insert your program definition(s) here
# Example:
#    PROGRAMS = foo
#    foo_SRCS = fooer.c error_msg.c
#    all_LDFLAGS = -pthread

PROGRAMS = game demo

game_SRCS = 								\
	Zephyr.cpp		 						\
	zephyr/Root.cpp							\
	zephyr/core/Task.cpp					\
	zephyr/core/Scheduler.cpp				\
	zephyr/core/Config.cpp					
	
	
demo_SRCS = example.cpp

all_LDFLAGS = -lglfw -lGLEW

CXXFLAGS= -std=c++11 -O2 -g -Wall

CPPFLAGS+= -I${srcdir}

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

${OBJS}: ${bindir}/%.o: %.cpp | make_dirs
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $^

.PHONY: make_dirs

make_dirs: | ${zephyrbindir}/core ${bindir}/example

${bindir}:
	mkdir $@
	
${zephyrbindir}: | ${bindir}
	mkdir $@
	
${bindir}/example: | ${bindir}
	mkdir $@
	
${zephyrbindir}/core: | ${zephyrbindir}
	mkdir $@
	
.PHONY: clean
clean:
	${RM} ${EXECS} ${OBJS} 
	rm -rf ${bindir}