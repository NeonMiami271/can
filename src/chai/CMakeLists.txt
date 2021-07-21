cmake_minimum_required(VERSION 3.9)
project(chai)

include_directories(
	./
	../
	../../include
	../../lib
)

set(sources
	chai-cbunp.c
	chai-lnx.c
	# bmarathon.c
	# boards-conf.c
	# boards.c
	# sja1000.c
	# statstr.c
	# sysdep.c
	# unicandrv-lnx.c
  unican.h
)

find_library(PTHREAD_LIBRARY pthread)

add_definitions(-DLINUX)

add_library(chai SHARED ${sources})

target_link_libraries(chai ftd2xx ${PTHREAD_LIBRARY})
