f32 Lerp(float a, float b, float t){
    return a + t * (b-a);
}

Vec3 Vec3Lerp(Vec3 a, Vec3 b, float t){
    Vec3 result = {};
    result.x = Lerp(a.x, b.x, t);
    result.y = Lerp(a.y, b.y, t);
    result.z = Lerp(a.z, b.z, t);
    return result;
}

b32 Vec4Intersects(Vec4 a, Vec4 b){
    b32 result = 0;
    if(a.x < b.x + b.z && a.x + a.z > b.x && a.y < b.y + b.w && a.y + a.w > b.y){
        result = 1;
    }
    return result;
}

Vec2 Vec2Direction(Vec2 a, Vec2 b){
    return Vec2(a.x - b.x, a.y - b.y);
}