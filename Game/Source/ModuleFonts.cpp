#include "App.h"
#include "ModuleFonts.h"
#include "Textures.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"
#include<string.h>

ModuleFonts::ModuleFonts() : Module()
{
	name.Create("fonts");

}

ModuleFonts::~ModuleFonts()
{
	
}

bool ModuleFonts::CleanUp()
{

	LOG("Freeing all fonts\n");

	for (uint i = 0; i < MAX_FONTS; ++i)
	{
		if (fonts[i].texture != nullptr)
		{
			app->tex->UnLoad(fonts[i].texture);
			fonts[i].texture = nullptr;
		}
	}
	
	return true;
}

// Load new texture from file path
int ModuleFonts::Load(const char* texturePath, const char* characters, uint rows, int imageW, int imageH)
{
	int id = -1;

	if(texturePath == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texturePath);

	if(tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texturePath, characters);
		return id;
	}

	id = 0;
	for(; id < MAX_FONTS; ++id)
		if(fonts[id].texture == nullptr)
			break;

	if(id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texturePath, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// charW --------	Width of each character
	// charH --------	Height of each character
	font.totalLength = strlen(characters);
	strcpy_s(font.table, characters);
	font.columns = font.totalLength / font.rows;

	font.charH = (imageH / font.rows);

	font.charW = (imageW / font.columns);

	LOG("Successfully loaded BMP font from %s", texturePath);

	return id;
}

void ModuleFonts::UnLoad(int fontId)
{
	LOG("Freeing a font texture\n");


	if(fontId >= 0 && fontId < MAX_FONTS && fonts[fontId].texture != nullptr)
	{
		app->tex->UnLoad(fonts[fontId].texture);
		fonts[fontId].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", fontId);
	}
}

void ModuleFonts::BlitText(int x, int y, int fontId, const char* text, bool greyText) const
{
	if(text == nullptr || fontId < 0 || fontId >= MAX_FONTS || fonts[fontId].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", fontId);
		return;
	}

	const Font* font = &fonts[fontId];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->charW;
	spriteRect.h = font->charH;
	spriteRect.x = spriteRect.y = 0;
	int columna = 0;



	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;

		// Find the character in the table, its position in the texture and then Blit
		for (uint j = 0; j < font->totalLength; j++) 
		{
			if (font->table[j] == text[i]) 
			{
				charIndex = j;
				break;
			}
		}
		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		if (greyText)
		{
			spriteRect.y *= 3;
			if (spriteRect.y == 0) spriteRect.y = font->charH * 2;
		}

		app->render->DrawTexture(font->texture, x, y, &spriteRect);
		// Advance the position where we blit the next character
		x += spriteRect.w + 2; 

	}

}

