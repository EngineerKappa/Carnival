#Global
PALETTE palette_all "gfx/font.png"
IMAGE custom_font  "gfx/font.png" BEST NONE

#Logo
PALETTE palette_engineerkappa_2 "gfx/palette_engineerkappa2.png"
IMAGE bg_engineerkappa "gfx/engineerkappa.png" BEST ALL
SPRITE  spr_kappa    "gfx/kappa.png" 4 4 FAST 5 NONE
SPRITE  spr_eris    "gfx/eris.png" 5 4 FAST 5 NONE
SPRITE  spr_boxer    "gfx/boxer.png" 3 2 FAST 5 NONE

#Title
IMAGE bg_titleswordsman "gfx/titleswordsman.png" BEST ALL
IMAGE bg_creditskappa "gfx/creditskappa.png" BEST ALL

#Game
IMAGE frame "gfx/textboxframe.png" BEST ALL
TILESET ui_tileset "map/uiframe.png" BEST
TILESET ui_icon_tiles "gfx/textboxframe.png" BEST
TILEMAP ui_map "map/uiframe.png" ui_tileset BEST

PALETTE castle_palette "gfx/tiles.png"
TILESET castle_tileset "gfx/tiles.png" BEST

PALETTE pal_swordsman "gfx/swordsman.png"
SPRITE  spr_swordsman    "gfx/swordsman.png" 2 2 FAST 0 NONE
SPRITE  spr_yorb    "gfx/yorb.png" 2 2 BEST  10
SPRITE  spr_gate    "gfx/gate.png" 2 2 BEST 10
SPRITE  spr_boneym    "gfx/boneym.png" 2 2 BEST 10
SPRITE  spr_pointy    "gfx/pointy.png" 2 2 BEST 10
SPRITE  spr_heart    "gfx/heart.png" 2 2 BEST 0


IMAGE bg_gameover "gfx/gameover.png" BEST ALL

#Ending
SPRITE  spr_uitext    "gfx/uitext.png" 4 4 BEST 10

#Cutscene
#IMAGE bg_nohbodee "gfx/nohbodee.png" BEST ALL