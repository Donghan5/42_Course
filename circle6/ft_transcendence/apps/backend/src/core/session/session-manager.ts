// apps/backend/src/core/session/session-manager.ts

import { WebSocket } from 'ws';

interface SessionInfo {
    userId: number;
    ws: WebSocket;
    connectedAt: number;
}

/**
 * SessionManager - Enforces single session per user
 * Tracks active sessions and disconnects old sessions when new login occurs
 */
export class SessionManager {
    private static instance: SessionManager;
    private activeSessions: Map<number, SessionInfo> = new Map();

    private constructor() {}

    static getInstance(): SessionManager {
        if (!SessionManager.instance) {
            SessionManager.instance = new SessionManager();
        }
        return SessionManager.instance;
    }

    /**
     * Register a new session for a user
     * If user already has an active session, disconnect the old one
     */
    public registerSession(userId: number, ws: WebSocket): void {
        const existingSession = this.activeSessions.get(userId);

        if (existingSession && existingSession.ws.readyState === WebSocket.OPEN) {
            console.log(`⚠️ User ${userId} logging in from new location, disconnecting old session`);
            
            // Send notification to old session
            existingSession.ws.send(JSON.stringify({
                type: 'session_replaced',
                message: 'You have been logged in from another location'
            }));

            // Close old connection
            existingSession.ws.close(1000, 'Session replaced');
        }

        // Register new session
        this.activeSessions.set(userId, {
            userId,
            ws,
            connectedAt: Date.now()
        });

        console.log(`✅ Session registered for user ${userId}`);

        // Listen for disconnection
        ws.on('close', () => {
            this.removeSession(userId);
        });
    }

    /**
     * Remove a session when user disconnects
     */
    public removeSession(userId: number): void {
        this.activeSessions.delete(userId);
        console.log(`🔌 Session removed for user ${userId}`);
    }

    /**
     * Check if user has an active session
     */
    public hasActiveSession(userId: number): boolean {
        const session = this.activeSessions.get(userId);
        return session !== undefined && session.ws.readyState === WebSocket.OPEN;
    }

    /**
     * Get active session for user
     */
    public getSession(userId: number): SessionInfo | undefined {
        return this.activeSessions.get(userId);
    }
}