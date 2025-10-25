import { appState } from '../state/state';
import { TournamentUI } from '../tournament/tournament-ui';
import { showSection } from '../services/ui';
import { cleanupCurrentGame } from '../game/game-manager';
import { cleanupTournamentUI, openTournament } from '../tournament/tournament-services';

export function returnToMainMenu() {
    console.log('returnToMainMenu called - checking context...', {
        gameMode: appState.currentGameMode,
        isSpectator: appState.isSpectatorMode,
        hasTournamentUI: !!appState.tournamentUI,
        currentTournamentId: appState.currentTournament?.id ?? 'N/A'
    });

    const isTournamentGame = appState.currentGameMode === 'tournament';
    const isSpectatorInTournament = appState.currentGameMode === 'spectator' && appState.currentTournament && appState.tournamentUI;

    if (isTournamentGame || isSpectatorInTournament) {
        console.log('Redirecting to tournament lobby instead of main menu');

        const tournamentId = localStorage.getItem('currentTournamentId') || appState.currentTournament?.id;

        if (tournamentId) {
            console.log(`Restoring tournament view for tournament: ${tournamentId}`);
            showSection('tournament');

            if (!appState.tournamentUI) {
                appState.tournamentUI = new TournamentUI('tournamentSection');
                if (appState.currentUser) {
                    appState.tournamentUI.setCurrentUser(appState.currentUser);
                }
            }

            openTournament(tournamentId).catch((error) => {
                console.error('Failed to restore tournament view, returning to main menu:', error);
                proceedToMainMenu();
            });

            localStorage.removeItem('currentTournamentId');
        } else {
            console.warn('Tournament context lost, returning to main menu.');
            proceedToMainMenu();
        }
    } else {
        proceedToMainMenu();
    }

    appState.currentGameMode = null;
    appState.isSpectatorMode = false;
}

function proceedToMainMenu() {
    console.log('Returning to main menu...');

    if (appState.currentSection === 'hero') {
        return;
    }

    if (appState.currentSection === 'tournament') {
        cleanupTournamentUI();
    }

    showSection('hero');

    const gameOverModal = document.getElementById('gameOverModal');
    if (gameOverModal) {
        gameOverModal.classList.add('hidden');
    }
}


export function toggleFullscreen() {
    if (!document.fullscreenElement) {
        document.documentElement.requestFullscreen().catch(err => {
            console.error(`Error attempting to enable full-screen mode: ${err.message} (${err.name})`);
        });
        console.log("Enter fullscreen");
    } else {
        document.exitFullscreen();
        console.log("Window mode");
    }
}

export function exitSpectatorMode() {
    console.log('Exiting spectator mode...');
    cleanupCurrentGame();
    
    if (appState.currentGameMode === 'spectator' && appState.tournamentUI && appState.currentTournament) {
        showSection('tournament');
        appState.tournamentUI.showTournamentView(appState.currentTournament);
    } else {
        returnToMainMenu();
    }
}