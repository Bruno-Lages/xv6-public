#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    char *virtual_addr = (char*)0x12345678;
    char *physical_addr = (char*)virt2real(virtual_addr);
    if (physical_addr == 0) {
        printf(1, "virt2real() failed for address %p\n", virtual_addr);
    } else {
        printf(1, "Virtual address %p maps to physical address %p\n", virtual_addr, physical_addr);
    }

    int num_pages_used = num_pages();
    printf(1, "The current process is using %d pages\n", num_pages_used);

    exit();
}