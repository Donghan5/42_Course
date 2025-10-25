// apps/backend/src/core/game/logic.ts
import { GameState, PlayerState, BallState, initialBallVelocity, GameStatus, Vector3D } from "@trans/common-types";
import { gameEngine } from "./game-engine";
import { strategy } from "./ai-strategy";
import WebSocket from 'ws';
import {
    WIN_SCORE,
    POINT_PER_GOAL,
    BALL_RADIUS,
    ARENA_DEPTH,
    PADDLE_WIDTH,
    PADDLE_HEIGHT,
    PADDLE_DEPTH,
    PADDLE_X_POSITION,
    PHYSICS_SUBSTEPS,
    BALL_MAX_SPEED,
    BALL_SPEED_INCREASE,
    BALL_ANGLE_MODIFIER,
    GOAL_LINE_LEFT,
    GOAL_LINE_RIGHT,
    PADDLE_SPEED,
    PADDLE_Z_LIMIT,
    FIXED_DELTA_TIME,
    BALL_INITIAL_SPEED,
    WALL_COLLISION_Z_TOP,
    WALL_COLLISION_Z_BOTTOM,
    PADDLE_Y_POSITION
} from "@trans/common-types";

function createInitialPlayerState(): PlayerState {
    return {
        position: { x: 0, y: 0, z: 0 },
        score: 0,
        paddleZ: 0,
    };
}

function createInitialBallState(): BallState {
    return {
        position: { x: 0, y: 0, z: 0 },
        velocity: initialBallVelocity,
    };
}

export function initState(gameId: string): GameState {
    return {
        gameId,
        player1: createInitialPlayerState(),
        player2: createInitialPlayerState(),
        player1Id: '',
        player2Id: '',
        ball: createInitialBallState(),
        status: 'waiting',
        lastUpdate: Date.now(),
        gameMode: '',
    };
}

export function addPoint(
    state: GameState,
    scorerId: string,
): GameState {
    if (state.status === 'finished') return state;
    const nextState: GameState = {
        ...state,
        player1: { ...state.player1 },
        player2: { ...state.player2 },
    };
    if (scorerId === state.player1Id) {
        nextState.player1.score += POINT_PER_GOAL;
    } else if (scorerId === state.player2Id) {
        nextState.player2.score += POINT_PER_GOAL;
    }
    if (nextState.player1.score >= WIN_SCORE || nextState.player2.score >= WIN_SCORE) {
        nextState.status = 'finished';
    }
    nextState.lastUpdate = Date.now();
    return nextState;
}

export function isGameOver(state: GameState): boolean {
    return state.status === 'finished';
}

/**
 * Update the physics of the game
 * @param game - The current game state
 */
function checkPaddleCollision(ballPos: Vector3D, ballVel: Vector3D, paddlePos: Vector3D) {
    const ballHalfSize = BALL_RADIUS;
    const paddleHalfSize = { x: PADDLE_WIDTH / 2, y: PADDLE_HEIGHT / 2, z: PADDLE_DEPTH / 2 };

    const dx = ballPos.x - paddlePos.x;
    const dy = ballPos.y - paddlePos.y;
    const dz = ballPos.z - paddlePos.z;

    const overlapX = (ballHalfSize + paddleHalfSize.x) - Math.abs(dx);
    const overlapZ = (ballHalfSize + paddleHalfSize.z) - Math.abs(dz);

    if (overlapX > 0 && overlapZ > 0) {
        if (overlapX < overlapZ) {
            // Collision is primarily on the X-axis (front/back of paddle)
            ballVel.x *= -1;
            ballPos.x += Math.sign(dx) * overlapX; // Move ball out of paddle

            // Add spin based on where it hits the paddle
            const relativeIntersectZ = (ballPos.z - paddlePos.z) / paddleHalfSize.z;
            ballVel.z += relativeIntersectZ * BALL_ANGLE_MODIFIER * 0.5;
        } else {
            // Collision is primarily on the Z-axis (top/bottom of paddle)
            ballVel.z *= -1;
            ballPos.z += Math.sign(dz) * overlapZ; // Move ball out of paddle
        }
        return true; // Collision detected
    }
    return false; // No collision
}


