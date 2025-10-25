import { initializeGame } from '../game/init'
import { appState } from '../state/state';
import { returnToMainMenu } from '../utils/tools';
import { showSection, showNotification } from '../services/ui';
import { updateConnectionStatus } from '../game/connection';

let isPageUnloading = false;

if (typeof window !== 'undefined') {
    window.addEventListener('beforeunload', () => {
        isPageUnloading = true;
    });
}

/**
 *
 * @param gameMode: string - The game mode to create (e.g., 'quick', 'ai', 'tournament')
 * @param playerName: string - The name of the player starting the game
 * @description Creates a new game by sending a request to the server and initializes the game
 */
export async function createNewGame(gameMode: string) {
    try {
        console.log('Requesting new game from server...')

        // startGame(dummyResponse.gameId)
        const player2Id = gameMode === 'ai' ? 'AI' : 'Player2_tmp';
        let requestBody: any;

        // switching based on game mode
        switch (gameMode) {
            case 'quick':
                requestBody = {
                    player1Id: appState.currentUser.id,
                    player1Nickname: appState.currentUser.nickname,
                    player2Id: 'Player2',
                    player2Nickname: 'Player2',
                    gameMode: 'LOCAL_PVP'
                };
                break;
            case 'ai':
                const selectedAiStrategy = await selectingAiStrategy(); // temp set

                if (!selectedAiStrategy) {
                    console.log('AI strategy selection cancelled');
                    return;
                }

                requestBody = {
                    player1Id: appState.currentUser.id,
                    player1Nickname: appState.currentUser.nickname,
                    player2Id: 'AI',
                    gameMode: 'AI',
                    aiStrategy: selectedAiStrategy
                };
                break;
            case 'pvp':
                requestBody = {
                    player1Id: appState.currentUser.id,
                    gameMode: 'PVP',
                };

                connectingMatchmaking();
                break;
            case 'tournament':
                showSection('tournament');
                return;
            default:
                throw new Error('Invalid game mode selected');
        }

        const response = await fetch('/api/games', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(requestBody),
        })

        if (!response.ok) {
            throw new Error('Failed to create game')
        }

        const data = await response.json()

        if (data.message && data.message.includes('Waiting')) {
            showSection('waiting');
            // add cancel waiting
        } else if (data.gameId) {
            appState.currentGameId = data.gameId
            console.log('Game created successfully:', data)
            showGameScreen()
            startGame(data.gameId, String(appState.currentUser.id), gameMode)
        } else {
            throw new Error('Unexpected response format')
        }
    } catch (error) {
        console.error('Failed to create game:', error)
        throw error
    }
}

/**
 * Store active game info for reconnection
 */
export function storeActiveGame(gameId: string, playerId: string, gameMode: string) {
    console.log('💾 storeActiveGame called with:', { gameId, playerId, gameMode });
    
    const gameInfo = {
        gameId,
        playerId,
        gameMode,
        timestamp: Date.now()
    };
    
    console.log('💾 Storing to localStorage:', gameInfo);
    localStorage.setItem('activeGame', JSON.stringify(gameInfo));
    
    // Verify it was stored
    const stored = localStorage.getItem('activeGame');
    console.log('💾 Verification - localStorage now contains:', stored);
}

/**
 * Clear stored game info
 */
export function clearActiveGame() {
    localStorage.removeItem('activeGame');
    console.log('🗑️ Cleared active game storage');
}

/**
 * Get stored game info
 */
export function getActiveGame(): { gameId: string, playerId: string, gameMode: string, timestamp: number } | null {
    try {
        const stored = localStorage.getItem('activeGame');
        if (!stored) return null;
        
        const gameInfo = JSON.parse(stored);
        
        // Check if game info is too old (> 5 minutes = stale)
        const age = Date.now() - gameInfo.timestamp;
        if (age > 5 * 60 * 1000) {
            clearActiveGame();
            return null;
        }
        
        return gameInfo;
    } catch (error) {
        console.error('Error reading active game:', error);
        return null;
    }
}

export function showGameScreen() {
    showSection('game');
    console.log('Game screen is shown');
}

/**
 * @description Enhanced startGame function to properly set tournament context
 */
