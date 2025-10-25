// apps/frontend/src/tournament/tournament-services.ts

import { appState } from "../state/state";
import { showSection, showNotification } from '../services/ui';
import { TournamentUI } from "../tournament/tournament-ui";
import { startGame } from '../game/game-manager';
import { renderActiveTournamentsList } from "./tournament-render";
import { connectWebSocketTournament } from "./tournament-ws";


export function cleanupTournamentUI() {
    if (appState.tournamentUI) {
        appState.tournamentUI.cleanup();
        appState.tournamentUI = null;
    }
}
    

export function startTournamentGame(gameId: string, playerId: string, tournamentId: string) {
    console.log(`Starting tournament game: ${gameId} for tournament: ${tournamentId}`);
    startGame(gameId, playerId, 'tournament', tournamentId);
}


export function setCurrentTournament(tournament: any) {
    appState.currentTournament = tournament;
}

/**
 * @description Loading active tournaments.
 */
export async function loadActiveTournaments(): Promise<any[]> {
    try {
        console.log('Loading active tournaments...');
        const response = await fetch('/api/tournament/active', {
            credentials: 'include'
        });

        if (response.ok) {
            const data = await response.json();
            console.log('Active tournaments loaded:', data.tournaments?.length || 0);
            appState.activeTournamentsData = data.tournaments || [];
            return appState.activeTournamentsData;
        } else {
            console.error('Failed to load active tournaments:', response.status);
            appState.activeTournamentsData = [];
            return [];
        }
    } catch (error) {
        console.error('Error loading active tournaments:', error);
        appState.activeTournamentsData = [];
        return [];
    }
}

/**
 * @description Render the home tournaments list.
 */
export async function loadHomeTournaments(): Promise<void> {
    await loadActiveTournaments();
    renderActiveTournamentsList('homeTournamentsList', false);
}

/**
 * @description Open a tournament from the home screen.
 * @param tournamentId - ID of the tournament to open
 * @param mode - 'join'(join), 'participant'(view as participant), 'spectator'(view as spectator)
 */
export async function openTournamentFromHome(tournamentId: string, mode: 'join' | 'participant' | 'spectator' = 'participant') {
    try {
        console.log(`Opening tournament ${tournamentId} in ${mode} mode`);

        if (appState.tournamentUI) {
            appState.tournamentUI.cleanup();
        }
        appState.tournamentUI = new TournamentUI('tournamentSection');

        if (appState.currentUser) {
            appState.tournamentUI.setCurrentUser(appState.currentUser);
        }
        
        showSection('tournament');

        switch (mode) {
            case 'join':
                await joinTournament(tournamentId);
                break;
            
            case 'participant':
                await openTournament(tournamentId);
                break;
            
            case 'spectator':
                await appState.tournamentUI.viewTournamentAsSpectator(tournamentId);
                break;
            
            default:
                await openTournament(tournamentId);
        }

    } catch (error) {
        console.error('Error opening tournament:', error);
        showNotification('Failed to open tournament. Please try again.', 'error');
    }
};

/**
 * @description Join as spectator
 * @param gameId - ID of the game to spectate
 */
export function spectateGame(gameId: string) {
    console.log(`Spectating game: ${gameId}`);
    showNotification('Joining game as spectator...', 'info');
    
    window.dispatchEvent(new CustomEvent('navigate', { 
        detail: { 
            sectionId: 'game',
            gameId: gameId,
            mode: 'spectator'
        }
    }));
};

export async function joinTournament(tournamentId: string): Promise<void> {
    try {
        const response = await fetch('/api/tournament/join', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            credentials: 'include',
            body: JSON.stringify({ tournamentId })
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.error || 'Failed to join tournament');
        }

        // Successfully joined, switch from spectator to participant mode
        appState.isSpectatorMode = false;
        await openTournament(tournamentId);
    } catch (error) {
        console.error('Error joining tournament:', error);
        showNotification('Failed to join tournament: ' + (error instanceof Error ? error.message : 'Unknown error'), 'error');
    }
}

export async function openTournament(tournamentId: string) {
    try {
        const response = await fetch(`/api/tournament/${tournamentId}`, {
            credentials: 'include'
        });

        if (!response.ok) {
            if (response.status === 404) {
                throw new Error('Tournament not found. It may have been deleted.');
            }
            throw new Error(`Failed to load tournament (${response.status})`);
        }

        const tournament = await response.json();
        appState.tournamentUI!.showTournamentView(tournament);
    } catch (error) {
        console.error('Error opening tournament:', error);
        const message = error instanceof Error ? error.message : 'Failed to load tournament';
        showNotification(message, 'error');
        
        setTimeout(() => {
            appState.tournamentUI!.showTournamentHome();
        }, 2000);
    }
}

export async function startTournament(tournamentId: string) {
    try {
        const response = await fetch('/api/tournament/start', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            credentials: 'include',
            body: JSON.stringify({ tournamentId })
        });

        if (!response.ok) {
            const error = await response.json();
            throw new Error(error.error || 'Failed to start tournament');
        }

        showNotification('Tournament started!', 'success');
        await openTournament(tournamentId);
    } catch (error) {
        console.error('Error starting tournament:', error);
        showNotification(error instanceof Error ? error.message : 'Failed to start tournament', 'error');
    }
}

