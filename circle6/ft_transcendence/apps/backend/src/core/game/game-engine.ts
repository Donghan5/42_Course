import { WebSocket } from 'ws';
import { GameState, GameDTO } from '@trans/common-types';
import * as gameLogics from './logic';
import { dbRun } from '../../database/helpers';
import { OnlineStatusManager } from '../../core/status/online-status-manager';
import { UserStatsManager } from '../../core/stats/user-stats-manager';
import { gameLogger, logGameEvent, logUserActivity } from '../../utils/logger';
import { ReconnectionManager } from './reconnection-manager';

class Enhanced3DPongGame {
	private games = new Map<string, GameState>();
	private gameLoops = new Map<string, NodeJS.Timeout>();
	private connectedPlayers = new Map<string, Map<string, WebSocket>>();
	private matchmakingSockets = new Map<string, WebSocket>();
	public waitingPlayer: { playerId: string } | null = null;

	private reconnectionManager: ReconnectionManager;
	private pausedGames: Map<string, { pausedAt: number, reason: string }> = new Map();

	private statusManager: OnlineStatusManager;
	private statsManager: UserStatsManager;

	private currentInputStates = new Map<string, Map<string, 'up' | 'down' | 'stop'>>();
	
	private gameLoopRunning = new Map<string, boolean>();

	private tournamentGames = new Map<string, { tournamentId: string; matchId: string }>();

	/***
	 * Constructor for the Enhanced 3D Pong Game Engine
	 */
	constructor() {
		console.log('Enhanced 3D Pong Game Engine Initialized');

		this.statusManager = OnlineStatusManager.getInstance();
		this.statsManager = UserStatsManager.getInstance();
		this.reconnectionManager = ReconnectionManager.getInstance();
	}

	/**
	 * Pause a game (when player disconnects)
	 */
	public pauseGame(gameId: string, playerId: string): void {
		const game = this.games.get(gameId);
		if (!game) return;

		console.log(`⏸️ Pausing game ${gameId} - player ${playerId} disconnected`);
		
		// Store pause state
		this.pausedGames.set(gameId, {
			pausedAt: Date.now(),
			reason: `Player ${playerId} disconnected`
		});

		this.stopGameLoop(gameId);

		// Get opponent's ID
		const opponentId = game.player1Id === playerId ? game.player2Id : game.player1Id;
		
		// Notify opponent about pause
		this.broadcastToGame(gameId, 'gamePaused', {
			message: 'Opponent disconnected. Waiting for reconnection...',
			disconnectedPlayerId: playerId,
			remainingTime: 20
		});
	}

	/**
	 * Resume a game (when player reconnects)
	 */
	public resumeGame(gameId: string): void {
		const game = this.games.get(gameId);
		if (!game) return;

		console.log(`▶️ Resuming game ${gameId}`);
		this.pausedGames.delete(gameId);
		
		// Set to 'resuming' (not 'playing' yet!)
		game.status = 'resuming';

		// Notify players about reconnection with countdown
		this.broadcastToGame(gameId, 'gameResumed', {
			message: 'Player reconnected. Game resuming...',
			countdown: 3
		});

		setTimeout(() => {
			const currentGame = this.games.get(gameId);
			if (currentGame && currentGame.status === 'resuming') {
				currentGame.status = 'playing';
				
				this.startGameLoop(gameId);
				
				console.log(`✅ Game ${gameId} fully resumed - loop restarted`);
			}
		}, 3000);
	}

	/**
	 * Check if game is paused
	 */
	public isGamePaused(gameId: string): boolean {
		return this.pausedGames.has(gameId);
	}

	public setTournamentInfo(gameId: string, tournamentId: string, matchId: string): void {
		this.tournamentGames.set(gameId, { tournamentId, matchId });
		
		const game = this.games.get(gameId);
		if (game) {
			game.isTournamentGame = true;
			console.log(`Game ${gameId} set as tournament game for tournament ${tournamentId}, match ${matchId}`);
		}
	}

	public getTournamentInfo(gameId: string): { tournamentId: string; matchId: string } | undefined {
		return this.tournamentGames.get(gameId);
	}

	/***
	 * Get the current game state by game ID
	 * @param gameId - The ID of the game
	 * @return GameState - The current state of the game
	 */
	public getGameState(gameId: string): GameState | undefined {
		return this.games.get(gameId);
	}

	public getGames(): Map<string, GameState> {
		return this.games;
	}