export async function startGame(
    gameId: string,
    playerId: string,
    gameMode: string, 
    tournamentId?: string,
    player1Avatar?: string,
    player2Avatar?: string,
    player1Nickname?: string,
    player2Nickname?: string
) {
    console.log(`Starting game: ${gameId}, Player: ${playerId}, Mode: ${gameMode}`);
    
    appState.currentGameId = gameId;
    appState.currentGameMode = gameMode;
    
    console.log('🎮 startGame called with:', { gameId, playerId, gameMode });
    
    console.log('💾 About to store active game...');
    storeActiveGame(gameId, playerId, gameMode);
    console.log('💾 Stored active game');
    
    if (tournamentId) {
        appState.currentTournamentId = tournamentId;
        console.log(`Game is part of tournament: ${tournamentId}`);
    }
    
    showGameScreen();
    
    // Fetch fresh user data to get latest avatar
    try {
        const response = await fetch('/api/auth/me', { credentials: 'include' });
        if (response.ok) {
            const freshUserData = await response.json();
            appState.currentUser = freshUserData; // Update currentUser with fresh data
        }
    } catch (error) {
        console.error('Failed to refresh user data:', error);
    }
    
    // Get current user's nickname and avatar (now with fresh data)
    const currentNickname = appState.currentUser?.nickname || appState.currentUser?.name || 'Player';
    const currentAvatar = appState.currentUser?.avatarUrl || appState.currentUser?.avatar_url || '/default-avatar.png';
    
    // Use provided avatars/nicknames or defaults
    const p1Avatar = player1Avatar || currentAvatar;
    const p2Avatar = player2Avatar || '/default-avatar.png';
    const p1Nick = player1Nickname || currentNickname;
    const p2Nick = player2Nickname || 'Player 2';
    
    // Initialize game with avatar and nickname support
    appState.currentGame = initializeGame(
        'gameSection',
        gameId,
        playerId,
        gameMode,
        currentNickname,
        p1Avatar,
        p2Avatar,
        p1Nick,
        p2Nick
    );
    
    if (appState.currentGame) {
        console.log('Game initialized successfully');
    } else {
        console.error('Failed to initialize game');
    }
}

/**
 * @param gameMode - The game mode to start
 * @description Handles the game start logic based on the selected game mode
 * @throws Error if the user is not logged in or game mode is invalid
 * @returns
 */
export async function handleGameStart(gameMode: string) {
    if (!appState.currentUser || !appState.currentUser.name) {
        alert('Please login to play.');
        return;
    }

    console.log(`Player ${appState.currentUser.name} is starting a ${gameMode} game.`);

    try {
        if (gameMode === 'tournament') {
            showSection('tournament');
            return;
        }
        await createNewGame(gameMode);
    } catch (error) {
        console.error('Failed to create game:', error);
        alert('Failed to create game. Please try again later.');
    }
}

/**
 * @description selecting AI strategy for the game, came from UI
 * @returns {Promise<string | null>} - The selected AI level or null if cancelled
 * @throws Error if the AI level selection fails
 */
export async function selectingAiStrategy(): Promise<string | null> {
	const aiStrategy = await showAiStrategySelectionUI();
	if (!aiStrategy) {
		throw new Error('AI strategy selection failed');
	}

	return aiStrategy;
}

/**
 * @description Showing a select UI
 * @returns {Promise<string | null>} - The selected AI strategy or null if cancelled
 */
export function showAiStrategySelectionUI(): Promise<string | null> {
    return new Promise((resolve) => {
        const modal = document.getElementById('aiStrategyModal');
        const cancelBtn = document.getElementById('cancelAiSelect');

        if (!modal || !cancelBtn) {
            console.error('AI Strategy Modal not found');
            resolve(null);
            return;
        }
        modal.classList.remove('hidden');

        const handleClick = (event: MouseEvent) => {
            const target = event.target as HTMLElement;

            if (target.matches('.ai-strategy-btn')) {
                cleanup();
                resolve(target.getAttribute('data-strategy'));
            } else if (target === cancelBtn) {
                cleanup();
            }
        }

        const cleanup = () => {
            modal.classList.add('hidden');
            modal.removeEventListener('click', handleClick);
        };

        modal.addEventListener('click', handleClick);
    });
}

export function triggerGameEnd(gameResult?: any) {
    document.dispatchEvent(new CustomEvent('gameEnded', { 
        detail: { 
            gameResult,
            gameMode: appState.currentGameMode,
            timestamp: Date.now()
        } 
    }));
}

export function handleGameStateUpdate(gameState: any) {
    // IMPORTANT: Don't process game state updates if game has ended
    // The gameEnd WebSocket event handles the end game flow
    if (gameState.status === 'finished') {
        console.log('Game finished - ignoring state update (gameEnd event will handle this)');
        return;
    }
    
    // Show game over modal if needed (this should not be reached for 'finished' status)
    const gameOverModal = document.getElementById('gameOverModal');
    if (gameOverModal) {
        const winnerMessage = document.getElementById('winnerMessage');
        const gameOverTitle = document.getElementById('gameOverTitle');
        
        if (winnerMessage && gameOverTitle) {
            const winnerId = gameState.winnerId;
            const winnerNickname = gameState.winnerNickname || 'Unknown';

            if (winnerId === appState.currentUser?.id?.toString()) {
                gameOverTitle.textContent = 'VICTORY!';
                winnerMessage.textContent = 'You won the match!';
            } else {
                gameOverTitle.textContent = 'DEFEAT!';
                winnerMessage.textContent = `${winnerNickname} won the match!`;
            }
            
            gameOverModal.classList.remove('hidden');
        }
    }
}

