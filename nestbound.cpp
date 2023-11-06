#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <bits/stdc++.h>
using namespace std;

#include "nestbound.h"

//g++ tp1.cpp -o tp1 -lGL -lGLU -lglut -lSOIL -lsfml-audio

//Generates a random integer except 0
float randomNumber(int range, int min){
  float val = rand() % range + min;
  if(!val) val = val + 1;
  return val;
}

double getRadians(double angle) {
  return angle * (PI/180.0);
}

//Set properties of the object
void setObject(OBJECT *obj, float x, float y, int width, int height, int lives, int level, float acceleration, GLuint texture, int totalFigures, int totalLines){
  obj->position.x = x;
  obj->position.y = y;
  obj->width = width;
  obj->height = height;
  obj->level = level;
  obj->lives = lives;
  obj->angle = 0;
  obj->velocity.x = 0;
  obj->velocity.y = 0;
  obj->acceleration.x = 0;
  obj->acceleration.y = acceleration;
  obj->texture = texture;
  obj->sprite.totalFigures = totalFigures - 1;
  obj->sprite.currentFigure = 0;
  obj->sprite.figureCounter = 0;
  obj->sprite.totalLines = totalLines - 1;
  obj->sprite.currentLine = totalLines - 1;
  obj->sprite.lineCounter = totalLines - 1;
}

// Sprite sheet animation
// The code only accepts sprites with an equal number of figures in each line
void designSpritesheet(OBJECT *obj, GLuint texture) {
  // Variables to calculate the figures and vertices to be drawn
  int figuresPerLine = (obj->sprite.totalFigures + 1) / (obj->sprite.totalLines + 1);
  float spriteWidth = 1 / (float)figuresPerLine;
  float spriteHeight = 1 / (float)(obj->sprite.totalLines + 1);

  // Conditions to correctly set which figure of each line will be printed
  if ((float)(obj->sprite.figureCounter + 1) / figuresPerLine > 1) {
    if (obj->sprite.figureCounter % figuresPerLine == 0) {
      obj->sprite.currentFigure = 0;
      obj->sprite.lineCounter--;
    }
    obj->sprite.currentLine = obj->sprite.lineCounter;
  } else {
    if ((obj->sprite.currentFigure) % figuresPerLine == 0) {
      obj->sprite.currentFigure = 0;
      obj->sprite.lineCounter = obj->sprite.totalLines;
    }
    obj->sprite.currentLine = obj->sprite.lineCounter;
  }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_POLYGON);
    glTexCoord2f(spriteWidth * obj->sprite.currentFigure, spriteHeight * obj->sprite.currentLine); glVertex3f(obj->position.x, obj->position.y, 0);
    glTexCoord2f(obj->sprite.currentFigure * spriteWidth + spriteWidth, spriteHeight * obj->sprite.currentLine); glVertex3f(obj->position.x + obj->width, obj->position.y, 0);
    glTexCoord2f(obj->sprite.currentFigure * spriteWidth + spriteWidth, spriteHeight * obj->sprite.currentLine + spriteHeight); glVertex3f(obj->position.x + obj->width, obj->position.y + obj->height, 0);
    glTexCoord2f(spriteWidth * obj->sprite.currentFigure, spriteHeight * obj->sprite.currentLine + spriteHeight); glVertex3f(obj->position.x, obj->position.y + obj->height, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  // Change the variables so that when the function is called again, the next figure is to be printed
  obj->sprite.currentFigure++;
  obj->sprite.figureCounter++;
  if (obj->sprite.figureCounter > obj->sprite.totalFigures) {
    obj->sprite.figureCounter = 0;
  } 
  if (obj->sprite.lineCounter < 0) {
    obj->sprite.lineCounter = obj->sprite.totalLines - 1;
  }
}