	public getConnectedPlayers(): Map<string, Map<string, WebSocket>> {
		return this.connectedPlayers;
	}

	/**
	 * Create a new game and initialize
	 * @param player1Id - ID of player 1
	 * @param player2Id - ID of player 2
	 * @return gameId - ID of the created game
	 */
	public createGame(player1Id: string, player2Id: string, gameMode: string, aiStrategy?: 'Defensive' | 'Aggressive' | 'Trickshot',
		player1Nickname?: string, player2Nickname?: string, player1Avatar?: string, player2Avatar?: string
	): string {
		const gameId = `game_${Date.now()}_${Math.random().toString(36).substring(2, 9)}`;
		
		// Log the game
		gameLogger.info(`Creating game ${gameId}`, {
			game_id: gameId,
			game_mode: gameMode,
			player1: { id: player1Id, nickname: player1Nickname },
			player2: { id: player2Id, nickname: player2Nickname },
			ai_strategy: aiStrategy
		});

		// Log game creation event
		logGameEvent(gameId, 'game_create', {
			players: [
				{ id: player1Id, nickname: player1Nickname || 'Player 1' },
				{ id: player2Id, nickname: player2Nickname || 'Player 2' }
			],
			game_mode: gameMode,
			ai_strategy: aiStrategy
		});
		
		const initialState = gameLogics.initState(gameId);

		const gameState: GameState = {
			...initialState,
			player1Id: player1Id,
			player2Id: player2Id,
			player1: { 
				...initialState.player1, 
				position: { x: -12, y: 1.5, z: 0 }, 
				paddleZ: 0,
				nickname: player1Nickname || player1Id,
				avatarUrl: player1Avatar
			},
			player2: { 
				...initialState.player2, 
				position: { x: 12, y: 1.5, z: 0 }, 
				paddleZ: 0,
				nickname: player2Nickname || player2Id,
				avatarUrl: player2Avatar
			},
			ball: {
				position: { x: 0, y: 1, z: 0 },
				velocity: { x: 20.0, y: 0, z: (Math.random() - 0.5) * 40 },
			},
			gameId,
			status: 'waiting',
			lastUpdate: Date.now(),
			gameMode: gameMode,
			aiStrategy: aiStrategy
		};

		this.games.set(gameId, gameState);
		this.connectedPlayers.set(gameId, new Map());
		this.gameLoopRunning.set(gameId, false);
		// this.startGameLoop(gameId);
		return gameId;
	}

	/**
	 * Add a player to the game
	 * @param gameId - The ID of the game
	 * @param playerId - The ID of the player
	 * @param ws - The WebSocket connection for the player
	 */
	public addPlayer(gameId: string, playerId: string, ws: WebSocket): void {
		if (!this.connectedPlayers.has(gameId)) {
			this.connectedPlayers.set(gameId, new Map());
		}

		const players = this.connectedPlayers.get(gameId)!;
		const game = this.games.get(gameId);
		
		if (!game) {
			console.warn(`Game ${gameId} not found for player ${playerId}`);
			ws.close();
			return;
		}
		
		const wasDisconnected = game.status === 'playing' && 
							this.reconnectionManager.isPlayerDisconnected(gameId, playerId);
		
		if (wasDisconnected) {
			console.log(`🔄 Player ${playerId} RECONNECTING to game ${gameId}`);
			
			// Cancel the timeout
			this.reconnectionManager.handleReconnection(gameId, playerId);
			
			// Re-add player
			players.set(playerId, ws);
			
			// Resume the game
			this.resumeGame(gameId);
			
			// Send reconnection confirmation to player
			ws.send(JSON.stringify({
				type: 'reconnected',
				message: 'Successfully reconnected to game'
			}));
			
			// Game state will be sent automatically by the game loop
			return;
		}

		players.set(playerId, ws);
		console.log(`✅ Player ${playerId} added to game ${gameId} (total players: ${players.size})`);

		gameLogger.info(`Player ${playerId} connected to game ${gameId}`);
		logGameEvent(gameId, 'player_connect', {
			player_id: playerId,
			total_players: players.size
		});

		this.statusManager.setUserInGame(parseInt(playerId), gameId);

		// Send current game state to new player
		if (ws.readyState === WebSocket.OPEN) {
			const message = JSON.stringify({
				type: 'gameState',
				payload: game
			});
			ws.send(message);
		}

		const isTournamentOrPvP = game.gameMode === 'TOURNAMENT' || game.gameMode === 'PVP';
		const isAIGame = game.player2Id === 'AI';
		const isLocalGame = game.gameMode === 'LOCAL_PVP';
		
		const actualPlayers = Array.from(players.keys()).filter(pid => 
			pid === game.player1Id || pid === game.player2Id
		);
		
		const isReadyToStart = 
			(isTournamentOrPvP && actualPlayers.length === 2) ||
			(isAIGame && players.size === 1) ||
			(isLocalGame);

		if (isTournamentOrPvP && !isReadyToStart && game.status === 'waiting') {
			const playerSide = playerId === game.player1Id ? 'player1' : 'player2';
			
			if (ws.readyState === WebSocket.OPEN) {
				ws.send(JSON.stringify({
					type: 'waitingForOpponent',
					playerSide: playerSide
				}));
			}
			
			console.log(`Game ${gameId}: Waiting for opponent. ${actualPlayers.length}/2 players connected.`);
		} else if (isReadyToStart && game.status === 'waiting') {
			console.log(`Game ${gameId}: All players connected, starting countdown!`);
			
			this.broadcastToGame(gameId, 'playerJoined', { playerId: playerId });
			
			this.startCountdown(gameId);
		}
	}

