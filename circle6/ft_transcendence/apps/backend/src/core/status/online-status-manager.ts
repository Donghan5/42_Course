import { WebSocket } from 'ws';
import { dbAll, dbGet } from '../../database/helpers';

export interface OnlineUser {
    userId: number;
    nickname: string;
    lastSeen: Date;
    status: 'online' | 'in_game' | 'away' | 'offline';
    gameId?: string;
}

export class OnlineStatusManager {
    private static instance: OnlineStatusManager;
    private onlineUsers: Map<number, OnlineUser> = new Map();
    private userSockets: Map<number, WebSocket> = new Map();
    private statusUpdateCallbacks: Set<(userId: number, status: OnlineUser) => void> = new Set();

    private constructor() {
        setInterval(() => this.cleanupInactiveUsers(), 60 * 1000); // Cleanup every 1 minute
    }

    public static getInstance(): OnlineStatusManager {
        if (!OnlineStatusManager.instance) {
            OnlineStatusManager.instance = new OnlineStatusManager();
        }
        return OnlineStatusManager.instance;
    }

    /**
     * @description Updates the last seen time for a user to keep them active.
     * @param userId
     */
    public updateUserLastSeen(userId: number): void {
        const user = this.onlineUsers.get(userId);
        if (user) {
            user.lastSeen = new Date();
        }
    }

    /**
     * @description Sends a friend update message to a specific user if they are online.
     * @param targetUserId 
     * @param message 
     */
    public sendFriendUpdate(targetUserId: number, eventType: string, payload: any): void {
        const socket = this.userSockets.get(targetUserId);
        if (socket && socket.readyState === WebSocket.OPEN) {
            socket.send(JSON.stringify({
                type: eventType,
                payload: payload
            }));
            console.log(`Sent ${eventType} to user ${targetUserId}`);
        }
    }

    /**
     * @description Notifies friends and the user themselves about profile changes
     * @param userId User whose profile changed
     * @param changeType Type of change (nickname, avatar, anonymize, delete)
     * @param data Additional data about the change
     */
    public async notifyProfileChange(
        userId: number, 
        changeType: 'nickname' | 'avatar' | 'anonymize' | 'delete',
        data: { nickname?: string; avatarUrl?: string; anonymousNickname?: string }
    ): Promise<void> {
        try {
            // Notify the user themselves (all their open sessions)
            const userSocket = this.userSockets.get(userId);
            if (userSocket && userSocket.readyState === WebSocket.OPEN) {
                userSocket.send(JSON.stringify({
                    type: 'profile_updated',
                    payload: {
                        userId,
                        changeType,
                        ...data
                    }
                }));
            }

            // Update the in-memory user data
            const user = this.onlineUsers.get(userId);
            if (user && changeType !== 'delete') {
                if (data.nickname) user.nickname = data.nickname;
                if (data.anonymousNickname) user.nickname = data.anonymousNickname;
            }

            // Notify all friends about the change
            const friends = await dbAll(
                `SELECT DISTINCT
                    CASE
                        WHEN uf.user_id = ? THEN uf.friend_id
                        ELSE uf.user_id
                    END as friend_id
                FROM users_friends uf
                WHERE (uf.user_id = ? OR uf.friend_id = ?)
                AND uf.status = 'accepted'
                `, [userId, userId, userId]
            );

            for (const friend of friends) {
                const friendSocket = this.userSockets.get(friend.friend_id);
                if (friendSocket && friendSocket.readyState === WebSocket.OPEN) {
                    friendSocket.send(JSON.stringify({
                        type: 'friend_profile_updated',
                        payload: {
                            userId,
                            changeType,
                            ...data
                        }
                    }));
                    console.log(`Notified friend ${friend.friend_id} of ${changeType} change for user ${userId}`);
                }
            }
        } catch (error) {
            console.error(`Error notifying profile change for user ${userId}:`, error);
        }
    }

