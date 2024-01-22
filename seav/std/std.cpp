#include <std/stdio.h>
#include <core/serial/serial.h>
#include <std/stdlib.h>

#include <stdarg.h>

namespace std {
    void reverse(char str[], int length) {
        int start = 0;
        int end = length - 1;
        while (start < end) {
            char temp = str[start];
            str[start] = str[end];
            str[end] = temp;
            start++;
            end--;
        }
    }

    void itoa(int num, char str[], int base) {
        int i = 0;
        int is_negative = 0;
        if (num == 0) {
            str[i++] = '0';
            str[i] = '\0';
            return;
        }
        if (num < 0 && base != 10) {
            is_negative = 1;
            num = -num;
        }
        while (num != 0) {
            int remainder = num % base;
            str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
            num = num / base;
        }
        if (is_negative && base != 10) {
            str[i++] = '-';
        }
        str[i] = '\0';
        std::reverse(str, i);
    }

    namespace SerialIO {
        void printf(const char *format, ...) {
            va_list args;
            va_start(args, format);

            while (*format != '\0') {
                if (*format == '%') {
                    format++;

                    switch (*format) {
                        case 'd': {
                            int value = va_arg(args, int);
                            char buffer[16];
                            std::itoa(value, buffer, 10);
                            Serial::write(buffer);
                            break;
                        }
                        case 's': {
                            const char *str = va_arg(args, const char *);
                            Serial::write(str);
                            break;
                        }
                        case 'x': {
                            int value = va_arg(args, int);
                            char buffer[16];
                            std::itoa(value, buffer, 16);
                            Serial::write("0x");
                            Serial::write(buffer);
                            break;
                        }
                        case 'c': {
                            char value = va_arg(args, int);
                            char buffer[2];
                            buffer[0]=value;
                            buffer[1]='\0';
                            Serial::write(buffer);
                            break;
                        }
                        default:
                            Serial::write('%');
                            Serial::write(*format);
                    }
                } else {
                    Serial::write(*format);
                }

                format++;
            }

            va_end(args);
        }
    }
}