// Sets the object's design and texture
void designObject(OBJECT obj, GLuint texture){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_POLYGON);
    glTexCoord2f(0, 0); glVertex3f(obj.position.x, obj.position.y, 0);
    glTexCoord2f(1, 0); glVertex3f(obj.position.x + obj.width, obj.position.y, 0);
    glTexCoord2f(1, 1); glVertex3f(obj.position.x + obj.width, obj.position.y + obj.height, 0);
    glTexCoord2f(0, 1); glVertex3f(obj.position.x, obj.position.y + obj.height, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void resetScene(int lives, int level) {
  setObject(&bird, randomNumber(1400, 50), randomNumber(600, 300), BIRD_WIDTH, BIRD_HEIGHT, lives, level, GRAVITY_ACCELERATION, birdTexture, 8, 2);
  setObject(&platform, randomNumber(1400, 50), PLATFORM_SPAWN_Y, PLATFORM_WIDTH, PLATFORM_HEIGHT, 0, 0, 0, platformTexture, 12, 3);
  setObject(&line, 1560, 716, LINE_WIDTH, LINE_HEIGHT, 0, 0, 0, lineTexture, 1, 1);
  for (int j = 0; j < CLOUD_QUANTITY; j++) {
    setObject(&clouds[j], randomNumber(300, -200 + j * 300), randomNumber(400, 550), CLOUD_WIDTH, CLOUD_HEIGHT, 0, 0, 0, cloudTexture, 1, 1);
    cloudSpeed[j] = 2 + j * 0.25;
  }
  hasEnergy = true;
}

// shows star on scoring
void stopStar(int idx){
  show_star = false;
}

// show explosion
void endExplosion(int idx){
  explosion = false;
}

// Draw the bird on the screen
void drawBird() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // If the coordinates or angle change, draw the bird in a different location
  glPushMatrix();
    glTranslatef(bird.position.x, bird.position.y, 0); // Translate the bird
    glRotatef(bird.angle, 0.0f, 0.0f, 1.0f); // Rotate the bird around the Z-axis
    glTranslatef(-bird.position.x - (bird.width / 2), -bird.position.y - (bird.width / 2), 0); // Necessary to rotate the bird around the center (of the square)
    designSpritesheet(&bird, birdTexture);
  glPopMatrix();
}

//draw platform
void drawPlatform(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
      designSpritesheet(&platform, platformTexture);
  glPopMatrix();
}


// Draw the clouds moving across the sky
void drawClouds(){
  for (int j = 0; j < CLOUD_QUANTITY; j++) {
    // If the cloud moves off the screen, change its movement direction
    if (clouds[j].position.x > WINDOW_WIDTH) {
      clouds[j].position.x = -CLOUD_WIDTH;
    }
    // Cloud's movement
    clouds[j].position.x = clouds[j].position.x + cloudSpeed[j];
    designObject(clouds[j], cloudTexture);
  }
}

// Draws the explosion when the bird hits the ground
void drawExplosion() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
    designSpritesheet(&smoke, smokeTexture);
  glPopMatrix();
  // Calls the endExplosion function after the explosion animation ends
  glutTimerFunc(210, endExplosion, 0);
}

void drawHeart() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
    designSpritesheet(&heart, heartTexture);
  glPopMatrix();
  // Calls the endExplosion function after the explosion animation ends
  glutTimerFunc(210, endExplosion, 0);
}

void drawStar() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
    designSpritesheet(&star, starTexture);
  glPopMatrix();
  // Calls the stopStar function after the star animation ends
  glutTimerFunc(210, stopStar, 0);
}

int scorePerTime() {
  if (phaseTime == 0) return 1000;
  return 1000 / phaseTime;
}

// Checks if the bird collided with the ground
void checkCollisionGround() {
  if (bird.position.y - (bird.width / 2) < ground.position.y + ground.height) {
    bird.lives--;
    screamFrameCounter = 0;
    if (bird.lives == 0) {
      loseFrameCounter = 0;
      score = score + 10 * (line.position.x - 1380) + scorePerTime();
      current_screen = LOST;
    } else {
      awFrameCounter = 0;
      // Draws only the explosion if explosion = true, otherwise draws the bird and the platform (see drawScene)
      explosion = true;
      // sets the smoke, draws the explosion, and resets the scenario
      setObject(&smoke, bird.position.x - SMOKE_OFFSET, bird.position.y - SMOKE_OFFSET, SMOKE_WIDTH, SMOKE_HEIGHT, 0, 0, 0, smokeTexture, 10, 1);
      setObject(&heart, bird.position.x - SMOKE_OFFSET, bird.position.y + 40, HEART_WIDTH, HEART_HEIGHT, 0, 0, 0, heartTexture, 9, 1);
      resetScene(bird.lives, bird.level);
      // sets the gravity time to 0 so the bird starts falling from rest
      gravityTime = 0;
    }
  } 
  else 
      awFrameCounter++;
}

