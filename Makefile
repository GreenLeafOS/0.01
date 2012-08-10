#################################################
#				Makefile for OS					#
#					ϵͳ����						#
#################################################
entrypoint			= 	0x100000
memoryinfo			=	0x15000
#------------����-----------
ld					=	ld
ld_flags			=	-s -Ttext $(entrypoint) -section-start memory=$(memoryinfo) -Map bin/kernel.map
#---------����Ŀ��----------
image		= 	bin/kernel.img
#------------Ŀ��-----------
boot		=	bin/boot.bin bin/loader.bin
sub_dir		=	arch micro-kernel lib
#  kernel
objs 		:= $(foreach path,$(sub_dir),bin/$(path).o)

.PHONY : clean all finalclean

# ����
all : finalclean $(objs) $(boot) $(image)

	@echo "linking kernel..."
	@$(ld) $(ld_flags) $(objs) -o bin/kernel.bin
	
	@echo "Writing image..."
	@dd conv=notrunc of=bin/kernel.img bs=512 count=1 if=bin/boot.bin 
	@dd conv=notrunc of=bin/kernel.img bs=512 count=2 if=bin/loader.bin seek=1
	@dd conv=notrunc of=bin/kernel.img bs=512 count=16 if=bin/kernel.bin seek=3
	

# ѭ��������Ŀ¼�е�makefile	
$(objs):bin/%.o : %
	make -s -C $<
	
# boot
$(boot):
	@echo "making boot..."
	@make -s -C boot
	
# image
$(image):
	@echo "Creating image..."
	@dd if=/dev/zero of=$(image) bs=512 count=2880
	
# ����	
finalclean:
	@rm -f bin/*.o
	@rm -f bin/*.bin
	
# ѭ������������Ŀ¼
clean :
	@make clean -s -C boot
	@for dir in $(sub_dir); \
		do make clean -s -C $$dir; \
		echo "cleaning "$$dir"...";\
	done

