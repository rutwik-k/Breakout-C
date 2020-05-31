#ifndef PLATFORM_H
#define PLATFORM_H

#include "types.h"
#include "key_list.h"

struct Platform {
    /* Window */
    u32 width;
    u32 height;
    
    /* Options */
    b32 should_quit;
    b32 is_initialised;
    b32 vsync;
    
    /* Memory */
    void *permanent_memory; //lasts for the whole app duration
    u32 permanent_memory_size;
    
    void *frame_memory; //lasts one frame
    u32 frame_memory_size;
    
    /* Timing */
    f32 current_time;
    f32 target_fps;
    f32 dt;
    f32 ms_per_frame;
    f32 fps_per_frame;
    
    /* Input Handling */
    b32 key_down[KEY_MAX];
    b32 left_mouse_down;
    b32 left_mouse_pressed;
    b32 right_mouse_down;
    b32 clicked;
    f32 cursor_x;
    f32 cursor_y;
    
    /* File Handling */
    void *(*PlatformReadEntireFileToString)(char *filename);
    b32   (*PlatformWriteToFile)(char *filename, u64 data_size, void *data);
    void  (*PlatformFreeFile)(void *data);
    
    /* Error Handling */
    void  (*PlatformDisplayError)(char *title, char *info);
    
};

global Platform platform = {};

#endif //PLATFORM_H
