import { FastifyInstance, FastifyRequest } from 'fastify';
import { gameEngine } from '../../../core/game/game-engine';
import { dbGet } from '../../../database/helpers';

const createGameBodySchema = {
  type: 'object',
  required: ['player1Id', 'gameMode'],
  properties: {
    player1Id: { type: 'string', minLength: 1 },
    player2Id: { type: 'string' },
    gameMode: { type: 'string', enum: ['PVP', 'AI', 'LOCAL_PVP', 'TOURNAMENT'] },
    aiStrategy: { type: 'string', enum: ['Defensive', 'Aggressive', 'Trickshot'] },
    tournamentId: { type: 'string' },
    matchId: { type: 'string' }
  },
};

export default async function createGameRoute(fastify: FastifyInstance) {
  fastify.post(
    '/',
    { schema: { body: createGameBodySchema } },
    async (request, reply) => {
      try {
        const {
          player1Id: joiningPlayerId, 
          gameMode,
          aiStrategy,
          tournamentId,
          matchId,
          
        } = request.body as any;

        if (gameMode === 'PVP') {
          if (gameEngine.waitingPlayer) {

            const player1Id = gameEngine.waitingPlayer.playerId; 
            const player2Id = joiningPlayerId;                  

            const player1Data = await dbGet(`SELECT nickname, avatar_url FROM users WHERE id = ?`, [player1Id]);
            const player2Data = await dbGet(`SELECT nickname, avatar_url FROM users WHERE id = ?`, [player2Id]);

            const p1Nickname = player1Data?.nickname || `Player ${player1Id}`;
            const p1Avatar = player1Data?.avatar_url || '/default-avatar.png';
            const p2Nickname = player2Data?.nickname || `Player ${player2Id}`;
            const p2Avatar = player2Data?.avatar_url || '/default-avatar.png';

            const gameId = gameEngine.createGame(
              player1Id,
              player2Id,
              'PVP',
              undefined,
              p1Nickname,
              p2Nickname,
              p1Avatar,
              p2Avatar
            );

            gameEngine.notifyMatchFound(player1Id, gameId);
            gameEngine.waitingPlayer = null;

            return reply.code(201).send({
              success: true,
              message: 'Match found!',
              gameId,
              gameState: gameEngine.getGameState(gameId)
            });
          } else {
            gameEngine.waitingPlayer = { playerId: joiningPlayerId };
            return reply.code(200).send({
              success: true,
              message: 'Waiting for another player to join...',
            });
          }
        }

        const player1Data = await dbGet(`SELECT nickname, avatar_url FROM users WHERE id = ?`, [joiningPlayerId]);
        const player1Nickname = player1Data?.nickname || `Player ${joiningPlayerId}`;
        const player1Avatar = player1Data?.avatar_url || '/default-avatar.png';
        let gameId: string;

        switch (gameMode) {
          case 'LOCAL_PVP':
            gameId = gameEngine.createGame(joiningPlayerId, 'local player', 'LOCAL_PVP', undefined, player1Nickname, 'Player 2', player1Avatar, '/default-avatar.png');
            break;
          case 'AI':
            gameId = gameEngine.createGame(joiningPlayerId, 'AI', 'AI', aiStrategy, player1Nickname, 'AI', player1Avatar, '/default-avatar.png');
            break;
          case 'TOURNAMENT':
            const { player2Id } = request.body as any;
            if (!player2Id) {
              return reply.code(400).send({ error: 'Player 2 ID is required for tournament games' });
            }
            const player2Data = await dbGet(`SELECT nickname, avatar_url FROM users WHERE id = ?`, [player2Id]);
            const player2Nickname = player2Data?.nickname || `Player ${player2Id}`;
            const player2Avatar = player2Data?.avatar_url || '/default-avatar.png';

            gameId = gameEngine.createGame(joiningPlayerId, player2Id, 'TOURNAMENT', undefined, player1Nickname, player2Nickname, player1Avatar, player2Avatar);
            if (tournamentId && matchId) gameEngine.setTournamentInfo(gameId, tournamentId, matchId);
            break;
          default:
            return reply.code(400).send({ error: 'Invalid game mode' });
        }

        const gameState = gameEngine.getGameState(gameId);
        return reply.code(201).send({ success: true, message: 'Game created', gameId, gameState });

      } catch (error) {
        fastify.log.error({ msg: '❌ Game creation error', error });
        return reply.code(500).send({ error: 'Internal Server Error' });
      }
    }
  );
}