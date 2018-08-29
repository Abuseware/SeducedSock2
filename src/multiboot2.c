#include <stdint.h>
#include <stddef.h>

#include <bochs.h>
#include <util.h>

#include <multiboot2.h>

static struct multiboot_tag *mb_table[22];

uint64_t MultibootInit(void) {
  uint64_t counter = 0;

  BochsPuts("Filling array with tags\n");

  for(struct multiboot_tag *tag = ((void *)mb_info + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *)((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
    BochsPuts("Found tag ");
    BochsPuti(tag->type);
    BochsPutc('\n');
    mb_table[tag->type] = tag;
    counter++;
  }

  BochsPuts("Total number of tags: ");
  BochsPuti(counter);
  BochsPutc('\n');
  return counter;
}

struct multiboot_tag *MultibootGetTag(multiboot_uint16_t type) {
  if(mb_table[type] == NULL) {
    BochsPuts("Tag ");
    BochsPuti(type);
    BochsPuts(" not found in table\n");
    return NULL;
  }

  return mb_table[type];
}

struct multiboot_tag *MultibootSearchTag(multiboot_uint16_t type) {
  BochsPuts("Searching MB2 tag: ");
  BochsPuti(type);
  BochsPutc('\n');

  for(struct multiboot_tag *tag = ((void *)mb_info + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *)((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
    if(tag->type == type) {
      BochsPuts("Found!\n");
      return tag;
    }
  }

  BochsPuts("Not found!\n");
  return 0;
}
