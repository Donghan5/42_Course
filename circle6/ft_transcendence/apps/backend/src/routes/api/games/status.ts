import { FastifyInstance, FastifyRequest } from 'fastify';
import { gameEngine } from '../../../core/game/game-engine';

/**
 * Registers routes for getting the status of games.
 * @param fastify The Fastify instance.
 */
export default async function gameStatusRoutes(fastify: FastifyInstance) {
    fastify.get('/status/:gameId', async (request: FastifyRequest, reply) => {
        const { gameId } = request.params as { gameId: string };

        try {
            const game = gameEngine.getGameState(gameId);

            if (!game) {
                return reply.code(404).send({ 
                    error: 'Game not found or has ended' 
                });
            }

            // Can rejoin if game is in countdown, playing, or waiting
            const canRejoin = game.status === 'playing' || 
                            game.status === 'countdown' || 
                            game.status === 'waiting';

            return reply.send({
                gameId: game.gameId,
                status: game.status,
                player1Id: game.player1Id,
                player2Id: game.player2Id,
                canRejoin: canRejoin
            });
        } catch (error) {
            fastify.log.error('Error checking game status:', error);
            return reply.code(500).send({ 
                error: 'Failed to check game status' 
            });
        }
    });

  // Route to get a list of all active games
  // Final path will be GET /api/games
  fastify.get('/', async (request, reply) => {
    const activeGames = [];
    const games = gameEngine.getGames(); // Assumes gameEngine has this method

    for (const [gameId, gameState] of games.entries()) {
      activeGames.push({
        gameId,
        status: gameState.status,
        player1Id: gameState.player1Id,
        player2Id: gameState.player2Id,
        scores: {
          player1: gameState.player1.score,
          player2: gameState.player2.score,
        },
        lastUpdate: gameState.lastUpdate,
      });
    }

    return reply.send({
      success: true,
      totalGames: activeGames.length,
      activeGames,
    });
  });

  // Route to get the status of a specific game by its ID
  // Final path will be GET /api/games/:gameId
  fastify.get('/:gameId', async (request, reply) => {
    const { gameId } = request.params as { gameId: string };
    const gameState = gameEngine.getGameState(gameId);

    if (!gameState) {
      return reply.code(404).send({ error: 'Game not found' });
    }

    return reply.send({
      success: true,
      gameId,
      gameState,
    });
  });
}