	/**
	 * @description Start the countdown for the game
	 * @param gameId - The ID of the game
	 * @returns
	 */
	public startCountdown(gameId: string): void {
        const game = this.games.get(gameId);
        if (!game || game.status !== 'waiting') return;

        game.status = 'countdown';
        let count = 3;
        game.countdownValue = count;

        const countdownInterval = setInterval(() => {
            gameLogics.broadcastGameState(gameId);
            count--;
            game.countdownValue = count;

            if (count < 0) {
                clearInterval(countdownInterval);
                game.status = 'playing';
				game.startTime = Date.now();
                delete game.countdownValue;
                gameLogics.broadcastGameState(gameId);
                this.startGameLoop(gameId);
            }
        }, 1000);
    }

	/**
	 * Update the paddle position of a player
	 * @param gameId - The ID of the game
	 * @param playerId - The ID of the player
	 * @param paddleZ - The new Z position of the paddle
	 */
	public updatePaddlePosition(gameId: string, playerId: string, paddleZ: number): void {
		  console.warn(`Legacy paddle update used for player ${playerId}. Consider migrating to input-based system.`);
		
		const game = this.games.get(gameId);
		if (!game) return;

		const playerKey = this.getPlayerKey(playerId, game);
		if (playerKey) {
			game[playerKey].paddleZ = Math.max(-13, Math.min(13, paddleZ));

			if (playerKey === 'player1') {
				game.player1.position.x = -12;
			}
			if (playerKey === 'player2') {
				game.player2.position.x = 12;
			}
		}
	}

	/**
	 * Get the player key based on player ID
	 * @param playerId - The ID of the player
	 * @param game - The current game state
	 * @return playerKey - The key for the player in the game state
	 */
	private getPlayerKey(playerId: string, game: GameState): 'player1' | 'player2' | null {
		if (game.player1Id === playerId) return 'player1';
		if (game.player2Id === playerId) return 'player2';
		return null; // Default to player1 if not found
	}

	/**
	 * Add a WebSocket connection for a waiting player
	 * @param playerId - The ID of the player
	 * @param ws - The WebSocket connection
	 */
	public addWaitingPlayerSocket(playerId: string, ws: WebSocket): void {
		this.matchmakingSockets.set(playerId, ws);
	}

	/**
	 * Remove a WebSocket connection for a waiting player
	 * @param playerId - The ID of the player
	 */
	public removeWaitingPlayerSocket(playerId: string): void {
		this.matchmakingSockets.delete(playerId);
	}

	/**
	 * Notify a player that a match has been found
	 * @param playerId - The ID of the player
	 * @param gameId - The ID of the game
	 */
	public notifyMatchFound(playerId: string, gameId: string): void {
		const ws = this.matchmakingSockets.get(playerId);
		if (ws && ws.readyState === WebSocket.OPEN) {
			const message = JSON.stringify({ type: 'matchFound', gameId });
			ws.send(message);
			this.matchmakingSockets.delete(playerId); // Remove after notifying
		} else {
			console.warn(`WebSocket for player ${playerId} is not open or does not exist.`);
		}
	}

