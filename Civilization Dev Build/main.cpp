#include<iostream>
#include<list>
#include"raylib.h"

#include "Logic.h"
#include "Item.h"
#include "Block.h"
#include "Debug.h"
#include "Recipe.h"
#include "GUI.h"

static Rectangle getBlockTarget(int x, int y) {
    return { Logic::snapGridPoint(x, 25), Logic::snapGridPoint(y,25), 25, 25 };
}

int main(void)
{
#pragma region variableDeclarations
    //Window Initialization

    InitWindow(800, 800, "From the Ground Up! Alpha 1.2: Items");
    SetTargetFPS(60);
    GetWorkingDirectory();

    //Audio Device Initialization
    InitAudioDevice();

    Sound pickup = LoadSound("assets/sfx/item pop.mp3");
    Sound defaultBreakSound = LoadSound("assets/sfx/dirt break.mp3");
    Sound dirtBreakSound = LoadSound("assets/sfx/dirt break.mp3");
    Sound stoneBreakSound = LoadSound("assets/sfx/stone break.mp3");
    Sound woodBreakSound = LoadSound("assets/sfx/wood break.mp3");
    Sound defaultBlockPlaceSound = LoadSound("assets/sfx/block place.mp3");
    Sound dirtPlaceSound = LoadSound("assets/sfx/dirt place.mp3");
    Sound workbenchCraftSound = LoadSound("assets/sfx/hammering.mp3");

    enum SOUNDS {
        ITEM_PICKUP,
        BREAK_DEFAULT,
        BREAK_DIRT,
        BREAK_STONE,
        BREAK_WOOD,
        PLACE_DEFAULT,
        PLACE_DIRT,
        CRAFT_WORKBENCH
    };


    const int soundCount = 8;
    Sound soundAtlas[soundCount] = {
        pickup,
        defaultBreakSound,
        dirtBreakSound,
        stoneBreakSound,
        woodBreakSound,
        defaultBlockPlaceSound,
        dirtPlaceSound,
        workbenchCraftSound
    };

    Music music = LoadMusicStream("assets/sfx/crea music.mp3");

    Debug::blankLine();
    Debug::Log("Loading block data...");

    //REFERENCE DATA INITIALIZATON
    //Block(const char* refName, const char* path, Vector2 sounds, int blockID, bool solid, int guiID, Vector3 itemInfo, Vector3 item2Info)
    const int blockCount = 17;
    Block blockData[blockCount] = {
        Block("Error",              "assets/art/no texture.png",         {SOUNDS::BREAK_DEFAULT, SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::EMPTY, 1, 1}),
        Block("Wood Block",         "assets/art/temp/wood planks.png",   {SOUNDS::BREAK_WOOD,    SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::WOOD_BLOCK, 1, 1}), 
        Block("Stone Block",        "assets/art/temp/stone.png",         {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::STONE_BLOCK, 1, 1}),
        Block("Bricks",             "assets/art/temp/bricks.png",        {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::BRICKS, 1, 1}),
        Block("Glass",              "assets/art/temp/glass.png",         {SOUNDS::BREAK_WOOD,    SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::GLASS, 1, 1}),
        Block("Flower",             "assets/art/temp/flower_test.png",   {SOUNDS::BREAK_DIRT,    SOUNDS::PLACE_DIRT},    Block::Collisions::UNSOLID, Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::FLOWER, 1, 1}),
        Block("Tree",               "assets/art/tree.png",               {SOUNDS::BREAK_WOOD,    SOUNDS::PLACE_DIRT},    Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::LOG, 3, 5},          {Item::NAMES::ACORN, 1, 2}), 
        Block("Coal Node",          "assets/art/coal node.png",          {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::ORE_COAL, 1, 3},     {Item::NAMES::STONE, 1, 3}),
        Block("Copper Node",        "assets/art/copper node.png",        {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::IRON,   GUI::NAMES::NONE,      {Item::NAMES::ORE_COPPER, 1, 3},   {Item::NAMES::STONE, 1, 3}),
        Block("Tin Node",           "assets/art/tin node.png",           {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::IRON,   GUI::NAMES::NONE,      {Item::NAMES::ORE_TIN, 1, 3},      {Item::NAMES::STONE, 1, 3}),
        Block("Iron Node",          "assets/art/iron node.png",          {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::NONE,      {Item::NAMES::ORE_IRON, 1, 3},     {Item::NAMES::STONE, 1, 3}),
        Block("Tungsten Node",      "assets/art/tungsten node.png",      {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::BRONZE, GUI::NAMES::NONE,      {Item::NAMES::ORE_TUNGSTEN, 1, 3}, {Item::NAMES::STONE, 1, 3}),
        Block("Uranium Node",       "assets/art/uranium node.png",       {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::STEEL,  GUI::NAMES::NONE,      {Item::NAMES::ORE_URANIUM, 1, 3},  {Item::NAMES::STONE, 1, 3}),
        Block("Workbench",          "assets/art/temp/workbench.png",     {SOUNDS::BREAK_WOOD,    SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::WORKBENCH, {Item::NAMES::WORKBENCH, 1, 1}),
        Block("Furnace",            "assets/art/furnace.png",       {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::FURNACE,   {Item::NAMES::FURNACE, 1, 1}),
        Block("Anvil",              "assets/art/anvil.png",         {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::ANVIL,     {Item::NAMES::ANVIL, 1, 1}),
        Block("Stellar Signaller",  "assets/art/signal.png",         {SOUNDS::BREAK_STONE,   SOUNDS::PLACE_DEFAULT}, Block::Collisions::SOLID,   Block::Toughness::ALL,    GUI::NAMES::SIGNALLER, {Item::NAMES::SIGNALLER, 1, 1})
    };

    //Load Block Textures
    for (int i = 0; i < blockCount; i++) {
        blockData[i].tex = LoadTexture(blockData[i].path);
    }

    Debug::blankLine();
    Debug::Log("Done.");

    Debug::blankLine();
    Debug::Log("Loading item data...");
    //Item(const char* name, const char* path, int type = NONE, bool stackable = false, int blockID = -1);
    const int itemCount = 36;
    Item itemData[itemCount] = {
        Item("Empty Slot",            "assets/art/empty.png",                    Item::TYPES::NONE,      -1, Item::STACK::YES),

        //materials                                                                                      Item::TOOL_STRENGTH::NOT_TOOL, 
        Item("Log",                   "assets/art/temp/log.png",                 Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Stone",                 "assets/art/rock.png",                     Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Sand",                  "assets/art/sand.png",                     Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Silicon",               "assets/art/silicon.png",                  Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Copper Wire",           "assets/art/copper wire.png",              Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Lightbulb",             "assets/art/light.png",                    Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Circuit Board",         "assets/art/circuit board.png",            Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Uranium Fuel",          "assets/art/uranium fuel.png",             Item::TYPES::MATERIAL,  -1, Item::STACK::YES),

        //ores                                                                                           Item::TOOL_STRENGTH::NOT_TOOL, 
        Item("Coal",                  "assets/art/coal ore.png",                 Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Copper Ore",            "assets/art/copper ore.png",               Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Tin Ore",               "assets/art/tin ore.png",                  Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Iron Ore",              "assets/art/iron ore.png",                 Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Tungsten Ore",          "assets/art/tungsten ore.png",             Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Uranium Ore",           "assets/art/uranium ore.png",              Item::TYPES::MATERIAL,  -1, Item::STACK::YES),

        //block items                                                                                    Item::TOOL_STRENGTH::NOT_TOOL, 
        Item("Block of Wood",         "assets/art/temp/wood block.png",          Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::WOOD),
        Item("Block of Stone",        "assets/art/temp/stone block.png",         Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::STONE),
        Item("Brick Block",           "assets/art/temp/bricks Item.png",         Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::BRICKS),
        Item("Glass Block",           "assets/art/temp/glass Item.png",          Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::GLASS),
        Item("Flower",                "assets/art/temp/flower Item.png",         Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::FLOWER),

        //tools
        Item("Makeshift Tool",        "assets/art/stone tool.png",               Item::TYPES::TOOL,      Item::TOOL_STRENGTH::MAKESHIFT,  Item::STACK::NO),
        Item("Iron Tool",             "assets/art/iron tool.png",                Item::TYPES::TOOL,      Item::TOOL_STRENGTH::IRON,  Item::STACK::NO),
        Item("Bronze Tool",           "assets/art/bronze tool.png",              Item::TYPES::TOOL,      Item::TOOL_STRENGTH::BRONZE,  Item::STACK::NO),
        //Item("Ferro-Tungsten Tool", "assets/art/ft tool.png",                  Item::TYPES::TOOL,      Item::TOOL_STRENGTH::NONE,  Item::STACK::NO),
        Item("Steel Tool",            "assets/art/steel tool.png",               Item::TYPES::TOOL,      Item::TOOL_STRENGTH::STEEL,  Item::STACK::NO),

        //crafting stations
        Item("Workbench",             "assets/art/temp/workbench item.png",      Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::WORKBENCH),
        Item("Furnace",               "assets/art/furnace item.png",             Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::FURNACE),
        Item("Anvil",                 "assets/art/anvil item.png",               Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::ANVIL),
        Item("Stellar Signaller",     "assets/art/signal item.png",           Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::SIGNALLER),

        //bars                                                                                           Item::TOOL_STRENGTH::NOT_TOOL, 
        Item("Copper Bar",            "assets/art/copper bar.png",               Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Tin Bar",               "assets/art/tin bar.png",                  Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Iron Bar",              "assets/art/iron bar.png",                 Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Tungsten Bar",          "assets/art/tungsten bar.png",             Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Bronze Alloy",          "assets/art/bronze alloy.png",             Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("Ferro-Tungsten Alloy",  "assets/art/ferro tungsten alloy.png",     Item::TYPES::MATERIAL,  -1, Item::STACK::YES),
        Item("F-T Steel Alloy",       "assets/art/steel alloy.png",              Item::TYPES::MATERIAL,  -1, Item::STACK::YES),

        //other items                                                                                    Item::TOOL_STRENGTH::NOT_TOOL, 
        Item("Acorn",                 "assets/art/acorn.png" ,                   Item::TYPES::BLOCK,     -1, Item::STACK::YES, Block::NAMES::TREE)
    };
    
    //Load Item Textures
    for (int i = 0; i < itemCount; i++) {
        itemData[i].tex = LoadTexture(itemData[i].path);
        std::cout << itemData[i].name << ": " << itemData[i].toolStrength << std::endl;
    }

    Debug::blankLine();
    Debug::Log("Done.");

    //Recipe(const char* name, Item dataSource[], int outID, int outAm, int p1ID, int p1Am, int p2ID, int p2Am, int station)
    const int recipeCount = 24;
    Recipe recipeData[recipeCount] = {
        Recipe("Wood Block (4)",         {Item::NAMES::WOOD_BLOCK,     4},    {Item::NAMES::LOG,            1}, {Item::NAMES::EMPTY,         0},   GUI::NAMES::WORKBENCH),
        Recipe("Stone Block (4)",        {Item::NAMES::STONE_BLOCK,    4},    {Item::NAMES::STONE,          1}, {Item::NAMES::EMPTY,         0},   GUI::NAMES::WORKBENCH),
        Recipe("Brick Block (4)",        {Item::NAMES::BRICKS,         4},    {Item::NAMES::STONE,          1}, {Item::NAMES::SAND,          1},   GUI::NAMES::WORKBENCH),
        Recipe("Furnace",                {Item::NAMES::FURNACE,        1},    {Item::NAMES::STONE,         10}, {Item::NAMES::LOG,           5},   GUI::NAMES::WORKBENCH),
        Recipe("Anvil",                  {Item::NAMES::ANVIL,          1},    {Item::NAMES::BAR_IRON,       5}, {Item::NAMES::EMPTY,         0},   GUI::NAMES::WORKBENCH),
        Recipe("Light",                  {Item::NAMES::LIGHTBULB,      1},    {Item::NAMES::COPPER_WIRE,    1}, {Item::NAMES::GLASS,         1},   GUI::NAMES::WORKBENCH),
        Recipe("Circuit Board",          {Item::NAMES::CIRCUIT_BOARD,  4},    {Item::NAMES::SILICON,        4}, {Item::NAMES::COPPER_WIRE,   8},   GUI::NAMES::WORKBENCH),
                                                                                                           
        Recipe("Glass",                  {Item::NAMES::GLASS,          4},    {Item::NAMES::SAND,           1},  {Item::NAMES::EMPTY,         0},  GUI::NAMES::FURNACE),
        Recipe("Silicon",                {Item::NAMES::SILICON,        2},    {Item::NAMES::SAND,           1}, {Item::NAMES::EMPTY,         0},   GUI::NAMES::FURNACE),
        Recipe("Copper Bar",             {Item::NAMES::BAR_COPPER,     1},    {Item::NAMES::ORE_COAL,       1}, {Item::NAMES::ORE_COPPER,    1},   GUI::NAMES::FURNACE),
        Recipe("Tin Bar",                {Item::NAMES::BAR_TIN,        1},    {Item::NAMES::ORE_COAL,       1}, {Item::NAMES::ORE_TIN,       1},   GUI::NAMES::FURNACE),
        Recipe("Iron Bar",               {Item::NAMES::BAR_IRON,       1},    {Item::NAMES::ORE_COAL,       1}, {Item::NAMES::ORE_IRON,      1},   GUI::NAMES::FURNACE),
        Recipe("Tungsten Bar",           {Item::NAMES::BAR_TUNGSTEN,   1},    {Item::NAMES::ORE_COAL,       1}, {Item::NAMES::ORE_TUNGSTEN,  1},   GUI::NAMES::FURNACE),
        Recipe("Bronze Alloy",           {Item::NAMES::ALLOY_BRONZE,   1},    {Item::NAMES::BAR_COPPER,     1}, {Item::NAMES::BAR_TIN,       1},   GUI::NAMES::FURNACE),
        Recipe("Ferro-Tungsten Alloy",   {Item::NAMES::ALLOY_FeTung,   1},    {Item::NAMES::BAR_IRON,       1}, {Item::NAMES::BAR_TUNGSTEN,  1},   GUI::NAMES::FURNACE),
        Recipe("F-T Steel Alloy",        {Item::NAMES::ALLOY_STEEL,    1},    {Item::NAMES::ORE_COAL,       1}, {Item::NAMES::ALLOY_FeTung,  1},   GUI::NAMES::FURNACE),
                                                                                                           
        Recipe("Sand",                   {Item::NAMES::SAND,           4},    {Item::NAMES::STONE,          1}, {Item::NAMES::EMPTY,         0},   GUI::NAMES::ANVIL),
        Recipe("Copper Wire",            {Item::NAMES::COPPER_WIRE,    4},    {Item::NAMES::BAR_COPPER,     1}, {Item::NAMES::EMPTY,         0},   GUI::NAMES::ANVIL),
        Recipe("Iron Tool",              {Item::NAMES::IRON_TOOL,      1},    {Item::NAMES::LOG,            3}, {Item::NAMES::BAR_IRON,      4},  GUI::NAMES::ANVIL),
        Recipe("Bronze Tool",            {Item::NAMES::BRONZE_TOOL,    1},    {Item::NAMES::LOG,            3}, {Item::NAMES::ALLOY_BRONZE,  4},  GUI::NAMES::ANVIL),
        Recipe("Steel Tool",             {Item::NAMES::STEEL_TOOL,     1},    {Item::NAMES::LOG,            3}, {Item::NAMES::ALLOY_STEEL,   4},  GUI::NAMES::ANVIL),
        Recipe("Uranium Fuel",           {Item::NAMES::URANIUM_FUEL,   1},    {Item::NAMES::ORE_URANIUM,    2}, {Item::NAMES::ALLOY_STEEL,   4},   GUI::NAMES::ANVIL),
        Recipe("Stellar Signaller",      {Item::NAMES::SIGNALLER,      1},    {Item::NAMES::ALLOY_STEEL,   10}, {Item::NAMES::LIGHTBULB,     5},   GUI::NAMES::ANVIL),
    
        Recipe("Send Signal",            {-1, 1},                             {Item::NAMES::CIRCUIT_BOARD, 10}, {Item::NAMES::URANIUM_FUEL, 10},   GUI::NAMES::SIGNALLER)
    };

    Texture2D background = LoadTexture("assets/art/grass.png");

    //Player shape
    Rectangle player = { 375, 375, 50, 50 };
    Texture2D playerTex = LoadTexture("assets/art/player.png");

    //Camera initialization
    Camera2D camera = { 0 };
    camera.target = { player.x + 25.0f, player.y + 25.0f };
    camera.offset = { 400.0f, 400.0f };
    camera.zoom = 1.0f;

    //Important variable declarations
    //Block Variables
    std::list<Block> blocks = {};
    std::list<Block> backBlocks = {};
    int spawnX, spawnY;
    bool place = true;
    int freeSeedCooldown = 0;
    int oreCounts[6] = {
        0,
        0,
        0,
        0,
        0,
        0
    };

    //Item variables
    Texture2D inventoryBack = LoadTexture("assets/art/temp/inventoryBack.png");
    Texture2D hotbarBack = LoadTexture("assets/art/temp/hotbarBack.png");
    Texture2D slotSelected = LoadTexture("assets/art/temp/slot select.png");
    std::list<Item> groundItems = {};

    //inventory data
    const int itemIndent = 8;
    Item inventory[30] = {
        //newItem array[], float x, float y, int id = 0, int state = DATA, int amount = 1
        Item(itemData, 600 + itemIndent, 40 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 632 + itemIndent, 40 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 664 + itemIndent, 40 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 696 + itemIndent, 40 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 728 + itemIndent, 40 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),

        Item(itemData, 600 + itemIndent, 80 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 632 + itemIndent, 80 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 664 + itemIndent, 80 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 696 + itemIndent, 80 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 728 + itemIndent, 80 + itemIndent,  Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),

        Item(itemData, 600 + itemIndent, 112 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 632 + itemIndent, 112 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 664 + itemIndent, 112 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 696 + itemIndent, 112 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 728 + itemIndent, 112 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),

        Item(itemData, 600 + itemIndent, 144 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 632 + itemIndent, 144 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 664 + itemIndent, 144 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 696 + itemIndent, 144 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 728 + itemIndent, 144 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),

        Item(itemData, 600 + itemIndent, 176 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 632 + itemIndent, 176 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 664 + itemIndent, 176 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 696 + itemIndent, 176 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 728 + itemIndent, 176 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),

        Item(itemData, 600 + itemIndent, 208 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 632 + itemIndent, 208 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 664 + itemIndent, 208 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 696 + itemIndent, 208 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
        Item(itemData, 728 + itemIndent, 208 + itemIndent, Item::NAMES::EMPTY, Item::STATES::INVENTORY, 0),
    };
    Item hand(itemData, GetMouseX(), GetMouseY(), Item::NAMES::EMPTY, Item::STATES::DATA, 0);
    int swapID;
    int swapAmount;
    int matchingInventoryID;
    Item* dropPtr = &hand;

    //inventory logic variables
    int hotbarIndex = 0;
    bool inventoryOpen = false;
    bool inventoryFull = false;
    bool pickUpDone = false;

    //GUI variables
    int currentGUI = 0;
    int recipeIndex = 0;
    bool recipeAvailable = false;
    Texture2D available = LoadTexture("assets/art/available.png");
    Texture2D notAvailable = LoadTexture("assets/art/not available.png");
    Rectangle buttonForm = { 405, 300, 25, 25 };
    int suitableP1Index = 0;
    int suitableP2Index = 0;

    Texture2D exit = LoadTexture("assets/art/backwards.png");
    Texture2D previous = LoadTexture("assets/art/backwards.png");
    Texture2D next = LoadTexture("assets/art/forwards.png");
    
    Rectangle exitBox = {250, 250, 25, 25};
    Rectangle previousBox = {250, 525, 25, 25};
    Rectangle nextBox = {525, 525, 25, 25};

    //Mission Variables
    int currentMissionIndex = 0;
    const char* missionNames[10] = {
        "Find and Touch Water",
        "Chop a Tree",
        "Plant a Tree with an Acorn",
        "Mine Iron Ore with your Tool",
        "Craft a Furnace at a Workbench",
        "Smelt Iron Ore and Coal into Iron Bars at a Furnace",
        "Craft a New Tool with Iron Bars at an Anvil",
        "Smelt Metals into an Alloy at a Furnace",
        "Craft Uranium Fuel at an Anvil",
        "Send Star Signal with Stellar Signaller"
    };
    
    Rectangle missionNameBox = { 20, 50, 100, 30 };
    Rectangle prefixBox = { 20, 20, 100, 30 };
    const char* prefix = "Current Mission:";

    //Debug variable declarations
    float mouseWorldX = 0, mouseWorldY = 0;

    float pickupTime = 0;
    bool displayHelp = false;
    bool outputDebug = false;

    float debugTimeInit = 0;
    float debugTimeEnd = 0;
    float debugTimeDelta = 0;

    float logicTime = 0.0;

    int keyPressed;
    int renderDistance = 600;

    /*
        TITLE SCREEN DATA
    */
    Texture2D title = LoadTexture("assets/art/title.png");
    const char* titleName = "FROM THE GROUND UP!";
    const char* subtitle = "By Dylan Garza and Stacie Acosta";
    Rectangle startButton = { 300, 400, 200, 100 };
    const char* startText = "START!";
    bool gameStart = false;

    /*
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    WIN CONDITIONS:
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */
    
    Texture2D levelUp = LoadTexture("assets/art/level up.png");
    int levelUpDisplayFrames = 0;
    Sound levelUpSound = LoadSound("assets/sfx/level up sound.mp3");

    Texture2D endScreen = LoadTexture("assets/art/ending.png");
    int endX = 0;
    int endY = 1600;


    bool gameWon = false;
    int worldReadinessLevel = 0;

    bool foundWater = false;      //done
    bool chopTree = false;        //done
    bool plantTree = false;       //done
    bool mineOre = false;         //done
    bool makeFurnace = false;     //done
    bool smeltBar = false;        //done
    bool newTool = false;         //done
    bool smeltAlloy = false;      //done
    bool makeUraniumFuel = false; //done 
    bool sendSignal = false;      //done

#pragma endregion
    while (!WindowShouldClose())
    {
        while (gameStart == false) {
            if (GetKeyPressed()) {
                gameStart = true;
            }

            BeginDrawing();
            DrawTexture(title, 0, 0, RAYWHITE);
            DrawText(titleName, 4,100,67,WHITE);
            DrawText(subtitle, 40, 180, 40, WHITE);
            
            DrawText("PRESS ANY KEY TO START", 100, 700, 40, RAYWHITE);
            EndDrawing();
        }

        //WORLD GENERATION
        groundItems.push_back(Item(itemData, player.x, player.y, Item::NAMES::WORKBENCH, Item::STATES::GROUND, 1));
        groundItems.push_back(Item(itemData, player.x, player.y, Item::NAMES::STONE_TOOL, Item::STATES::GROUND, 1));

        groundItems.push_back(Item(itemData, player.x, player.y, Item::NAMES::CIRCUIT_BOARD, Item::STATES::GROUND, 10));
        groundItems.push_back(Item(itemData, player.x, player.y, Item::NAMES::URANIUM_FUEL, Item::STATES::GROUND, 10));
        groundItems.push_back(Item(itemData, player.x, player.y, Item::NAMES::SIGNALLER, Item::STATES::GROUND, 1));

        debugTimeInit = GetTime();
        //spawn 100 of each ore
        Debug::startEvent("BLOCK GENERATION");
        for (int i = Block::NAMES::NODE_COAL; i < Block::NAMES::NODE_URANIUM + 1; i++) {
            Debug::displayInt("Spawning Blocks of ID: ", i);
            for (int j = 0; j < 100; j++) {
                place = true;
                spawnX = Logic::Random(2375, -800);
                spawnY = Logic::Random(2375, -800);
                std::list<Block>::iterator placing;
                for (placing = blocks.begin(); placing != blocks.end(); placing++)
                {
                    if ((placing->x == Logic::snapGridPoint(spawnX, 25)) && placing->y == Logic::snapGridPoint(spawnY, 25)) {
                        Debug::Log("Block was not placed: Space already full.", outputDebug);
                        j--;
                        place = false;
                        break;
                    }
                }
                Debug::blankLine();
                if (Logic::recCollide(player, getBlockTarget(spawnX, spawnY))) {
                    Debug::Log("Block collides with player. Replacing...");
                    place = false;
                    j--;
                }

                if (place) {
                    Debug::Log("Block placed successfully.");
                    blocks.push_back(Block(blockData, spawnX, spawnY, i));
                    oreCounts[i - Block::NAMES::NODE_COAL]++;
                }
                Debug::blankLine();
                //    Block dataSource, x,      y,      blockID = 0) {
            }
        }

        //spawn trees
        for (int i = 0; i < 100; i++) {
            place = true;
            spawnX = Logic::Random(2375, -800);
            spawnY = Logic::Random(2375, -800);

            Debug::blankLine();
            std::list<Block>::iterator placing;
            for (placing = blocks.begin(); placing != blocks.end(); placing++)
            {
                if ((placing->x == Logic::snapGridPoint(spawnX, 25)) && placing->y == Logic::snapGridPoint(spawnY, 25)) {
                    Debug::Log("Block was not placed: Space already full.", outputDebug);
                    i--;
                    place = false;
                    break;
                }
            }
            if (Logic::recCollide(player, getBlockTarget(spawnX, spawnY))) {
                Debug::Log("Block collides with player. Replacing...");
                i--;
                place = false;
            }

            if (place) {
                Debug::Log("Block placed successfully.");
                blocks.push_back(Block(blockData, spawnX, spawnY, Block::NAMES::TREE));
            }
            Debug::blankLine();
        }

        //spawn flowers
        for (int i = 0; i < 50; i++) {
            blocks.push_back(Block(blockData, (rand() % 2375 - 800), (rand() % 2375 - 800), Block::NAMES::FLOWER));
        }
        Debug::endEvent("BLOCK GENERATION");

        debugTimeEnd = GetTime();
        debugTimeDelta = debugTimeEnd - debugTimeInit;
        Debug::displayFloat("Time to load blocks (s): ", debugTimeDelta, outputDebug);
        Debug::displayFloat("Time to load blocks (ms): ", debugTimeDelta * 1000, outputDebug);
        Debug::displayFloat("Frame time (1f = 16.667ms)", (debugTimeDelta * 1000) / 16.667, outputDebug);

        while (!gameWon) {
            logicTime = GetTime();

            /*UpdateMusicStream(music);
            if (!IsMusicStreamPlaying(music)) {
                PlayMusicStream(music);
            }*/

            //UPDATE
            if (currentGUI == GUI::NAMES::NONE)
            {
                #pragma region regularLogic
                //mouse position
                Vector2 WorldClick = GetScreenToWorld2D(GetMousePosition(), camera);

                mouseWorldX = WorldClick.x;
                mouseWorldY = WorldClick.y;

                //PLAYER CONTROLS
                player.x += 5 * (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
                player.y += 5 * (IsKeyDown(KEY_S) - IsKeyDown(KEY_W));

                //PLAYER COLLISION WITH WORLD BOUNDS
                //FIND WATER MISSION
                if (player.x < -800) {
                    player.x = -800;
                    if (!foundWater) {
                        foundWater = true;
                        currentMissionIndex = 1;
                        levelUpDisplayFrames = 180;
                        PlaySoundMulti(levelUpSound);
                    }
                }
                if (player.y < -800) {
                    player.y = -800;
                    if (!foundWater) {
                        foundWater = true;
                        currentMissionIndex = 1;
                        levelUpDisplayFrames = 180;
                        PlaySoundMulti(levelUpSound);
                    }
                }
                if (player.x + 50 > 1600) {
                    player.x = 1550;
                    if (!foundWater) {
                        foundWater = true;
                        currentMissionIndex = 1;
                        levelUpDisplayFrames = 180;
                        PlaySoundMulti(levelUpSound);
                    }
                }
                if (player.y + 50 > 1600) {
                    player.y = 1550;
                    if (!foundWater) {
                        foundWater = true;
                        currentMissionIndex = 1;
                        levelUpDisplayFrames = 180;
                        PlaySoundMulti(levelUpSound);
                    }
                }

                //block collisions
                std::list<Block>::iterator colliding;
                for (colliding = blocks.begin(); colliding != blocks.end(); colliding++)
                {
                    if (!Logic::recCollide(player, colliding->form)) {
                        continue;
                    }

                    if (!colliding->solid) {
                        continue;
                    }

                    player.x -= 5 * (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
                    player.y -= 5 * (IsKeyDown(KEY_S) - IsKeyDown(KEY_W));
                    break;
                }

                // Camera controls / alignment
                camera.target = { player.x + 25, player.y + 25 };
                camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

                if (camera.zoom > 3.0f) camera.zoom = 3.0f;
                else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

                renderDistance += 5 * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));
                if (renderDistance > 1700) renderDistance = 1700;
                else if (renderDistance < 200) renderDistance = 200;

                if (IsKeyPressed(KEY_R)) {
                    camera.zoom = 1.0f;
                    renderDistance = 600;
                }

                //Check if any occupied item slots are empty and clear them accordingly
                for (int i = 0; i < 30; i++) {
                    if (inventory[i].type == Item::TYPES::NONE) {
                        continue;
                    }

                    if (inventory[i].amount > 0) {
                        continue;
                    }

                    inventory[i].setSlotEmpty(itemData);
                }
                if (hand.amount < 1 && hand.type != Item::TYPES::NONE) {
                    hand.setSlotEmpty(itemData);
                }
                inventoryFull = Item::isInventoryFull(inventory);

                //Item pickup
                pickupTime = GetTime();
                pickUpDone = false;
                std::list<Item>::iterator collect;
                for (collect = groundItems.begin(); collect != groundItems.end(); collect++) {
                    //if not colliding with item, skip
                    if (inventoryFull) break;

                    if (!Logic::recCollide(player, collect->form)) continue;

                    //if item not old enough, stop
                    if (GetTime() - collect->spawnTime < 1) break;

                    matchingInventoryID = Item::indexOfItem(inventory, collect->ID);
                    //add to existing stack
                    if (collect->stackable) {
                        //add to existing stack
                        if (matchingInventoryID != -1) {
                            inventory[matchingInventoryID].changeAmount(collect->amount);
                            pickUpDone = true;
                            break;
                        }
                    }

                    //create new stack
                    inventory[Item::indexOfItem(inventory, 0)].updateFromArray(itemData, collect->ID, collect->amount);
                    pickUpDone = true;
                    break;
                }
                if (pickUpDone) {
                    //play pickup sound
                    PlaySoundMulti(pickup);
                    //collect->Unload();
                    collect = groundItems.erase(collect);
                    inventoryFull = Item::isInventoryFull(inventory);
                    pickupTime = GetTime() - pickupTime;
                    Debug::displayFloat("Item pickup time (ms): ", pickupTime * 1000);
                }

                //when to display cursor:
                if (inventory[hotbarIndex].type == Item::TYPES::BLOCK && !inventoryOpen) {
                    HideCursor();
                }
                else {
                    ShowCursor();
                }

                //Left-Click events
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    for (int i = 0; i < 30; i++) {
                        if (!inventoryOpen) {
                            break;
                        }

                        if (!Logic::clickRec(GetMouseX(), GetMouseY(), inventory[i].form)) {
                            continue;
                        }

                        if (hand.ID == inventory[i].ID && hand.stackable == false) {
                            inventory[i].amount += hand.amount;
                            hand.setSlotEmpty(itemData);
                            break;
                        }

                        swapID = inventory[i].ID;
                        swapAmount = inventory[i].amount;

                        inventory[i].updateFromArray(itemData, hand.ID, hand.amount, true);
                        hand.updateFromArray(itemData, swapID, swapAmount, true);
                        break;
                    }
                }
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    if (inventory[hotbarIndex].type == Item::TYPES::BLOCK) {
                        place = false;
                        std::list<Block>::iterator placing;
                        for (placing = blocks.begin(); placing != blocks.end(); placing++)
                        {
                            if (inventoryOpen) {
                                break;
                            }
                            if ((placing->x == Logic::snapGridPoint(mouseWorldX, 25)) && placing->y == Logic::snapGridPoint(mouseWorldY, 25)) {
                                Debug::Log("Block was not placed: Space already full.", outputDebug);
                                place = false;
                                break;
                            }
                            if (Logic::recCollide(player, getBlockTarget(mouseWorldX, mouseWorldY))) {
                                Debug::Log("Block was not placed: touching player.", outputDebug);
                                break;
                            }
                            if (Logic::distance(player.x + 25, player.y + 25, Logic::snapGridPoint(mouseWorldX, 25), Logic::snapGridPoint(mouseWorldY, 25)) > 100.0f) {
                                Debug::Log("Block was not placed: Too far away.", outputDebug);
                                break;
                            }
                            place = true;
                        }
                        if (place) {
                            if (inventory[hotbarIndex].ID == Item::NAMES::ACORN) {
                                if (chopTree) {
                                    if (!plantTree) {
                                        plantTree = true;
                                        currentMissionIndex = 3;
                                        levelUpDisplayFrames = 180;
                                        PlaySoundMulti(levelUpSound);
                                    }
                                }
                            }

                            Block block1(blockData, mouseWorldX, mouseWorldY, inventory[hotbarIndex].blockID);
                            blocks.push_back(block1);
                            inventory[hotbarIndex].amount--;
                            PlaySoundMulti(soundAtlas[(int)block1.sounds.y]);
                            Debug::Log("Block successfully placed.", outputDebug);
                        }
                        Debug::endEvent("BLOCK PLACE ATTEMPT", outputDebug);
                    }
                    if (inventory[hotbarIndex].type == Item::TYPES::TOOL) {
                        std::list<Block>::iterator destroy = blocks.begin();
                        for (destroy = blocks.begin(); destroy != blocks.end(); destroy++) {
                            //check if in range
                            if (!(Logic::distance(player.x + 25, player.y + 25, Logic::snapGridPoint(mouseWorldX, 25), Logic::snapGridPoint(mouseWorldY, 25)) < 125.0f)) {
                                break;
                            }

                            //spawn free acorn
                            if (freeSeedCooldown == 0) {
                                groundItems.push_back(Item(itemData, Logic::snapGridPoint(mouseWorldX, 25), Logic::snapGridPoint(mouseWorldY, 25), Item::NAMES::ACORN, Item::STATES::GROUND, 1));
                                freeSeedCooldown = 3600;
                            }

                            //check if clicked on
                            if (!Logic::clickRec(mouseWorldX, mouseWorldY, destroy->form)) {
                                continue;
                            }

                            //check if tool is strong enough
                            if (destroy->toughness > itemData[inventory[hotbarIndex].ID].toolStrength) {
                                Debug::Log("Current tool too weak.");
                                Debug::displayInt("Target toughness: ", destroy->toughness);
                                Debug::displayInt("Tool strength: ", inventory[hotbarIndex].toolStrength);
                                break;
                            }

                            //Block broken successfully
                            //Chop tree goal
                            if (destroy->blockID == Block::NAMES::TREE) {
                                if (foundWater) {
                                    if (!chopTree) {
                                        chopTree = true;
                                        currentMissionIndex = 2;
                                        levelUpDisplayFrames = 180;
                                        PlaySoundMulti(levelUpSound);
                                    }
                                }
                            }
                            if (destroy->blockID == Block::NAMES::NODE_IRON) {
                                if (plantTree) {
                                    if (!mineOre) {
                                        mineOre = true;
                                        currentMissionIndex = 4;
                                        levelUpDisplayFrames = 180;
                                        PlaySoundMulti(levelUpSound);
                                    }
                                }
                            }

                            //spawn item(s)
                            groundItems.push_back(Item(itemData, destroy->x, destroy->y, blockData[destroy->blockID].itemID, Item::STATES::GROUND, (rand() % (blockData[destroy->blockID].dropMax - blockData[destroy->blockID].dropMin + 1) + blockData[destroy->blockID].dropMin)));
                            if (blockData[destroy->blockID].item2ID != Item::NAMES::EMPTY) {
                                groundItems.push_back(Item(itemData, destroy->x + 8, destroy->y + 8, blockData[destroy->blockID].item2ID, Item::STATES::GROUND, (rand() % (blockData[destroy->blockID].drop2Max - blockData[destroy->blockID].drop2Min + 1) + blockData[destroy->blockID].drop2Min)));
                            }

                            //subtract from oreCount array for ore regen system
                            if (destroy->blockID >= Block::NAMES::NODE_COAL && destroy->blockID <= Block::NAMES::NODE_URANIUM) {
                                oreCounts[destroy->blockID - Block::NAMES::NODE_COAL] -= 1;
                            }
                            PlaySoundMulti(soundAtlas[(int)destroy->sounds.x]);
                            //destroy->Unload();
                            destroy = blocks.erase(destroy);
                            break;
                        }
                    }
                }
                if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                    Debug::startEvent("Interaction attempt", outputDebug);
                    for (std::list<Block>::iterator i = blocks.begin(); i != blocks.end(); i++) {
                        //find block clicked
                        if (!Logic::clickRec(mouseWorldX, mouseWorldY, i->form)) {
                            continue;
                        }

                        //check if block has a gui
                        if (i->guiID == GUI::NAMES::NONE) {
                            break;
                        }

                        //check if in range
                        if (!(Logic::distance(player.x + 25, player.y + 25, Logic::snapGridPoint(mouseWorldX, 25), Logic::snapGridPoint(mouseWorldY, 25)) < 100.0f)) {
                            break;
                        }

                        //check if gui has recipes
                        recipeIndex = GUI::firstInStation(recipeData, recipeCount, i->guiID);
                        if (recipeIndex == -1) {
                            break;
                        }

                        //Prepare GUI
                        currentGUI = i->guiID;
                        Debug::displayInt("gui: ", currentGUI);
                        recipeAvailable = GUI::isRecipeAvailable(recipeData, inventory, recipeIndex);
                        //recipeData[recipeIndex].Load();
                        inventoryOpen = true;
                    }
                    Debug::endEvent("Interaction attempt", outputDebug);
                }

                //TOGGLE INVENTORY

                if (IsKeyPressed(KEY_E)) {
                    inventoryOpen = !inventoryOpen;
                }

                //HOTBAR INDEX SELECTION
                keyPressed = GetKeyPressed();
                if (Logic::numberInRangeI(keyPressed, KEY_ONE, KEY_FIVE)) {
                    hotbarIndex = keyPressed - KEY_ONE;
                }

                //DROP ITEM
                if (IsKeyPressed(KEY_Q)) {
                    //Set source of dropped item
                    if (!inventoryOpen) {
                        dropPtr = &inventory[hotbarIndex];
                    }
                    else {
                        dropPtr = &hand;
                    }

                    //Drop logic
                    if (dropPtr->ID != Item::NAMES::EMPTY && dropPtr->amount > 0) {
                        groundItems.push_back(Item(itemData, player.x, player.y, dropPtr->ID, Item::STATES::GROUND, 1));
                        dropPtr->amount--;
                    }
                    inventoryFull = Item::isInventoryFull(inventory);
                }

                //Toggle Debug text in game:
                if (IsKeyPressed(KEY_F1)) {
                    //displayHelp = !displayHelp;
                    displayHelp = false;
                }
                //Toggle Debug Event Logs/Messages
                if (IsKeyPressed(KEY_F2)) {
                    //outputDebug = !outputDebug;
                    outputDebug = false;
                }

                //Set empty slots empty
                for (int i = 0; i < 30; i++) {
                    if (inventory[i].amount < 1 && inventory[i].type != Item::TYPES::NONE) {
                        inventory[i].setSlotEmpty(itemData);
                    }
                }
                //Set empty hand empty
                if (hand.amount < 1 && hand.type != Item::TYPES::NONE) {
                    hand.setSlotEmpty(itemData);
                }
                inventoryFull = Item::isInventoryFull(inventory);

                //Spawn ores when less than 40
                for (int i = Block::NAMES::NODE_COAL; i < Block::NAMES::NODE_URANIUM + 1; i++) {
                    if (oreCounts[i - Block::NAMES::NODE_COAL] >= 40) {
                        continue;
                    }

                    spawnX = Logic::Random(2375, -800);
                    spawnY = Logic::Random(2375, -800);
                    place = true;

                    if (Logic::recCollide(player, getBlockTarget(spawnX, spawnY))) {
                        Debug::Log("Block collides with player. Attempting on next iteration...");
                        continue;
                    }

                    std::list<Block>::iterator foundBlock;
                    for (foundBlock = blocks.begin(); foundBlock != blocks.end(); foundBlock++)
                    {
                        if ((foundBlock->x == Logic::snapGridPoint(spawnX, 25)) && foundBlock->y == Logic::snapGridPoint(spawnY, 25)) {
                            Debug::Log("Block was not placed: Space already full.", outputDebug);
                            place = false;
                            break;
                        }
                    }

                    if (!place) {
                        continue;
                    }

                    Debug::Log("Block placed successfully.");
                    blocks.push_back(Block(blockData, spawnX, spawnY, i));
                    oreCounts[i - Block::NAMES::NODE_COAL]++;
                }

#pragma endregion
            }
            else {
                #pragma region GUILogic

                //item pickup
                pickUpDone = false;
                std::list<Item>::iterator collect;
                for (collect = groundItems.begin(); collect != groundItems.end(); collect++) {
                    //if not colliding with item, skip
                    if (inventoryFull) break;

                    if (!Logic::recCollide(player, collect->form)) continue;

                    //if item not old enough, stop
                    if (GetTime() - collect->spawnTime < 1) break;

                    matchingInventoryID = Item::indexOfItem(inventory, collect->ID);
                    //add to existing stack
                    if (collect->stackable) {
                        //add to existing stack
                        if (matchingInventoryID != -1) {
                            inventory[matchingInventoryID].changeAmount(collect->amount);
                            pickUpDone = true;
                            break;
                        }
                    }

                    //create new stack
                    inventory[Item::indexOfItem(inventory, 0)].updateFromArray(itemData, collect->ID, collect->amount);
                    pickUpDone = true;
                    break;
                }
                if (pickUpDone) {
                    //play pickup sound
                    PlaySoundMulti(pickup);
                    //collect->Unload();
                    collect = groundItems.erase(collect);
                    inventoryFull = Item::isInventoryFull(inventory);
                    pickupTime = GetTime() - pickupTime;
                    Debug::displayFloat("Item pickup time (ms): ", pickupTime * 1000);
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (Logic::clickRec(GetMouseX(), GetMouseY(), buttonForm)) {
                        if (recipeAvailable) {
                            //decrease item amount in inventory
                            suitableP1Index = Item::indexOfItem(inventory, recipeData[recipeIndex].part1Data.x, recipeData[recipeIndex].part1Data.y);
                            suitableP2Index = Item::indexOfItem(inventory, recipeData[recipeIndex].part2Data.x, recipeData[recipeIndex].part2Data.y);

                            inventory[suitableP1Index].amount -= recipeData[recipeIndex].part1Data.y;
                            inventory[suitableP2Index].amount -= recipeData[recipeIndex].part2Data.y;

                            PlaySound(soundAtlas[SOUNDS::CRAFT_WORKBENCH]);

                            if (recipeData[recipeIndex].outputData.x == -1) {
                                if (makeUraniumFuel) {
                                    if (!sendSignal) {
                                        sendSignal = true;
                                        currentMissionIndex = 9;
                                    }
                                }
                            }
                            else {
                                //drop the item
                                Item newItem(itemData, player.x, player.y, recipeData[recipeIndex].outputData.x, Item::STATES::GROUND, recipeData[recipeIndex].outputData.y);

                                newItem.spawnTime = -1;
                                groundItems.push_back(newItem);

                                //CRAFT GOALS
                                if (newItem.ID == Item::NAMES::FURNACE) {
                                    if (mineOre) {
                                        if (!makeFurnace) {
                                            makeFurnace = true;
                                            currentMissionIndex = 5;
                                            levelUpDisplayFrames = 180;
                                            PlaySoundMulti(levelUpSound);
                                        }
                                    }
                                }
                                if (Logic::numberInRangeI(newItem.ID, Item::NAMES::BAR_COPPER, Item::NAMES::BAR_TUNGSTEN)) {
                                    if (makeFurnace) {
                                        if (!smeltBar) {
                                            smeltBar = true;
                                            currentMissionIndex = 6;
                                            levelUpDisplayFrames = 180;
                                            PlaySoundMulti(levelUpSound);
                                        }
                                    }
                                }
                                if (Logic::numberInRangeI(newItem.ID, Item::NAMES::IRON_TOOL, Item::NAMES::STEEL_TOOL)) {
                                    if (smeltBar) {
                                        if (!newTool) {
                                            newTool = true;
                                            currentMissionIndex = 7;
                                            levelUpDisplayFrames = 180;
                                            PlaySoundMulti(levelUpSound);
                                        }
                                    }
                                }
                                if (Logic::numberInRangeI(newItem.ID, Item::NAMES::ALLOY_BRONZE, Item::NAMES::ALLOY_STEEL)) {
                                    if (newTool) {
                                        if (!smeltAlloy) {
                                            smeltAlloy = true;
                                            currentMissionIndex = 8;
                                            levelUpDisplayFrames = 180;
                                            PlaySoundMulti(levelUpSound);
                                        }
                                    }
                                }
                                
                                if (newItem.ID == Item::NAMES::URANIUM_FUEL) {
                                    if (makeFurnace) {
                                        if (!makeUraniumFuel) {
                                            makeUraniumFuel = true;
                                            currentMissionIndex = 9;
                                            levelUpDisplayFrames = 180;
                                            PlaySoundMulti(levelUpSound);
                                        }
                                    }
                                }
                                //END OF CRAFT GOALS

                                //Set empty slots empty
                                for (int i = 0; i < 30; i++) {
                                    if (inventory[i].amount < 1 && inventory[i].type != Item::TYPES::NONE) {
                                        inventory[i].setSlotEmpty(itemData);
                                    }
                                }
                                //Set empty hand empty
                                if (hand.amount < 1 && hand.type != Item::TYPES::NONE) {
                                    hand.setSlotEmpty(itemData);
                                }
                                inventoryFull = Item::isInventoryFull(inventory);

                                //set recipe availability as needed
                                recipeAvailable = GUI::isRecipeAvailable(recipeData, inventory, recipeIndex);
                            }
                            
                        }
                    }
                    if (Logic::clickRec(GetMouseX(), GetMouseY(), exitBox)) {
                        currentGUI = GUI::NAMES::NONE;
                        inventoryOpen = false;
                    }
                    if (Logic::clickRec(GetMouseX(), GetMouseY(), previousBox)) {
                        recipeIndex = GUI::retreatByStation(recipeData, recipeCount, recipeIndex, currentGUI);
                        recipeAvailable = GUI::isRecipeAvailable(recipeData, inventory, recipeIndex);
                    }
                    if (Logic::clickRec(GetMouseX(), GetMouseY(), nextBox)) {
                        recipeIndex = GUI::advanceByStation(recipeData, recipeCount, recipeIndex, currentGUI);
                        recipeAvailable = GUI::isRecipeAvailable(recipeData, inventory, recipeIndex);
                    }
                }
                if (IsKeyPressed(KEY_SPACE)) {
                    currentGUI = GUI::NAMES::NONE;
                    inventoryOpen = false;
                }

                keyPressed = GetKeyPressed();
                if (keyPressed == KEY_RIGHT || keyPressed == KEY_LEFT) {
                    recipeIndex = GUI::shiftByStation(recipeData, recipeCount, recipeIndex, currentGUI, keyPressed - KEY_RIGHT);
                    recipeAvailable = GUI::isRecipeAvailable(recipeData, inventory, recipeIndex);
                }
#pragma endregion
            }

            freeSeedCooldown -= (int)(bool)freeSeedCooldown;
            #pragma region WorldReadinessCalc
            worldReadinessLevel = foundWater + chopTree + plantTree + mineOre + smeltBar + newTool + smeltAlloy + makeFurnace + makeUraniumFuel + sendSignal;
            #pragma endregion
            if (worldReadinessLevel == 10) {
                gameWon = true;
            }

            logicTime = GetTime() - logicTime;

            #pragma region Render
            //RENDER
            debugTimeInit = GetTime();
            BeginDrawing();
            BeginMode2D(camera);

            //Draw background and elements
            ClearBackground(DARKBLUE);
            DrawTexture(background, -800, -800, RAYWHITE);

            //draw blocks
            Block::DrawList(blocks, blockData, player, renderDistance);
            //draws placement indicator
            if (inventory[hotbarIndex].type == Item::TYPES::BLOCK && !inventoryOpen) {
                if (Logic::distance(player.x + 25, player.y + 25, Logic::snapGridPoint(mouseWorldX, 25), Logic::snapGridPoint(mouseWorldY, 25)) < 100.0f) {
                    DrawRectangleRec({ roundf((mouseWorldX) / 25) * 25, roundf((mouseWorldY) / 25) * 25, 25, 25 }, { 80, 80, 80, 100 });
                }
            }

            //Draw items
            Item::DrawList(groundItems, itemData, player, renderDistance);

            //draw item name if touching it
            std::list<Item>::iterator itemHover = groundItems.begin();
            for (itemHover; itemHover != groundItems.end(); itemHover++) {
                if (!CheckCollisionPointRec({ mouseWorldX, mouseWorldY }, itemHover->form)) {
                    continue;
                }
                DrawText(itemHover->name, mouseWorldX, mouseWorldY - 10, 15, BLACK);
                break;
            }

            //draw player
            DrawTexture(playerTex, player.x, player.y, RAYWHITE);

            EndMode2D();

            //Draw hotbar and inventory
            DrawText(inventory[hotbarIndex].name, 600, 25, 15, BLACK);
            DrawTexture(hotbarBack, 600, 40, RAYWHITE);
            DrawTexture(slotSelected, 600 + 32 * hotbarIndex, 40, RAYWHITE);
            if (inventoryOpen) {
                DrawTexture(inventoryBack, 600, 80, RAYWHITE);
            }
            for (int i = 0; i < 30; i++) {
                if (i > 4) {
                    if (!inventoryOpen) {
                        break;
                    }
                }
                if (inventory[i].ID == Item::NAMES::EMPTY) {
                    continue;
                }
                inventory[i].Draw(itemData, inventory[i].x, inventory[i].y);
            }
            if (inventoryOpen) {
                hand.Draw(itemData, GetMouseX(), GetMouseY());
            }

            if (currentGUI != GUI::NAMES::NONE) {
                GUI::drawStation(recipeData, itemData, inventory, recipeIndex, recipeAvailable, available, notAvailable);      
                switch (currentGUI) {
                case GUI::NAMES::WORKBENCH:
                    DrawText("Workbench", 550 - MeasureText("Workbench", 15)-10, 258, 15, BLACK);
                    break;
                case GUI::NAMES::FURNACE:
                    DrawText("Furnace", 550 - MeasureText("Furnace", 15)-10, 258, 15, BLACK);
                    break;
                case GUI::NAMES::ANVIL:
                    DrawText("Anvil", 550 - MeasureText("Anvil", 15)-10, 258, 15, BLACK);
                    break;
                case GUI::NAMES::SIGNALLER:
                    DrawText("Stellar Signaller", 550 - MeasureText("Stellar Signaller", 15)-10, 258, 15, BLACK);
                    break;
                }

                DrawTexture(exit, exitBox.x, exitBox.y, RAYWHITE);
                DrawText("Exit (SPACE)", exitBox.x + exitBox.width + 5, exitBox.y + 8, 15, BLACK);
                DrawTexture(previous, previousBox.x, previousBox.y, RAYWHITE);
                if (currentGUI != GUI::NAMES::SIGNALLER) {
                    DrawText(itemData[(int)recipeData[GUI::retreatByStation(recipeData, recipeCount, recipeIndex, currentGUI)].outputData.x].name, previousBox.x + previousBox.width + 5, previousBox.y + 8, 15, BLACK);
                }
                DrawTexture(next, nextBox.x, nextBox.y, RAYWHITE);
                if (currentGUI != GUI::NAMES::SIGNALLER) {
                    DrawText(itemData[(int)recipeData[GUI::advanceByStation(recipeData, recipeCount, recipeIndex, currentGUI)].outputData.x].name, nextBox.x - MeasureText(itemData[(int)recipeData[GUI::advanceByStation(recipeData, recipeCount, recipeIndex, currentGUI)].outputData.x].name, 15) - 5, previousBox.y + 8, 15, BLACK);
                }
            }

            //draw item names in inventory
            for (int i = 0; i < 30; i++) {
                if (!CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, inventory[i].form)) {
                    continue;
                }
                if (inventory[i].ID == Item::NAMES::EMPTY) {
                    break;
                }
                DrawText(inventory[i].name, GetMouseX(), GetMouseY() - 10, 15, BLACK);
                break;
            }
            debugTimeEnd = GetTime();
            debugTimeDelta = debugTimeEnd - debugTimeInit;

            DrawFPS(100, 400);

            //Debug purposes
            if (displayHelp) {
                DrawText(TextFormat("FPS: %02i", GetFPS()), 20, 20, 10, BLACK);
                DrawText(TextFormat("Camera zoom: %03.2f", camera.zoom), 20, 40, 10, BLACK);
                DrawText(TextFormat("Render Distance: %04i", renderDistance), 20, 60, 10, BLACK);
                DrawText(TextFormat("BLOCK LIST SIZE: %04i", blocks.size()), 20, 80, 10, BLACK);
                DrawText(TextFormat("GROUND ITEM LIST SIZE: %05i", groundItems.size()), 20, 100, 10, BLACK);
                DrawText(TextFormat("Logic Time (all) (ms): %03.3f", logicTime * 1000), 20, 120, 10, BLACK);
                DrawText(TextFormat("Draw Time (all) (ms): %03.1f", debugTimeDelta * 1000), 20, 140, 10, BLACK);
                DrawText(TextFormat("Tool strength: %01i", itemData[inventory[hotbarIndex].ID].toolStrength), 20, 160, 10, BLACK);
                DrawText(TextFormat("World Readiness: %02i", worldReadinessLevel), 20, 180, 10, BLACK);

                for (int i = 0; i < 6; i++) {
                    DrawText(TextFormat("ORE AMOUNT: %03i", oreCounts[i]), 20, 200 + (20 * i), 10, BLACK);
                }
            }

            prefixBox.width = MeasureText(prefix, 20) + 10;
            DrawRectangleRec(prefixBox, DARKBLUE);
            DrawText(prefix, prefixBox.x + 5, prefixBox.y + 5, 20, BLACK);

            missionNameBox.width = MeasureText(missionNames[currentMissionIndex], 20) + 10;
            DrawRectangleRec(missionNameBox, BLUE);
            DrawText(missionNames[currentMissionIndex], missionNameBox.x + 5, missionNameBox.y + 5, 20, BLACK);
            
            if (levelUpDisplayFrames > 0) {
                levelUpDisplayFrames--;
                DrawTexture(levelUp, 200, 100, RAYWHITE);
            }

            EndDrawing();

#pragma endregion
        }

        #pragma region END_CUTSCENE
        //THIS IS THE ENDING
        //COOL
        camera.offset = {0,0};
        BeginDrawing();
        DrawTexture(endScreen, 0, -1600, RAYWHITE);
        EndDrawing();

        WaitTime(2);

        while (endY != 0) {
            BeginDrawing();
            BeginMode2D(camera);
            
            ClearBackground(BLACK);
            DrawTexture(endScreen, 0, 0, RAYWHITE);
            DrawTexture(levelUp, 400, 400, RAYWHITE);
            camera.target = { (float)endX, (float)endY };
            endY -= 5;

            EndMode2D();
            EndDrawing();
        }
        
        WaitTime(1);
        break;
        #pragma endregion
    }
    
    //unload item and block textures
    for (int i = 0; i < blockCount; i++) {
        UnloadTexture(blockData[i].tex);
    }
    for (int i = 0; i < itemCount; i++) {
        UnloadTexture(itemData[i].tex);
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}