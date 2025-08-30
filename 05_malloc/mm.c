/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * In this version of malloc, we're using Segregated Free Lists method manage
 * our blocks.
 *
 *
 *
 * author: abb00717
 */

/*
 *  Design Approach: Segregated Fits
 *
 *  # Segregated Lists
 *  Size of the list:
 *      Power of 2:
 *      1, 2, 4, 8, 16, 32, ...
 *
 *  # Block Structure
 *  min = 16 bytes
 *
 *  ## Free Block
 *  | Block Size | Allocate | (WSIZE)
 *  (ptr)
 *  | Previous Free Block   | (WSIZE)
 *  | Next Free Block       | (WSIZE)
 *  | Foot                  | (WSIZE)
 *
 *  ## Allocated Block
 *  | Block Size | Allocate | (WSIZE)
 *  (ptr)
 *  | Payload               | (size)
 *
 *  ## Heap
 *  Prologue Blocks | ... | Epilogue Block
 *    8/1   |  8/1  | ... | 0/1
 *          ^
 *      heap_listp
 */

#include "mm.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/

team_t team = {
    /* Team name */
    "ABB00717",
    /* First member's full name */
    "abb00717",
    /* First member's email address */
    "abb00717@gmail.com",
    /* Second member's full name */
    "",
    /* Second member's email address */
    "",
};

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE 4             /* Word and header/footer size (bytes) */
#define DSIZE 8             /* Double word size (bytes) */
#define ALIGNMENT 8         /* single word (4) or double word (8) alignment */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define LISTS_SIZE 20

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
/* $end mallocmacros */

enum ALLOCATION {
    FREE = 0,
    ALLOCATED = 1,
};

static char *heap_listp = 0;

/*
 *  mm_init - initialize the malloc package.
 *  Returns 0 if successful and -1 otherwise;
 */
int mm_init(void) {
    if ((heap_listp = mem_sbrk((4 + LISTS_SIZE) * WSIZE)) == (void *)-1) {
        return -1;
    }

    // init free lists
    for (int i = 0; i < LISTS_SIZE; i++) {
        PUT(heap_listp + i * WSIZE, (unsigned int)NULL);
    }

    /* Prologue Block */
    PUT(heap_listp + LISTS_SIZE * WSIZE, 0);
    PUT(heap_listp + (LISTS_SIZE + 1) * WSIZE, PACK(DSIZE, ALLOCATED));
    PUT(heap_listp + (LISTS_SIZE + 2) * WSIZE, PACK(DSIZE, ALLOCATED));
    /* Epilogue Block */
    PUT(heap_listp + (LISTS_SIZE + 3) * WSIZE, PACK(0, ALLOCATED));
    heap_listp += (LISTS_SIZE + 2) * WSIZE;

    /* Extend the empty heap wiith a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) return -1;

    return 0;
}

/*
 *  mm_malloc - Allocate a block by incrementing the brk pointer.
 *  Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    if (size == 0) return NULL;

    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap */
    char *bp;

    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);

    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    // if there's no suitable free block for a given size in the heap
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL) return NULL;
    place(bp, asize);
    return bp;
}

/*
 *  mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
    size_t size = GET_SIZE(HDRP(ptr));

    // set the allocatedd bit to free
    PUT(HDRP(ptr), PACK(size, FREE));
    PUT(FTRP(ptr), PACK(size, FREE));
    coalesce(ptr);
}

/*
 *  mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
    if (ptr == NULL) return mm_malloc(size);
    if (size == 0) {
        mm_free(ptr);
        return 0;
    }

    size_t old_size = GET_SIZE(HDRP(ptr));
    if (old_size > size) size = old_size;

    char *new_ptr = mm_malloc(size);
    memcpy(new_ptr, ptr, old_size);
    mm_free(ptr);
    return new_ptr;
}

/*
 *  extend_heap - extend the heap by requesting more memory from the OS
 */
void *extend_heap(size_t words) {
    /* Round up to nearest DSIZE */
    size_t size = (words % 2) == 1 ? (words + 1) * WSIZE : words * WSIZE;

    /* Requests additional heap space from memory system */
    char *bp;
    if ((long)(bp = mem_sbrk(size)) == -1) return NULL;

    /* Initialize header/footer */
    PUT(HDRP(bp), PACK(size, FREE));
    PUT(FTRP(bp), PACK(size, FREE));
    /* Don't forget to put the new epilogue header */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, ALLOCATED));

    return coalesce(bp);
}

/*
 *  coalesce - merge the block with the adjacent free blocks
 */
void *coalesce(void *ptr) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HDRP(ptr));

    if (prev_alloc && next_alloc) return ptr;

    if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(ptr), PACK(size, FREE));
        // FTRP reference to HDRP, since we've changed the size in HDRP, there's
        // no need to use NEXT_BLKP
        PUT(FTRP(ptr), PACK(size, FREE));
    } else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, FREE));
        PUT(FTRP(ptr), PACK(size, FREE));
        ptr = PREV_BLKP(ptr);
    } else if (!prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, FREE));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, FREE));
        ptr = PREV_BLKP(ptr);
    }

    return ptr;
}

/*
 *  place - splits a free block into an allocated block and a new free block
 */
extern void place(void *ptr, size_t size) {
    size_t remain_size = GET_SIZE(HDRP(ptr)) - size;

    if (remain_size >= 2 * DSIZE) {
        PUT(HDRP(ptr), PACK(size, ALLOCATED));
        PUT(FTRP(ptr), PACK(size, ALLOCATED));
        // ptr points to the remaining free block
        ptr = NEXT_BLKP(ptr);
        PUT(HDRP(ptr), PACK(remain_size, FREE));
        PUT(FTRP(ptr), PACK(remain_size, FREE));
    } else {  // the remaining free block isn't large enough
        PUT(HDRP(ptr), PACK(GET_SIZE(HDRP(ptr)), ALLOCATED));
        PUT(FTRP(ptr), PACK(GET_SIZE(HDRP(ptr)), ALLOCATED));
    }
}

/*
 *  find_fit - first fit
 */
extern void *find_fit(size_t size) {
    char *bp;
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && size <= GET_SIZE(HDRP(bp))) return bp;
    }

    return NULL;
}
