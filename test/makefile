###################################
#	@file
#	@author Florian Evaldsson
#	@version 0.1
#	
#	@section LICENSE
#	
#	Copyright (c) 2015 Florian Evaldsson <florian.evaldsson@telia.com>
#
#
#	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
#	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#	
#	@section DESCRIPTION
#
#	Makefile for the statistik library
#
###################################

CC=gcc

OUT=main

CFLAGS =-I./../src/
CFLAGS += -std=c99 
CFLAGS += -g -Wall

LDFLAGS = -lm

all:
	$(CC) main.c -o $(OUT) $(LDFLAGS) $(CFLAGS) -DNO_LINKING

run: all
	./$(OUT)
	
noalloc:
	$(CC) main.c -o $(OUT) $(LDFLAGS) $(CFLAGS) -DNO_LINKING -DNO_ALLOC

runnoalloc: noalloc
	./$(OUT)

valgrind: all
	valgrind  --track-origins=yes --tool=memcheck ./$(OUT)
	
noallocvalgrind: noalloc
	valgrind  --track-origins=yes --tool=memcheck ./$(OUT)
