#include <stdarg.h>
#include "mmap.h"
#include "paging.h"
#include "pmm.h"
#include "mystdio.h"
unsigned long __stack_chk_guard = 0x123456789ABCDEF0;















void __stack_chk_fail(void) {
    clean_screen();
    vga_print_string("KERNEL PANIC: Stack smashing detected!");
    while (1);
}


__attribute__((noinline)) void purice_stiva(volatile char *p) {
    for (int i = 0; i < 64; i++) {
        p[i] = 'A';
    }
}

__attribute__((noinline)) void trigger_stack_crash() {
    volatile char buf[8];
    purice_stiva(buf);
}

void trigger_page_fault(void) {
    volatile unsigned long *ptr = (volatile unsigned long *)0x0000123456789000;
    volatile unsigned long val = *ptr;
    (void)val;
}

void test_memory(uint64_t mboot_addr) {
  unsigned long adresa_test = 0xB8000;

  kprintf("Adresa: 0x%x\n", adresa_test);
  kprintf("PML4 Index: %d\n", PML4_INDEX(adresa_test));
  kprintf("PDPT Index: %d\n", PDPT_INDEX(adresa_test));
  kprintf("PD Index:   %d\n", PD_INDEX(adresa_test));
  kprintf("PT Index:   %d\n", PT_INDEX(adresa_test));
  kprintf("Offset:     0x%x\n", PAGE_OFFSET(adresa_test));
  kprintf("\n");
  unsigned free_memory = detect_memory(mboot_addr);
  kprintf ("Free memory in ram is : %d" , free_memory / 1024 / 1024);

  
  
}

void test_pmm(void) {
    kprintf("[PMM TEST] Starting PMM verification...\n");

    uint64_t a = pmm_alloc_block();
    uint64_t b = pmm_alloc_block();
    uint64_t c = pmm_alloc_block();

    kprintf("[PMM TEST] Block A: 0x%x\n", a);
    kprintf("[PMM TEST] Block B: 0x%x\n", b);
    kprintf("[PMM TEST] Block C: 0x%x\n", c);

    if (b == a + 4096 && c == b + 4096) {
        kprintf("[PMM TEST] SUCCESS: Sequential allocation (+4096 bytes).\n");
    } else {
        kprintf("[PMM TEST] FAIL: Allocation is not sequential!\n");
    }

    pmm_free_block(b);
    kprintf("[PMM TEST] Freed Block B (0x%x)\n", b);

    uint64_t d = pmm_alloc_block();
    kprintf("[PMM TEST] Block D: 0x%x\n", d);

    if (d == b) {
        kprintf("[PMM TEST] SUCCESS: Reused freed memory correctly!\n");
    } else {
        kprintf("[PMM TEST] FAIL: Did not reuse Block B!\n");
    }

    pmm_free_block(a);
    pmm_free_block(c);
    pmm_free_block(d);

    kprintf("[PMM TEST] Test completed successfully.\n");
}

void kernel_main(uint64_t mboot_addr) {
  clean_screen();
  pmm_init(mboot_addr);
  test_pmm ();
  
  
  while (1) {

    
  }
}