	public updatePlayerInputState(gameId: string, playerId: string, direction: 'up' | 'down' | 'stop'): void {
		// Log player input for activity tracking
		if (direction !== 'stop') {
			logGameEvent(gameId, 'player_input', {
				player_id: playerId,
				input: direction
			});
		}
		
		if (!this.currentInputStates.has(gameId)) {
			this.currentInputStates.set(gameId, new Map());
		}

		const gameInputs = this.currentInputStates.get(gameId)!;
		gameInputs.set(playerId, direction);

		console.log(`Player ${playerId} input: ${direction}`);
	}

	private stopGameLoop(gameId: string): void {
		const interval = this.gameLoops.get(gameId);

		if (interval) {
			clearTimeout(interval);
			this.gameLoops.delete(gameId);
		}

		this.gameLoopRunning.set(gameId, false);
		this.currentInputStates.delete(gameId);
		console.log(`Game loop stopped for game ${gameId}`);

		// Log game loop stop event
		const game = this.games.get(gameId);
		if (game) {
			gameLogger.info(`Game loop stopped for game ${gameId}`);
			logGameEvent(gameId, 'game_end', {
				final_status: game.status,
				player1_score: game.player1?.score || 0,
				player2_score: game.player2?.score || 0,
				duration: this.calculateGameDuration(game),
				ended_at: new Date().toISOString()
			});
		}
	}

	/**
	 * @description Calculate the duration of the game in milliseconds
	 * @param game 
	 * @returns duration in ms
	 */
	private calculateGameDuration(game: any): number {
		if (typeof game.startTime === 'number') {
			return Date.now() - game.startTime;
		}
		return 0;
	}

	/**
	 * Remove a player from a game
	 * @param gameId - The ID of the game
	 * @param playerId - The ID of the player
	 */
	public removePlayer(gameId: string, playerId: string): void {
		const players = this.connectedPlayers.get(gameId);
		if (!players?.has(playerId)) {
			console.log(`Player ${playerId} not found in game ${gameId}`);
			return;
		}

		const game = this.games.get(gameId);
			
		if (game && (game.status === 'playing' || game.status === 'countdown')) {
			console.log(`Player ${playerId} disconnected from ACTIVE game ${gameId} (status: ${game.status})`);
			
			// Remove player temporarily
			players.delete(playerId);
			
			const gameInputs = this.currentInputStates.get(gameId);
			if (gameInputs?.has(playerId)) {
				gameInputs.delete(playerId);
			}

			// Start reconnection countdown
			this.reconnectionManager.handleDisconnection(
				gameId,
				playerId,
				// On timeout (player didn't reconnect)
				async (gId, pId) => {
					console.log(`💀 Player ${pId} failed to reconnect, forfeiting game ${gId}`);
					
					// Determine winner (the other player)
					const g = this.games.get(gId);
					if (g) {
						const winnerId = g.player1Id === pId ? g.player2Id : g.player1Id;
						await this.endGame(gId, winnerId);
					}
				},
				// On pause
				(gId, pId) => {
					this.pauseGame(gId, pId);
				}
			);
		} else {
			// For non-active games (waiting, finished), remove player normally
			players.delete(playerId);
			
			gameLogger.info(`Player ${playerId} removed from game ${gameId}`);
			logGameEvent(gameId, 'player_disconnect', {
				player_id: playerId,
				remaining_players: this.connectedPlayers.get(gameId)?.size || 0
			});

			const gameInputs = this.currentInputStates.get(gameId);
			if (gameInputs?.has(playerId)) {
				gameInputs.delete(playerId);
			}

			this.statusManager.setUserBackOnline(parseInt(playerId));
		}
	}

