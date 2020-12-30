#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _c_string_view
    {
        const char* str;
        int size;
    } c_string_view;

#ifdef __cplusplus
}
#endif