export async function createTournament(name: string, maxPlayers: number): Promise<void> {
    try {
        const response = await fetch('/api/tournament/create', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            credentials: 'include',
            body: JSON.stringify({ name, maxPlayers })
        });

        if (!response.ok) {
            const error = await response.json();
            throw new Error(error.error || 'Failed to create tournament');
        }

        const data = await response.json();
        
        showNotification('Tournament created successfully!', 'success');
        
        const tournamentResponse = await fetch(`/api/tournament/${data.tournamentId}`, {
            credentials: 'include'
        });
        
        if (tournamentResponse.ok) {
            const tournament = await tournamentResponse.json();
            appState.tournamentUI!.showTournamentView(tournament);
        } else {
            appState.tournamentUI!.showTournamentHome();
        }
    } catch (error) {
        console.error('Error creating tournament:', error);
        showNotification(error instanceof Error ? error.message : 'Failed to create tournament', 'error');
    }
}

export async function deleteTournament(tournamentId: string) {
    const confirmDelete = () => {
        return new Promise<boolean>((resolve) => {
            const notification = document.createElement('div');
            notification.className = 'fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50';
            notification.innerHTML = `
                <div class="bg-yellow-300 border-thick shadow-sharp p-8 max-w-md animate-pop">
                    <h2 class="text-2xl font-bold mb-4 text-white text-outline-md-black">DELETE TOURNAMENT?</h2>
                    <p class="mb-6 text-lg">This action cannot be undone. All players will be removed.</p>
                    <div class="flex gap-4">
                        <button id="confirm-delete-yes" class="flex-1 bg-red-500 text-white py-3 px-6 text-lg font-bold border-thick shadow-sharp hover-anarchy">
                            DELETE
                        </button>
                        <button id="confirm-delete-no" class="flex-1 bg-gray-500 text-white py-3 px-6 text-lg font-bold border-thick shadow-sharp hover-anarchy">
                            CANCEL
                        </button>
                    </div>
                </div>
            `;
            
            document.body.appendChild(notification);
            
            document.getElementById('confirm-delete-yes')?.addEventListener('click', () => {
                notification.remove();
                resolve(true);
            });
            
            document.getElementById('confirm-delete-no')?.addEventListener('click', () => {
                notification.remove();
                resolve(false);
            });
        });
    };

    const confirmed = await confirmDelete();
    
    if (!confirmed) {
        return;
    }

    try {
        const response = await fetch('/api/tournament/delete', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            credentials: 'include',
            body: JSON.stringify({ tournamentId })
        });

        if (!response.ok) {
            throw new Error('Failed to delete tournament');
        }

        showNotification('Tournament deleted successfully', 'success');
        appState.tournamentUI!.showTournamentHome();
    } catch (error) {
        console.error('Error deleting tournament:', error);
        showNotification('Failed to delete tournament', 'error');
    }
}

export async function confirmMatch(tournamentId: string, matchId: string) {
    try {
        console.log(`Confirming match ${matchId} in tournament ${tournamentId}`);
        
        const confirmButton = document.querySelector(`[data-match-id="${matchId}"]`) as HTMLButtonElement;
        if (confirmButton) {
            confirmButton.disabled = true;
            confirmButton.textContent = 'CONFIRMING...';
            confirmButton.classList.add('bg-gray-500');
            confirmButton.classList.remove('bg-green-500');
        }

        const response = await fetch('/api/tournament/confirm-match', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            credentials: 'include',
            body: JSON.stringify({ tournamentId, matchId })
        });

        const result = await response.json();

        if (!response.ok) {
            throw new Error(result.error || 'Failed to confirm match');
        }

        console.log('Match confirmed successfully:', result);
        showNotification('Match confirmed successfully!', 'success');

        // The UI will be updated via WebSocket, but if WebSocket is slow, 
        // we can also manually refresh the tournament view
        setTimeout(async () => {
            try {
                const tournamentResponse = await fetch(`/api/tournament/${tournamentId}`, {
                    credentials: 'include'
                });
                if (tournamentResponse.ok) {
                    const updatedTournament = await tournamentResponse.json();
                    appState.currentTournament = updatedTournament;
                    appState.tournamentUI!.showTournamentView(updatedTournament);
                }
            } catch (error) {
                console.error('Error refreshing tournament after confirmation:', error);
            }
        }, 1000);

        // setTimeout(() => {
        //     this.refreshCurrentTournament();
        // }, 500);

    } catch (error) {
        console.error('Error confirming match:', error);
        showNotification(error instanceof Error ? error.message : 'Failed to confirm match', 'error');
        
        const confirmButton = document.querySelector(`[data-match-id="${matchId}"]`) as HTMLButtonElement;
        if (confirmButton) {
            confirmButton.disabled = false;
            confirmButton.textContent = 'CONFIRM READY';
            confirmButton.classList.remove('bg-gray-500');
            confirmButton.classList.add('bg-green-500');
        }
    }
}

export async function refreshCurrentTournament(): Promise<void> {
    if (!appState.currentTournament) return;

    try {
        const response = await fetch(`/api/tournament/${appState.currentTournament.id}`, {
            credentials: 'include'
        });
        
        if (response.ok) {
            const updatedTournament = await response.json();
            appState.currentTournament = updatedTournament;
            appState.tournamentUI!.showTournamentView(updatedTournament);
        }
    } catch (error) {
        console.error('Error refreshing current tournament:', error);
    }
}
