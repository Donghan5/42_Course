# 3D Pong Game - Project Documentation

## Overview
This is a **real-time multiplayer 3D Pong game** built with TypeScript (Vanilla), Babylon.js for 3D graphics, and WebSocket for real-time communication. The game runs entirely in the browser with server-side game logic to ensure fair play.

## Architecture Overview

```
┌────────────────────────────────────────────────────────────────────┐
│                        Backend Server                              │
│                                                                    │
│  ┌──────────────┐        ┌─────────────┐        ┌────────────┐     │
│  │   Fastify    │◄──────►│ Game Engine │◄──────►│   SQLite   │     │
│  │  (API/WS)    │        │ (Game Logic)│        │  (Storage) │     │
│  └──────────────┘        └─────────────┘        └────────────┘     │
│         ▲                                                          │
└─────────┼──────────────────────────────────────────────────────────┘
          │
          │ WebSocket (Real-time)
          │ HTTP (Login, Profile)
          │
┌─────────▼───────────┐
│   Frontend          │
│   (Browser)         │
│                     │
│  • Babylon.js (3D)  │
│  • TypeScript       │
│  • Game Controls    │
└─────────────────────┘
```

### Communication Flow:
- **HTTP API**: Used for login, profile updates, getting stats
- **WebSocket**: Used for real-time game updates (60 times per second)
- **SQLite**: Stores user accounts, game history, tournament data (not real-time)

## Tech Stack

### Frontend
- **TypeScript** (Vanilla - no React/Vue/Angular)
- **Babylon.js** - 3D game engine for rendering
- **Tailwind CSS** - Styling framework
- **WebSocket** - Real-time communication

### Backend  
- **Node.js** with **Fastify** framework
- **SQLite** database
- **WebSocket** for real-time game state sync
- **JWT** for authentication

### Shared
- **Common Types Package** (`@trans/common-types`) - Shared TypeScript types between frontend and backend

---

## How The Game Works (Simple Flow)

### 1. User Opens Website
```
User visits website → main.ts loads → Checks if logged in → Shows login or main menu
```

### 2. User Login Flow
```
Login Form → Backend validates → Returns JWT token → Saved as cookie → User authenticated
```

### 3. Starting a Game
```
User clicks "Play" → Creates game request → Backend creates game session → Returns game ID → Frontend connects via WebSocket
```

### 4. Real-time Game Loop
```
Player moves paddle → Send input to server → Server updates game state → Broadcasts to all players → Frontend renders new positions
```

---

## Project Structure

```
project/
├── apps/
│   ├── frontend/           # Browser application
│   │   ├── main.ts         # Entry point - handles routing & initialization
│   │   ├── src/
│   │   │   ├── game/       # Game logic
│   │   │   │   ├── render.ts        # Main game class using Babylon.js
│   │   │   │   ├── connection.ts    # WebSocket connection handler
│   │   │   │   ├── scene-builder.ts # 3D scene setup (paddles, ball, etc)
│   │   │   │   └── init.ts          # Game initialization
│   │   │   ├── status/     # User online status
│   │   │   └── tournament/ # Tournament system
│   │   └── auth/           # Authentication
│   │
│   └── backend/            # Server application
│       ├── src/
│       │   ├── main.ts     # Server entry point
│       │   ├── routes/     # API endpoints
│       │   │   └── api/
│       │   │       ├── games/        # Game routes
│       │   │       ├── user/         # User routes
│       │   │       └── tournament/   # Tournament routes
│       │   ├── core/       # Business logic
│       │   │   └── game/
│       │   │       └── game-engine.ts # Main game logic
│       │   └── database/   # SQLite database
│       │       └── migrations/       # Database tables
│
└── packages/
    └── common-types/       # Shared TypeScript types
```

---

## Key Components Explained

### Frontend Components

#### 1. **main.ts** - Application Entry Point
- Handles page routing (login, game, profile, etc)
- Manages user authentication state
- Initializes game when needed
- Controls navigation between different screens

#### 2. **render.ts** - Game Renderer (PongGame3D class)
- Creates 3D scene using Babylon.js
- Handles keyboard/mouse input
- Updates paddle and ball positions from server
- Manages particle effects and animations
- Runs at 60 FPS for smooth gameplay

#### 3. **connection.ts** - WebSocket Manager
- Connects to game server
- Sends player inputs (paddle movement)
- Receives game state updates
- Handles reconnection if connection drops
- Manages heartbeat to keep connection alive

#### 4. **scene-builder.ts** - 3D Scene Setup
- Creates the game arena (walls, floor)
- Sets up camera and lighting
- Creates paddle and ball meshes
- Adds visual effects (skybox, particles)

### Backend Components

#### 1. **game-engine.ts** - Core Game Logic
- Manages all active games
- Updates ball physics (position, velocity, collisions)
- Handles paddle movement
- Detects scoring and game end
- Broadcasts state to all players 60 times per second

