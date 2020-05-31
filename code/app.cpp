#include "app.h"

/* Given a string of data, loads a level */
internal Level LoadLevel(char *level_data){
    Level level = {};
    u32 blocks = 0;
    u32 lines = 1;
    u32 index = 0;
    u32 state[1024];
    u32 state_counter = 0;
    while(level_data[index] != '\0'){
        if(level_data[index] == '\n') {
            index++;
            lines++;
            continue;
        }
        else if(level_data[index] - '0' >= 0){
            blocks++;
            state[state_counter] = level_data[index] - '0';
            state_counter++;
        }
        index++;
    }
    level.blocks = blocks;
    level.positions = (Vec2 *)MemoryArenaAllocate(&app->permanent_arena, blocks * sizeof(Vec2));
    level.scales = (Vec2 *)MemoryArenaAllocate(&app->permanent_arena, blocks * sizeof(Vec2));
    level.colors = (Vec4 *)MemoryArenaAllocate(&app->permanent_arena, blocks * sizeof(Vec4));
    level.state = (u32 *)MemoryArenaAllocate(&app->permanent_arena, blocks * sizeof(b32));
    
    i32 blocks_per_row = blocks / lines;
    i32 blocks_per_column = blocks / blocks_per_row;
    i32 offset = 0;
    
    for(int i = 0; i < blocks; i++){
        i32 row_pos = i % blocks_per_row;
        i32 column_pos = i / blocks_per_row;
        Vec2 desired_scale = Vec2((PROGRAM_WIDTH - (offset * blocks_per_row)) / blocks_per_row, ((PROGRAM_HEIGHT / 2) - offset) / blocks_per_column);
        level.scales[i] = desired_scale;
        level.positions[i] = Vec2((row_pos * level.scales[i].x) + (row_pos * offset), PROGRAM_HEIGHT - desired_scale.y - ((column_pos * level.scales[i].y) + (column_pos * offset)));
        level.colors[i] = Vec4(((f32)row_pos + 1) / (f32)blocks_per_row, ((f32)column_pos + 1) / (f32)blocks_per_column, 0.3f, 1);
        level.state[i] = state[i];
    }
    return level;
}

internal void InitLevels(){
    app->level_manager = {};
    app->level_manager.count = 2;
    app->level_manager.levels = (Level *)MemoryArenaAllocate(&app->permanent_arena, app->level_manager.count * sizeof(Level));
    app->level_manager.levels[0] = LoadLevel((char *)app_platform->PlatformReadEntireFileToString("level1.data"));
    app->level_manager.levels[1] = LoadLevel((char *)app_platform->PlatformReadEntireFileToString("level2.data"));
    
    app->level_manager.current_level = app->level_manager.levels[1];
}

internal void InitApp(){
    app = (App *)app_platform->permanent_memory;
    app->permanent_arena = MemoryArenaInit((u8 *)app_platform->permanent_memory + sizeof(App), app_platform->permanent_memory_size - sizeof(App));
    RendererInit(&app->renderer);
    app->font = InitFont("game_font");
    
    app->player = {};
    app->player.pos = Vec2(0, 10);
    app->player.scale = Vec2(128, 24);
    app->player.color = Vec4(0, 1, 0, 1);
    
    app->ball = {};
    app->ball.scale = Vec2(16, 16);
    app->ball.pos = Vec2((PROGRAM_WIDTH / 2) - (app->ball.scale.x / 2), (PROGRAM_HEIGHT / 2 - 50) - (app->ball.scale.y / 2));
    app->ball.color = Vec4(1, 0, 0, 1);
    app->ball.velocity.y = -1;
    app->ball.speed = 500;
    InitLevels();
}

