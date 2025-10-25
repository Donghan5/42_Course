import { appState } from './src/state/state';
import { showSection, setupPongLogoRedirect, showFriendsScreen, showNicknameSetupScreen, showAppScreen, showNotification } from './src/services/ui';
import { handleGameStart, cancelMatchmaking, cleanupCurrentGame, forfeitCurrentGame, showGameScreen, startGame, triggerGameEnd, getActiveGame, clearActiveGame } from './src/game/game-manager';
import { returnToMainMenu, toggleFullscreen } from './src/utils/tools';
import { showProfileScreen } from './src/services/user';
import { setupLocalAuthHandlers, updateLoginStatus } from './src/services/auth';
import { cleanupTournamentUI } from './src/tournament/tournament-services';
import { StatusManager } from './src/status/status-manager';
import { navigateBack } from './src/utils/navigation';
// CHAT - Import du widget
import { ChatWidget } from './components/chat-widget';

// CHAT - Variable globale
let chatWidget: ChatWidget | null = null;

// CHAT - Exposer handleGameStart globalement pour le chat widget
(window as any).handleGameStart = handleGameStart;

// CHAT - Initialiser le chat
function initChatWidget() {
	if (chatWidget) {
		chatWidget.destroy();
	}
	chatWidget = new ChatWidget();
	(window as any).chatWidget = chatWidget;
	console.log('Chat widget initialized');
}

// CHAT - Détruire le chat
function destroyChatWidget() {
	if (chatWidget) {
		chatWidget.destroy();
		chatWidget = null;
		(window as any).chatWidget = null;
		console.log('Chat widget destroyed');
	}
}

// --- MAIN SCRIPT LOGIC ---

document.addEventListener('DOMContentLoaded', () => {
	const path = window.location.pathname;
	let initialSection: any = 'hero';

	if (path === '/game') initialSection = 'game';
	else if (path === '/profile') initialSection = 'profile';
	else if (path === '/login') initialSection = 'login';
	else if (path === '/nickname-setup') initialSection = 'nicknameSetup';
	else if (path === '/friends') initialSection = 'friends';
	else if (path === '/public-profile') initialSection = 'publicProfile';
	else if (path === '/waiting') initialSection = 'waiting';
	else if (path === '/tournament-lobby') initialSection = 'tournamentLobby';
	const state = { sectionId: initialSection };
	history.replaceState(state, document.title, path);
	appState.currentSection = initialSection;

	// checking to hash
	window.addEventListener('hashchange', (e) => {
		console.log('Hash changed:', {
			oldURL: e.oldURL,
			newURL: e.newURL,
			hash: window.location.hash
		});
		console.trace(); // check
	});

	setupLocalAuthHandlers(); // Single call to set up all auth UI and logic
	updateLoginStatus();

	setTimeout(() => {
        checkAndRejoinGame();
    }, 1000);

	console.log('Start beautiful PONG game!');
	setupEventListeners();
	
	// ✅ CHAT - Initialiser si déjà connecté
	if (appState.currentUser && appState.currentUser.id) {
		initChatWidget();
	}
})

// ✅ CHAT - Écouter la connexion
document.addEventListener('userLoggedIn', () => {
	console.log('🔐 User logged in - initializing chat');
	initChatWidget();

	setTimeout(() => {
        checkAndRejoinGame();
    }, 1000);
});

// ✅ CHAT - Écouter la déconnexion
document.addEventListener('userLoggedOut', () => {
	console.log('🚪 User logged out - destroying chat');
	destroyChatWidget();
});

document.addEventListener('requestReturnToMainMenu', () => {
	returnToMainMenu();
});