// Checks when the bird collides with the platform
void checkCollisionPlatform() {
  if ((bird.position.x - (bird.width / 2) >= (platform.position.x - PLATFORM_X_TOLERANCE) && bird.position.x + (bird.width / 2) <= (platform.position.x + platform.width + PLATFORM_X_TOLERANCE))) { // Tolerance for the bird to be slightly off on the x-axis
    if (bird.position.y - (bird.height / 2) < platform.position.y + platform.height - PLATFORM_Y_OFFSET) {
      if (bird.angle < TOLERANCE_ANGLE && bird.angle > -TOLERANCE_ANGLE) {
        levelFrameCounter = 0;
        levelFrameCounter++;
        show_star = true;
        gravityTime = 0;
        bird.level = bird.level + 1;
        setObject(&star, platform.position.x, platform.position.y + STAR_HEIGHT_OFFSET, STAR_WIDTH, STAR_HEIGHT, 0, 0, 0, starTexture, 10, 2);
        drawStar();
        resetScene(bird.lives, bird.level);
        score = score + 2000 + (line.position.x - 1380) + scorePerTime();
        phaseTime = 0;
      }
    }
  } 
  else 
      levelFrameCounter++;
}

// Sets the acceleration on the x and y axes, assigning the value of each component associated with the angle using trigonometry
void accelerateBird() {
  bird.position.x += (MOVEMENT * sin(-getRadians(bird.angle))) + bird.velocity.x;
  bird.position.y += (MOVEMENT * cos(-getRadians(bird.angle))) + bird.velocity.y;
}

// Gravity
void gravity() {  
  // Sets accelerations on the x and y axes
  bird.acceleration.x = 0;
  bird.acceleration.y = GRAVITY_ACCELERATION;
  
  // Application of the formulas: v = v0 + a * t
  bird.velocity.x = bird.velocity.x + bird.acceleration.x * gravityTime;
  bird.velocity.y = bird.velocity.y + bird.acceleration.y * gravityTime;
  
  // Application of the formulas: s = s0 + v0 * t + (1/2) * a * t^2
  bird.position.x = bird.position.x + bird.velocity.x * gravityTime + (0.5 * bird.acceleration.x * pow(gravityTime, 2));
  bird.position.y = bird.position.y + bird.velocity.y * gravityTime + (0.5 * bird.acceleration.y * pow(gravityTime, 2));
  
  gravityTime++;
  
  // If time grows too large, the ship's acceleration becomes extremely high when the W key is held (since it is squared)
  // Therefore, by setting a maximum time limit, the gameplay becomes much better
  if(gravityTime > 25){
    gravityTime = 25;
  }
}

// Bird's propulsion. When "fly" is called, "gravity" is NOT called, and vice versa
void fly() {
  // Sets accelerations on the x and y axes, combining gravity and propulsion
  bird.acceleration.x = MOTOR_ACCELERATION * sin(-getRadians(bird.angle));
  bird.acceleration.y = GRAVITY_ACCELERATION + MOTOR_ACCELERATION * cos(-getRadians(bird.angle));
  
  // Application of the formulas: v = v0 + a*t
  bird.velocity.x = bird.velocity.x + bird.acceleration.x * motorTime; // can be problem
  bird.velocity.y = bird.velocity.y + bird.acceleration.y * motorTime;
  
  // Application of the formulas: s = s0 + v0 * t + (1/2) * a * t^2
  bird.position.x = bird.position.x + bird.velocity.x * motorTime + (0.5 * bird.acceleration.x * pow(motorTime, 2));
  bird.position.y = bird.position.y + bird.velocity.y * motorTime + (0.5 * bird.acceleration.y * pow(motorTime, 2));
  
  motorTime++;
  
  // If the time grows too large, the ship's acceleration becomes extremely high when the W key is held (since it is squared)
  // Therefore, by setting a maximum time limit, the gameplay becomes much better
  if(motorTime > 25) {
    motorTime = 25;
  }
}

void energy(OBJECT *obj) {
  obj->position.x = obj->position.x - 0.2 * bird.level;
  if (obj->position.x <= 1380) {
    obj->position.x = 1380;
    hasEnergy = false;
  }
}

// Print function to be called by the HUD
void print(int x, int y, int z, char *string){
  // Set the text color to black
  glColor3f(0, 0, 0);
  // Set the text position using the provided coordinates
  glRasterPos2f(x, y);
  // Get the length of the string to be displayed
  int len = (int) strlen(string);
  // Loop through to write character by character
  for (int i = 0; i < len; i++){
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
  }
  // Set the text color back to white
  glColor3f(1, 1, 1);
}

