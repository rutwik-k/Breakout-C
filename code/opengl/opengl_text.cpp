struct Font{
    Texture texture;
    u32 size;
    
    //95 characters in font set
    i32 id[95];
    i32 x[95];
    i32 y[95];
    i32 width[95];
    i32 height[95];
    i32 x_advance[95];
    i32 x_offset[95];
    i32 y_offset[95];
    
    i32 baseline;
    i32 line_height;
};

internal Font InitFont(char *font_name){
    char info_filename[256];
    char texture_filename[256];
    
    snprintf(info_filename, 256, "font/%s.fnt", font_name);
    snprintf(texture_filename, 256, "font/%s.png", font_name);
    
    Font font = {};
    u8 *font_info = (u8 *)app_platform->PlatformReadEntireFileToString(info_filename);
    /*
This is hardcoded and probs breaks if another font file is used with a different format
*/
    font.baseline = 53;
    font.line_height = 87 / 57;
    
    i32 data_offset = 89;
    i32 line_length = 111;
    for(int i = 0; i < 95; i++){
        {   /* id */
            i32 id_offset = data_offset + ((line_length + 1) * i) + 8;
            char id[8];
            i32 id_index = 0;
            while(font_info[id_offset + id_index] != 32){
                id[id_index] = font_info[id_offset + id_index];
                id_index++;
            }
            id[id_index] = '\0';
            font.id[i] = atoi(id);
        }
        
        {   /* x */
            i32 x_offset = data_offset + ((line_length + 1) * i) + 18;
            char x[8];
            i32 x_index = 0;
            while(font_info[x_offset + x_index] != 32){
                x[x_index] = font_info[x_offset + x_index];
                x_index++;
            }
            x[x_index] = '\0';
            font.x[i] = atoi(x);
        }
        
        {   /* y */
            i32 y_offset = data_offset + ((line_length + 1) * i) + 25;
            char y[8];
            i32 y_index = 0;
            while(font_info[y_offset + y_index] != 32){
                y[y_index] = font_info[y_offset + y_index];
                y_index++;
            }
            y[y_index] = '\0';
            font.y[i] = atoi(y);
        }
        
        {   /* width */
            i32 width_offset = data_offset + ((line_length + 1) * i) + 36;
            char width[8];
            i32 width_index = 0;
            while(font_info[width_offset + width_index] != 32){
                width[width_index] = font_info[width_offset + width_index];
                width_index++;
            }
            width[width_index] = '\0';
            font.width[i] = atoi(width);
        }
        
        {   /* height */
            i32 height_offset = data_offset + ((line_length + 1) * i) + 48;
            char height[8];
            i32 height_index = 0;
            while(font_info[height_offset + height_index] != 32){
                height[height_index] = font_info[height_offset + height_index];
                height_index++;
            }
            height[height_index] = '\0';
            font.height[i] = atoi(height);
        }
        
        {   /* x offset */
            i32 x_offset_offset = data_offset + ((line_length + 1) * i) + 61;
            char x_offset[8];
            i32 x_offset_index = 0;
            while(font_info[x_offset_offset + x_offset_index] != 32){
                x_offset[x_offset_index] = font_info[x_offset_offset + x_offset_index];
                x_offset_index++;
            }
            x_offset[x_offset_index] = '\0';
            font.x_offset[i] = atoi(x_offset);
        }
        
        {   /* y offset */
            i32 y_offset_offset = data_offset + ((line_length + 1) * i) + 74;
            char y_offset[8];
            i32 y_offset_index = 0;
            while(font_info[y_offset_offset + y_offset_index] != 32){
                y_offset[y_offset_index] = font_info[y_offset_offset + y_offset_index];
                y_offset_index++;
            }
            y_offset[y_offset_index] = '\0';
            font.y_offset[i] = atoi(y_offset);
        }
        
        {   /* x advance */
            i32 x_advance_offset = data_offset + ((line_length + 1) * i) + 88;
            char x_advance[8];
            i32 x_advance_index = 0;
            while(font_info[x_advance_offset + x_advance_index] != 32){
                x_advance[x_advance_index] = font_info[x_advance_offset + x_advance_index];
                x_advance_index++;
            }
            x_advance[x_advance_index] = '\0';
            font.x_advance[i] = atoi(x_advance);
        }
    }
    Texture texture = InitTexture(texture_filename, GL_LINEAR, GL_RGBA, false);
    font.texture = texture;
    return font;
}

internal f32 GetTextWidth(Font *font, char *text, float scale){
    f32 width = 0;
    char *t;
    for(t = text; *t != '\0'; t++){
        width += (f32)font->x_advance[*t - 32];
    }
    return (width / 57) * scale;
}

internal f32 GetTextHeight(Font *font, char *text, float scale){
    f32 height = 0;
    //finds the char with the highest height.
    return (height / 57) * scale;
}
