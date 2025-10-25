// apps/backend/src/core/stats/user-stats-manager.ts
import { dbAll, dbGet, dbRun } from '../../database/helpers';

export interface UserStats {
    userId: number;
    nickname: string;
    totalGames: number;
    wins: number;
    losses: number;
    winRate: number;
    averageScore: number;
    maxScore: number;
    totalPlayTime: number; // in minutes
    currentStreak: number;
    maxStreak: number;
    rankPoints: number;
    rank: string;
    gameTypes: {
        [gameType: string]: {
            games: number;
            wins: number;
            losses: number;
            winRate: number;
        };
    };
    recentGames: GameSummary[];
    achievements: Achievement[];
}

export interface GameSummary {
    id: number;
    gameType: string;
    opponentNickname: string;
    playerScore: number;
    opponentScore: number;
    result: 'win' | 'loss';
    finishedAt: Date;
    rankPointsChange?: number;
}

export interface Achievement {
    id: string;
    name: string;
    description: string;
    unlockedAt: Date;
    rarity: 'common' | 'rare' | 'epic' | 'legendary';
}

export interface RankingEntry {
    userId: number;
    nickname: string;
    rankPoints: number;
    wins: number;
    totalGames: number;
    winRate: number;
    rank: number;
}

export class UserStatsManager {
    private static instance: UserStatsManager;
    
    private rankThresholds = [
        { rank: 'Bronze', min: 0, max: 999 },
        { rank: 'Silver', min: 1000, max: 1499 },
        { rank: 'Gold', min: 1500, max: 1999 },
        { rank: 'Platinum', min: 2000, max: 2499 },
        { rank: 'Diamond', min: 2500, max: 2999 },
        { rank: 'Master', min: 3000, max: 3499 },
        { rank: 'Grandmaster', min: 3500, max: Infinity }
    ];

    private constructor() {}

    public static getInstance(): UserStatsManager {
        if (!UserStatsManager.instance) {
            UserStatsManager.instance = new UserStatsManager();
        }
        return UserStatsManager.instance;
    }

    /**
     * @description Retrieves user statistics including game stats, recent games, achievements, and rank information.
     */
    public async getUserStats(userId: number): Promise<UserStats | null> {
        try {
            const user = await dbGet('SELECT id, nickname FROM users WHERE id = ?', [userId]);
            if (!user) return null;

            const gameStats = await this.calculateGameStats(userId);
            
            const recentGames = await this.getRecentGames(userId, 10);
            
            const achievements = await this.getUserAchievements(userId);

            const rankInfo = await this.getUserRankInfo(userId);

            const stats: UserStats = {
                userId: user.id,
                nickname: user.nickname,
                ...gameStats,
                ...rankInfo,
                recentGames,
                achievements
            };

            return stats;
        } catch (error) {
            console.error('Error getting user stats:', error);
            return null;
        }
    }

    /**
     * @description Calculates game statistics for a user.
     */
    private async calculateGameStats(userId: number) {
        const games = await dbAll(`
            SELECT 
                g.*,
                CASE 
                    WHEN g.player1_id = ? THEN g.player1_score 
                    ELSE g.player2_score 
                END as player_score,
                CASE 
                    WHEN g.player1_id = ? THEN g.player2_score 
                    ELSE g.player1_score 
                END as opponent_score,
                CASE 
                    WHEN g.winner_id = ? THEN 'win' 
                    ELSE 'loss' 
                END as result
            FROM games g 
            WHERE (g.player1_id = ? OR g.player2_id = ?) 
            AND g.finished_at IS NOT NULL
            ORDER BY g.finished_at DESC
        `, [userId, userId, userId, userId, userId]);

        const totalGames = games.length;
        const wins = games.filter(g => g.result === 'win').length;
        const losses = totalGames - wins;
        const winRate = totalGames > 0 ? (wins / totalGames) * 100 : 0;

        const totalScore = games.reduce((sum, g) => sum + g.player_score, 0);
        const averageScore = totalGames > 0 ? totalScore / totalGames : 0;

        const maxScore = games.length > 0 ? Math.max(...games.map(g => g.player_score)) : 0;

        const streakInfo = this.calculateStreaks(games);

        const gameTypes = this.calculateGameTypeStats(games);

        return {
            totalGames,
            wins,
            losses,
            winRate: Math.round(winRate * 100) / 100,
            averageScore: Math.round(averageScore * 100) / 100,
            maxScore,
            totalPlayTime: totalGames * 2, // Assuming each game takes 2 minutes
            currentStreak: streakInfo.current,
            maxStreak: streakInfo.max,
            gameTypes
        };
    }

