#include "mmmap.h"
#include "multiboot.h"

unsigned long detect_memory(unsigned long mboot_addr) {
    struct multiboot_tag *multiboot_tag = (struct multiboot_tag *)(mboot_addr + 8);
    unsigned long memorie_libera = 0;

    while (multiboot_tag->type != 0) {
        if (multiboot_tag->type == 6) {
            struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)multiboot_tag;
            unsigned char *base = (unsigned char *)multiboot_tag + sizeof(struct multiboot_tag_mmap);
            unsigned int offset = 0;

            while (offset < (mmap->header.size - sizeof(struct multiboot_tag_mmap))) {
                struct multiboot_mmap_entry *entry = (struct multiboot_mmap_entry *)(base + offset);

                if (entry->type == 1) {
                    memorie_libera += entry->length;
                }

                offset += mmap->entry_size;
            }
            break;
        } else {
            multiboot_tag = (struct multiboot_tag *)((unsigned long)multiboot_tag + ((multiboot_tag->size + 7) & ~7));
        }
    }

    return memorie_libera;
}
