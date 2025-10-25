// apps/backend/src/routes/api/user/privacy.controller.ts

import { FastifyInstance, FastifyRequest, FastifyReply } from 'fastify';
import jwt from 'jsonwebtoken';
import { dbGet, dbAll, dbRun, getDatabase } from '../../../database/helpers';
import crypto from 'crypto';
import { tournamentManager } from '../../../core/tournament/tournament-manager';
import { OnlineStatusManager } from '../../../core/status/online-status-manager';
import { broadcastProfileChange } from '../chat';

// --- DEFINE THE SENTINEL USER ID ---
const DELETED_USER_ID = -99;

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

export default async function privacyRoute(fastify: FastifyInstance) {
    fastify.get('/privacy/export', { preHandler: [verifyJwt] }, async (request: FastifyRequest, reply: FastifyReply) => {
        const userId = request.user?.userId;

        if (!userId) {
            return reply.code(401).send({ error: 'Unauthorized: Invalid token' });
        }

        try {
            const user = await dbGet('SELECT * FROM users WHERE id = ?', [userId]);
            if (!user) {
                return reply.code(404).send({ error: 'User not found' });
            }
            const games = await dbAll(`SELECT g.*, p1.nickname as player1_nickname, p2.nickname as player2_nickname FROM games g LEFT JOIN users p1 ON g.player1_id = p1.id LEFT JOIN users p2 ON g.player2_id = p2.id WHERE g.player1_id = ? OR g.player2_id = ? ORDER BY g.finished_at DESC`, [userId, userId]);
            const friends = await dbAll(`SELECT u.id, u.nickname, u.email, uf.created_at, uf.status FROM users u JOIN users_friends uf ON u.id = uf.friend_id WHERE uf.user_id = ?`, [userId]);
            const sentRequests = await dbAll(`SELECT fr.*, u.nickname as receiver_nickname FROM friend_requests fr LEFT JOIN users u ON fr.receiver_id = u.id WHERE fr.requester_id = ?`, [userId]);
            const receivedRequests = await dbAll(`SELECT fr.*, u.nickname as requester_nickname FROM friend_requests fr LEFT JOIN users u ON fr.requester_id = u.id WHERE fr.receiver_id = ?`, [userId]);
            const stats = await dbGet('SELECT * FROM user_stats WHERE user_id = ?', [userId]);
            const tournaments = await dbAll(`SELECT t.*, tp.joined_at FROM tournaments t JOIN tournament_participants tp ON t.id = tp.tournament_id WHERE tp.user_id = ?`, [userId]);

            const sanitizedUser = { ...user };
            delete sanitizedUser.password_hash;
            delete sanitizedUser.token;

            const exportData = {
                exportDate: new Date().toISOString(),
                user: sanitizedUser,
                gameHistory: games,
                friends: friends,
                friendRequests: { sent: sentRequests, received: receivedRequests },
                statistics: stats,
                tournaments: tournaments,
                metadata: { totalGames: games.length, totalFriends: friends.length, accountCreated: user.created_at || 'Unknown' }
            };

            reply.header('Content-Type', 'application/json');
            reply.header('Content-Disposition', `attachment; filename="pong-data-export-${userId}-${Date.now()}.json"`);
            return reply.send(exportData);
        } catch (error: any) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Failed to export data' });
        }
    });

 	/**
     * @description Anonymize user account.
     * This version correctly handles live tournaments and historical cache updates.
     */
    fastify.post('/privacy/anonymize', { preHandler: [verifyJwt] }, async (request: FastifyRequest, reply: FastifyReply) => {
        const userId = request.user?.userId;
        
        if (!userId) {
            return reply.code(401).send({ error: 'Unauthorized: Invalid token' });
        }

        try {
            const user = await dbGet('SELECT * FROM users WHERE id = ?', [userId]);
            if (!user) {
                return reply.code(404).send({ error: 'User not found' });
            }

            const db = await getDatabase();
            await dbRun('BEGIN TRANSACTION');

            try {
                // Generate anonymous identifier
                const anonymousId = `Anonymous_${crypto.randomBytes(4).toString('hex')}`;

                // STEP 1 & 2: Terminate any unfinished tournaments the user is in.
                const unfinishedTournaments = await dbAll(`
                    SELECT DISTINCT t.id
                    FROM tournaments t
                    LEFT JOIN tournament_participants tp ON t.id = tp.tournament_id
                    WHERE (t.created_by = ? OR tp.user_id = ?) AND t.status != 'finished'
                `, [userId, userId]);
                
                for (const tournament of unfinishedTournaments) {
                    await tournamentManager.forceDeleteTournament(tournament.id);
                }

                // STEP 3: Anonymize user data in the database
                await dbRun(`
                    UPDATE users SET
                        email = ?, name = ?, nickname = ?, google_id = NULL, avatar_url = NULL,
                        password_hash = NULL, token = NULL, auth_provider = 'anonymized'
                    WHERE id = ?
                `, [`anonymized_${userId}@deleted.local`, anonymousId, anonymousId, userId]);

                // STEP 4: Synchronize the in-memory cache with the new anonymous nickname.
                await tournamentManager.anonymizeUserInTournaments(userId.toString(), anonymousId);

                // Notify about anonymization via WebSocket
                const statusManager = OnlineStatusManager.getInstance();
                await statusManager.notifyProfileChange(userId, 'anonymize', {
                    anonymousNickname: anonymousId
                });

                broadcastProfileChange(userId, 'anonymize', {
                    anonymousNickname: anonymousId
                });

                // STEP 5: Delete social connections
                await dbRun('DELETE FROM users_friends WHERE user_id = ? OR friend_id = ?', [userId, userId]);
                await dbRun('DELETE FROM friend_requests WHERE requester_id = ? OR receiver_id = ?', [userId, userId]);

                await dbRun('COMMIT');

                reply.clearCookie('auth_token', { path: '/', httpOnly: true, secure: true, sameSite: 'lax' });

                return reply.send({ 
                    success: true, 
                    message: 'Account anonymized successfully.' 
                });
            } catch (error) {
                await dbRun('ROLLBACK');
                throw error;
            }
        } catch (error: any) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Failed to anonymize account' });
        }
    });

    /**
     * @description Permanently delete user account.
     * This version correctly handles live tournaments, historical data, and cache invalidation.
     */
    fastify.delete('/privacy/delete', { preHandler: [verifyJwt] }, async (request: FastifyRequest, reply: FastifyReply) => {
        const userId = request.user?.userId;
        const { confirmText } = request.body as { confirmText: string };
        
        if (!userId) {
            return reply.code(401).send({ error: 'Unauthorized: Invalid token' });
        }

        try {
            const user = await dbGet('SELECT * FROM users WHERE id = ?', [userId]);
            if (!user) {
                return reply.code(404).send({ error: 'User not found' });
            }

            if (confirmText !== 'DELETE MY ACCOUNT') {
                return reply.code(400).send({ error: 'Please type "DELETE MY ACCOUNT" to confirm' });
            }

            const db = await getDatabase();
            await dbRun('BEGIN TRANSACTION');

            try {
                // STEP 1: Handle UNFINISHED tournaments
                const unfinishedTournaments = await dbAll(`
                    SELECT DISTINCT t.id
                    FROM tournaments t
                    LEFT JOIN tournament_participants tp ON t.id = tp.tournament_id
                    WHERE (t.created_by = ? OR tp.user_id = ?) AND t.status != 'finished'
                `, [userId, userId]);

                for (const tournament of unfinishedTournaments) {
                    await tournamentManager.forceDeleteTournament(tournament.id);
                }
                
                // STEP 2: Use the manager to scrub the user from all historical records.
                await tournamentManager.scrubUserFromHistory(userId.toString());

                // Notify about account deletion via WebSocket
                const statusManager = OnlineStatusManager.getInstance();
                await statusManager.notifyProfileChange(userId, 'delete', {});

                broadcastProfileChange(userId, 'delete', {});
                
                // STEP 3: Delete all remaining direct, non-historical associations
                await dbRun('DELETE FROM tournament_participants WHERE user_id = ?', [userId]);
                await dbRun('DELETE FROM users_friends WHERE user_id = ? OR friend_id = ?', [userId, userId]);
                await dbRun('DELETE FROM friend_requests WHERE requester_id = ? OR receiver_id = ?', [userId, userId]);
                await dbRun('DELETE FROM user_stats WHERE user_id = ?', [userId]);

                // STEP 4: Finally, delete the user's own record
                await dbRun('DELETE FROM users WHERE id = ?', [userId]);

                await dbRun('COMMIT');

                reply.clearCookie('auth_token', { path: '/', httpOnly: true, secure: true, sameSite: 'lax' });

                return reply.send({ 
                    success: true, 
                    message: 'Account permanently deleted. All your data has been removed from our system.' 
                });
            } catch (error) {
                await dbRun('ROLLBACK');
                throw error;
            }
        } catch (error: any) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Failed to delete account' });
        }
    });

    fastify.get('/privacy/info', { preHandler: [verifyJwt] }, async (request: FastifyRequest, reply: FastifyReply) => {
        const userId = request.user?.userId;
        if (!userId) { return reply.code(401).send({ error: 'Unauthorized: Invalid token' }); }
        try {
            const user = await dbGet('SELECT * FROM users WHERE id = ?', [userId]);
            if (!user) { return reply.code(404).send({ error: 'User not found' }); }
            const gamesCount = await dbGet('SELECT COUNT(*) as count FROM games WHERE player1_id = ? OR player2_id = ?', [userId, userId]);
            const friendsCount = await dbGet('SELECT COUNT(*) as count FROM users_friends WHERE user_id = ?', [userId]);
            const tournamentsCount = await dbGet('SELECT COUNT(*) as count FROM tournament_participants WHERE user_id = ?', [userId]);
            const dataInfo = {
                personalData: { stored: ['Email address', 'Nickname', 'Profile avatar', user.auth_provider === 'google' ? 'Google account ID' : 'Password (encrypted)', 'Last seen timestamp'], usage: 'Used for authentication, profile display, and game matchmaking' },
                gameData: { count: gamesCount.count, stored: ['Game results', 'Scores', 'Opponent information', 'Game timestamps'], usage: 'Used for statistics, leaderboards, and game history' },
                socialData: { friendsCount: friendsCount.count, stored: ['Friend connections', 'Friend requests', 'Online status'], usage: 'Used for social features and friend interactions' },
                tournamentData: { count: tournamentsCount.count, stored: ['Tournament participation', 'Tournament results', 'Rankings'], usage: 'Used for competitive play and rankings' },
                statistics: { stored: ['Win/loss record', 'Rating points', 'Tournament points'], usage: 'Used for matchmaking and leaderboards' },
                dataRetention: { policy: 'Data is retained while your account is active. You can request anonymization or deletion at any time.', location: 'All data is stored locally on our servers (SQLite database)' },
                yourRights: ['Right to access your data (Export)', 'Right to rectification (Edit profile)', 'Right to erasure (Delete account)', 'Right to be forgotten (Anonymize account)', 'Right to data portability (Export to JSON)']
            };
            return reply.send(dataInfo);
        } catch (error: any) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Failed to get data info' });
        }
    });
}