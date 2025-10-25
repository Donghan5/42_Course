import { FastifyInstance, FastifyReply, FastifyRequest } from 'fastify';
import jwt from 'jsonwebtoken';
import { UserStatsManager } from '../../../core/stats/user-stats-manager';

async function verifyJwt(request: FastifyRequest, reply: FastifyReply) {
    try {
        const token = (request.cookies as any).auth_token;
        if (!token) {
            return reply.code(401).send({ error: 'Unauthorized' });
        }

        const decoded = jwt.verify(token, process.env.JWT_SECRET!) as { userId: number };
        request.user = decoded;

    } catch (err) {
        return reply.code(401).send({ error: 'Unauthorized' });
    }
}

export default async function statsRoute(fastify: FastifyInstance) {
    const statsManager = UserStatsManager.getInstance();
    
    fastify.get('/', { preHandler: [verifyJwt] }, async (request: FastifyRequest, reply: FastifyReply) => {
        const userId = request.user?.userId;
        
        if (typeof userId !== 'number') {
            return reply.code(401).send({ error: 'Unauthorized' });
        }
        
        try {
            const stats = await statsManager.getUserStats(userId);
            if (!stats) {
                return reply.code(404).send({ error: 'User stats not found' });
            }
            
            return reply.send({ stats });
        } catch (error) {
            fastify.log.error('Error getting user stats:', error);
            return reply.code(500).send({ error: 'Internal server error' });
        }
    });

    fastify.get('/leaderboard', async (request: FastifyRequest, reply: FastifyReply) => {
        const { limit = 50 } = request.query as { limit?: number };
        
        try {
            const leaderboard = await statsManager.getLeaderboard(limit);
            return reply.send({ leaderboard });
        } catch (error) {
            fastify.log.error('Error getting leaderboard:', error);
            return reply.code(500).send({ error: 'Internal server error' });
        }
    });

    fastify.get('/metrics', async (request: FastifyRequest, reply: FastifyReply) => {
        try {
            const systemMetrics = await statsManager.getSystemMetrics();
            
            const metrics = {
                ...systemMetrics
            };
            
            return reply.send({ metrics });
        } catch (error) {
            fastify.log.error('Error getting system metrics:', error);
            return reply.code(500).send({ error: 'Internal server error' });
        }
    });

    fastify.get('/:userId', { preHandler: [verifyJwt] }, async (request: FastifyRequest, reply: FastifyReply) => {
        const { userId: targetUserId } = request.params as { userId: string };
        
        try {
            const stats = await statsManager.getUserStats(parseInt(targetUserId));
            if (!stats) {
                return reply.code(404).send({ error: 'User stats not found' });
            }
            
            const publicStats = {
                userId: stats.userId,
                nickname: stats.nickname,
                totalGames: stats.totalGames,
                wins: stats.wins,
                losses: stats.losses,
                winRate: stats.winRate,
                rank: stats.rank,
                rankPoints: stats.rankPoints,
                currentStreak: stats.currentStreak,
                maxStreak: stats.maxStreak,
                recentGames: stats.recentGames.slice(0, 5) 
            };
            
            return reply.send({ stats: publicStats });
        } catch (error) {
            fastify.log.error('Error getting public user stats:', error);
            return reply.code(500).send({ error: 'Internal server error' });
        }
    });

}