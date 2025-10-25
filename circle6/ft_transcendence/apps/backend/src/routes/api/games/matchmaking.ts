import { FastifyInstance } from 'fastify';
import { WebSocket } from 'ws';
import { gameEngine } from '../../../core/game/game-engine';

export default async function matchmakingRoutes(fastify: FastifyInstance) {
	fastify.get('/matchmaking/ws', { websocket: true }, (connection, req) => {
		const playerId = (req.query as Record<string, string>).playerId;

		if (!playerId) {
			connection.close();
			return;
		}

		fastify.log.info(`Player ${playerId} connected to matchmaking`);

		gameEngine.addWaitingPlayerSocket(playerId, connection as unknown as WebSocket);

		connection.on('close', () => {
			fastify.log.info(`Player ${playerId} disconnected from matchmaking`);
			gameEngine.removeWaitingPlayerSocket(playerId);

			if (gameEngine.waitingPlayer?.playerId === playerId) {
				fastify.log.info(`Removing waiting player ${playerId} from matchmaking`);
				gameEngine.waitingPlayer = null;
			}
		});

		connection.on('error', (error) => {
			fastify.log.error(`WebSocket error for player ${playerId}: ${error}`);
			gameEngine.removeWaitingPlayerSocket(playerId);
		});
	});
}
