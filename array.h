#ifndef __ARRAY_H__
#define __ARRAY_H__

// once alloc, size will never change
struct array {
    char *data;
    int   sizeof_type;
    int   size;
    int   pos;
};

#endif