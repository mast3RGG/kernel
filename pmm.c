#include "pmm.h"
#include "mmap.h"
#include "multiboot.h"


static uint8_t *bitmap;
static uint64_t max_blocks;

static void bitmap_set(uint64_t bit) {
    uint64_t byte_index = bit >> 3;
    uint64_t byte_offset = bit & 7;
    bitmap[byte_index] |= (1 << byte_offset);
}

static void bitmap_reset(uint64_t bit) {
    uint64_t byte_index = bit >> 3;
    uint64_t byte_offset = bit & 7;
    bitmap[byte_index] &= ~(1 << byte_offset);
}

uint8_t bitmap_test(uint64_t bit) {
    uint64_t byte_index = bit >> 3;
    uint64_t byte_offset = bit & 7;
    return (bitmap[byte_index] & (1 << byte_offset)) != 0;
}

void pmm_init(uint64_t mboot_addr) {
    uint64_t mem_size = detect_memory(mboot_addr);
    max_blocks = mem_size / 4096;
    uint64_t bitmap_size = max_blocks / 8;

    bitmap = (uint8_t *)0x200000;

    for (uint64_t i = 0; i < bitmap_size; i++) {
        bitmap[i] = 0xFF;
    }

    struct multiboot_tag *tag = (struct multiboot_tag *)(mboot_addr + 8);

    while (tag->type != 0) {
        if (tag->type == 6) {
            struct multiboot_tag_mmap *tag_mmap = (struct multiboot_tag_mmap *)tag;
            uint8_t *base = (uint8_t *)tag + sizeof(struct multiboot_tag_mmap);
            uint32_t offset = 0;

            while (offset < (tag_mmap->header.size - sizeof(struct multiboot_tag_mmap))) {
                struct multiboot_mmap_entry *entry = (struct multiboot_mmap_entry *)(base + offset);

                if (entry->type == 1) {
                    uint64_t start_block = entry->base_addr / 4096;
                    uint64_t num_blocks = entry->length / 4096;

                    for (uint64_t b = start_block; b < start_block + num_blocks; b++) {
                        bitmap_reset(b);
                    }
                }

                offset += tag_mmap->entry_size;
            }
        }
        tag = (struct multiboot_tag *)((uint64_t)tag + ((tag->size + 7) & ~7));
    }

    uint64_t reserved_blocks = (0x200000 + bitmap_size + 4095) / 4096;
    for (uint64_t b = 0; b < reserved_blocks; b++) {
        bitmap_set(b);
    }
}

uint64_t pmm_alloc_block(void) {
    uint64_t *bitmap_64 = (uint64_t *)bitmap;
    uint64_t max_block_64 = (max_blocks + 63) / 64;

    for (uint64_t i = 0; i < max_block_64; i++) {
        if (bitmap_64[i] != 0xFFFFFFFFFFFFFFFFULL) {
            uint64_t free_bits = ~bitmap_64[i];
            uint64_t bit = __builtin_ctzll(free_bits);

            uint64_t block = (i * 64) + bit;
            bitmap_set(block);
            return block * 4096;
        }
    }

    return 0;
}

void pmm_free_block(uint64_t addr) {
    uint64_t block = addr / 4096;
    if (block >= max_blocks) {
        return;
    }
    bitmap_reset(block);
}