// Print on the screen, call the print function
void hud(){
  char buffer[50];
  int n;
  char *A;
  const int VALUE_FOR_DISPLAY = 100;

  // Print time, height, and energy
  n = sprintf(buffer, "Time: %d:%.d", totalMinutes, totalSeconds);
  A = (char *)buffer;
  print(1380, 840, 2, A);

  n = sprintf(buffer, "Height: %.1f", bird.position.y - ground.height - bird.height/2);
  A = (char *)buffer;
  print(1380, 800, 2, A);

  n = sprintf(buffer, "Energy:");
  A = (char *)buffer;
  print(1380, 760, 2, A);

  // Print velocity and acceleration
  n = sprintf(buffer, "Speed: %.1f", VALUE_FOR_DISPLAY * (pow(pow(bird.velocity.x, 2) + pow(bird.velocity.y, 2), 0.5)));
  A = (char *)buffer;
  print(1380, 680, 2, A);

  n = sprintf(buffer, "Acceleration: %.3f", VALUE_FOR_DISPLAY * (pow(pow(bird.acceleration.x, 2) + pow(bird.acceleration.y, 2), 0.5)));
  A = (char *)buffer;
  print(1380, 640, 2, A);

  // Print level, score, and lives
  n = sprintf(buffer, "Level %d", bird.level);
  A = (char *)buffer;
  print(50, 840, 2, A);

  n = sprintf(buffer, "Score: %d", score);
  A = (char *)buffer;
  print(50, 800, 2, A);

  n = sprintf(buffer, "Lives: ");
  A = (char *)buffer;
  print(50, 760, 2, A);

  if (bird.lives == 1) {
    designObject(life, lifeTexture);
  }

  if (bird.lives == 2) {
    designObject(life, lifeTexture);
    designObject(life2, lifeTexture);
  }

  if (bird.lives == 3) {
    designObject(life, lifeTexture);
    designObject(life2, lifeTexture);
    designObject(life3, lifeTexture);
  }
}

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  switch (current_screen) {
    case INTRO:
      designObject(image, introTexture);
      break;
    case MENU:
      designObject(image, menuTexture);
      break;
    case TUTORIAL:
      designObject(image, tutorialTexture);
      break;
    case PAUSE:
      designObject(image, pauseTexture);
      break;
    case GAME:
      designObject(background, backgroundTexture);
      drawClouds();
      designObject(bar, barTexture);
      designObject(line, lineTexture);
      if (explosion) {
        drawExplosion();
        drawHeart();
      } else {
        drawBird();
        drawPlatform();
        if (show_star) {
          drawStar();
        }
      }
      hud();
      break;
    case LOST:
      designObject(image, loseTexture);
      char buffer[50];
      int n;
      char *A;
      {
        n = sprintf(buffer, "Score: %.d", score);
        A = (char*)buffer;
        glColor3f(1, 1, 1);
        print(500, 490, 2, A);
      }
      {
        n = sprintf(buffer, "Level %.d", bird.level);
        A = (char*)buffer;
        glColor3f(1, 1, 1);
        print(500, 465, 2, A);
      }
      break;
  }
  glFlush();
}

void update(int idx) {
  frameCounter++;
  if (frameCounter % 40 == 0) { // 40 is equivalent to 1 second, considering it's called every 25 milliseconds
    timeCounter++;
    totalSeconds++;
    phaseTime++;
    if (totalSeconds > 59) {
      totalMinutes++;
      totalSeconds = 0;
    }
  }

  switch (current_screen) {
    case INTRO:
      gameFrameCounter = 0;
      if (timeCounter > 2) {
        current_screen = MENU;
        frameCounter = 0;
        timeCounter = 0;
      }
      break;
    case TUTORIAL:
      menuFrameCounter++;
      break;
    case MENU:
      gameFrameCounter = 0;
      menuFrameCounter++;
      break;
    case GAME:
      gameFrameCounter = 0;
      menuFrameCounter = 0;
      hud();
      checkCollisionPlatform();
      checkCollisionGround();
      if (keys['w']) {
        energy(&line);
        if (hasEnergy) {
          fly();
        } else {
          gravity();
          bird.angle += 8;
        }
      } else {
        gravity();
        if (!hasEnergy) {
          bird.angle += 8;
        }
      }
      break;
    case PAUSE:
      gameFrameCounter = 0;
      menuFrameCounter = 0;
      break;
    case LOST:
      gameFrameCounter = 0;
      menuFrameCounter = 0;
      loseFrameCounter++;
      break;
  }
  glutPostRedisplay();
  glutTimerFunc(25, update, 0);
}

