#include "raylib.h"



//Structure for window screen
struct window
{
    int width = 1200;
    int height = 800;
}window;


//Structure for the player
struct Players
{
    float width = 50;
    float height = 80;
    int velocity = 0;
    float posY;
    float posX;
    int jumpVelocity = 600;
    int health = 100;
    int punchCount = 0;
    int hitCount = 0;
    int dashCount = 0;
    int knockCount = 0;
    int totalHitCount = 0;
    int evasiveCount = 0;
    int clashCount = 0;
    double stunTime = 0;
    double punchCooldown = 0;
    double punchResetCount = 0;
    double dashCooldown = 0;
    double hitResetCount = 0;
    double airCooldown = 0;
    double crouchCooldown = 0;
    double blockCooldown = 0;
    double roundTimer = 0;
    double time = 0;
    bool isGrounded = true;   
    bool isBlocking = false;
    bool isCrouching = false;
    bool doubleJump = false;
    bool isStunned = false;
    bool isFacingRight = false;
    bool isBlockBroken = false;
    bool isAttacking = false;
    bool roundTwo = false;
    bool isClashing = false;
}player,enemy;


//Platform structure
struct Platform
{
    int posX = 0;
    int posY = 600;
    int width = 1500;
    int height = 100;
}platform;


//Health bar  
void DrawHealthBar(int posX, int posY, int width, int height, int currentHealth, int maxHealth, Color barColor, Color backgroundColor)
{
    int barWidth = (width * currentHealth) / maxHealth;

    DrawRectangle(posX, posY, width, height, backgroundColor);

    DrawRectangle(posX, posY, barWidth, height, barColor);
}


