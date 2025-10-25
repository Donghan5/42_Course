// apps/backend/src/routes/api/tournament/index.ts

import { FastifyInstance } from 'fastify';
import { tournamentManager } from '../../../core/tournament/tournament-manager';
import { authMiddleware } from '../../../middleware/auth.middleware';
import jwt from 'jsonwebtoken';
import { WebSocket } from 'ws';

export default async function tournamentRoutes(fastify: FastifyInstance) {
    // Get user's hosted tournaments (tournaments where user was the host)
    fastify.get('/my-tournaments', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            const tournaments = await tournamentManager.getUserHostedTournaments(userId.toString());

            return reply.send({
                tournaments,
                count: tournaments.length
            });
        } catch (error) {
            fastify.log.error('Get user hosted tournaments error:', error);
            return reply.code(500).send({
                error: 'Failed to retrieve hosted tournaments',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Create tournament
    fastify.post('/create', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const { name, maxPlayers = 8 } = request.body as { name: string; maxPlayers?: number };
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            if (!name || name.trim().length === 0) {
                return reply.code(400).send({ error: 'Tournament name is required' });
            }

            // Check for duplicate names
            const existingTournaments = await tournamentManager.getActiveTournaments();
            if (existingTournaments.some(t => t.name.toLowerCase() === name.trim().toLowerCase())) {
                return reply.code(400).send({ error: 'Tournament name already exists' });
            }

            const tournamentId = await tournamentManager.createTournament(
                userId.toString(), 
                name.trim(), 
                maxPlayers
            );

            // Auto-join the creator
            await tournamentManager.joinTournament(tournamentId, userId.toString());

            tournamentManager.broadcastActiveTournamentsUpdate();

            return reply.code(201).send({
                success: true,
                message: 'Tournament created successfully',
                tournamentId
            });
        } catch (error) {
            fastify.log.error('Tournament creation error:', error);
            return reply.code(500).send({
                error: 'Failed to create tournament',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Join tournament
    fastify.post('/join', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const { tournamentId } = request.body as { tournamentId: string };
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            if (!tournamentId) {
                return reply.code(400).send({ error: 'Tournament ID is required' });
            }

            const success = await tournamentManager.joinTournament(tournamentId, userId.toString());
            
            if (!success) {
                return reply.code(404).send({ error: 'Tournament not found or already full' });
            }

            return reply.send({
                success: true,
                message: 'Successfully joined the tournament'
            });
        } catch (error) {
            fastify.log.error('Tournament join error:', error);
            return reply.code(500).send({
                error: 'Failed to join tournament',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Start tournament (host only)
    fastify.post('/start', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const { tournamentId } = request.body as { tournamentId: string };
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            if (!tournamentId) {
                return reply.code(400).send({ error: 'Tournament ID is required' });
            }

            const tournament = await tournamentManager.getTournament(tournamentId);
            
            if (!tournament) {
                return reply.code(404).send({ error: 'Tournament not found' });
            }

            if (tournament.hostId !== userId.toString()) {
                return reply.code(403).send({ error: 'Only the tournament host can start it' });
            }

            const success = await tournamentManager.startTournament(tournamentId);

            if (!success) {
                return reply.code(400).send({ error: 'Tournament cannot be started (need at least 4 players)' });
            }

            return reply.send({
                success: true,
                message: 'Tournament started successfully'
            });

        } catch (error) {
            fastify.log.error('Tournament start error:', error);
            return reply.code(500).send({
                error: 'Failed to start tournament',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Delete tournament (host only, waiting status only)
    fastify.post('/delete', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const { tournamentId } = request.body as { tournamentId: string };
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            if (!tournamentId) {
                return reply.code(400).send({ error: 'Tournament ID is required' });
            }

            const success = await tournamentManager.deleteTournament(tournamentId, userId.toString());

            if (!success) {
                return reply.code(403).send({ error: 'Cannot delete tournament (not host or already started)' });
            }

            tournamentManager.broadcastActiveTournamentsUpdate();

            return reply.send({
                success: true,
                message: 'Tournament deleted successfully'
            });

        } catch (error) {
            fastify.log.error('Tournament delete error:', error);
            return reply.code(500).send({
                error: 'Failed to delete tournament',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Get active tournaments list
    fastify.get('/active', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const tournaments = await tournamentManager.getActiveTournaments();

            return reply.send({
                tournaments,
                count: tournaments.length
            });
        } catch (error) {
            fastify.log.error('Get active tournaments error:', error);
            return reply.code(500).send({
                error: 'Failed to retrieve active tournaments',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Get specific tournament
    fastify.get('/:tournamentId', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const { tournamentId } = request.params as { tournamentId: string };

            if (!tournamentId) {
                return reply.code(400).send({ error: 'Tournament ID is required' });
            }

            const tournament = await tournamentManager.getTournament(tournamentId);

            if (!tournament) {
                return reply.code(404).send({ error: 'Tournament not found' });
            }

            return reply.send(tournament);
        } catch (error) {
            fastify.log.error('Get tournament info error:', error);
            return reply.code(500).send({
                error: 'Failed to retrieve tournament information',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Confirm match participation
    fastify.post('/confirm-match', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const { tournamentId, matchId } = request.body as { tournamentId: string; matchId: string };
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            if (!tournamentId || !matchId) {
                return reply.code(400).send({ error: 'Tournament ID and Match ID are required' });
            }

            const success = await tournamentManager.confirmMatch(tournamentId, matchId, userId.toString());

            if (!success) {
                return reply.code(400).send({ error: 'Failed to confirm match' });
            }

            return reply.send({
                success: true,
                message: 'Match confirmed successfully'
            });

        } catch (error) {
            fastify.log.error('Match confirmation error:', error);
            return reply.code(500).send({
                error: 'Failed to confirm match',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Finish match (called by game system when match ends)
    fastify.post('/finish-match', async (request, reply) => {
        try {
            const { tournamentId, matchId, winnerId } = request.body as { 
                tournamentId: string; 
                matchId: string; 
                winnerId: string 
            };

            console.log(`Finishing tournament match: ${matchId} in tournament: ${tournamentId}, winner: ${winnerId}`);

            if (!tournamentId || !matchId || !winnerId) {
                return reply.code(400).send({ error: 'Tournament ID, Match ID, and Winner ID are required' });
            }

            const success = await tournamentManager.finishMatch(tournamentId, matchId, winnerId);

            if (!success) {
                return reply.code(400).send({ error: 'Failed to finish match' });
            }

            return reply.send({
                success: true,
                message: 'Match finished successfully'
            });

        } catch (error) {
            fastify.log.error('Finish match error:', error);
            return reply.code(500).send({
                error: 'Failed to finish match',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // Get user tournament history
    fastify.get('/history/user', { preHandler: [authMiddleware] }, async (request, reply) => {
        try {
            const userId = (request as any).user?.userId;

            if (!userId) {
                return reply.code(401).send({ error: 'Unauthorized' });
            }

            const history = await tournamentManager.getUserTournamentHistory(userId.toString());

            return reply.send({
                history,
                count: history.length
            });
        } catch (error) {
            fastify.log.error('Get tournament history error:', error);
            return reply.code(500).send({
                error: 'Failed to retrieve tournament history',
                message: error instanceof Error ? error.message : 'Unknown error'
            });
        }
    });

    // WebSocket endpoint for real-time tournament updates
    fastify.get('/ws/:tournamentId', { websocket: true }, (connection, request) => {
        const { tournamentId } = request.params as { tournamentId: string };
        const token = (request.cookies as any).auth_token;

        if (!token) {
            connection.close();
            return;
        }

        try {
            const decoded = jwt.verify(token, process.env.JWT_SECRET!) as { userId: number };
            const userId = decoded.userId.toString();
            
            if (!userId || !tournamentId) {
                connection.close();
                return;
            }

            // Add socket to tournament manager
            tournamentManager.addSocket(tournamentId, userId, connection as unknown as WebSocket);

            // Handle socket close
            connection.on('close', () => {
                tournamentManager.removeSocket(tournamentId, userId);
            });

            // Handle socket errors
            connection.on('error', (error) => {
                console.error('Tournament WebSocket error:', error);
                tournamentManager.removeSocket(tournamentId, userId);
            });

        } catch (error) {
            console.error('WebSocket authentication error:', error);
            connection.close();
        }
    });

    /**
     * @description WebSocket for active tournaments list updates
     */
    fastify.get('/ws/active', { websocket: true }, (connection, request) => {
        tournamentManager.addActiveTournamentsSocket(connection as unknown as WebSocket);
        connection.on('close', () => {
            tournamentManager.removeActiveTournamentsSocket(connection as unknown as WebSocket);
        });
    });    
}