	/**
	 * Start the game loop for a specific game
	 * @param gameId - The ID of the game
	 */
	public startGameLoop(gameId: string): void {
		if (this.gameLoopRunning.get(gameId)) {
			console.log(`Game loop for ${gameId} is already running`);
			return;
		}

		const game = this.games.get(gameId);
		if (!game) {
			gameLogger.error(`Game ${gameId} not found, cannot start loop`);
			console.log(`Game ${gameId} not found, cannot start loop`);
			return;
		}

		this.gameLoopRunning.set(gameId, true);
		console.log(`Starting game loop for game ${gameId}`);

		gameLogger.info(`Starting game loop for game ${gameId}`);
		logGameEvent(gameId, 'game_start', {
			status: 'game_loop_started',
			start_time: new Date().toISOString()
		});

		const targetFPS = 60;
		const frameTime = 1000 / targetFPS;
		let lastFrameTime = Date.now();

		let updateCounter = 0;
		const gameLoop = () => {
			if (!this.gameLoopRunning.get(gameId)) return;

			const currentGame = this.games.get(gameId);
			if (!currentGame || currentGame.status === 'finished') {
				this.stopGameLoop(gameId);
				return;
			}

			updateCounter++;

			if (updateCounter % 100 === 0) {
				logGameEvent(gameId, 'performance_metric', {
					update_count: updateCounter,
					active_players: this.connectedPlayers.get(gameId)?.size || 0,
					game_status: currentGame.status,
					timestamp: new Date().toISOString()
				});
			}

			// if (currentGame.status === 'resuming') {
			// 	gameLogics.broadcastGameState(gameId);
			// 	return;
			// }

			if (currentGame.status === 'playing') {
				const currentTime = Date.now();
				const deltaTime = (currentTime - lastFrameTime) / 1000;
				lastFrameTime = currentTime;

				const gameInputs = this.currentInputStates.get(gameId);
				if (gameInputs) {
					gameInputs.forEach((direction, playerId) => {
						gameLogics.processPlayerInput(currentGame, playerId, direction);
					});
				}

				gameLogics.updatePhysics(currentGame, deltaTime);
			}
			
			gameLogics.broadcastGameState(gameId);
		};

		const scheduleNextFrame = () => {
			if (!this.gameLoopRunning.get(gameId)) {
				return;
			}

			const timeToNextFrame = Math.max(0, frameTime - (Date.now() - lastFrameTime));
			
			const timeoutId = setTimeout(() => {
				gameLoop();
				scheduleNextFrame(); 
			}, timeToNextFrame);

			this.gameLoops.set(gameId, timeoutId);
		};

		scheduleNextFrame();
	}

	/**
	 * @description Broadcast a message to all players in a game
	 * @param gameId 
	 * @param type 
	 * @param payload 
	 * @returns 
	 */
	public broadcastToGame(gameId: string, type: string, payload: any): void {
		const players = this.connectedPlayers.get(gameId);
		if (!players) {
			console.warn(`No players found for game ${gameId}`);
			return;
		}

		const message = JSON.stringify({
			type,
			...payload
		});

		players.forEach((ws, playerId) => {
			if (ws.readyState === 1) { // WebSocket.OPEN
				ws.send(message);
			} else {
				console.warn(`WebSocket for player ${playerId} is not open`);
			}
		});
	}