int main(void)
{
    //Player variables
    player.posY = window.height - (player.height+101);
    player.posX = window.width/2 - player.width/2 - 350;
       
       
    //Enemy variables
    enemy.posY = window.height - (enemy.height+101);
    enemy.posX = window.width/2 - enemy.width/2 + 325;
     
     
    //Window Screen
    InitWindow(window.width, window.height, "Geometry Fighters");
    
    
    //Generating a random background
    char fileName[] = "background1.png";
    int num =(GetRandomValue(49,52));
    fileName[10] = num;
    Texture2D background = LoadTexture(fileName);
    
    
    //Frames
    SetTargetFPS(60); 
    bool gameStart;
    
    
    //Gravity
    const int gravity = 1000;


    //Main menu screen
    while (!WindowShouldClose())
    {
        //Begins Drawing
        BeginDrawing();
        
            //Drawing main menu
            ClearBackground(BLACK);      
            Texture2D background = LoadTexture("background5.png");
            DrawTexture(background, 0, 0, WHITE);
            DrawText("WELCOME TO", 400, 60, 55, BLACK);
            DrawText("GEOMETRY", 275, 140, 55, BLUE);
            DrawText("FIGHTERS", 620, 140, 55, MAROON);
            DrawText("PRESS ENTER TO \n     CONTINUE", 455, 400, 32, BLACK);
            
            
            //Player Controls
            DrawText("Player Controls:", 50, 280, 32, BLUE);
            DrawText("Movement: WASD", 50, 335, 23, SKYBLUE);
            DrawText("Attack: E", 50, 370, 23, SKYBLUE);
            DrawText("Dash: Q", 50, 405, 23, SKYBLUE);
            DrawText("Crouch: Left CTRL", 50, 440, 23, SKYBLUE);
            DrawText("Evasive: R", 50, 475, 23, SKYBLUE);
            
            //Enemy Controls       
            DrawText("Enemy Controls:", 880, 280, 32, MAROON);
            DrawText("Movement: ARROW KEYS", 880, 335, 23, RED);
            DrawText("Attack: KP 0", 880, 370, 23, RED);
            DrawText("Dash: KP 2", 880, 405, 23, RED);
            DrawText("Crouch: Right CTRL", 880, 440, 23, RED);
            DrawText("Evasive: KP 1", 880, 475, 23, RED);
            
            //Loading actual game
            if(IsKeyPressed(KEY_ENTER))
            {
                gameStart = true;
                break; 
            }
            else if(IsKeyPressed(KEY_A))
            {
                gameStart = true;
                player.health == 10000;
                break; 
            }

        EndDrawing();
    } 


    // Main game loop
    while (!WindowShouldClose()) 
    {       


        //Frame Time
        const float dT = GetFrameTime();   


        //Player and Enemy collision
        bool collision = false;
        if (CheckCollisionRecs((Rectangle){ player.posX, player.posY, player.width, player.height }, (Rectangle){ enemy.posX, enemy.posY, enemy.width, enemy.height }))
        {          
            collision = true;        
        }
          
          
        //HitBoxes for player attacks
        if (player.isFacingRight ? CheckCollisionRecs((Rectangle){ player.posX+player.width, player.posY + 10, 30, 10 }, (Rectangle){enemy.posX, enemy.posY, enemy.width, enemy.height }) && GetTime() - player.time < 0.15 : CheckCollisionRecs((Rectangle){player.posX-player.width + 20,player.posY+10,30,10}, (Rectangle){enemy.posX, enemy.posY, enemy.width, enemy.height }) && GetTime() - player.time < 0.15)
        {
            //Checks if enemy is blocking
            if(!enemy.isBlocking && !player.isCrouching && player.isGrounded)
            {
                //Checks for hitcount to prevent rapid punches just from one click
                if (player.hitCount >= 1)
                {
                    player.totalHitCount += 1;
                    player.hitCount = 0;
                    enemy.stunTime = 1.2;
                    enemy.evasiveCount += 10;
                    enemy.health -= 4; 
                    if(player.isFacingRight)
                    {
                        enemy.posX += 2;
                    }
                    else
                    {
                        enemy.posX -=2;
                    }                    
                }
            }
       
       
            //Updates time
            player.punchResetCount = GetTime();
            player.hitResetCount = GetTime();  
            
            
            //Adding knockback on 4th hit
            if (player.punchCount >= 4 && !player.isCrouching)
            {               
                //Player Block Breaking the enemy
                if(enemy.isBlocking)
                {                    
                    enemy.isBlocking = false;
                    player.totalHitCount += 1;
                    enemy.evasiveCount += 4;
                    player.hitCount = 0;
                    enemy.health -= 2;
                    enemy.stunTime = 1.5;
                    if(player.isFacingRight)
                    {
                        enemy.knockCount += 1;
                    }
                    else
                    {
                        enemy.knockCount += 1;
                    }
                    player.stunTime = 0.25;
                    player.punchCount = 0;                         
                }
                else
                {                    
                    player.hitCount = 0;
                    enemy.health -= 1;
                    enemy.stunTime = 0.40;
                    if(player.isFacingRight)
                    {
                        enemy.knockCount += 1;                            
                    }
                    else
                    {
                        enemy.knockCount += 1;
                    }
                    player.stunTime = 0.25;
                    player.punchCount = 0;                    
                }                                 
            }                                                    
        }
           
           
        //Player crouch attack
        if (player.isFacingRight ? CheckCollisionRecs((Rectangle){player.posX+player.width,player.posY-15,15,40}, (Rectangle){enemy.posX, enemy.posY, enemy.width, enemy.height }) && GetTime() - player.time < 0.2 : CheckCollisionRecs((Rectangle){player.posX-player.width + 35,player.posY-15,15,40}, (Rectangle){enemy.posX, enemy.posY, enemy.width, enemy.height }) && GetTime() - player.time < 0.2)
        {
            //Checks for hitcount to prevent rapid punches just from one click
            if (player.hitCount >= 1 && player.isCrouching)
            {
                enemy.isBlocking = false;
                player.isCrouching = false;
                player.height *= 2;
                player.hitCount = 0;
                enemy.evasiveCount += 12;
                player.totalHitCount += 1;
                enemy.stunTime = 1.2;
                enemy.health -= 9;
                player.stunTime = 0.5;
                if(enemy.isCrouching)
                {
                    enemy.isCrouching = false;
                    enemy.height *= 2;
                }                
            }
        }
        
        
        //Player Aerial attack
        if (player.isFacingRight ? CheckCollisionRecs((Rectangle){player.posX+player.width,player.posY+70,15,40}, (Rectangle){enemy.posX, enemy.posY, enemy.width, enemy.height }) && GetTime() - player.time < 0.35 : CheckCollisionRecs((Rectangle){player.posX-player.width + 35,player.posY+70,15,40}, (Rectangle){enemy.posX, enemy.posY, enemy.width, enemy.height }) && GetTime() - player.time < 0.35)
        {
            //Checks if enemy is blocking
            if(!enemy.isBlocking && !player.isGrounded && enemy.isGrounded)
            {
                //Checks for hitcount to prevent rapid punches just from one click
                if (player.hitCount >= 1)
                {
                    player.hitCount = 0;
                    enemy.evasiveCount += 5;
                    player.totalHitCount += 1;
                    enemy.health -= 2; //More of a combo extender thats easy to hit rather than good damage
                    enemy.stunTime = 0.75;
                    player.stunTime = 0.5;  
                    if(enemy.isCrouching)
                    {
                        enemy.isCrouching = false;
                        enemy.height *= 2;
                    }                    
                }
            }          
            else if(!enemy.isGrounded)
            {
                if (player.hitCount >= 1)
                {
                    player.hitCount = 0;
                    enemy.evasiveCount += 7;
                    player.totalHitCount += 1;
                    enemy.health -= 4;                
                }
            }                              
            if(enemy.isBlocking && !player.isGrounded)
            {
                enemy.knockCount += 1;
            }           
        }
        
        
        //Hitboxes for enemy attacks
        if (enemy.isFacingRight ? CheckCollisionRecs((Rectangle){ enemy.posX+enemy.width, enemy.posY + 10, 30, 10 }, (Rectangle){player.posX, player.posY, player.width, player.height }) && GetTime() - enemy.time < 0.15 : CheckCollisionRecs((Rectangle){ enemy.posX-enemy.width + 20,enemy.posY+10,30,10}, (Rectangle){player.posX, player.posY, player.width, player.height }) && GetTime() - enemy.time < 0.15)
        {
            //Checks if player is blocking
            if(!player.isBlocking && !enemy.isCrouching)
            {
                //Checks for hitcount to prevent rapid punches just from one input            
                if (enemy.hitCount >= 1)
                {
                    enemy.totalHitCount += 1;
                    enemy.hitCount = 0;
                    player.evasiveCount += 10;
                    player.stunTime = 1.2;
                    player.health -= 4;   
                    if(enemy.isFacingRight)
                    {
                        player.posX += 2;
                    }
                    else
                    {
                        player.posX-=2;
                    }                   
                }               
            }
                
                
                //Updates Time
                enemy.punchResetCount = GetTime();
                enemy.hitResetCount = GetTime();


                //Adding knockback on 4th hit
                if (enemy.punchCount >= 4 && !enemy.isCrouching)
                {                   
                    //Enemy blockbreaking Player
                    if(player.isBlocking)
                    {                      
                        player.isBlocking = false;
                        enemy.totalHitCount += 1;
                        enemy.hitCount = 0;
                        player.evasiveCount += 4;
                        player.health -= 2; 
                        player.stunTime = 1.5;
                        if(enemy.isFacingRight)
                        {
                            player.knockCount += 1;;
                        }
                        else
                        {
                            player.knockCount += 1;
                        }
                        enemy.stunTime = 0.75;
                        enemy.punchCount = 0;                      
                    }  
                    else
                    {                      
                        enemy.hitCount = 0;
                        player.health -= 1;  
                        player.stunTime = 0.4;
                        if(enemy.isFacingRight)
                        {
                            player.knockCount += 1;
                        }
                        else
                        {
                            player.knockCount += 1;
                        }
                        enemy.stunTime = 0.75;
                        enemy.punchCount = 0;                       
                    }                    
                }                              
        }
 
 
         //Enemy crouch attack
        if (enemy.isFacingRight ? CheckCollisionRecs((Rectangle){enemy.posX+enemy.width,enemy.posY-15,15,40}, (Rectangle){player.posX, player.posY, player.width, player.height }) && GetTime() - enemy.time < 0.2 : CheckCollisionRecs((Rectangle){enemy.posX-enemy.width + 35,enemy.posY-15,15,40}, (Rectangle){player.posX, player.posY, player.width, player.height }) && GetTime() - enemy.time < 0.2)
        {
            //Checks for hitcount to prevent rapid punches just from one click
            if (enemy.hitCount >= 1 && enemy.isCrouching)
            {
                enemy.isCrouching = false;
                enemy.height *= 2;
                enemy.hitCount = 0;
                player.evasiveCount += 12;
                enemy.totalHitCount += 1;
                player.stunTime = 1.2;
                enemy.stunTime = 0.5;
                player.health -= 9;
                if(player.isCrouching)
                {
                    player.isCrouching = false;
                    player.height *= 2;
                }   
            }
        }
     
     
        //Enemy Aerial attack
        if (enemy.isFacingRight ? CheckCollisionRecs((Rectangle){enemy.posX+enemy.width,enemy.posY+70,15,40}, (Rectangle){player.posX, player.posY, player.width, player.height }) && GetTime() - enemy.time < 0.35 : CheckCollisionRecs((Rectangle){enemy.posX-enemy.width + 35,enemy.posY+70,15,40}, (Rectangle){player.posX, player.posY, player.width, player.height }) && GetTime() - enemy.time < 0.35)
        {
            //Checks if enemy is blocking
            if(!player.isBlocking && !enemy.isGrounded && player.isGrounded)
            {
                //Checks for hitcount to prevent rapid punches just from one click
                if (enemy.hitCount >= 1)
                {
                    enemy.hitCount = 0;
                    enemy.totalHitCount = 1;
                    player.evasiveCount += 5;
                    player.health -= 2;
                    player.stunTime = 0.75;
                    enemy.stunTime = 0.5; 
                    if(player.isCrouching)
                    {
                        player.isCrouching = false;
                        player.height *= 2;
                    }  
                }
            }              
            else if(!player.isGrounded)
            {
                if (enemy.hitCount >= 1)
                {
                    enemy.hitCount = 0;
                    player.evasiveCount += 7;
                    enemy.totalHitCount += 1;
                    player.health -= 4;                
                }
            }
            if(player.isBlocking && !enemy.isGrounded)
            {
                player.knockCount += 1;
            }
        }


        //Checks to see if their attacks hit at the exact same time
        if (CheckCollisionRecs((Rectangle){player.posX+player.width,player.posY+10,30,10 }, (Rectangle){enemy.posX-enemy.width + 20,enemy.posY+10,30,10}) && GetTime() - enemy.time < 0.1 && GetTime() - player.time < 0.1)
        {
            player.isClashing = true;
            enemy.isClashing = true;
        }
        
        
        //Other side
        if (CheckCollisionRecs((Rectangle){player.posX-player.width + 20,player.posY+10,30,10 }, (Rectangle){enemy.posX+enemy.width,enemy.posY+10,30,10}) && GetTime() - enemy.time < 0.1 && GetTime() - player.time < 0.1)
        {
            player.isClashing = true;
            enemy.isClashing = true;
        }
        
        
        //Adding up clashing to keep track and check who will win the clash
        if(player.isClashing && IsKeyPressed(KEY_ONE))
        {
            player.clashCount += 1;
        }  
        
        
        //Adds up the count for the player so they are able to win the clash
        if(player.clashCount >= 15)
        {
            player.clashCount = 0;
            enemy.clashCount = 0;
            player.stunTime = 0;
            enemy.stunTime = 0;
            enemy.isClashing = false;
            player.isClashing = false;
            enemy.knockCount += 1;
            enemy.health -= 30;
        }
        
        
        //Adding up clashing to keep track and check who will win the clash
        if(enemy.isClashing && IsKeyPressed(KEY_KP_ENTER))
        {
            enemy.clashCount += 1;
        }  
        
        
        //Adds up the count for the enemy so they are able to win the clash
        if(enemy.clashCount >= 15)
        {
            enemy.clashCount = 0;
            player.clashCount = 0;
            player.stunTime = 0;
            enemy.stunTime = 0;
            player.isClashing = false;
            enemy.isClashing = false;
            player.knockCount += 1;
            player.health -= 30;
        }
               
               
        //Stunning them when they are clashing
        if(player.isClashing && enemy.isClashing)
        {
            player.stunTime = 10000000000;
            enemy.stunTime = 10000000000;
        }
             
             
        //Player and enemy jump movement update
        player.posY -= player.velocity*dT;
        enemy.posY -= enemy.velocity*dT;
        
        if(player.posX == enemy.posX && player.posY != enemy.posY)
        {
            player.posX -= 1;
        }
        if (enemy.evasiveCount > 100)
        {
            enemy.evasiveCount = 100;
        }
        if (player.evasiveCount > 100)
        {
            player.evasiveCount = 100;
        }
        
        
        //Giving knockback a smooth animation for enemy
        if(enemy.knockCount != 0)
        {
            enemy.knockCount += 1;
            if(enemy.knockCount >= 8)
            {               
                enemy.knockCount = 0;           
            }        
            if(player.posX > enemy.posX)
            {           
                enemy.posX -= 20;              
            }
            else
            {    
                enemy.posX += 20;            
            }            
            //Prevents players from going through either border when knockbacked
            if (enemy.posX < player.posX) 
            {               
                if(enemy.posX <=0)
                { 
                    enemy.posX=1;
                    enemy.posX -= 1;
                    player.posX += 20;
                } 
                if (IsKeyDown(KEY_LEFT) && enemy.posX > 0)
                {                   
                    enemy.posX -= 3;                 
                }          
            }
            else
            {          
                if (enemy.posX + enemy.width > window.width)
                {   
                    enemy.posX = window.width - enemy.width - 1;
                    player.posX -= 20;      
                }
            }           
        }
        
        
        //Giving knockback a smooth animation for player
        if(player.knockCount != 0)
        {
            player.knockCount += 1;
            if(player.knockCount >= 8)
            {              
                player.knockCount = 0;             
            }  
            if(enemy.posX > player.posX)
            { 
                player.posX -= 20;      
            }      
            else
            {  
                player.posX += 20;               
            }   
            //Prevents players from going through either border when knockbacked
            if (player.posX < enemy.posX) 
            {
                if(player.posX <=0)
                {  
                    player.posX=1;
                    player.posX -= 1;
                    enemy.posX += 20;                    
                }
                
                if (IsKeyDown(KEY_LEFT) && player.posX > 0)
                {                 
                    player.posX -= 3;             
                }
            }  
            else
            {
                if (player.posX + player.width > window.width)
                {   
                    player.posX = window.width - player.width - 1;
                    enemy.posX -= 20;        
                }  
            }  
        }


        //Dash Cooldown
        if (player.dashCooldown > 0.0)
        {   
            player.dashCooldown -= dT;    
        }      
        if (enemy.dashCooldown > 0.0)
        {  
            enemy.dashCooldown -= dT;   
        }       
        
        
        //Block Cooldown
        if (player.blockCooldown > 0.0)
        {           
            player.blockCooldown -= dT;   
        }      
        if (enemy.blockCooldown > 0.0)
        {
            enemy.blockCooldown -= dT;  
        }   
        
        
        //Enemy stun
        if (enemy.stunTime > 0) 
        {           
            enemy.stunTime -= dT;
            enemy.isStunned = true;
        }
        else
        {          
            enemy.isStunned = false;    
        }         
        
        
        //Player Stun
        if (player.stunTime > 0 ) 
        {           
            player.stunTime -= dT;
            player.isStunned = true;           
        }               
        else
        {            
            player.isStunned = false;        
        }   
         
         
        //Resetting the punch count after a certain time for player
        if (GetTime() >= player.punchResetCount + 3)
        {         
            player.punchCount = 0;
            player.punchResetCount = GetTime();    
        }
        
        
        //Resetting the punch count after a certain time for enemy
        if (GetTime() >= enemy.punchResetCount + 3)
        {
            enemy.punchCount = 0;
            enemy.punchResetCount = GetTime();    
        }
 
 
        //Reset the hitcount after a certain amount of time for the visual counter on each side of the screen PLAYER
        if (GetTime() >= player.hitResetCount+1.5)
        {      
            player.totalHitCount = 0;              
        }  
        //Reset the hitcount after a certain amount of time for the visual counter on each side of the screen ENEMY
        if (GetTime() >= enemy.hitResetCount+1.5)
        {    
            enemy.totalHitCount = 0;                 
        }
    
    
        //Checking for jumping collision and applying gravity for PLAYER
        if (player.posY >= window.height - player.height)
        {  
            player.velocity = 0;
            player.isGrounded = true;         
        }      
        else if (player.posY < platform.posY - player.height)
        {
            //Fall down more 
            if (IsKeyDown(KEY_S))
            {               
                player.velocity -= (gravity*dT + 45);
                player.isGrounded = false;          
            }   
            else
            {
                player.velocity -= gravity*dT;
                player.isGrounded = false;                                   
            }           
        }         
        else
        {           
            player.isGrounded = true;
            player.posY = platform.posY - player.height;
            player.velocity = 0;           
        }
          
          
        //Checking for jumping collision and applying gravity for ENEMY
        if (enemy.posY >= window.height - enemy.height)
        {          
            enemy.velocity = 0;
            enemy.isGrounded = true;           
        }   
        else if (enemy.posY < platform.posY - enemy.height)
        {
            //Fall down more
            if (IsKeyDown(KEY_DOWN))
            {             
                enemy.velocity -= (gravity*dT + 45);
            }            
            else
            {       
                enemy.velocity -= gravity*dT;
                enemy.isGrounded = false;          
            }            
        }  
        else
        {    
            enemy.isGrounded = true;
            enemy.posY = platform.posY - enemy.height;
            enemy.velocity = 0;      
        }


        //Movement for player
        //Handling Player collision in different positions (Left or right)
        if (!collision) 
        {     
            if(!player.isStunned)
            {
                if (IsKeyDown(KEY_D) && player.posX < 1150)
                {
                    //Player dashing to the right
                    if ((IsKeyPressed(KEY_Q) && player.posX < 1150 && player.dashCooldown <= 0.0) || (player.dashCount != 0 && !player.isStunned && !player.isBlocking))
                    {
                        player.dashCount += 1;
                        if(player.dashCount >= 12)
                        {
                            player.dashCount = 0;
                        }
                        player.posX += 5;
                        player.dashCooldown = 2.5;                  
                    }
                    
                    if(player.isBlocking)
                    {
                        player.posX += 0.5;
                    }
                    else
                    {
                        player.posX += 3;
                    }
                }                              
                if (IsKeyDown(KEY_A) && player.posX > 0)
                {
                    //Player dashing to the left
                    if ((IsKeyPressed(KEY_Q) && player.posX > 0 && player.dashCooldown <= 0.0) || (player.dashCount != 0 && !player.isStunned && !player.isBlocking))
                    {
                        player.dashCount += 1;
                        if(player.dashCount >= 12)
                        {                            
                            player.dashCount = 0;                            
                        }                        
                        player.posX -= 5;
                        player.dashCooldown = 2.5;
                    }                    
                    if(player.isBlocking)
                    {
                        player.posX -= 0.5;
                    }            
                    else
                    {
                        player.posX -= 3;
                    }                   
                }     
            }            
        }
        
        
        //If the players are colliding
        else if (collision)
        {    
            //Prevents the players from going past the border
            if (player.posX < enemy.posX) 
            { 
                if(player.posX <=0)
                {      
                    player.posX=1;                 
                }              
                player.posX -= 1.5;
                enemy.posX += 1.5;                               
                if (IsKeyDown(KEY_A) && player.posX > 0)
                {                   
                    player.posX -= 3;                  
                }        
            }          
            else
            {                
                if (player.posX + player.width > window.width)
                {        
                    player.posX = window.width - player.width;      
                }
              
                if (IsKeyDown(KEY_D) && player.posX < window.width - player.width)
                {     
                    player.posX += 3;          
                }             
            }       
        }
            
            
        //Player jumping
        if (IsKeyPressed(KEY_W) && !player.isStunned)
        {
            if (player.isGrounded)
            {  
                player.velocity = player.jumpVelocity - 80;
                player.posY-= 40;
                player.height = 80;
                player.isGrounded = false;
                player.doubleJump = true;
                player.isCrouching = false;
                player.airCooldown = GetTime();
            }  
            else if (player.doubleJump)
            { 
                player.velocity = player.jumpVelocity - 130;
                player.doubleJump = false;           
            }
        }
        
        
        //PLayer Crouching
        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            if(player.isGrounded && !player.isStunned && !player.isCrouching && !player.isAttacking && !player.isBlocking)
            {    
                player.isCrouching = true;
                player.posY += player.height / 2;
                player.height /=2;
                player.crouchCooldown = GetTime();
            }
        }              
        else
        {
            if(player.isCrouching)
            {
                player.isCrouching = false;
                player.height *=2;
                player.posY -= player.height/2;
            }
        }


        //Player attacking               
        if (IsKeyPressed(KEY_E) && !player.isStunned && !player.isBlocking) 
        {        
            if(!player.isGrounded && GetTime() - player.airCooldown > 0.5)
            {
                player.punchCount += 1;
                player.hitCount += 1;
                player.time = GetTime();
                player.stunTime = 0.75;
                player.dashCount = 0;
                player.isAttacking = true;//do airial attack
            }
            else if(player.isCrouching && GetTime() - player.crouchCooldown > 0.5)
            {
                player.punchCount += 1;
                player.hitCount += 1;
                player.time = GetTime();
                player.stunTime = 0.75;
                player.dashCount = 0;
                player.isAttacking = true;
            }
            else if (player.isGrounded && !player.isCrouching)
            {
                player.punchCount += 1;
                player.hitCount += 1;
                player.time = GetTime();
                player.stunTime = 0.75;
                player.dashCount = 0;
                player.isAttacking = true;    
            }  
        }
        

        //Player Blocking
        if (IsKeyDown(KEY_F) && player.isGrounded && player.blockCooldown <= 0) 
        {         
            player.isBlocking = true;                     
        }      
        else
        {            
            if(player.isBlocking)
            {
                player.blockCooldown = 2.5;
                player.isBlocking = false;
                player.stunTime = 0;
            }   
        }   
        
        
        //Player evasive
        if(IsKeyPressed(KEY_R) && player.evasiveCount == 100)
        {
            player.knockCount += 1;
            player.evasiveCount = 0;
        }
        
        
        //Enemy Movement
        //Handling Enemy Collision in different positions (Left or Right)
        if (!collision) 
        { 
            if (!enemy.isStunned)
            { 
                if (IsKeyDown(KEY_RIGHT) && enemy.posX < 1150)
                {
                    //Enemy Dashing to the right
                    if ((IsKeyPressed(KEY_KP_2) && enemy.posX < 1150 && enemy.dashCooldown <= 0.0) || (enemy.dashCount != 0 && !enemy.isStunned && !enemy.isBlocking))
                    {
                        enemy.dashCount += 1;
                        if(enemy.dashCount >= 12)
                        {
                            enemy.dashCount = 0;
                        }
                        enemy.posX += 5;
                        enemy.dashCooldown = 2.5;                  
                    }
                    if(enemy.isBlocking)
                    {
                        enemy.posX += 0.5;
                    }
                    else
                    {
                        enemy.posX += 3;
                    } 
                }         
                if (IsKeyDown(KEY_LEFT) && enemy.posX > 0 )
                {
                    //Enemy Dashing to the left
                    if ((IsKeyPressed(KEY_KP_2) && enemy.posX > 0 && enemy.dashCooldown <= 0.0) || (enemy.dashCount != 0 && !enemy.isBlocking && !enemy.isStunned))
                    {
                        enemy.dashCount += 1;
                        if(enemy.dashCount >= 12)
                        {
                            enemy.dashCount = 0;
                        }
                        enemy.posX -= 5;
                        enemy.dashCooldown = 2.5;
                    }                    
                    if(enemy.isBlocking)
                    {
                        enemy.posX -= 0.5;
                    }
                    else
                    {
                        enemy.posX -= 3;
                    }
                }       
            }      
        }
        
        
        //If the players are colliding
        else
        {
            if (enemy.posX < player.posX) 
            {       
                //Prevents players from going through either border
                if(enemy.posX <=0)
                {      
                    enemy.posX=1;             
                }             
                enemy.posX -= 1.5;
                player.posX += 1.5;             
                if (IsKeyDown(KEY_LEFT) && enemy.posX > 0)
                {      
                    enemy.posX -= 3;            
                }               
            }           
            else
            {             
                if (enemy.posX + enemy.width > window.width)
                {
                    
                    enemy.posX = window.width - enemy.width - 1;
                    
                }
                if (IsKeyDown(KEY_RIGHT) && enemy.posX < window.width - enemy.width)
                {     
                    enemy.posX += 3;        
                }
            }    
        }
       
       
        //Enemy jumping
        if(!enemy.isStunned)
        {   
            if (IsKeyPressed(KEY_UP))
            { 
                if (enemy.isGrounded)
                {
                    enemy.velocity = enemy.jumpVelocity - 80;
                    enemy.posY-= 40;
                    enemy.height = 80;
                    enemy.isGrounded = false;
                    enemy.doubleJump = true;               
                    enemy.isCrouching = false;
                    enemy.airCooldown = GetTime();                   
                }
                else if (enemy.doubleJump)
                {  
                    enemy.velocity = enemy.jumpVelocity - 130;
                    enemy.doubleJump = false;    
                }   
            }
        }
        
        
        //Enemy Crouching 
        if (IsKeyDown(KEY_RIGHT_CONTROL) )
        {
            if(enemy.isGrounded && !enemy.isStunned && !enemy.isCrouching && !enemy.isAttacking && !enemy.isBlocking)
            {    
                enemy.isCrouching = true;
                enemy.posY += enemy.height / 2;
                enemy.height /=2;
                enemy.crouchCooldown = GetTime();
            }
        }
        else
        {
            if(enemy.isCrouching)
            {
                enemy.isCrouching = false;
                enemy.height *=2;
                enemy.posY -= enemy.height/2;
            }
        }
              
              
        //Enemy attacking               
        if (IsKeyPressed(KEY_KP_0) && !enemy.isStunned && !enemy.isBlocking) 
        {                
            if(!enemy.isGrounded && GetTime() - enemy.airCooldown > 0.5)
            {
                enemy.punchCount += 1;
                enemy.hitCount += 1;
                enemy.time = GetTime();
                enemy.stunTime = 0.75;
                enemy.dashCount = 0;
                enemy.isAttacking = true;//do airial attack
            }   
            else if(enemy.isCrouching && GetTime() - enemy.crouchCooldown > 0.5)
            {
                enemy.punchCount += 1;
                enemy.hitCount += 1;
                enemy.time = GetTime();
                enemy.stunTime = 0.75;
                enemy.dashCount = 0;
                enemy.isAttacking = true;
            }
            else if (enemy.isGrounded && !enemy.isCrouching)
            {
                enemy.punchCount += 1;
                enemy.hitCount += 1;
                enemy.time = GetTime();
                enemy.stunTime = 0.75;
                enemy.dashCount = 0;
                enemy.isAttacking = true;        
            }
        }
          
          
        //Enemy Blocking
        if (IsKeyDown(KEY_KP_DECIMAL) && enemy.isGrounded && enemy.blockCooldown <= 0) 
        {    
            enemy.isBlocking = true;                
        }        
        else
        {     
            if(enemy.isBlocking)
            {
                enemy.blockCooldown = 2.5;
                enemy.isBlocking = false;
                enemy.stunTime = 0;
            }   
        }         


        //Enemy evasive
        if(IsKeyPressed(KEY_KP_1) && enemy.evasiveCount == 100)
        {
            enemy.knockCount += 1;
            enemy.evasiveCount = 0;
        }


        //Begins Drawing
        BeginDrawing();

            //Drawing background and declaring transparent color for platform
            ClearBackground(GRAY);
            Color transparentColor = {0, 0, 0, 150};
            DrawTexture(background, 0, 0, WHITE);
            
            
            //draw blockCooldown
            if(player.isBlocking)
            {
                if(player.posX < enemy.posX)
                {
                    DrawRectangle(player.posX+5,player.posY+20,55,20,WHITE);
                }
                else
                {
                    DrawRectangle(player.posX -10,player.posY+20,55,20,WHITE);
                }
            }
            
            
            if(enemy.isBlocking)
            {
                if(enemy.posX < player.posX)
                {
                    DrawRectangle(enemy.posX+5,enemy.posY+20,55,20,WHITE);
                }
                else
                {
                    DrawRectangle(enemy.posX -10,enemy.posY+20,55,20,WHITE);
                }
            }
            
            
            if(player.isClashing && enemy.isClashing)
            {
                DrawText("SPAM ONE", 100, 350, 45, BLUE);
                DrawText("SPAM ENT", 850, 350, 45, RED);
            }
            
            
            //Draws characters
            DrawRectangle(player.posX, player.posY, player.width, player.height, BLUE);
            DrawRectangle(enemy.posX, enemy.posY, enemy.width, enemy.height, RED);
            
            //Drawing player attacks
            if(GetTime()-player.time < 0.75) //if they are punching
            {
                player.isAttacking = true;
                //Crouch attack
                if (player.isCrouching)
                {
                    player.isAttacking = true;
                    player.crouchCooldown = GetTime();
                    //Draws hitboxes and checks which side each of the players are to determine where their attack hitbox should be
                    if(player.posX < enemy.posX)
                    {
                        player.isFacingRight = true;
                        DrawRectangle(player.posX+player.width,player.posY-15,15,40,WHITE);
                    }                
                    else if(player.posX > enemy.posX)
                    {
                        player.isFacingRight = false;
                        DrawRectangle(player.posX-player.width + 35,player.posY-15,15,40,WHITE);
                    }   
                }
                //Aerial attack
                else if(!player.isGrounded)
                {
                    player.isAttacking = true;
                    //Draws hitboxes and checks which side each of the players are to determine where their attack hitbox should be                   
                    if(player.posX < enemy.posX)
                    {
                        player.posX += 2.5;
                        player.posY += 9;
                        player.isFacingRight = true;
                        DrawRectangle(player.posX+player.width,player.posY+70,15,40,WHITE);
                    }                
                    else if(player.posX > enemy.posX)
                    {
                        player.posX -= 2.5;
                        player.posY += 9;
                        player.isFacingRight = false;
                        DrawRectangle(player.posX-player.width + 35,player.posY+70,15,40,WHITE);
                    } 
                    if(player.isFacingRight && player.posX >= enemy.posX) //fix for other direction, should fix small graphical glitch
                    {
                        player.posX -= 2.5;
                    }
                }
                //Normal attack
                else
                {
                    player.isAttacking = true;     
                    //Draws hitboxes and checks which side each of the players are to determine where their attack hitbox should be                    
                    if(player.posX < enemy.posX)
                    {
                        player.isFacingRight = true;
                        DrawRectangle(player.posX+player.width,player.posY+10,30,10,WHITE);
                    }                
                    else if(player.posX > enemy.posX)
                    {
                        player.isFacingRight = false;
                        DrawRectangle(player.posX-player.width + 20,player.posY+10,30,10,WHITE);
                    }
                }                
            }                
            else
            {
                //if there is no attack displaying
                player.isAttacking = false;
            }

            
            //Enemy attacks (Identical to player)
            if(GetTime()-enemy.time < 0.75)
            {
                if (enemy.isCrouching)
                {
                    enemy.isAttacking = true;
                    enemy.crouchCooldown = GetTime();
                    if(enemy.posX < player.posX)
                    {
                        enemy.isFacingRight = true;
                        DrawRectangle(enemy.posX+enemy.width,enemy.posY-15,15,40,WHITE);
                    }                
                    else if(enemy.posX > player.posX)
                    {
                        enemy.isFacingRight = false;
                        DrawRectangle(enemy.posX-enemy.width + 35,enemy.posY-15,15,40,WHITE);
                    }   
                }
                else if(!enemy.isGrounded)
                {
                    enemy.isAttacking = true;
                    if(enemy.posX < player.posX)
                    {
                        enemy.posX += 2.5;
                        enemy.posY += 9;
                        enemy.isFacingRight = true;
                        DrawRectangle(enemy.posX+enemy.width,enemy.posY+70,15,40,WHITE);
                    }                
                    else if(enemy.posX > player.posX)
                    {
                        enemy.posX -= 2.5;
                        enemy.posY += 9;
                        enemy.isFacingRight = false;
                        DrawRectangle(enemy.posX-enemy.width + 35,enemy.posY+70,15,40,WHITE);
                    } 
                    if(enemy.isFacingRight && enemy.posX >= player.posX)
                    {
                        enemy.posX -= 2.5;
                    }
                }  
                else
                {
                    enemy.isAttacking = true;
                    if(enemy.posX < player.posX)
                    {
                        enemy.isFacingRight = true;
                        DrawRectangle(enemy.posX+enemy.width,enemy.posY+10,30,10,WHITE);
                    }
                    
                    else if(enemy.posX > player.posX)
                    {
                        enemy.isFacingRight = false;
                        DrawRectangle(enemy.posX-enemy.width + 20,enemy.posY+10,30,10,WHITE);
                    }                
                }
            } 
            else
            {
                enemy.isAttacking = false;
            }
            
            
            //Platform
            DrawRectangle(platform.posX, platform.posY, 10000, 500, transparentColor);
            
            //Healthbars
            DrawText("PLAYER HEALTH", 140, 25, 25, WHITE);
            DrawHealthBar(100, 60, 300, 20, player.health, 100, BLUE, DARKBLUE);
            DrawText("ENEMY HEALTH", 850, 25, 25, WHITE);
            DrawHealthBar(790, 60, 300, 20, enemy.health, 100, RED, MAROON);
            
            //Evasive Text and Bars
            if(player.evasiveCount < 100)
            {
                DrawText("Evasive", 50, 90, 20, WHITE);
            }
            else
            {
                DrawText("Evasive Ready", 50, 90, 20, WHITE);
            }
            DrawHealthBar(50,120,150,10,player.evasiveCount, 100, WHITE, GRAY);
             
            //Enemy Evasive text and bar
            if(enemy.evasiveCount < 100)
            {
                DrawText("Evasive", 985, 90, 20, WHITE);
            }
            else
            {
                DrawText("Evasive Ready", 985, 90, 20, WHITE);
            }
            DrawHealthBar(985,120,150,10,enemy.evasiveCount, 100, WHITE, GRAY);
             
             
            //CLashing Bars to display when they attack at the same time
            if(player.isClashing)
            {
                DrawText("Clash", 50, 170, 20, WHITE);
                DrawHealthBar(50,200,150,10,player.clashCount, 15, WHITE, GRAY);
            }
            if(enemy.isClashing)
            {
                DrawText("Clash", 985, 170, 20, WHITE);
                DrawHealthBar(985,200,150,10,enemy.clashCount, 15, WHITE, GRAY);
            }
            
            
            //Track total hits of players during the fight
            if(player.totalHitCount != 0)
            {              
                DrawText(TextFormat("HITS \n: %i ",player.totalHitCount), 50, 200, 36, WHITE);         
            }           
            if(enemy.totalHitCount != 0)
            {
                
                DrawText(TextFormat("HITS \n: %i ",enemy.totalHitCount), 1050, 200, 36, WHITE);
                
            }
            
            
            //Ending round one and drawing text for a certain amount of time
            if (GetTime() - player.roundTimer < 1.5)
            {
                DrawText("ROUND 1 IS OVER, ENEMY WINS", 270, 300, 40, WHITE);
            }
        
            if (GetTime() - enemy.roundTimer < 1.5)
            {
                DrawText("ROUND 1 IS OVER, PLAYER WINS", 270, 300, 40, WHITE);
            }
            
            
            //Going into Round two
            if(player.health <= 0 && !player.roundTwo)
            {
                player.roundTimer = GetTime();
                player.health = 100;
                player.posY = window.height - (player.height+101);
                player.posX = window.width/2 - player.width/2 - 350;
                enemy.posY = window.height - (enemy.height+101);
                enemy.posX = window.width/2 - enemy.width/2 + 325;      
                player.stunTime = 1;
                enemy.stunTime = 1;
                player.roundTwo = true;
            }
            
            
            //If player loses and are in round two
            if (player.roundTwo && player.health <= 0)
            {
                DrawText("ENEMY WINS THE MATCH", 330, 300, 40, WHITE);
                player.stunTime = 1000000;
                enemy.stunTime = 1000000;
            }
            
            
            //Visual indication of which round player is on
            if(!player.roundTwo)
            {
                DrawText("I", 450, 50, 40, BLUE);
            }
            if(player.roundTwo)
            {
                DrawText("II", 450, 50, 40, BLUE);
            }
            
            
            //Visual indication of which round enemy is on            
            if(!enemy.roundTwo)
            {
                DrawText("I", 720, 50, 40, RED);
            }
            if(enemy.roundTwo)
            {
                DrawText("II", 720, 50, 40, RED);
            }
            
            
            //if both player and enemy are in round two
            if(enemy.roundTwo && player.roundTwo)
            {
                DrawText("FINAL ROUND", 490, 20, 30, WHITE);
            }
            
            
            //enemy going into round two
            if(enemy.health <= 0 && !enemy.roundTwo)
            {
                enemy.roundTimer = GetTime();
                enemy.health = 100;
                player.posY = window.height - (player.height+101);
                player.posX = window.width/2 - player.width/2 - 350;
                enemy.posY = window.height - (enemy.height+101);
                enemy.posX = window.width/2 - enemy.width/2 + 325;      
                enemy.stunTime = 1;
                player.stunTime = 1;
                enemy.roundTwo = true;
            }
                
                
            //if enemy loses in round two
            if (enemy.roundTwo && enemy.health <= 0)
            {
                DrawText("PLAYER WINS THE MATCH", 330, 300, 40, WHITE);
                player.stunTime = 1000000;
                enemy.stunTime = 1000000;
            }
            
            
        //End drawing  
        EndDrawing();
        
        
    }
   
   
   //Unloading textures and closing window
    UnloadTexture(background);
    CloseWindow(); 
    
    
}