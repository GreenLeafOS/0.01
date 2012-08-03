asm					=	as
asm_flags			=	-I ../lib/include/

cc					=	gcc
cc_flags			=	-I include/ -c

cpp					=	g++
cpp_flags			=	-I ../kernel/include -I ../micro-kernel/include/ \ 
						-I ../lib/include -I ../kernel-lib/include \
						-I ../arch/include \
						-c -fno-rtti \
						-Wwrite-strings

ld					=	ld
ld_flags			=	-r


objs		+=	$(cpp_objs) $(cc_objs) $(asm_objs) 

.PHONY : clean


$(final_obj) : $(objs)
	$(ld) $(ld_flags) $(objs) -o $@	

$(cc_objs): %.o : %.c
	$(cc) $(cc_flags) $< -o $@

$(cpp_objs): %.o : %.cc
	$(cpp) $(cpp_flags) $< -o $@

$(asm_objs): %.a.o : %.asm
	$(asm) $(asm_flags) $< -o $@
	
	
clean :
	-rm -f $(objs)
	-rm -f $(final_obj)
