// ts/apps/backend/src/routes/api/games/cancel.ts

import { FastifyInstance, FastifyRequest } from 'fastify';
import { gameEngine } from '../../../core/game/game-engine';

export default async function cancelRoutes(fastify: FastifyInstance) {
  fastify.post('/cancel', async (request: FastifyRequest, reply) => {
    const { playerId } = request.body as { playerId: string };

    if (!playerId) {
      return reply.code(400).send({ error: 'Player ID is required' });
    }

    if (gameEngine.waitingPlayer && String(gameEngine.waitingPlayer.playerId) === String(playerId)) {
      gameEngine.waitingPlayer = null;
      fastify.log.info(`Matchmaking cancelled by player ${playerId}`);
      return reply.send({ success: true, message: 'Matchmaking cancelled' });
    }

    fastify.log.info(`Matchmaking cancellation request for player ${playerId}, but player was not found in the waiting queue. Acknowledging as success.`);
    return reply.send({ success: true, message: 'Matchmaking successfully cancelled.' });
  });
}
