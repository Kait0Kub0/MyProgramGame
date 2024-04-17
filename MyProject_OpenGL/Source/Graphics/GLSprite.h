#pragma once

#include "glm/glm.hpp"


using namespace glm;

typedef struct _Sprite
{
	int		flag;
	vec3	pos;
	vec3	rot;
	vec3	scale;
	vec4	uv;
	int		texNo;
	vec4	color;

	GLint	shaderNo;

} Sprite;



#define SPRITE_MAX	1000

// スプライト
extern Sprite   sprite[SPRITE_MAX];


void SpriteSet(Sprite* p, int max);

void SpriteInit(Sprite* pSprite, int max);

void SpriteParam(int no, int tno, float px, float py, float pz, float rx, float ry, float rz);

void SpriteFlag(int no, int flag);
void SpriteFlagAll(int flag, int sNo = -1, int eNo = -1);


void SpriteTex(int no, int tno);

void SpritePos(int no, vec3 pos);

void SpritePos(int no, float rx, float ry, float rz);


void SpriteRot(int no, float px, float py, float pz);

void SpriteScale(int no, float sx, float sy, float sz);

void SpriteUV(int no, float u0, float v0, float u1, float v1);

void SpriteColor(int no, float r, float g, float b, float a);
void SpriteColor(int no, vec4 color);

void DrawLoop();

void SpriteDraw();

void SpriteDraw(Sprite* p, int max);


void SpriteShader(int no, GLint sNo);
void SpriteShaderAll(GLint sNo);
