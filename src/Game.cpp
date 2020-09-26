#include "Game.h"

Game::Game()
{
    sAppName = "Pac-Man";
}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    ImageAssets::get().LoadSprites();

    level = std::make_unique<Level>(this);
    level->SetSprite("Wall", true);
    
    player = std::make_unique<Player>(this);
    player->SetTileID({ 0, 0 });
    player->SetSprite("Pacman", true);
    player->SetMovement(olc::vf2d(15.5f, 23.0f), 5.0f, olc::vi2d(-1, 0));    
    
    ghost = std::make_unique<Ghost>(this, GhostHouse::IN, 4.0f, 60);
    ghost->SetTileID({4, 3});
    ghost->SetSprite("Ghost", true);
    ghost->SetMovement(olc::vf2d(17.5f, 14.5f), 5.0f, olc::vi2d(-1, 0));
    ghost->CheckMode();
    
    deadPacman = std::make_unique<Object>(this);
    deadPacman->SetSprite("DeadPacman", true);
    
    wait = 0.0f;
    waitDead = 0.0f;
    
    isDead = false;

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
   wait += fElapsedTime;
   if (!isDead)
   {
       if (wait > 5.0f)
       {
           if (level->GetTile(player->position) == '*' ||
               level->GetTile(player->position) == '0')
           {
               if (level->GetTile(player->position) == '*')
               {
                   player->energized = true;
                   player->energizedTimer = 0.0f;
               }

               level->SetTile(player->position, '_');
               level->pelletCounter++;
               level->noPelletTimer = 0.0f;

               if (ghost->mode != GhostMode::FRIGHTENED)
                   ghost->invertDir = true;
           }
           else if (level->GetTile(player->position) == 'x' ||
               level->GetTile(player->position) == 'X')
           {
               if (level->GetTile(player->position) == 'X')
               {
                   player->energized = true;
                   player->energizedTimer = 0.0f;
               }
               level->SetTile(player->position, '+');
               level->pelletCounter++;
               level->noPelletTimer = 0.0f;

               if (ghost->mode != GhostMode::FRIGHTENED)
                    ghost->invertDir = true;
           }

           // Ghost goes back to Scatter or Chase mode
           if (player->energizedTimer > 6.9f)
           {
               ghost->mode = ghost->lastmode;
               ghost->tileID.y = ghost->initTileID.y;
           }

           player->Update(fElapsedTime, level.get());
   
           ghost->Update(fElapsedTime, level.get(), player.get(), { 26, 3 }, player->position);
   
           level->Update(fElapsedTime);
       }       
   }
   
   if (cmp(player->position, ghost->position, 0.5f) ||
       cmp(ghost->position, player->position, 0.5f) &&
       waitDead < 1.5f)
   {
       if (player->energized)
       {           
           std::cout << "Immune" << std::endl;
       }
       else
       {
           waitDead += fElapsedTime;
           isDead = true;
       }
   }   
   
   if (deadPacman->tileID.x == 13)
   {
       if (wait > 0.5f)
       {
           deadPacman->tileID = { 0, 0 };
           player->Restart();
           ghost->Restart();
           wait = 3.5f;
           waitDead = 0.0f;
           isDead = false;
       }
   }
   
   Clear(olc::BLACK);
   
   if (waitDead < 1.5f)
   {
       ghost->DrawPartial({ 168.0f + ghost->position.x * 16.0f, 32.0f + ghost->position.y * 16.0f }, { 32, 32 });
   
       player->DrawPartial({ 168.0f + player->position.x * 16.0f, 32.0f + player->position.y * 16.0f }, { 32, 32 });
   }
   
   if (isDead && deadPacman->tileID.x != 13 && waitDead > 1.5f)
   {
       if (wait >= 0.1f)
       {
           deadPacman->Animate(0, 13, fElapsedTime, 0.0f);
           wait = 0.0f;
       }
       deadPacman->DrawPartial({ 168.0f + player->position.x * 16.0f, 32.0f + player->position.y * 16.0f }, { 32, 32 });
   }
   
   level->Draw();

    return !GetKey(olc::ESCAPE).bPressed;
}