	public async endGame(gameId: string, predeterminedWinner?: string): Promise<void> {
		this.reconnectionManager.cleanupGame(gameId);
		this.pausedGames.delete(gameId);
		console.log(`Ending game ${gameId}`, predeterminedWinner ? `with predetermined winner: ${predeterminedWinner}` : 'determining winner by score');
		
		this.stopGameLoop(gameId);
		
		await new Promise(resolve => setTimeout(resolve, 100));
		
		const game = this.games.get(gameId);
		const players = this.connectedPlayers.get(gameId);
		
		if (game) {
			game.status = 'finished';
			
			// Use predetermined winner if provided (forfeit case), otherwise use score
			const winnerId = predeterminedWinner || 
							(game.player1.score > game.player2.score ? game.player1Id : game.player2Id);
			
			const winner = winnerId === game.player1Id ? game.player1 : game.player2;
			
			console.log(`Game ${gameId} winner determined: ${winner.nickname} (${winnerId})`);
			
			// Update player stats and set them back online
			await this.updatePlayersStats(game, winnerId);
			await this.setPlayersBackOnline(game);
			
			// HANDLE TOURNAMENT GAMES
			const tournamentInfo = this.getTournamentInfo(gameId);
			if (tournamentInfo) {
				console.log(`Tournament game ${gameId} ended, notifying tournament system. Winner: ${winnerId}`);
				try {
					const response = await fetch('http://localhost:3000/api/tournament/finish-match', {
						method: 'POST',
						headers: {
							'Content-Type': 'application/json',
						},
						body: JSON.stringify({
							tournamentId: tournamentInfo.tournamentId,
							matchId: tournamentInfo.matchId,
							winnerId: winnerId
						})
					});
					
					if (response.ok) {
						console.log('Tournament match finished successfully');
						const result = await response.json();
						console.log('Tournament API response:', result);
					} else {
						const errorText = await response.text();
						console.error('Failed to finish tournament match:', response.status, errorText);
					}
				} catch (error) {
					console.error('Error finishing tournament match:', error);
				}
				
				this.tournamentGames.delete(gameId);
			}
			
			// Save to database...
			try {
				const isP1User = game.player1Id !== 'AI' && game.player1Id !== 'Player2';
				const isP2User = game.player2Id !== 'AI' && game.player2Id !== 'Player2';
				
				if (isP1User || isP2User || game.gameMode === 'LOCAL_PVP') {
					await dbRun(
						`INSERT INTO games (game_id, player1_id, player2_id, player1_score, player2_score, winner_id, game_type, finished_at)
						VALUES (?, ?, ?, ?, ?, ?, ?, ?)`,
						[
							game.gameId,
							isP1User ? game.player1Id : null,
							isP2User ? game.player2Id : null,
							game.player1.score,
							game.player2.score,
							(isP1User || isP2User) ? winnerId : null,
							game.gameMode,
							new Date().toISOString()
						]
					);
					console.log(`[DB] Game ${gameId} result saved`);
				}
			} catch (error) {
				console.error(`Error saving game result for ${gameId}:`, error);
			}
			
			// IMPORTANT: Only broadcast gameEnd ONCE, after everything else is done
			if (players) {
				const message = JSON.stringify({
					type: 'gameEnd',
					payload: {
						winnerId: winnerId,
						winnerNickname: winner.nickname,
						finalScore: {
							player1: game.player1.score,
							player2: game.player2.score
						}
					}
				});
				
				console.log(`Broadcasting gameEnd message to ${players.size} players`);
				players.forEach((ws, playerId) => {
					if (ws.readyState === WebSocket.OPEN) {
						console.log(`Sending gameEnd to player ${playerId}`);
						ws.send(message);
					}
				});
			}
		}
		
		console.log(`Game ${gameId} ended successfully`);
		
		// Clean up game data after 30 seconds
		setTimeout(() => {
			this.games.delete(gameId);
			this.connectedPlayers.delete(gameId);
			this.gameLoopRunning.delete(gameId);
			this.currentInputStates.delete(gameId);
			console.log(`Game ${gameId} data cleaned up`);
		}, 30000);
	}

	private async updatePlayersStats(game: GameState, winnerId: string): Promise<void> {
        try {
            const player1Id = parseInt(game.player1Id);
            const player2Id = parseInt(game.player2Id);

            if (game.player1Id !== 'AI' && !isNaN(player1Id)) {
                const isWinner = winnerId === game.player1Id;
                const pointsChange = this.calculateRankPoints(isWinner, game.player1.score, game.player2.score);
                await this.statsManager.updateRankPoints(player1Id, pointsChange);
            }

            if (game.player2Id !== 'AI' && !isNaN(player2Id)) {
                const isWinner = winnerId === game.player2Id;
                const pointsChange = this.calculateRankPoints(isWinner, game.player2.score, game.player1.score);
                await this.statsManager.updateRankPoints(player2Id, pointsChange);
            }
        } catch (error) {
            console.error('Error updating player stats:', error);
        }
    }

    /**
     * Sets players back online after a game ends
     */
    private async setPlayersBackOnline(game: GameState): Promise<void> {
        try {
            if (game.player1Id !== 'AI') {
                const player1Id = parseInt(game.player1Id);
                if (!isNaN(player1Id)) {
                    await this.statusManager.setUserBackOnline(player1Id);
                }
            }

            if (game.player2Id !== 'AI') {
                const player2Id = parseInt(game.player2Id);
                if (!isNaN(player2Id)) {
                    await this.statusManager.setUserBackOnline(player2Id);
                }
            }
        } catch (error) {
            console.error('Error setting players back online:', error);
        }
    }

    /**
     * Rank points calculation based on game outcome
     */
    private calculateRankPoints(isWinner: boolean, playerScore: number, opponentScore: number): number {
        const basePoints = 25;
        const scoreDifference = playerScore - opponentScore;
        
        if (isWinner) {
            return Math.max(15, basePoints + Math.floor(scoreDifference / 2));
        } else {
            return Math.min(-15, -basePoints + Math.floor(scoreDifference / 2));
        }
    }

}

// Sigleton instance of the game engine
export const gameEngine = new Enhanced3DPongGame();
