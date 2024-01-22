
#ifndef SERIAL_H
#define SERIAL_H

#define PORT 0x3f8

namespace Serial {
    void initialize();
    int trans_status();
    void write(char ch);
    void write(const char* str);
}

#endif