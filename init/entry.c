#include "console.h"
#include "debug.h"

int kern_entry()
{
    init_debug();

    console_clear();


    printk("Hello , Sirius\n");

    panic("test");

    return 0;
}
