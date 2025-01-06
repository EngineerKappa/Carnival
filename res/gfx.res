PALETTE palette_all "gfx/font.png"
PALETTE pal_swordsman "gfx/swordsman.png"
IMAGE custom_font  "gfx/font.png" BEST NONE

#IMAGE bg_nohbodee "gfx/nohbodee.png" BEST ALL
TILESET ui_tileset "map/uiframe.png" BEST
MAP ui_map "map/uiframe.png" ui_tileset BEST

IMAGE frame "gfx/textboxframe.png" BEST ALL

TILESET canyon_tileset "map/canyon1.png" BEST
MAP canyon_map "map/canyon1.png" canyon_tileset BEST

TILESET canyon_bg_tileset "map/canyonbg.png" BEST
MAP canyon_bg_map "map/canyonbg.png" canyon_bg_tileset BEST 0

PALETTE canyon_palette "map/canyon1.png"

SPRITE  spr_swordsman    "gfx/swordsman.png" 2 2 BEST 0
SPRITE  spr_yorb    "gfx/yorb.png" 2 2 NONE 10