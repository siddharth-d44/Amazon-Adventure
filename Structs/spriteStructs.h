




struct sprite {
  const unsigned short *image; // ptr->image
  int32_t width; // width
  int32_t height; // height
  int32_t lives;
  int32_t points;
  int32_t x;      // x coordinate
  int32_t y;      // y coordinate
};

typedef struct sprite sprite_t;

/*
typedef struct sprite fishSprite;

fishSprite fishGuy;

fishGuy.lives = 0;
fishStruct.points = 10;
fishSprite.image = fishy;


typedef struct sprite piranhaSprite;
piranhaStruct.lives = -1;
piranhaStruct.points = 0;
piranhaStruct.image = piranha;

typedef struct sprite playerSprite;
playerSprite.x;
playerSprite.y;
playerSprite.lives = 2;
playerSprite.points = 0;
playerSprite.image = player;

typedef struct sprite crocSprite;
crocSprite.x;
crosSprite.y = 70;
crocSprite.lives = -2;
crocSprite.points = 10;
crocSprite.image = crocodile;

*/

