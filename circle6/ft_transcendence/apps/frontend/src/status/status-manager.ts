export interface FriendStatus {
    userId: number;
    nickname: string;
    status: 'online' | 'in_game' | 'away' | 'offline';
    lastSeen: string;
    gameId?: string;
}

export interface UserStats {
    userId: number;
    nickname: string;
    totalGames: number;
    wins: number;
    losses: number;
    winRate: number;
    rank: string;
    rankPoints: number;
    currentStreak: number;
    maxStreak: number;
    recentGames: Array<{
        id: number;
        gameType: string;
        opponentNickname: string;
        result: 'win' | 'loss';
        finishedAt: string;
    }>;
}

export class StatusManager {
    private static instance: StatusManager;
    private statusWs: WebSocket | null = null;
    private friends: Map<number, FriendStatus> = new Map();
    private statusUpdateCallbacks: Set<(friends: FriendStatus[]) => void> = new Set();
    private tournamentInviteCallbacks: Set<(invite: any) => void> = new Set();
    private currentUser: any = null;
    private heartbeatInterval: ReturnType<typeof setInterval> | null = null;
    private friendUpdateCallbacks: Set<() => void> = new Set();
    private connectionState: 'disconnected' | 'connecting' | 'connected' = 'disconnected';

    private constructor() {}

    public static getInstance(): StatusManager {
        if (!StatusManager.instance) {
            StatusManager.instance = new StatusManager();
        }
        return StatusManager.instance;
    }

    public initializeStatusConnection(token: string, user: any): Promise<void> {
        if (this.connectionState === 'connecting' || this.connectionState === 'connected') {
            console.log('StatusManager: Connection already in progress or established. Ignoring call.');
            return Promise.resolve();
        }
        this.connectionState = 'connecting';

        this.disconnect(); // Disconnect existing connection if any
        
        this.currentUser = user;
        
        return new Promise((resolve, reject) => {
            // Set a timeout for the connection
            const connectionTimeout = setTimeout(() => {
                console.error('StatusManager: WebSocket connection timeout');
                if (this.statusWs) {
                    this.statusWs.close();
                }
                this.connectionState = 'disconnected';
                reject(new Error('WebSocket connection timeout'));
            }, 10000); // 10 second timeout

            try {
                const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
                // Don't include token in URL since backend reads from cookies
                const wsUrl = `${protocol}//${window.location.host}/api/user/status/ws`;
                
                console.log('StatusManager: Connecting to WebSocket:', wsUrl);
                this.statusWs = new WebSocket(wsUrl);
                
                this.statusWs.onopen = async () => {
                    this.connectionState = 'connected';
                    console.log('StatusManager: WebSocket connected successfully');
                    clearTimeout(connectionTimeout);
                    
                    try {
                        await this.loadFriendsStatus();
                        this.startHeartbeat();
                        resolve(); 
                    } catch (error) {
                        console.error('StatusManager: Error loading friends status:', error);
                        reject(error); 
                    }
                };

                this.statusWs.onmessage = (event) => {
                    try {
                        const data = JSON.parse(event.data);
                        this.handleStatusMessage(data);
                    } catch (error) {
                        console.error('StatusManager: Error parsing message:', error);
                    }
                };
                
                this.statusWs.onclose = (event) => {
                    console.log('StatusManager: WebSocket disconnected', event.code, event.reason);
                    this.connectionState = 'disconnected';
                    this.stopHeartbeat();
                    // Only try to reconnect if this was an unexpected close
                    if (event.code !== 1000 && this.currentUser) {
                        setTimeout(() => this.reconnectStatus(), 5000);
                    }
                };
                
                this.statusWs.onerror = (error) => {
                    console.error('StatusManager: WebSocket error:', error);
                    this.connectionState = 'disconnected';
                    clearTimeout(connectionTimeout);
                    reject(new Error('WebSocket connection failed'));
                };
                
            } catch (error) {
                this.connectionState = 'disconnected';
                clearTimeout(connectionTimeout);
                console.error('StatusManager: Failed to create WebSocket:', error);
                reject(error);
            }
        });
    }

    private startHeartbeat(): void {
        // Send heartbeat every 30 seconds
        this.heartbeatInterval = setInterval(() => {
            this.sendHeartbeat();
        }, 30000);
    }

    private stopHeartbeat(): void {
        if (this.heartbeatInterval) {
            clearInterval(this.heartbeatInterval);
            this.heartbeatInterval = null;
        }
    }