#### 2. **Game Routes** (`/api/games/*`)
- `POST /create` - Creates new game session
- `GET /ws/:gameId` - WebSocket connection for real-time updates
- `POST /cancel` - Cancel/forfeit a game
- `GET /matchmaking/ws` - Automatic player matching

#### 3. **User Routes** (`/api/user/*`)
- `POST /login` - User authentication
- `GET /profile` - Get user data
- `POST /status/ws` - Online status WebSocket
- `GET /stats` - Game statistics

---

## Frontend → API → Backend Flow Examples

### Example 1: Starting a PvP Game

1. **Frontend** (main.ts):
   ```typescript
   // User clicks "Find Match" button
   connectToMatchmaking()
   ```

2. **Frontend** → **Backend** (WebSocket):
   ```
   WS Connect: /api/games/matchmaking/ws?playerId=123
   ```

3. **Backend** (matchmaking.ts):
   ```typescript
   // Add player to waiting queue
   // When 2 players available → Create game
   // Send "matchFound" message to both
   ```

4. **Backend** → **Frontend** (WebSocket):
   ```json
   { "type": "matchFound", "gameId": "game_xyz" }
   ```

5. **Frontend** (connection.ts):
   ```typescript
   // Connect to game WebSocket
   new WebSocket(`/api/games/ws/game_xyz`)
   ```

6. **Game Loop Starts** - Updates every 16ms (60 FPS)

### Example 2: Player Movement

1. **Frontend** (render.ts):
   ```typescript
   // Player presses UP arrow key
   onKeyDown('ArrowUp')
   ```

2. **Frontend** → **Backend** (WebSocket):
   ```json
   {
     "type": "playerInput",
     "inputState": "up",
     "playerId": "123"
   }
   ```

3. **Backend** (game-engine.ts):
   ```typescript
   // Update player's paddle velocity
   // Calculate new position
   // Check boundaries
   ```

4. **Backend** → **Frontend** (Broadcast to all players):
   ```json
   {
     "type": "gameState",
     "payload": {
       "player1": { "position": {...}, "score": 0 },
       "player2": { "position": {...}, "score": 0 },
       "ball": { "position": {...}, "velocity": {...} }
     }
   }
   ```

5. **Frontend** (render.ts):
   ```typescript
   // Update 3D object positions
   this.player1Paddle.position = newPosition
   ```

---

## Database Schema (SQLite)

### Main Tables

1. **users** - Player accounts
   ```sql
   id, nickname, email, password_hash, rating, created_at
   ```

2. **games** - Game history
   ```sql
   id, game_id, player1_id, player2_id, winner_id, scores, finished_at
   ```

3. **tournaments** - Tournament data
   ```sql
   id, name, bracket, status, winner_id, created_at
   ```

---

## WebSocket Communication Protocol

### Message Types

#### Client → Server:
- `playerInput` - Paddle movement (up/down/stop)
- `ping` - Keep connection alive

#### Server → Client:
- `gameState` - Complete game state update
- `gameEnd` - Game finished with winner
- `playerJoined` - Another player connected
- `playerLeft` - Player disconnected
- `pong` - Response to ping

### Game State Structure:
```typescript
{
  player1: { position: {x,y,z}, score: number },
  player2: { position: {x,y,z}, score: number },
  ball: { position: {x,y,z}, velocity: {x,y,z} },
  status: 'waiting' | 'countdown' | 'playing' | 'finished'
}
```

---

## Game Modes

1. **PvP** - Player vs Player online
2. **Local PvP** - Two players on same computer
3. **AI** - Player vs Computer (Easy/Medium/Hard)
4. **Tournament** - Bracket-style competition

---

## Authentication Flow

1. **Google OAuth**:
   ```
   Click "Login with Google" → Redirect to Google → Return with token → Create session
   ```

2. **Local Authentication**:
   ```
   Enter email/password → Validate credentials → Generate JWT → Set cookie → Authenticated
   ```

---

## Key Features

- **Real-time Multiplayer** - No lag with server-side physics
- **3D Graphics** - Immersive Babylon.js rendering
- **Matchmaking** - Automatic player pairing
- **Tournaments** - Bracket competitions
- **Friend System** - Add friends and see online status
- **Statistics** - Track wins, losses, and rating

---

## TO RESUME

### To understand the game flow:
1. Start at `frontend/main.ts` - See how app initializes
2. Look at `frontend/src/game/render.ts` - Main game loop
3. Check `backend/src/core/game/game-engine.ts` - Server logic
4. Review `packages/common-types` - Data structures

### To add new features:
1. Define types in `common-types`
2. Add backend logic in `game-engine.ts`
3. Create API route in `backend/routes`
4. Update frontend to use new API
5. Test with multiple browser windows

---

## Important Notes

- Game physics run on server for fairness
- WebSocket required for real-time play
- All positions use 3D coordinates (x, y, z)
- Ball speed increases after each paddle hit
- First to 5 points wins the game