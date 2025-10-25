// apps/backend/src/core/game/reconnection-manager.ts

import { WebSocket } from 'ws';

interface DisconnectedPlayer {
    playerId: string;
    gameId: string;
    disconnectedAt: number;
    timeoutId: NodeJS.Timeout;
}

/**
 * ReconnectionManager - Handles player reconnections with timeout logic
 * When player disconnects:
 * 1. Pause the game
 * 2. Notify opponent
 * 3. Start 20-second countdown
 * 4. If player reconnects in time, resume game
 * 5. If timeout expires, player forfeits
 */
export class ReconnectionManager {
    private static instance: ReconnectionManager;
    private disconnectedPlayers: Map<string, DisconnectedPlayer> = new Map();
    private readonly RECONNECT_TIMEOUT = 20000; // 20 seconds

    private constructor() {}

    static getInstance(): ReconnectionManager {
        if (!ReconnectionManager.instance) {
            ReconnectionManager.instance = new ReconnectionManager();
        }
        return ReconnectionManager.instance;
    }

    /**
     * Handle player disconnection
     * Start countdown and pause game
     */
    public handleDisconnection(
        gameId: string,
        playerId: string,
        onTimeout: (gameId: string, playerId: string) => void,
        onPause: (gameId: string, playerId: string) => void
    ): void {
        // Check if already tracking this disconnection
        const key = `${gameId}-${playerId}`;
        if (this.disconnectedPlayers.has(key)) {
            console.log(`Already tracking disconnection for player ${playerId} in game ${gameId}`);
            return;
        }

        console.log(`🔌 Player ${playerId} disconnected from game ${gameId}, starting 20s countdown`);

        // Pause the game
        onPause(gameId, playerId);

        // Start timeout
        const timeoutId = setTimeout(() => {
            console.log(`⏰ Reconnection timeout expired for player ${playerId} in game ${gameId}`);
            this.disconnectedPlayers.delete(key);
            onTimeout(gameId, playerId);
        }, this.RECONNECT_TIMEOUT);

        // Track disconnection
        this.disconnectedPlayers.set(key, {
            playerId,
            gameId,
            disconnectedAt: Date.now(),
            timeoutId
        });
    }

    /**
     * Handle player reconnection
     * Cancel timeout and resume game
     */
    public handleReconnection(gameId: string, playerId: string): boolean {
        const key = `${gameId}-${playerId}`;
        const disconnectedPlayer = this.disconnectedPlayers.get(key);

        if (!disconnectedPlayer) {
            console.log(`No pending disconnection found for player ${playerId} in game ${gameId}`);
            return false;
        }

        console.log(`✅ Player ${playerId} reconnected to game ${gameId}`);

        // Cancel timeout
        clearTimeout(disconnectedPlayer.timeoutId);
        this.disconnectedPlayers.delete(key);

        return true;
    }

    /**
     * Check if player is in reconnection period
     */
    public isPlayerDisconnected(gameId: string, playerId: string): boolean {
        const key = `${gameId}-${playerId}`;
        return this.disconnectedPlayers.has(key);
    }

    /**
     * Get remaining time for reconnection (in seconds)
     */
    public getRemainingTime(gameId: string, playerId: string): number {
        const key = `${gameId}-${playerId}`;
        const disconnectedPlayer = this.disconnectedPlayers.get(key);

        if (!disconnectedPlayer) {
            return 0;
        }

        const elapsed = Date.now() - disconnectedPlayer.disconnectedAt;
        const remaining = Math.max(0, this.RECONNECT_TIMEOUT - elapsed);
        return Math.ceil(remaining / 1000); // Return seconds
    }

    /**
     * Cancel pending reconnection (e.g., when game ends)
     */
    public cancelReconnection(gameId: string, playerId: string): void {
        const key = `${gameId}-${playerId}`;
        const disconnectedPlayer = this.disconnectedPlayers.get(key);

        if (disconnectedPlayer) {
            clearTimeout(disconnectedPlayer.timeoutId);
            this.disconnectedPlayers.delete(key);
            console.log(`Cancelled reconnection tracking for player ${playerId} in game ${gameId}`);
        }
    }

    /**
     * Clean up all reconnections for a specific game
     */
    public cleanupGame(gameId: string): void {
        const keysToDelete: string[] = [];

        this.disconnectedPlayers.forEach((disconnection, key) => {
            if (disconnection.gameId === gameId) {
                clearTimeout(disconnection.timeoutId);
                keysToDelete.push(key);
            }
        });

        keysToDelete.forEach(key => this.disconnectedPlayers.delete(key));
        console.log(`Cleaned up ${keysToDelete.length} pending reconnections for game ${gameId}`);
    }
}