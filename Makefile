#################################################
#				Makefile for OS					#
#					系统编译						#
#################################################
entrypoint			= 	0x10000
#------------连接-----------
ld					=	ld
ld_flags			=	-s -Ttext $(entrypoint) -Map bin/kernel.map
#---------最终目标----------
image		= 	bin/kernel.img
#------------目标-----------
boot		=	bin/boot.bin bin/loader.bin
sub_dir		=	arch
# micro-kernel kernel kernel-lib lib
objs 		:= $(foreach path,$(sub_dir),bin/$(path).o)

.PHONY : clean all finalclean

# 编译
all : finalclean $(objs) $(boot) $(image)

	@echo "linking kernel..."
	@$(ld) $(ld_flags) $(objs) -o bin/kernel.bin
	
	@echo "Writing image..."
	@dd conv=notrunc of=bin/kernel.img bs=512 count=1 if=bin/boot.bin 
	@dd conv=notrunc of=bin/kernel.img bs=512 count=2 if=bin/loader.bin seek=1
	@dd conv=notrunc of=bin/kernel.img bs=512 count=16 if=bin/kernel.bin seek=3
	

# 循环调用子目录中的makefile	
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
	
# 清理	
finalclean:
	@rm -f bin/*.o
	@rm -f bin/*.bin
	
# 循环清理各个子目录
clean :
	@make clean -s -C boot
	@for dir in $(sub_dir); \
		do make clean -s -C $$dir; \
		echo "cleaning "$$dir"...";\
	done


