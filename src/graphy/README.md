graphy is a version of the graphx library for Column-Major mode on the ti84ce. It is designed to allow any graphx program to run in Column-Major mode.

To add graphy to your project, copy graphy.h and graphy.c to your src folder, then do a find and replace of `gfx_` to `gfy_` and `GFX_` to `GYX_`, and replace `<graphx.h>` with `"graphy.h"`. Make sure you also do this with any convimg output.