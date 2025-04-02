#ifndef vargfx_appvar_include_file
#define vargfx_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define vargfx_appvar_size 2546
#define sizeof_global_palette 494
#define global_palette (vargfx_appvar[0])
#define sprites_palette_offset 0
#define ubuntu_width 32
#define ubuntu_height 32
#define vargfx_sprites_ubuntu_index 1
#define ubuntu ((gfy_sprite_t*)vargfx_appvar[1])
#define mint_width 32
#define mint_height 32
#define vargfx_sprites_mint_index 2
#define mint ((gfy_sprite_t*)vargfx_appvar[2])
#define vargfx_entries_num 3
extern unsigned char *vargfx_appvar[3];
unsigned char vargfx_init(void);

#ifdef __cplusplus
}
#endif

#endif