    /**
     * @description computes current and max streaks based on game results.
     * @param games
     * @returns { current: number, max: number } - current and max streaks
     */
    private calculateStreaks(games: any[]) {
        let currentStreak = 0;
        let maxStreak = 0;
        let tempStreak = 0;
        let lastResult = '';

        for (const game of games) {
            if (currentStreak === 0) {
                currentStreak = game.result === 'win' ? 1 : 0;
            } else if (game.result === 'win') {
                currentStreak++;
            } else {
                break;
            }
        }

        for (const game of games.reverse()) {
            if (game.result === lastResult) {
                tempStreak++;
            } else {
                if (lastResult === 'win') {
                    maxStreak = Math.max(maxStreak, tempStreak);
                }
                tempStreak = 1;
                lastResult = game.result;
            }
        }

        if (lastResult === 'win') {
            maxStreak = Math.max(maxStreak, tempStreak);
        }

        return { current: currentStreak, max: maxStreak };
    }

    /**
     * @description Stat for each game type
     * @param games
     */
    private calculateGameTypeStats(games: any[]) {
        const typeStats: { [key: string]: any } = {};

        for (const game of games) {
            const type = game.game_type || 'casual';
            
            if (!typeStats[type]) {
                typeStats[type] = { games: 0, wins: 0, losses: 0, winRate: 0 };
            }

            typeStats[type].games++;
            if (game.result === 'win') {
                typeStats[type].wins++;
            } else {
                typeStats[type].losses++;
            }
        }

        // Compute win rates for each game type
        for (const type in typeStats) {
            const stats = typeStats[type];
            stats.winRate = stats.games > 0 ? (stats.wins / stats.games) * 100 : 0;
            stats.winRate = Math.round(stats.winRate * 100) / 100;
        }

        return typeStats;
    }

    /**
     * @description Retrieves recent games for a user.
     */
    private async getRecentGames(userId: number, limit: number = 10): Promise<GameSummary[]> {
        const games = await dbAll(`
            SELECT 
                g.*,
                CASE 
                    WHEN g.player1_id = ? THEN p2.nickname 
                    ELSE p1.nickname 
                END as opponent_nickname,
                CASE 
                    WHEN g.player1_id = ? THEN g.player1_score 
                    ELSE g.player2_score 
                END as player_score,
                CASE 
                    WHEN g.player1_id = ? THEN g.player2_score 
                    ELSE g.player1_score 
                END as opponent_score,
                CASE 
                    WHEN g.winner_id = ? THEN 'win' 
                    ELSE 'loss' 
                END as result
            FROM games g 
            LEFT JOIN users p1 ON g.player1_id = p1.id
            LEFT JOIN users p2 ON g.player2_id = p2.id
            WHERE (g.player1_id = ? OR g.player2_id = ?) 
            AND g.finished_at IS NOT NULL
            ORDER BY g.finished_at DESC
            LIMIT ?
        `, [userId, userId, userId, userId, userId, userId, limit]);

        return games.map(game => ({
            id: game.id,
            gameType: game.game_type || 'casual',
            opponentNickname: game.opponent_nickname || 'Unknown',
            playerScore: game.player_score,
            opponentScore: game.opponent_score,
            result: game.result as 'win' | 'loss',
            finishedAt: new Date(game.finished_at),
            rankPointsChange: this.calculateRankPointsChange(game.result, game.player_score, game.opponent_score)
        }));
    }

    /**
     * @description Notifies friends of a user's status change.
     * @notice This method have to change --> now just prototype
     * @param userId 
     * @param userStatus 
     * @returns Promise<void>
     */
    private calculateRankPointsChange(result: string, playerScore: number, opponentScore: number): number {
        const basePoints = 25;
        const scoreDiff = playerScore - opponentScore;
        
        if (result === 'win') {
            return Math.max(15, basePoints + Math.floor(scoreDiff / 2));
        } else {
            return Math.min(-15, -basePoints + Math.floor(scoreDiff / 2));
        }
    }

    /**
     * @description Get user's rank information.
     * @param userId
     */
    private async getUserRankInfo(userId: number) {
        await dbRun(`
            CREATE TABLE IF NOT EXISTS user_stats (
                user_id INTEGER PRIMARY KEY,
                rank_points INTEGER DEFAULT 1000,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (user_id) REFERENCES users(id)
            )
        `, []);

        let userRank = await dbGet('SELECT * FROM user_stats WHERE user_id = ?', [userId]);
        
        if (!userRank) {
            await dbRun('INSERT INTO user_stats (user_id, rank_points) VALUES (?, ?)', [userId, 1000]);
            userRank = { user_id: userId, rank_points: 1000 };
        }

        const rankPoints = userRank.rank_points;
        const rank = this.getRankByPoints(rankPoints);

        return { rankPoints, rank };
    }

