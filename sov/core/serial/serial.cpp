#include <core/serial/serial.h>
#include <core/serial/portio.h>

#include <stdarg.h>

namespace Serial {
    void initialize() {
        PortIO::outb(PORT + 1, 0x00);   
        PortIO::outb(PORT + 3, 0x80);   
        PortIO::outb(PORT + 0, 0x03); 
        PortIO::outb(PORT + 1, 0x00);  
        PortIO::outb(PORT + 3, 0x03); 
        PortIO::outb(PORT + 2, 0xC7);   
        PortIO::outb(PORT + 4, 0x0B);  
        PortIO::outb(PORT + 4, 0x1E);   
        PortIO::outb(PORT + 0, 0xAE);    
        if(PortIO::inb(PORT + 0) != 0xAE) {
            return;
        }
        PortIO::outb(PORT + 4, 0x0F);
    }

    int trans_status() {
        return PortIO::inb(PORT + 5) & 0x20;
    }

    void write(char ch) {
        while (Serial::trans_status() == 0);

        PortIO::outb(PORT, ch);
    }

    void write(const char* str) {
        while (*str) {
            Serial::write(*str);
            str++;
        }
        Serial::write('\0');
    }
}