//trace.c
//authored by Jared Hull
//
//use the GCC flag -finstrument-functions to enable tracing
//these trace functions will output the pointer to
//the function in which they are called by gcc
//to find the function name, look at the kernel.map

//you must add __attribute__((no_instrument_function))
//to any function you do not want traced
//especially ones which are printing out the trace

#include <video.h>
int depth = 0;

__attribute__((no_instrument_function))
void __cyg_profile_func_enter (void *this_fn, void *call_site){
	if(loaded == 2){
		int intflag;
		__asm volatile ("mrs %0, cpsr" : "=r" (intflag));
		//uspi_EnterCritical();

		depth++;
		char m[depth + 1];
		for(int i = 0; i < depth; i++){
			m[i] = ' ';
		}
		m[depth + 0] = 0; //null termination

		if((intflag & 0x80)){
			m[0] = '#';
			printHex(m, (int)this_fn, 0xFF007F00); //green for call
			//uspi_LeaveCritical();
			return;
		}

		m[0] = '.';
		printHex(m, (int)this_fn, 0xFF00FF00); //green for call
		//uspi_LeaveCritical();
	}
}

__attribute__((no_instrument_function))
void __cyg_profile_func_exit  (void *this_fn, void *call_site){
	if(loaded == 2){
		int intflag;
		__asm volatile ("mrs %0, cpsr" : "=r" (intflag));
		//uspi_EnterCritical();

		char m[depth + 1];
		for(int i = 0; i < depth; i++){
			m[i] = ' ';
		}
		m[depth + 0] = 0; //null termination

		if((intflag & 0x80)){
			m[0] = '#';
			printHex(m, (int)this_fn, 0xFF7F0000); //red for return
			depth--;
			//uspi_LeaveCritical();
			return;
		}

		m[0] = '.';
		printHex(m, (int)this_fn, 0xFFFF0000); //red for return
		depth--;
		//uspi_LeaveCritical();
	}
}