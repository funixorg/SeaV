#include <boot/bootapi/api.h>

extern "C" {
	void _start(void) {
		
		BootAPI::feat_load();
		for (;;) {}
	}
}