export async function forfeitCurrentGame() {
    if (!appState.currentGame || !appState.currentUser) {
        console.log('No active game or user to forfeit');
        return;
    }

    console.log('Player forfeiting game...');

    try {
        const response = await fetch(`/api/games/forfeit`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                gameId: appState.currentGame.state?.gameId || appState.currentGameId,
                playerId: appState.currentUser.id
            }),
            credentials: 'include'
        });

        if (response.ok) {
            console.log('Game forfeited successfully');
            showNotification('You forfeited the game', 'info');
            
            // The backend will send gameEnd WebSocket message to all players
            // The render.ts onGameEnd() will show the modal
            // User can then click "Return" button to leave
            
        } else {
            console.error('Failed to forfeit game:', response.status);
            showNotification('Failed to forfeit game', 'error');
            
            // If forfeit API failed, still clean up and return
            cleanupCurrentGame();
            returnToMainMenu();
        }
    } catch (error) {
        console.error('Error forfeiting game:', error);
        showNotification('Error forfeiting game', 'error');
        
        // If forfeit API failed, still clean up and return
        cleanupCurrentGame();
        returnToMainMenu();
    }
    
    // Let the gameEnd WebSocket event flow handle it naturally:
    // 1. Backend sends gameEnd event
    // 2. render.ts onGameEnd() shows modal
    // 3. User clicks "Return" button
    // 4. Button handler calls returnToMainMenu()
    // This ensures forfeiter sees the game end screen just like the winner
}

export function connectingMatchmaking() {
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.host}/api/games/matchmaking/ws?playerId=${appState.currentUser.id}`;

    appState.matchmakingWs = new WebSocket(wsUrl);

    appState.matchmakingWs.onopen = () => {
        console.log('Connected to matchmaking server');
    }

    appState.matchmakingWs.onmessage = (event) => {
        const data = JSON.parse(event.data);
        if (data.type === 'matchFound') {
            console.log('Match found! Game ID: ', data.gameId);
            appState.currentGameId = data.gameId;

            if (appState.matchmakingWs) {
                appState.matchmakingWs.close();
                appState.matchmakingWs = null;
            }

            showGameScreen();
            startGame(data.gameId, String(appState.currentUser.id), 'pvp');
        }
    };

    appState.matchmakingWs.onerror = (error) => {
        console.error('Matchmaking WebSocket error:', error);
        alert('Matchmaking connection failed');
        returnToMainMenu();
    }

    appState.matchmakingWs.onclose = () => {
        console.log('Matchmaking WebSocket connection closed');
    }
}

export async function cancelMatchmaking() {
	console.log('Cancelling matchmaking...');

	if (!appState.currentUser || !appState.currentUser.id) {
		console.error('Current user is not logged in. Cannot cancel matchmaking.');
		returnToMainMenu();
		return;
	}

	try {
		const response = await fetch('/api/games/cancel', {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
			},
			body: JSON.stringify({ playerId: appState.currentUser.id }),
			credentials: 'include'
		});

		if (!response.ok) {
			throw new Error('Failed to cancel matchmaking');
		}

		const data = await response.json();
		console.log('Matchmaking cancelled:', data);
	} catch (error) {
		console.error('Error cancelling matchmaking:', error);
		alert('Failed to cancel matchmaking. Please try again later.');
	} finally {
		if (appState.matchmakingWs) {
			try {
				appState.matchmakingWs.close();
				console.log('Matchmaking WebSocket closed');
			} catch (error) {
				console.error('Error closing matchmaking WebSocket:', error);
			} finally {
				appState.matchmakingWs = null;
			}
		}
		returnToMainMenu();
	}
}

export function cleanupCurrentGame() {
    console.log('🧹 cleanupCurrentGame called, isPageUnloading:', isPageUnloading);
    
    if (appState.currentGame) {
        console.log('Disposing current game...');
        try {
            appState.currentGame.dispose();
            console.log('Current game disposed successfully');
        } catch (error) {
            console.error('Error disposing current game:', error);
        } finally {
            appState.currentGame = null;
            console.log('Current game reference cleared');
        }
    }
    
    // ✅ ONLY clear localStorage if NOT during page unload
    if (!isPageUnloading) {
        console.log('🗑️ Clearing localStorage (normal cleanup)');
        clearActiveGame();
    } else {
        console.log('⚠️ Skipping localStorage clear (page unloading - might reconnect)');
    }
}

/**
 * @description Check if a game is currently active/running
 * @param gameId - Game ID to check
 * @returns boolean - true if game is active, false otherwise
 */
export function isGameCurrentlyActive(gameId: string): boolean {
    // Check if we have an active game with this ID
    return appState.currentGame !== null && 
           appState.currentGameId === gameId && 
           appState.currentSection === 'game';
}