    /**
     * @description Compute rank based on points
     * @param points 
     * @returns string
     */
    private getRankByPoints(points: number): string {
        for (const rankInfo of this.rankThresholds) {
            if (points >= rankInfo.min && points <= rankInfo.max) {
                return rankInfo.rank;
            }
        }
        return 'Bronze';
    }

    /**
     * @description Updates the user's rank points.
     * @param userId 
     * @param pointsChange
     * @returns Promise<void>
     * @throws Error if the user is not found in the database.
     */
    public async updateRankPoints(userId: number, pointsChange: number): Promise<void> {
        try {
            await dbRun(`
                INSERT INTO user_stats (user_id, rank_points) 
                VALUES (?, 1000 + ?) 
                ON CONFLICT(user_id) 
                DO UPDATE SET 
                    rank_points = rank_points + ?,
                    updated_at = CURRENT_TIMESTAMP
            `, [userId, pointsChange, pointsChange]);
        } catch (error) {
            console.error('Error updating rank points:', error);
        }
    }

    /**
     * @description Retrieves user achievements.
     */
    private async getUserAchievements(userId: number): Promise<Achievement[]> {
        // This is a placeholder implementation.
        // In a real application, you would query the database for achievements.
        // For now, we return a static list of achievements.
        return [
            {
                id: 'first_win',
                name: 'First Victory',
                description: 'Win your first game',
                unlockedAt: new Date(),
                rarity: 'common'
            }
        ];
    }

    /**
     * Getting leaderboard
     */
    public async getLeaderboard(limit: number = 50): Promise<RankingEntry[]> {
        try {
            const rankings = await dbAll(`
                SELECT 
                    u.id as userId,
                    u.nickname,
                    COALESCE(us.rank_points, 1000) as rankPoints,
                    COUNT(CASE WHEN g.winner_id = u.id THEN 1 END) as wins,
                    COUNT(g.id) as totalGames,
                    CASE 
                        WHEN COUNT(g.id) > 0 
                        THEN ROUND((COUNT(CASE WHEN g.winner_id = u.id THEN 1 END) * 100.0 / COUNT(g.id)), 2)
                        ELSE 0 
                    END as winRate
                FROM users u
                LEFT JOIN user_stats us ON u.id = us.user_id
                LEFT JOIN games g ON (g.player1_id = u.id OR g.player2_id = u.id) 
                    AND g.finished_at IS NOT NULL
                GROUP BY u.id, u.nickname, us.rank_points
                HAVING totalGames >= 5 OR us.rank_points IS NOT NULL
                ORDER BY rankPoints DESC, wins DESC
                LIMIT ?
            `, [limit]);

            return rankings.map((entry, index) => ({
                ...entry,
                rank: index + 1
            }));
        } catch (error) {
            console.error('Error getting leaderboard:', error);
            return [];
        }
    }

    /**
     * User stats detailed (Metric for Prometheus/Grafana)
     */
    public async getUserMetrics(userId: number): Promise<{ [key: string]: number }> {
        const stats = await this.getUserStats(userId);
        if (!stats) return {};

        return {
            user_total_games: stats.totalGames,
            user_wins: stats.wins,
            user_losses: stats.losses,
            user_win_rate: stats.winRate,
            user_average_score: stats.averageScore,
            user_max_score: stats.maxScore,
            user_current_streak: stats.currentStreak,
            user_max_streak: stats.maxStreak,
            user_rank_points: stats.rankPoints,
            user_total_play_time: stats.totalPlayTime
        };
    }

    /**
     * All game metrics (System metrics for Prometheus/Grafana)
     */
    public async getSystemMetrics(): Promise<{ [key: string]: number }> {
        try {
            const totalUsers = await dbGet('SELECT COUNT(*) as count FROM users');
            const totalGames = await dbGet('SELECT COUNT(*) as count FROM games WHERE finished_at IS NOT NULL');
            const totalOnlineUsers = 0;
            
            const avgGameDuration = await dbGet(`
                SELECT AVG(
                    CASE 
                        WHEN finished_at IS NOT NULL 
                        THEN (julianday(finished_at) - julianday(created_at)) * 24 * 60 
                        ELSE 0 
                    END
                ) as avg_duration 
                FROM games 
                WHERE finished_at IS NOT NULL
            `);

            return {
                total_users: totalUsers?.count || 0,
                total_games: totalGames?.count || 0,
                total_online_users: totalOnlineUsers,
                average_game_duration_minutes: avgGameDuration?.avg_duration || 0
            };
        } catch (error) {
            console.error('Error getting system metrics:', error);
            return {};
        }
    }
}