#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

struct multiboot_tag {
    unsigned int type;
    unsigned int size;
};

struct multiboot_tag_mmap {
    struct multiboot_tag header;
    unsigned int entry_size;
    unsigned int entry_version;
};

struct multiboot_mmap_entry {
    unsigned long base_addr;
    unsigned long length;
    unsigned int type;
    unsigned int reserved;
} __attribute__((packed));

#endif
