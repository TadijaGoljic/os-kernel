#include "../h/KernelConsole.hpp"
#include "../lib/hw.h"
#include "../h/syscall_c.h"

char KernelConsole::getchar() {

    while (true) {

        if ((*(char *) CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT) == 0) {
            thread_dispatch();
        } else {
            return *(char *) CONSOLE_RX_DATA;
        }

    }

}