    private handleStatusMessage(data: any): void {
        switch (data.type) {
            case 'friendStatusUpdate':
                this.updateFriendStatus(data.payload);
                break;
            case 'tournamentInvite':
                this.handleTournamentInvite(data.payload);
                break;
            case 'pong':
                // Heartbeat response
                break;
            case 'profile_updated':
            case 'friend_profile_updated':
                // Handle profile updates
                this.handleProfileUpdateMessage(data.payload);
                break;
            case 'friend_request_received':
            case 'friend_request_accepted':
            case 'friend_removed':  // This should trigger reload
            case 'friends_list_updated':
                // Reload friends and trigger UI update
                this.loadFriendsStatus().then(() => {
                    this.notifyFriendUpdate();
                }).catch(err => {
                    console.error('Failed to reload friend statuses:', err);
                    this.notifyFriendUpdate(); // Still notify even if reload fails
                });
                break;
            default:
                console.log('StatusManager: Unknown message type:', data.type);
        }
    }

    private handleProfileUpdateMessage(payload: any): void {
        // Dynamically import the handler to avoid circular dependencies
        import('../services/ui').then(({ handleProfileUpdate }) => {
            handleProfileUpdate(payload);
        }).catch(err => {
            console.error('Failed to handle profile update:', err);
        });
    }

    private notifyFriendUpdate(): void {
        this.friendUpdateCallbacks.forEach(callback => callback());
    }

    public onFriendUpdate(callback: () => void): void {
        this.friendUpdateCallbacks.add(callback);
    }

    public offFriendUpdate(callback: () => void): void {
        this.friendUpdateCallbacks.delete(callback);
    }

    private updateFriendStatus(friendData: FriendStatus): void {
        this.friends.set(friendData.userId, friendData);
        this.notifyStatusUpdate();
    }

    private handleTournamentInvite(invite: any): void {
        this.tournamentInviteCallbacks.forEach(callback => callback(invite));
    }

    public onTournamentInvite(callback: (invite: any) => void): void {
        this.tournamentInviteCallbacks.add(callback);
    }

    public offTournamentInvite(callback: (invite: any) => void): void {
        this.tournamentInviteCallbacks.delete(callback);
    }

    private async loadFriendsStatus(): Promise<void> {
        try {
            console.log('StatusManager: Loading friends status...');
            const response = await fetch('/api/user/status/friends', {
                credentials: 'include'
            });
            
            if (!response.ok) {
                throw new Error(`Failed to load friends status: ${response.status}`);
            }
            
            const data = await response.json();
            console.log('StatusManager: Friends status loaded:', data);
            this.friends.clear();
            
            for (const friend of data.friends) {
                this.friends.set(friend.userId, friend);
            }
            
            this.notifyStatusUpdate();
        } catch (error) {
            console.error('StatusManager: Error loading friends status:', error);
            throw error;
        }
    }

    private notifyStatusUpdate(): void {
        const friendsList = Array.from(this.friends.values());
        console.log('StatusManager: Notifying status update, friends count:', friendsList.length);
        this.statusUpdateCallbacks.forEach(callback => {
            try {
                callback(friendsList);
            } catch (error) {
                console.error('StatusManager: Error in status update callback:', error);
            }
        });
    }

    public onStatusUpdate(callback: (friends: FriendStatus[]) => void): void {
        this.statusUpdateCallbacks.add(callback);
        // Immediately call with current friends list
        const friendsList = Array.from(this.friends.values());
        if (friendsList.length > 0) {
            try {
                callback(friendsList);
            } catch (error) {
                console.error('StatusManager: Error in immediate status update callback:', error);
            }
        }
    }

    private sendHeartbeat(): void {
        if (this.statusWs && this.statusWs.readyState === WebSocket.OPEN) {
            this.statusWs.send(JSON.stringify({ type: 'ping', timestamp: Date.now() }));
        }
    }

    private async reconnectStatus(): Promise<void> {
        if (this.currentUser) {
            console.log('StatusManager: Attempting to reconnect...');
            try {
                const response = await fetch('/api/auth/me', { credentials: 'include' });
                if (response.ok) {
                    const userData = await response.json();
                    await this.initializeStatusConnection('', userData);
                }
            } catch (error) {
                console.error('StatusManager: Failed to reconnect:', error);
            }
        }
    }

    public getFriends(): FriendStatus[] {
        return Array.from(this.friends.values());
    }

    public isConnected(): boolean {
        return this.statusWs !== null && this.statusWs.readyState === WebSocket.OPEN;
    }

    public disconnect(): void {
        console.log('StatusManager: Disconnecting...');
        this.connectionState = 'disconnected';
        this.stopHeartbeat();
        if (this.statusWs) {
            this.statusWs.onclose = null;
            this.statusWs.close();
            this.statusWs = null;
        }
        this.friends.clear();
        this.statusUpdateCallbacks.clear();
        this.friendUpdateCallbacks.clear();
    }
}