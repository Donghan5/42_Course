// packages/common-types/src/game.ts
export type GameStatus = 'WAITING' | 'PLAYING' | 'FINISHED' | 'CANCELED';

export interface Vector3D {
  x: number;
  y: number;
  z: number;
}

export interface PlayerState {
  position: Vector3D;
  score: number;
  paddleZ: number;
  nickname?: string;
  avatarUrl?: string;
  justHitBall?: boolean;
}

export interface BallState {
  position: Vector3D;
  velocity: Vector3D;
}

export interface GameState {
  player1: PlayerState;
  player2: PlayerState;
  ball: BallState;
  gameId: string;
  player1Id: string;
  player2Id: string;
  status: 'waiting' | 'countdown' | 'playing' | 'finished' | 'resuming';
  startTime?: number;
  lastUpdate: number;
  aiStrategy?: 'Defensive' | 'Aggressive' | 'Trickshot';
  countdownValue?: number;
  gameMode: string;
  isTournamentGame?: boolean;
}

export interface GameDTO {
  id: number;
  mode: 'LOCAL_PVP' | 'PVP' | 'AI' | 'TOURNAMENT';
  status: GameStatus;
  players: { id: number; nickname: string; score: number }[];
  createdAt: string;
  startedAt?: string;
  finishedAt?: string;
}

export type GameUpdate = {
	type: 'gameState';
	payload: GameState;
} | {
    type: 'gameEnd';
    winner: string;
} | {
    type: 'playerJoined' | 'playerLeft';
    playerId: string;
}

export const initialBallVelocity: Vector3D = {
  x: 0.12,
  y: 0,
  z: 0.08,
};


export const initialBallPosition: Vector3D = {
  x: 0,
  y: 20,
  z: 0,
};

export function scaleVector(vec: Vector3D, scalar: number): Vector3D {
  return { x: vec.x * scalar, y: vec.y * scalar, z: vec.z * scalar };
}

export function addVectors(vec1: Vector3D, vec2: Vector3D): Vector3D {
  return { x: vec1.x + vec2.x, y: vec1.y + vec2.y, z: vec1.z + vec2.z };
}
