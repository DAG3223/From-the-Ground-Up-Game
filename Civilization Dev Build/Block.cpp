#include<iostream>
#include"raylib.h"
#include"Logic.h"
#include "Item.h"
#include "Block.h"


//constructor
Block::Block(Block dataSource[], float x, float y, int blockID) {
    this->x = Logic::snapGridPoint(x, 25);
    this->y = Logic::snapGridPoint(y, 25);
    form = { this->x, this->y, 25, 25 };
    this->solid = dataSource[blockID].solid;
    this->sounds = dataSource[blockID].sounds;
    this->toughness = dataSource[blockID].toughness;
    this->blockID = blockID;
    this->guiID = dataSource[blockID].guiID;
}

Block::Block(const char* refName, const char* path, Vector2 sounds, bool solid, int toughness, int guiID, Vector3 itemInfo, Vector3 item2Info) {
    this->name = refName;
    this->path = path;
    this->sounds = sounds;
    this->solid = solid;
    this->toughness = toughness;
    this->itemID = (int)itemInfo.x;
    this->dropMin = (int)itemInfo.y;
    this->dropMax = (int)itemInfo.z;
    this->item2ID = (int)item2Info.x;
    this->drop2Min = (int)item2Info.y;
    this->drop2Max = (int)item2Info.z;
    this->guiID = guiID;
}

void Block::DrawList(std::list<Block> list, Block dataSource[], Rectangle player, int renderDistance) {
    std::list<Block>::iterator drawing;
    for (drawing = list.begin(); drawing != list.end(); drawing++)
    {
        if (Logic::distance(player.x + 25, player.y + 25, drawing->x, drawing->y) > renderDistance) {
            continue;
        }
        drawing->Draw(dataSource);
    }
}

//Draws the block
void Block::Draw(Block dataSource[]) {
    DrawTexture(dataSource[this->blockID].tex, (int)x, (int)y, RAYWHITE);
}