    /**
     * @description Sets a user as online and sends their online status to their friends.
     * @param userId 
     * @param ws 
     * @returns Promise<void>
     * @throws Error if the user is not found in the database.
     */
    public async setUserOnline(userId: number, ws: WebSocket): Promise<void> {
        try {
            const user = await dbGet(
                `SELECT id, nickname FROM users WHERE id = ?`, [userId]
            );
            if (!user) {
                console.error(`User with ID ${userId} not found.`);
                return;
            }

            const onlineUser: OnlineUser = {
                userId,
                nickname: user.nickname,
                lastSeen: new Date(),
                status: 'online'
            };

            this.onlineUsers.set(userId, onlineUser);
            this.userSockets.set(userId, ws);

            await this.notifyFriendsStatusChange(userId, onlineUser);

            ws.on('close', () => {
                this.setUserOffline(userId);
            });

            ws.on('error', () => {
                this.setUserOffline(userId);
            });

            console.log(`User ${user.nickname} (ID: ${userId}) is now online.`);
        } catch (error) {
            console.error(`Error setting user ${userId} online:`, error);
        }
    }

    /**
     * @description Sets a user as offline and sends their offline status to their friends.
     * @param userId 
     */
    public async setUserOffline(userId: number): Promise<void> {
        const user = this.onlineUsers.get(userId);
        if (user) {
            user.status = 'offline';
            user.lastSeen = new Date();
            
            await this.notifyFriendsStatusChange(userId, user);
            
            this.onlineUsers.set(userId, user);
            this.userSockets.delete(userId);

            console.log(`User ${user.nickname} (ID: ${userId}) is now offline.`);
        }
    }

    /**
     * @description Notifies friends of a user's status change to in-game.
     * @param userId 
     * @param gameId 
     */
    public async setUserInGame(userId: number, gameId: string): Promise<void> {
        const user = this.onlineUsers.get(userId);
        if (user) {
            user.status = 'in_game';
            user.gameId = gameId;
            user.lastSeen = new Date();

            await this.notifyFriendsStatusChange(userId, user);

            console.log(`User ${user.nickname} (ID: ${userId}) is now in-game.`);
        }
    }

    /**
     * @description Notifies friends of a user's status change to online(game-ended).
     * @param userId 
     */
    public async setUserBackOnline(userId: number): Promise<void> {
        const user = this.onlineUsers.get(userId);
        if (user) {
            user.status = 'online';
            delete user.gameId;
            user.lastSeen = new Date();

            await this.notifyFriendsStatusChange(userId, user);
            console.log(`User ${user.nickname} (ID: ${userId}) is back online.`);
        }
    }

    /**
     * @description Notifies friends of a user's status change.
     * @param userId 
     * @param userStatus 
     * @returns Promise<void>
     */
    private async notifyFriendsStatusChange(userId: number, userStatus: OnlineUser): Promise<void> {
        try {
            const friends = await dbAll(
                `SELECT DISTINCT
                    CASE
                        WHEN uf.user_id = ? THEN uf.friend_id
                        ELSE uf.user_id
                    END as friend_id
                FROM users_friends uf
                WHERE (uf.user_id = ? OR uf.friend_id = ?)
                AND uf.status = 'accepted'
                `, [userId, userId, userId]);

            for (const friend of friends) {
                const friendSocket = this.userSockets.get(friend.friend_id);
                if (friendSocket && friendSocket.readyState === WebSocket.OPEN) {
                    const message = JSON.stringify({
                        type: 'friendStatusUpdate',
                        payload: {
                            userId: userStatus.userId,
                            nickname: userStatus.nickname,
                            status: userStatus.status,
                            lastSeen: userStatus.lastSeen.toISOString(),
                            gameId: userStatus.gameId
                        }                
                    });
                    friendSocket.send(message);
                }
            }
        } catch (error) {
            console.error(`Error notifying friends of user ${userId} status change:`, error);
        }
    }

