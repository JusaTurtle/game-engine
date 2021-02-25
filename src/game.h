#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <math.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

#if INTERNAL
struct debug_read_file_result
{
  void* contents;
  u32 contentSize;
};


#define DEBUG_PLATFORM_READ_FILE(name) debug_read_file_result name(char* fileName)
typedef DEBUG_PLATFORM_READ_FILE(debug_platform_read_file);

#define DEBUG_PLATFORM_FREE_MEMORY(name)void name (void* memory) 
typedef DEBUG_PLATFORM_FREE_MEMORY(debug_platform_free_memory);

#define DEBUG_PLATFORM_WRITE_FILE(name) bool name(char* fileName, u32 memorySize, void* memory) 
typedef DEBUG_PLATFORM_WRITE_FILE(debug_platform_write_file);

DEBUG_PLATFORM_READ_FILE(DEBUGPlatformReadFile);
DEBUG_PLATFORM_FREE_MEMORY(DEBUGPlatformFreeMemory);
DEBUG_PLATFORM_WRITE_FILE(DEBUGPlatformWriteFile);
#endif

#if SLOW
#define ASSERT(Expression) if(!(Expression)) {*(int *)0 = 0;}
inline u32 SafeTruncateUInt64(u64 value);
#else
#define ASSERT(Expression) 
#endif

#define KILOBYTES(value) ((value) * 1024LL) 
#define MEGABYTES(value) (KILOBYTES(value) * 1024LL) 
#define GIGABYTES(value) (MEGABYTES(value) * 1024LL) 
#define TERABYTES(value) (GIGABYTES(value) * 1024LL) 

struct game_offscreen_buffer
{
  void* memory;
  int width;
  int height;
  int pitch;
};

struct game_sound_output
{
  int samplesPerSecond;
  int sampleCount;
  i16* samples;
};

struct button_state
{
  bool isDown;
  u32 halfTransitionCount;
};

struct game_input
{
  // button
  union
  {
    button_state buttons[4];
    struct
    {
      button_state up;
      button_state down;
      button_state left;
      button_state right;
    };
  };

  // Mouse
  int mouseX, mouseY, mouseZ;
  bool mouseButtonState[5];

  // clock
  float timeStep;
}; // TODO: Clean up

struct v2
{
  float x, y;

  v2 operator*=(v2 lhs);
  v2 operator/=(v2 lhs);
  v2 operator+=(v2 lhs);
  v2 operator-=(v2 lhs);
  bool operator==(v2 lhs);
  v2 operator*=(float lhs);
  v2 operator/=(float lhs);
  v2 operator+=(float lhs);
  v2 operator-=(float lhs);
  bool operator==(float lhs);
};
v2 operator*(v2 lhs, v2 rhs);
v2 operator/(v2 lhs, v2 rhs);
v2 operator+(v2 lhs, v2 rhs);
v2 operator-(v2 lhs, v2 rhs);
v2 operator*(v2 lhs, float rhs);
v2 operator/(v2 lhs, float rhs);
v2 operator+(v2 lhs, float rhs);
v2 operator-(v2 lhs, float rhs);

struct rec
{
  v2 pos;
  v2 size;
};

struct game_state
{
  float tSine;
  int volume;
  int toneHZ;

  rec player;
};

struct game_memory
{
  bool initialized;

  u64 permanentStorageSize;
  void* permanentStorage; // TODO: REQUIRE clear to zero on startup

  u64 transientStorageSize;
  void* transientStorage; // TODO: REQUIRE clear to zero on startup

  debug_platform_free_memory* DEBUGPlatformFreeMemory;
  debug_platform_read_file* DEBUGPlatformReadFile;
  debug_platform_write_file* DEBUGPlatformWriteFile;
};


#define GAME_UPDATE_AND_RENDER(name) void name(game_memory* gameMemory, game_offscreen_buffer* buffer, game_input input)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

#define GAME_OUTPUT_SOUND(name) void name(game_memory* gameMemory, game_sound_output* soundBuffer)
typedef GAME_OUTPUT_SOUND(game_output_sound);

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender);
extern "C" GAME_OUTPUT_SOUND(GameOutputSound);

#endif
