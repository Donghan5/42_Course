// shared-constants.ts
// Single source of truth for ALL game constants
// Used by both frontend and backend
// ============================================
// CORE PHYSICS & GAMEPLAY
// ============================================
// Arena dimensions
export const ARENA_WIDTH = 60.0; // Increased for bigger game
export const ARENA_DEPTH = 40.0; // Increased for bigger game
export const ARENA_HEIGHT = 10.0;
// Paddle constants
export const PADDLE_X_POSITION = 28.0; // Closer to walls
export const PADDLE_START_X = 28.0;
export const PADDLE_WIDTH = 1.5;
export const PADDLE_HEIGHT = 1.4;
export const PADDLE_DEPTH = 6.0;
export const PADDLE_Y_POSITION = 0.7;
export const PADDLE_Z_LIMIT = 16.0; // Adjusted for larger depth
export const PADDLE_SPEED = 0.6;
// Ball constants
export const BALL_DIAMETER = 1.3;
export const BALL_RADIUS = BALL_DIAMETER / 2;
export const BALL_Y_POSITION = 0.65;
export const BALL_INITIAL_SPEED = 15.0;
export const BALL_MAX_SPEED = 25.0;
export const BALL_SPEED_INCREASE = 1.015;
export const BALL_ANGLE_MODIFIER = 15.0;
// Wall positions
export const WALL_Z_POSITION = 20.0; // ARENA_DEPTH / 2
export const COLLISION_WALL_Z = WALL_Z_POSITION - BALL_RADIUS;
// Calculated collision planes
export const PADDLE_COLLISION_X_LEFT = -PADDLE_X_POSITION + (PADDLE_WIDTH / 2) + BALL_RADIUS;
export const PADDLE_COLLISION_X_RIGHT = PADDLE_X_POSITION - (PADDLE_WIDTH / 2) - BALL_RADIUS;
export const WALL_COLLISION_Z_TOP = WALL_Z_POSITION - BALL_RADIUS;
export const WALL_COLLISION_Z_BOTTOM = -WALL_Z_POSITION + BALL_RADIUS;
// Scoring boundaries
export const GOAL_LINE_LEFT = -29.0; // Behind paddle
export const GOAL_LINE_RIGHT = 29.0;
// ============================================
// VISUAL RENDERING
// ============================================
// Visual borders
export const VISUAL_PLAYFIELD_WIDTH = 60.0;
export const VISUAL_PLAYFIELD_DEPTH = 40.0;
export const BORDER_HEIGHT = 0.2;
export const BORDER_THICKNESS = 0.2;
// Camera settings
export const CAMERA_RADIUS_MIN = 70;
export const CAMERA_RADIUS_MAX = 160;
export const CAMERA_RADIUS_DEFAULT = 120;
export const CAMERA_BETA_LIMIT = Math.PI / 3;
// ============================================
// GAME RULES
// ============================================
export const WIN_SCORE = 5;
export const POINT_PER_GOAL = 1;
export const POINTS_PER_GOAL = 1;
export const COUNTDOWN_DURATION = 3;
// ============================================
// PHYSICS ENGINE
// ============================================
export const PHYSICS_SUBSTEPS = 4;
export const TARGET_FPS = 60;
export const FIXED_DELTA_TIME = 1 / TARGET_FPS;
// ============================================
// NETWORK & UPDATES
// ============================================
export const SERVER_UPDATE_INTERVAL = 1000 / 60;
export const INTERPOLATION_DELAY = 1.5;