    /**
     * @description Gets the online status of a user (specific).
     * @param userId 
     */
    public getUserStatus(userId: number): OnlineUser | null {
        return this.onlineUsers.get(userId) || null;
    }

    /**
     * @description Gets the online status of users friends
    */
    public async getFriendsStatus(userId: number): Promise<OnlineUser[]> {
        try {
            const friends = await dbAll(
                `SELECT DISTINCT
                    u.id,
                    u.nickname,
                    CASE
                        WHEN uf.user_id = ? THEN uf.friend_id
                        ELSE uf.user_id
                    END as friend_id
                    FROM users_friends uf
                    JOIN users u ON u.id = CASE
                        WHEN uf.user_id = ? THEN uf.friend_id
                        ELSE uf.user_id
                    END
                    WHERE (uf.user_id = ? OR uf.friend_id = ?)
                    AND uf.status = 'accepted'`, [userId, userId, userId, userId]);
            
            const friendsStatus: OnlineUser[] = [];

            for (const friend of friends) {
                const onlineStatus = this.onlineUsers.get(friend.friend_id);
                if (onlineStatus) {
                    friendsStatus.push(onlineStatus);
                } else {
                    friendsStatus.push({
                        userId: friend.friend_id,
                        nickname: friend.nickname,
                        lastSeen: new Date(), // Default to current time if not online
                        status: 'offline'
                    });
                }
            }

            return friendsStatus;
        } catch (error) {
            console.error(`Error getting friends status for user ${userId}:`, error);
            return [];
        }
    }

    /**
     * @description Cleans up inactive users who have not been seen for a certain period.
     * This method runs every minute to remove users who have been inactive for more than 5
     */
    private cleanupInactiveUsers(): void {
        const now = new Date();
        const timeout = 5 * 60 * 1000; // 5 minutes timeout

        for (const [userId, user] of this.onlineUsers.entries()) {
            const timeSinceLastSeen = now.getTime() - user.lastSeen.getTime();
            if (timeSinceLastSeen > timeout) {
                this.setUserOffline(userId);
            }
        }
    }

    public onStatusUpdate(callback: (userId: number, status: OnlineUser) => void): void {
        this.statusUpdateCallbacks.add(callback);
    }

    public getAllOnlineUsers(): OnlineUser[] {
        return Array.from(this.onlineUsers.values());
    }

    /**
     * @description Gets the WebSocket connection for a user.
     * @param userId 
     * @returns socket of User or null
     */
    public getUserSocket(userId: number): WebSocket | null {
        return this.userSockets.get(userId) || null;
    }

    /**
     * @description Sends a tournament invitation to a user if they are online.
     * @param targetUserId 
     * @param invitation 
     * @returns boolean - true if sent successfully, false otherwise
     */
    public async sendTournamentInvitation(
        targetUserId: number,
        invitation : {
            tournamentId: number,
            tournamentName: string,
            invitedBy: string,
            invitedById: number,
        }
    ): Promise<boolean> {
        const socket = this.userSockets.get(targetUserId);

        if (!socket || socket.readyState !== WebSocket.OPEN) {
            console.log(`User ${targetUserId} is not online. Cannot send tournament invitation.`);
            return false;
        }

        try {
            socket.send(JSON.stringify({
                type: 'tournamentInvite', 
                payload: invitation
            }));
            console.log(`Tournament invitation sent to user ${targetUserId}.`);
            return true;
        } catch (error) {
            console.error(`Error sending tournament invitation to user ${targetUserId}:`, error);
            return false;
        }
    }

    /**
     * @description Gets online friends who can be invited to a tournament.
     * @param userId 
     * @returns Promise<OnlineUser[]> - list of online friends
     */
    public async getInvitableFriends(userId: number): Promise<OnlineUser[]> {
        try {
            const friends = await this.getFriendsStatus(userId);

            return friends.filter(friend => friend.status === 'online' || friend.status === 'away');
        } catch (error) {
            console.error(`Error getting invitable friends for user ${userId}:`, error);
            return [];
        }
    }
}