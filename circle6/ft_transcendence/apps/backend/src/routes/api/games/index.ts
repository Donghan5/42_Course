// Import the Fastify instance and core game engine
import { FastifyInstance, FastifyRequest } from 'fastify';
import { WebSocket } from 'ws';
import { gameEngine } from '../../../core/game/game-engine';
import { OnlineStatusManager } from '../../../core/status/online-status-manager';

import createGameRoutes from './create';
import gameStatusRoutes from './status';
import cancelRoutes from './cancel';
import matchmakingRoutes from './matchmaking';

export default async function gameRoutes(fastify: FastifyInstance) {
	const statusManager = OnlineStatusManager.getInstance();

	fastify.register(createGameRoutes);
	fastify.register(cancelRoutes);
	fastify.register(gameStatusRoutes);
	fastify.register(matchmakingRoutes);

	fastify.post('/forfeit', async (request, reply) => {
		try {
			console.log('Forfeit request received:', request.body);
			const { gameId, playerId } = request.body as { gameId: string; playerId: string };
			
			if (!gameId || !playerId) {
				return reply.code(400).send({ 
					error: 'Game ID and Player ID are required',
					received: { gameId, playerId }
				});
			}

			const game = gameEngine.getGameState(gameId);
			if (!game) {
				return reply.code(404).send({ error: 'Game not found' });
			}

			// console.log('=== FORFEIT DEBUG ===');
			// console.log('Forfeiting player:', playerId);
			// console.log('Player1:', game.player1Id, game.player1.nickname);
			// console.log('Player2:', game.player2Id, game.player2.nickname);

			// Determine winner (the OTHER player who didn't forfeit)
			let winnerId: string;
			
			if (game.player1Id === playerId || game.player1Id === playerId.toString()) {
				winnerId = game.player2Id;  // Player2 wins because Player1 forfeited
				console.log(`${game.player1.nickname} FORFEITED → ${game.player2.nickname} WINS`);
			} else if (game.player2Id === playerId || game.player2Id === playerId.toString()) {
				winnerId = game.player1Id;  // Player1 wins because Player2 forfeited  
				console.log(`${game.player2.nickname} FORFEITED → ${game.player1.nickname} WINS`);
			} else {
				return reply.code(400).send({ error: 'Player not in this game' });
			}

			console.log('Winner determined:', winnerId);
			console.log('=====================');

			// End the game with the predetermined winner
			await gameEngine.endGame(gameId, winnerId);  // ← PASS THE WINNER!

			return reply.send({
				success: true,
				message: 'Game forfeited successfully',
				winnerId: winnerId
			});

		} catch (error) {
			console.error('Forfeit error:', error);
			return reply.code(500).send({
				error: 'Failed to forfeit game',
				message: error instanceof Error ? error.message : 'Unknown error'
			});
		}
	});
	
	fastify.get('/ws/:gameId', { websocket: true }, (connection, req: FastifyRequest) => {
		const { gameId } = req.params as { gameId: string };
		const playerId = (req.query as Record<string, string>).playerId ?? `player_${Date.now()}`;

		fastify.log.info(`WebSocket connection established for game ${gameId} with player ${playerId}`);

		statusManager.setUserInGame(parseInt(playerId), gameId);

		gameEngine.addPlayer(gameId, playerId, connection as unknown as WebSocket);

		connection.on('message', (message: Buffer) => {
			try {
				const data = JSON.parse(message.toString());
				
				if (data.type === 'playerInput') {
					const { inputState, playerId: inputPlayerId } = data; // 'up' | 'down' | 'stop'
					
					if (!['up', 'down', 'stop'].includes(inputState)) {
						fastify.log.warn(`❌ Invalid input state: ${inputState} from player ${playerId}`);
						return;
					}
					
					const targetPlayerId = inputPlayerId || playerId;
					gameEngine.updatePlayerInputState(gameId, targetPlayerId, inputState);
					
				} else if (data.type === 'gameAction' && data.action === 'updatePaddle') {
					const { paddleZ } = data.data;
					fastify.log.warn(`⚠️ Legacy paddle update used by player ${playerId}`);
					gameEngine.updatePaddlePosition(gameId, playerId, paddleZ);
					
				} else if (data.type === 'ping') {
					if (connection.readyState === 1) {
						connection.send(JSON.stringify({ type: 'pong', timestamp: Date.now() }));
					}
				}
			} catch (error) {
				fastify.log.error(`❌ Error processing message from player ${playerId}: ${error}`);
			}
		});

		connection.on('close', () => {
			fastify.log.info(`WebSocket connection closed for player ${playerId}`);
			statusManager.setUserBackOnline(parseInt(playerId));
			gameEngine.removePlayer(gameId, playerId);
		});

		connection.on('error', (error: Error) => {
			fastify.log.error(`WebSocket error for player ${playerId}: ${error.message}`);
			statusManager.setUserBackOnline(parseInt(playerId));
			gameEngine.removePlayer(gameId, playerId);
		});
	});

}