int loadTexture(const char *file_path){
  return SOIL_load_OGL_texture(
    file_path,
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );
}

void initialize(void) {
  glClearColor(0, 0, 0, 0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  birdTexture = loadTexture("img/bird.png");
  smokeTexture = loadTexture("img/smoke.png");
  barTexture = loadTexture("img/energybar.png");
  lineTexture = loadTexture("img/line.png");
  lifeTexture = loadTexture("img/life.png");
  platformTexture = loadTexture("img/egg-sprite.png");
  cloudTexture = loadTexture("img/cloud.png");
  starTexture = loadTexture("img/star.png");
  heartTexture = loadTexture("img/heart.png");
  backgroundTexture = loadTexture("img/background.png");
  introTexture = loadTexture("img/introduction.jpg");
  pauseTexture = loadTexture("img/pause.png");
  tutorialTexture = loadTexture("img/tutorial.png");
  menuTexture = loadTexture("img/menu.png");
  loseTexture = loadTexture("img/lost.png");

  // Load other textures similarly...

  setObject(&background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, backgroundTexture, 1, 1);
  setObject(&ground, 0, 0, WINDOW_WIDTH, GROUND_HEIGHT, 0, 0, 0, nullTexture, 1, 1);
  setObject(&image, 350, 80, IMAGE_SIDE, IMAGE_SIDE, 0, 0, 0, nullTexture, 1, 1);
  setObject(&bar, 1380, 720, BAR_WIDTH, BAR_HEIGHT, 0, 0, 0, barTexture, 1, 1);
  setObject(&life, 50, 720, HEART_SIDE, HEART_SIDE, 0, 0, 0, lifeTexture, 1, 1);
  setObject(&life2, 80, 720, HEART_SIDE, HEART_SIDE, 0, 0, 0, lifeTexture, 1, 1);
  setObject(&life3, 110, 720, HEART_SIDE, HEART_SIDE, 0, 0, 0, lifeTexture, 1, 1);
}

void reshape(int w, int h){
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyUp(unsigned char key, int x, int y) {
  keys[key] = false; // Sets the state of the current key to not pressed
  // For some reason, when the Caps Lock was activated and the uppercase W was pressed, the lowercase w remained as true
  if (key == 'W')
    keys['w'] = false;
}

void press(unsigned char key, int x, int y) {
  switch (key) {
    // ESC key
    case 27:
      if (current_screen == PAUSE || current_screen == LOST) {
        exit(0); // exits the game only if it's in pause or the losing screen
      } else {
        current_screen = PAUSE; // if not in pause, enters pause first
      }
      break;
    case 'P':
    case 'p':
      if (current_screen == PAUSE) { // if in pause, resumes the game
        current_screen = GAME;
      } else if (current_screen == GAME) { // if in game, enters pause
        current_screen = PAUSE;
      }
      break;
    case 'R':
    case 'r':
      if (current_screen == GAME) { // if in game, goes to pause
        current_screen = PAUSE;
      } else {
        score = 0;
        resetScene(INITIAL_LIVES, INITIAL_COINS);
        current_screen = GAME;
      }
      break;
    case 'T':
    case 't':
      current_screen = TUTORIAL;
      break;
    case 'W':
    case 'w':
      keys['w'] = true;
      break;
    case 'A':
    case 'a':
      keys['a'] = true;
      bird.angle += 8;
      break;
    case 'D':
    case 'd':
      keys['d'] = true;
      bird.angle -= 8;
      break;
    default:
      break;
  }
  if (bird.angle % 360 == 0) {
    bird.angle = 0;
  }
}

int main(int argc, char **argv){
  srand(time(NULL));
  // initialize the GLUT library
  glutInit(&argc, argv);

  // Defining the version of OpenGL we'll use
  glutInitContextVersion(1, 1);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  // Initial configuration of the GLUT window
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);

  // Open the window
  glutCreateWindow("NESTBOUND GAME");
  // Initialization
  initialize();
  // Register callbacks for certain events
  glutDisplayFunc(renderScene);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(press);
  glutKeyboardUpFunc(keyUp);
  // Register functions for timers, gravity, and ground checking
  glutTimerFunc(25, update, 0);

  // Enter a loop and never exit
  glutMainLoop();
  return 0;
}