function setupEventListeners() {
	const quickPlayButton = document.getElementById('quickPlayBtn');
	if (quickPlayButton) {
		quickPlayButton.addEventListener('click', () => {
			console.log("Quickly play!");
			handleGameStart('quick');
		});
	}

	const pvpPlayButton = document.getElementById('pvpPlayBtn');
	if (pvpPlayButton) {
		pvpPlayButton.addEventListener('click', () => {
			console.log("PVP play!");
			handleGameStart('pvp');
		});
	}

	const aiPlayButton = document.getElementById('aiPlayBtn');
	if (aiPlayButton) {
		aiPlayButton.addEventListener('click', () => {
			console.log("AI play!");
			handleGameStart('ai');
		});
	}

	const tournamentPlayButton = document.getElementById('tournamentPlayBtn');
	if (tournamentPlayButton) {
		tournamentPlayButton.addEventListener('click', () => {
			console.log("Tournament play!");
			handleGameStart('tournament');
		});
	}

	const logoutButton = document.getElementById('logoutBtn');
	if (logoutButton) {
		logoutButton.addEventListener('click', () => {
			window.location.href = '/api/auth/logout';
			console.log('Logout action needed');
		});
	}

	const cancelMatchmakingButton = document.getElementById('cancelMatchmakingBtn');
	if (cancelMatchmakingButton) {
		cancelMatchmakingButton.addEventListener('click', cancelMatchmaking);
	}

	const profileWidgetBtn = document.getElementById('profileWidgetBtn');
	const profileDropdown = document.getElementById('profileDropdown');
	profileWidgetBtn?.addEventListener('click', (event) => {
		// Stop the click from immediately being caught by our new document listener
		event.stopPropagation();
		profileDropdown?.classList.toggle('hidden');
	});

	document.addEventListener('click', (event) => {
		if (profileDropdown?.classList.contains('hidden')) {
			return;
		}

		const isClickInsideWidget = profileWidgetBtn?.contains(event.target as Node);
		const isClickInsideDropdown = profileDropdown?.contains(event.target as Node);

		if (!isClickInsideWidget && !isClickInsideDropdown) {
			profileDropdown?.classList.add('hidden');
		}
	});

	document.getElementById('dropdownProfileBtn')?.addEventListener('click', () => {
		showProfileScreen();
		profileDropdown?.classList.add('hidden');
	});

	document.getElementById('dropdownFriendsBtn')?.addEventListener('click', async () => {
		profileDropdown?.classList.add('hidden');
		await showFriendsScreen();
	});

	document.getElementById('dropdownLogoutBtn')?.addEventListener('click', () => {
		profileDropdown?.classList.add('hidden');
		window.location.href = '/api/auth/logout';
	});

    document.getElementById('profileReturnBtn')?.addEventListener('click', () => {
        console.log('Profile back button clicked');
        navigateBack();
    });

    document.getElementById('friendsReturnBtn')?.addEventListener('click', () => {
        console.log('Friends back button clicked');
        navigateBack();
    });

    document.getElementById('tournament-return-btn')?.addEventListener('click', () => {
        console.log('Tournament back button clicked');
        navigateBack();
    });

    document.getElementById('publicProfileReturnBtn')?.addEventListener('click', () => {
        console.log('Public profile back button clicked');
        navigateBack();
    });

	window.addEventListener('navigate', (event: any) => {
		const detail = event.detail;
		console.log('Custom navigate event:', detail);

		if (typeof detail === 'string') {
			showSection(detail as any);
		} else if (detail && typeof detail === 'object') {
			if (detail.sectionId === 'game' && detail.gameId) {
				console.log(`Navigating to game: ${detail.gameId} (mode: ${detail.mode || 'player'})`);
				showGameScreen();

				// Determine game mode based on context
				let gameMode = 'tournament';
				if (detail.mode === 'spectator') {
					gameMode = 'spectator';
                    showNotification('Spectator mode - you can watch but not play', 'info');
				}

				startGame(detail.gameId, String(appState.currentUser.id), gameMode);
			} else if (detail.sectionId) {
				showSection(detail.sectionId);
			}
		}
	});

	//CHAT  Écouter l'événement personnalisé pour lancer le jeu depuis le chat widget
	window.addEventListener('startGame', (event: any) => {
		const detail = event.detail;
		console.log('🎮 Custom startGame event received from chat:', detail);
		
		if (detail && detail.mode) {
			handleGameStart(detail.mode);
		}
	});

	setupPongLogoRedirect();
}

/**
 * Check if user was in a game and attempt to reconnect
 */
async function checkAndRejoinGame() {
    console.log('🔍 checkAndRejoinGame called');
    
    if (!appState.currentUser?.id) {
        console.log('❌ No user logged in');
        // ✅ Clear stale data if no user
        const stored = getActiveGame();
        if (stored) {
            console.log('🗑️ Clearing stale game data (no user)');
            clearActiveGame();
        }
        return;
    }

    const activeGame = getActiveGame();
    if (!activeGame) {
        console.log('❌ No active game to rejoin');
        return;
    }

    console.log('🔄 Found active game:', activeGame);

    try {
        const response = await fetch(`/api/games/status/${activeGame.gameId}`, {
            credentials: 'include'
        });

        if (!response.ok) {
            console.log('❌ Game not found, clearing storage');
            clearActiveGame();
            return;
        }

        const gameStatus = await response.json();
        
        if (gameStatus.canRejoin) {
            console.log('✅ Rejoining game!');
            showGameScreen();
            await startGame(
                activeGame.gameId,
                String(appState.currentUser.id),
                activeGame.gameMode
            );
        } else {
            console.log('❌ Cannot rejoin, clearing storage');
            clearActiveGame();
        }
    } catch (error) {
        console.error('💥 Error checking game:', error);
        clearActiveGame();
    }
}

document.addEventListener('DOMContentLoaded', () => {
	document.addEventListener('click', (e) => {
		if ((e.target as HTMLElement)?.id === 'gameOverReturnBtn') {
			console.log('Game over return button clicked');

			const gameOverModal = document.getElementById('gameOverModal');
			if (gameOverModal) {
				gameOverModal.classList.add('hidden');
			}

			// Only process if we're still in game section
			if (appState.currentSection === 'game') {
				cleanupCurrentGame();
				returnToMainMenu();
			}
		}
	});
});


(window as any).triggerGameEnd = triggerGameEnd;

document.addEventListener('keydown', (event) => {
    if (event.key === 'Escape' && appState.currentGame && appState.currentSection === 'game') {
        event.preventDefault();

        if (appState.currentGameMode === 'spectator') {
            // For spectators, just close without forfeit
            console.log('Spectator exiting game...');
            
            // Hide waiting overlay for spectators
            const waitingOverlay = document.getElementById('waiting-opponent-overlay');
            if (waitingOverlay) {
                waitingOverlay.remove();
            }
            
            cleanupCurrentGame();
            returnToMainMenu();
        } else {
            // For players, forfeit the game
            forfeitCurrentGame();
        }
    }

    if (event.key === 'F11') {
        event.preventDefault();
        toggleFullscreen();
    }
});

window.addEventListener('beforeunload', () => {
	cleanupTournamentUI();
	cleanupCurrentGame();

	if (appState.matchmakingWs) {
		try {
			appState.matchmakingWs.close();
		} catch (error) {
			console.error('Error closing matchmaking WebSocket on unload:', error);
		}
	}

	if (appState.statusManager) {
		appState.statusManager.disconnect();
	}
	
	// CHAT - Nettoyer à la fermeture
	destroyChatWidget();
});

console.log('Frontend main.ts loaded successfully')