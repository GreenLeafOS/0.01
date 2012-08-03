#################################################
#				Makefile for Boot				#
#					∆Ù∂Ø±‡“Î						#
#################################################
.PHONY: all clean finalclean

all: finalclean
	as boot.s -o boot.o
	as loader.s -o loader.o
	ld boot.o -Ttext 0x7c00 --oformat=binary -o ../bin/boot.bin
	ld loader.o -Ttext 0x8000 --oformat=binary -o ../bin/loader.bin
	
clean: finalclean
	-rm -f boot.o
	-rm -f loader.o
	
finalclean:
	-rm -f ../bin/boot.bin
	-rm -f ../bin/loader.bin