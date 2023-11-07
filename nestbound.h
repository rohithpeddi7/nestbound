bool keys[256];

// Window
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;

// Screens
const int IMAGE_SIDE = 830;

// Bird
const int BIRD_SPAWN_X = 50;
const int BIRD_SPAWN_Y = 900;
const int BIRD_WIDTH = 75;
const int BIRD_HEIGHT = 50;

// Platform (nest)
const int PLATFORM_SPAWN_Y = 125;
const int PLATFORM_WIDTH = 120;
const int PLATFORM_HEIGHT = 83;

// Ground
const int GROUND_HEIGHT = 125;

// Energy Bar
const int BAR_WIDTH = 185;
const int BAR_HEIGHT = 19;
bool hasEnergy;

// Heart icons for the number of lives
const int HEART_SIDE = 20;

// Heart when losing a life
const int HEART_WIDTH = 50;
const int HEART_HEIGHT = 47;

// Star when advancing a level
const int STAR_WIDTH = 50;
const int STAR_HEIGHT = 47;
const int STAR_HEIGHT_OFFSET = 40;
bool show_star = false;

// Energy bar line
const int LINE_WIDTH = 5;
const int LINE_HEIGHT = 28;

// Smoke when destroyed
const int SMOKE_WIDTH = 75;
const int SMOKE_HEIGHT = 75;
bool explosion = false;

// Clouds
const int CLOUD_WIDTH = 250;
const int CLOUD_HEIGHT = 141;
const int NUMBER_OF_CLOUDS = 6;
float cloudSpeed[NUMBER_OF_CLOUDS];

// Constants and variables for movements
const float GRAVITY_ACCELERATION = -0.0005;
const float MOTOR_ACCELERATION = 0.0015;
int gravityTime = 0;
int motorTime = 0;

// Other constants
const int INITIAL_LIVES = 3;
const int INITIAL_COINS = 1;
const double PI = 3.14159265358979323846;
const int MOVEMENT = 5;
const int SMOKE_OFFSET = 30;
const int PLATFORM_X_TOLERANCE = 5;
const int PLATFORM_Y_OFFSET = 40;
const int TOLERANCE_ANGLE = 40;

// Time
int totalSeconds = 0;
int totalMinutes = 0;
int phaseTime;

// Counters
int timeCounter = 0;
int frameCounter = 0;

int screamFrameCounter = 0;
int menuFrameCounter = 0;
int gameFrameCounter = 0;
int levelFrameCounter = 0;
int awFrameCounter = 0;
int loseFrameCounter = 0;

// Score
int score = 0;

enum SCREEN { INTRO, MENU, TUTORIAL, PAUSE, GAME, LOST };
SCREEN current_screen = INTRO;

// textures of objects
GLuint birdTexture;
GLuint lifeTexture;
GLuint platformTexture;
GLuint groundTexture;
GLuint backgroundTexture;
GLuint smokeTexture;
GLuint cloudTexture;
GLuint barTexture;
GLuint lineTexture;
GLuint starTexture;
GLuint heartTexture;
// Textures - Screens
GLuint introTexture;
GLuint menuTexture;
GLuint tutorialTexture;
GLuint pauseTexture;
GLuint loseTexture;
GLuint nullTexture;

typedef struct {
    int totalFigures;    // Total number of figures
    int currentFigure;   // Currently active figure
    int figureCounter;   // Counter for figures
    int totalLines;      // Total number of lines
    int currentLine;     // Currently active line
    int lineCounter;     // Counter for lines
} SPRITESHEET;

typedef struct {
    float x;
    float y;
} COORDINATES;

typedef struct {
    int width;
    int height;
    int angle;
    COORDINATES position;
    COORDINATES velocity;
    COORDINATES acceleration;
    int level;
    int lives;
    GLuint texture;
    SPRITESHEET sprite;
} OBJECT;

OBJECT bird;
OBJECT platform;
OBJECT bar;
OBJECT line;
OBJECT ground;
OBJECT background;
OBJECT smoke;
OBJECT clouds[NUMBER_OF_CLOUDS];
OBJECT image;
OBJECT life, life2, life3;
OBJECT star;
OBJECT heart;