internal void UpdateApp(Platform *_platform){
    app_platform = _platform;
    if(app_platform->is_initialised == 0){
        InitApp();
        app_platform->dt = 1 / app_platform->target_fps;
        app_platform->is_initialised = 1;
    }
    
    if(app_platform->left_mouse_down){
        app->playing = 1;
    }
    
    
    /* Gets relative cursor for the viewport */
    Vec2 relative_cursor = Vec2((app_platform->cursor_x) * ((f32)PROGRAM_WIDTH / (f32)app_platform->width), (app_platform->cursor_y) * ((f32)PROGRAM_HEIGHT / (f32)app_platform->height));
    
    Vec2 previous_player_pos;
    previous_player_pos = app->player.pos;
    
    app->player.pos.x = relative_cursor.x;
    if(app->player.pos.x < 0) app->player.pos.x = 0;
    if(app->player.pos.x > (PROGRAM_WIDTH - app->player.scale.x)) app->player.pos.x = PROGRAM_WIDTH - app->player.scale.x;
    
    
    if(app->playing){
        
        /* Update Ball */
        app->ball.pos.y += app->ball.velocity.y * app_platform->dt * app->ball.speed;
        app->ball.pos.x += app->ball.velocity.x * app_platform->dt * app->ball.speed;
        
        {   /* Ball Collisions */
            if(Vec4Intersects(Vec4(app->player.pos.x, app->player.pos.y, app->player.scale.x, app->player.scale.y), Vec4(app->ball.pos.x, app->ball.pos.y, app->ball.scale.x, app->ball.scale.y))){
                f32 paddle_center_x = app->player.pos.x + (app->player.scale.x / 2);
                f32 ball_center_x = app->ball.pos.x + (app->ball.scale.x / 2);
                f32 relative_intersect_x = paddle_center_x - ball_center_x;
                f32 normalised_intersect_x = relative_intersect_x / (app->player.scale.x / 2);
#if 0
                f32 bounce_angle = normalised_intersect_x * MAX_BOUNCE_ANGLE;
                
                app->ball.velocity.y = cos(bounce_angle);
                app->ball.velocity.x = -sin(bounce_angle);
#endif
                
                app->ball.velocity.y *= -1;
                app->ball.velocity.x = -normalised_intersect_x;
                
                char log[256];
                snprintf(log, 256, "%f\n", normalised_intersect_x);
                OutputDebugString(log);
            }
            
            /* Wall Collisions */
            if(Vec4Intersects(Vec4(PROGRAM_WIDTH, 0, 10, PROGRAM_HEIGHT), Vec4(app->ball.pos.x, app->ball.pos.y, app->ball.scale.x, app->ball.scale.y)) || Vec4Intersects(Vec4(-10, 0, 10, PROGRAM_HEIGHT), Vec4(app->ball.pos.x, app->ball.pos.y, app->ball.scale.x, app->ball.scale.y))){
                app->ball.velocity.x *= -1;
            }
            if(Vec4Intersects(Vec4(-10, PROGRAM_HEIGHT, PROGRAM_WIDTH, 10), Vec4(app->ball.pos.x, app->ball.pos.y, app->ball.scale.x, app->ball.scale.y))){
                app->ball.velocity.y *= -1;
            }
            
            /* Level Collisions */
            for(int i = 0; i < app->level_manager.current_level.blocks; i++){
                if(Vec4Intersects(Vec4(app->level_manager.current_level.positions[i].x, app->level_manager.current_level.positions[i].y, app->level_manager.current_level.scales[i].x - 8, app->level_manager.current_level.scales[i].y - 8), Vec4(app->ball.pos.x, app->ball.pos.y, app->ball.scale.x, app->ball.scale.y)) &&    app->level_manager.current_level.state[i] == SOLID){
                    OutputDebugString("Hit!\n");
                    /* Destroy Block */
                    app->level_manager.current_level.state[i] = EMPTY;
                    /* Apply collision to ball */
                    f32 ball_right = app->ball.pos.x + app->ball.scale.x;
                    f32 block_right = app->level_manager.current_level.positions[i].x + app->level_manager.current_level.scales[i].x;
                    f32 ball_top = app->ball.pos.y + app->ball.scale.y;
                    f32 block_top = app->level_manager.current_level.positions[i].y + app->level_manager.current_level.scales[i].y;
                    
                    f32 bottom_collision = fabs(ball_top - app->level_manager.current_level.positions[i].y);
                    f32 top_collision = fabs(block_top - app->ball.pos.y);
                    f32 left_collision  = fabs(ball_right - app->level_manager.current_level.positions[i].x);
                    f32 right_collision = fabs(block_right - app->ball.pos.x);
                    
                    if((bottom_collision < top_collision && bottom_collision < left_collision && bottom_collision < right_collision) || (top_collision < bottom_collision && top_collision < left_collision && top_collision < right_collision)){
                        //reverse y
                        app->ball.velocity.y *= -1;
                    }
                    else if((left_collision < top_collision && left_collision < bottom_collision && left_collision < right_collision) || (right_collision < bottom_collision && right_collision < left_collision && right_collision < top_collision)){
                        //reverse x
                        app->ball.velocity.x *= -1;
                    }
                }
            }
        }
        
        if(app->ball.pos.y < 0){
            app->playing = 0;
        }
    }
    
    RendererStart(&app->renderer, Vec2(0, 0));
    
    {                            /* Player Rendering */
        RendererDrawQuad(&app->renderer, Vec4(app->player.pos.x, app->player.pos.y, app->player.scale.x, app->player.scale.y), app->player.color);
    }
    
    {                            /* Ball Rendering */
        RendererDrawQuad(&app->renderer, Vec4(app->ball.pos.x, app->ball.pos.y, app->ball.scale.x, app->ball.scale.y), app->ball.color);
    }
    
    {                            /* Level Rendering */
        for(int i = 0; i < app->level_manager.current_level.blocks; i++){
            if(app->level_manager.current_level.state[i] == SOLID){
                RendererDrawQuad(&app->renderer, Vec4(app->level_manager.current_level.positions[i].x, app->level_manager.current_level.positions[i].y, app->level_manager.current_level.scales[i].x - 8, app->level_manager.current_level.scales[i].y - 8), app->level_manager.current_level.colors[i]);
                RendererDrawQuad(&app->renderer, Vec4(app->level_manager.current_level.positions[i].x, app->level_manager.current_level.positions[i].y, app->level_manager.current_level.scales[i].x, app->level_manager.current_level.scales[i].y), Vec4(0, 0, 0, 1));
            }
        }
    }
    
    if(!app->playing){
        /* Restart level */
        app->ball.pos = Vec2((PROGRAM_WIDTH / 2) - (app->ball.scale.x / 2), (PROGRAM_HEIGHT / 2 - 50) - (app->ball.scale.y / 2));
        app->ball.velocity.x = 0;
        app->ball.velocity.y = -1;
        app->level_manager.current_level = LoadLevel((char *)app_platform->PlatformReadEntireFileToString("level2.data"));
    }
    
    RendererFinish(&app->renderer);
    
    app_platform->left_mouse_pressed = 0;
}