// New function to handle a single substep with proper collision timing
function simulateSubstep(
    game: GameState,
    dt: number,
): void {
    // Update ball position
    game.ball.position.x += game.ball.velocity.x * dt;
    game.ball.position.y += game.ball.velocity.y * dt;
    game.ball.position.z += game.ball.velocity.z * dt;

    // Wall collision
    if (game.ball.position.z >= WALL_COLLISION_Z_TOP || game.ball.position.z <= WALL_COLLISION_Z_BOTTOM) {
        game.ball.velocity.z *= -1;
        game.ball.position.z = Math.max(WALL_COLLISION_Z_BOTTOM, Math.min(WALL_COLLISION_Z_TOP, game.ball.position.z));
    }

    // Paddle positions for collision detection
    const paddle1Pos = { x: -PADDLE_X_POSITION, y: PADDLE_Y_POSITION, z: game.player1.paddleZ };
    const paddle2Pos = { x: PADDLE_X_POSITION, y: PADDLE_Y_POSITION, z: game.player2.paddleZ };

    // Check for paddle collisions
    let collisionOccurred = false;
    if (game.ball.velocity.x < 0) {
        if (checkPaddleCollision(game.ball.position, game.ball.velocity, paddle1Pos)) {
            collisionOccurred = true;
        }
    } else {
        if (checkPaddleCollision(game.ball.position, game.ball.velocity, paddle2Pos)) {
            collisionOccurred = true;
            if (game.player2Id === 'AI') {
                game.player2.justHitBall = true;
            }
        }
    }

    if (collisionOccurred) {
        // Increase ball speed after a paddle hit
        const speed = Math.sqrt(game.ball.velocity.x ** 2 + game.ball.velocity.z ** 2);
        if (speed < BALL_MAX_SPEED) {
            game.ball.velocity.x *= BALL_SPEED_INCREASE;
            game.ball.velocity.z *= BALL_SPEED_INCREASE;
        }
    }
}

export function updatePhysics(game: GameState, deltaTime: number): void {
    const dt = deltaTime / PHYSICS_SUBSTEPS;

    for (let i = 0; i < PHYSICS_SUBSTEPS; i++) {
        simulateSubstep(game, dt);
    }

    if (game.player2Id === 'AI') {
        strategy.updateAIPaddle(game);
    }

    const scorerSide = game.ball.position.x < GOAL_LINE_LEFT ? 'right' :
                       game.ball.position.x > GOAL_LINE_RIGHT ? 'left' : null;

    if (scorerSide) {
        const scorerId = scorerSide === 'left' ? game.player1Id : game.player2Id;
        const updatedState = addPoint(game, scorerId);
        gameEngine.getGames().set(game.gameId, updatedState);
        resetBall(updatedState);
        if (isGameOver(updatedState)) {
            updatedState.status = 'finished';
            gameEngine.getGames().set(game.gameId, updatedState);
            gameEngine.endGame(game.gameId);
        }
    }
}

export function processPlayerInput(
    game: GameState,
    playerId: string,
    direction: 'up' | 'down' | 'stop',
    deltaTime: number = FIXED_DELTA_TIME
): void {
    const playerKey = getPlayerKey(playerId, game);
    if (!playerKey) return;
    let currentPaddleZ = game[playerKey].paddleZ;
    switch (direction) {
        case 'up':
            currentPaddleZ += PADDLE_SPEED;
            break;
        case 'down':
            currentPaddleZ -= PADDLE_SPEED;
            break;
        case 'stop':
            break;
    }
    if (currentPaddleZ > PADDLE_Z_LIMIT) currentPaddleZ = PADDLE_Z_LIMIT;
    if (currentPaddleZ < -PADDLE_Z_LIMIT) currentPaddleZ = -PADDLE_Z_LIMIT;
    game[playerKey].paddleZ = currentPaddleZ;
    if (playerKey === 'player1') {
        game.player1.position.x = -PADDLE_X_POSITION;
        game.player1.position.z = currentPaddleZ;
    } else {
        game.player2.position.x = PADDLE_X_POSITION;
        game.player2.position.z = currentPaddleZ;
    }
}

function getPlayerKey(playerId: string, game: GameState): 'player1' | 'player2' | null {
    if (game.player1Id === playerId) {
        return 'player1';
    }
    if (game.player2Id === playerId) {
        return 'player2';
    }
    return null;
}

export function resetBall(game: GameState): void {
    game.ball.position = { x: 0, y: 1, z: 0 };
    game.ball.velocity = {
        x: game.ball.velocity.x > 0 ? -BALL_INITIAL_SPEED : BALL_INITIAL_SPEED,
        y: 0,
        z: (Math.random() - 0.5) * 40
    };
}

export function broadcastGameState(gameId: string): void {
    const game = gameEngine.getGames().get(gameId);
    const players = gameEngine.getConnectedPlayers().get(gameId);
    if (!game || !players) return;
    const message = JSON.stringify({
        type: 'gameState',
        payload: game
    });
    players.forEach((ws, playerId) => {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send(message);
        } else {
            console.warn(`WebSocket for player ${playerId} is not open`);
        }
    });
}
