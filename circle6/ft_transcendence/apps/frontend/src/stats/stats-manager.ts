import { UserStats } from '../status/status-manager';

export class StatsManager {
    private static instance: StatsManager;

    private constructor() {}

    public static getInstance(): StatsManager {
        if (!StatsManager.instance) {
            StatsManager.instance = new StatsManager();
        }
        return StatsManager.instance;
    }

    /**
     * @description Get current user's stats
     */
    public async getUserStats(): Promise<UserStats | null> {
        try {
            const response = await fetch('/api/user/stats', {
                credentials: 'include'
            });
            
            if (response.ok) {
                const data = await response.json();
                return data.stats;
            }
            
            return null;
        } catch (error) {
            console.error('Error getting user stats:', error);
            return null;
        }
    }

    /**
     * @description Get public stats of a user by userId
     */
    public async getPublicStats(userId: number): Promise<UserStats | null> {
        try {
            const response = await fetch(`/api/user/stats/${userId}`, {
                credentials: 'include'
            });
            
            if (response.ok) {
                const data = await response.json();
                return data.stats;
            }
            
            return null;
        } catch (error) {
            console.error('Error getting public stats:', error);
            return null;
        }
    }

    /**
     * @description Get leaderboard of top players
     */
    public async getLeaderboard(limit: number = 50): Promise<any[]> {
        try {
            const response = await fetch(`/api/user/leaderboard?limit=${limit}`, {
                credentials: 'include'
            });
            
            if (response.ok) {
                const data = await response.json();
                return data.leaderboard;
            }
            
            return [];
        } catch (error) {
            console.error('Error getting leaderboard:', error);
            return [];
        }
    }
}