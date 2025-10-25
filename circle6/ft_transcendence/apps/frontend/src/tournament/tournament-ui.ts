// apps/frontend/src/tournament/tournament-ui.ts

import { Tournament, BracketMatch, TournamentPlayer } from '../../../../packages/common-types/src/tournament';
import { appState } from '../state/state';
import { renderActiveTournamentsList, showMyTournaments, showTournamentHistory, renderBracketView, renderMatch, renderPlayersView } from './tournament-render';
import { setCurrentTournament, startTournamentGame, openTournament, joinTournament, startTournament, deleteTournament, confirmMatch } from './tournament-services';
import { spectateGame, createTournament } from './tournament-services';
import { showPublicProfileScreen } from '../services/user';
import { connectWebSocketTournament, disconnectWebSocketTournament } from './tournament-ws';
import { showNotification } from '../services/ui';

export class TournamentUI {
    private container: HTMLElement;
    private currentTournament: Tournament | null = null;
    private currentUser: any | null = null;

    constructor(containerId: string) {
        const container = document.getElementById(containerId);
        if (!container) {
            throw new Error(`Container with id ${containerId} not found`);
        }
        this.container = container;
    }

    setCurrentUser(user: any) {
        this.currentUser = user;
    }

    /**
     * View tournament as a spectator (non-participant)
     */
    async viewTournamentAsSpectator(tournamentId: string) {
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
            appState.isSpectatorMode = true;
            this.showTournamentView(tournament);
        } catch (error) {
            console.error('Error viewing tournament as spectator:', error);
            const message = error instanceof Error ? error.message : 'Failed to load tournament';
            showNotification(message, 'error');
            
            setTimeout(() => {
                this.showTournamentHome();
            }, 2000);
        }
    }

    showTournamentHome() {
        this.container.innerHTML = `
            <div class="tournament-home animate-pop">
                <button id="tournament-return-btn" class="mb-4 bg-black text-white px-6 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy">&lt; Back</button>
                <div class="bg-yellow-300 border-thick shadow-sharp p-8 mb-6">
                    <h1 class="text-4xl text-white text-outline-lg-black text-center mb-6">TOURNAMENTS</h1>
                    <div class="flex flex-wrap gap-4 justify-center">
                        <button id="create-tournament-btn" 
                                class="bg-pink-500 text-white py-3 px-8 text-xl font-bold border-thick shadow-sharp hover-anarchy">
                            CREATE TOURNAMENT
                        </button>
                        <button id="my-tournaments-btn" 
                                class="bg-cyan-400 text-white py-3 px-8 text-xl font-bold border-thick shadow-sharp hover-anarchy">
                            MY TOURNAMENTS
                        </button>
                        <button id="tournament-history-btn" 
                                class="bg-purple-500 text-white py-3 px-8 text-xl font-bold border-thick shadow-sharp hover-anarchy">
                            TOURNAMENT HISTORY
                        </button>
                    </div>
                </div>
                
                <div class="bg-yellow-300 border-thick shadow-sharp p-8">
                    <h2 class="text-3xl text-white text-outline-md-black mb-6">ACTIVE TOURNAMENTS</h2>
                    <div id="tournaments-container"></div>
                </div>
            </div>
        `;

        this.setupTournamentHomeListeners();
        this.loadActiveTournaments();
    }

    private setupTournamentHomeListeners() {
        document.getElementById('tournament-return-btn')?.addEventListener('click', () => {
            window.dispatchEvent(new CustomEvent('navigate', { detail: 'hero' }));
        });
        
        document.getElementById('create-tournament-btn')?.addEventListener('click', () => {
            this.showCreateTournament();
        });

        document.getElementById('my-tournaments-btn')?.addEventListener('click', async () => {
            try {
                console.log('Loading my tournaments...');
                const response = await fetch('/api/tournament/my-tournaments', {
                    credentials: 'include'
                });
                
                if (response.ok) {
                    const data = await response.json();
                    showMyTournaments(data.tournaments);
                } else {
                    showNotification('Failed to load your tournaments', 'error');
                }
            } catch (error) {
                console.error('Error loading my tournaments:', error);
                showNotification('Failed to load your tournaments', 'error');
            }
        });

        document.getElementById('tournament-history-btn')?.addEventListener('click', async () => {
            try {
                console.log('Loading tournament history...');
                const response = await fetch('/api/tournament/history/user', {
                    credentials: 'include'
                });
                
                if (response.ok) {
                    const data = await response.json();
                    showTournamentHistory(data.history);
                } else {
                    showNotification('Failed to load tournament history', 'error');
                }
            } catch (error) {
                console.error('Error loading tournament history:', error);
                showNotification('Failed to load tournament history', 'error');
            }
        });
    }

    private setupCreateTournamentListeners() {
        const confirmBtn = document.getElementById('create-confirm-btn');
        const cancelBtn = document.getElementById('create-cancel-btn');
        const nameInput = document.getElementById('tournament-name-input') as HTMLInputElement;
        const errorDiv = document.getElementById('create-error-message');

        if (!confirmBtn || !cancelBtn || !nameInput) {
            console.error('Tournament create form elements not found');
            return;
        }

        const showError = (message: string) => {
            if (errorDiv) {
                errorDiv.textContent = message;
                errorDiv.classList.remove('hidden');
            }
            showNotification(message, 'error');
        };

        const hideError = () => {
            if (errorDiv) {
                errorDiv.classList.add('hidden');
            }
        };

        nameInput.addEventListener('input', hideError);

        confirmBtn.addEventListener('click', async () => {
            hideError();
            
            const name = nameInput.value.trim();
            const maxPlayers = 4;

            if (!name || name.length === 0) {
                showError('Please enter a tournament name');
                nameInput.focus();
                return;
            }

            if (name.length < 3) {
                showError('Tournament name must be at least 3 characters');
                nameInput.focus();
                return;
            }

            if (name.length > 50) {
                showError('Tournament name must be less than 50 characters');
                nameInput.focus();
                return;
            }

            confirmBtn.setAttribute('disabled', 'true');
            (confirmBtn as HTMLButtonElement).textContent = 'CREATING...';

            await createTournament(name, maxPlayers);

            confirmBtn.removeAttribute('disabled');
            (confirmBtn as HTMLButtonElement).textContent = 'CREATE';
        });

        cancelBtn.addEventListener('click', () => {
            this.showTournamentHome();
        });

        nameInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                confirmBtn.click();
            }
        });
    }

    showCreateTournament() {
        this.container.innerHTML = `
            <div class="create-tournament bg-yellow-300 border-thick shadow-sharp p-8 animate-pop">
                <h1 class="text-4xl text-white text-outline-lg-black text-center mb-8">CREATE TOURNAMENT</h1>
                
                <div class="max-w-md mx-auto">
                    <div class="mb-6">
                        <label class="block text-xl mb-2 font-bold text-white text-outline-md-black">Tournament Name:</label>
                        <input type="text" 
                            id="tournament-name-input" 
                            class="w-full p-3 text-xl border-thick" 
                            placeholder="Enter tournament name"
                            maxlength="50"
                            required>
                        <p class="text-sm text-gray-600 mt-1">3-50 characters</p>
                    </div>
                    
                    <div class="mb-6 bg-blue-100 border-thick p-4">
                        <p class="text-lg font-bold text-center">This tournament is for 4 PLAYERS</p>
                        <p class="text-sm text-gray-700 text-center mt-1">Semi-finals + Final</p>
                    </div>
                    
                    <div id="create-error-message" class="hidden mb-4 p-3 bg-red-100 border-thick text-red-700"></div>
                    
                    <div class="flex gap-4 justify-center">
                        <button id="create-confirm-btn" 
                                class="bg-pink-500 text-white py-3 px-8 text-xl border-thick shadow-sharp hover-anarchy">
                            CREATE
                        </button>
                        <button id="create-cancel-btn" 
                                class="bg-gray-500 text-white py-3 px-8 text-xl border-thick shadow-sharp hover-anarchy">
                            CANCEL
                        </button>
                    </div>
                </div>
            </div>
        `;

        this.setupCreateTournamentListeners();
    }

    showTournamentView(tournament: Tournament) {
        disconnectWebSocketTournament();
        this.currentTournament = tournament;
        setCurrentTournament(tournament);
        
        const isHost = !appState.isSpectatorMode && this.currentUser?.id?.toString() === tournament.hostId;
        const isParticipant = !appState.isSpectatorMode && tournament.players.some(p => p.id === this.currentUser?.id?.toString());

        this.container.innerHTML = `
            <div class="tournament-view animate-pop">
                <div class="bg-yellow-300 border-thick shadow-sharp p-8 mb-6">
                    <button id="tournament-view-return-btn" class="mb-4 bg-black text-white px-6 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy">&lt; Back</button>
                    
                    <h1 class="text-4xl text-white text-outline-lg-black text-center mb-4">${tournament.name}</h1>
                    <div class="text-center text-xl mb-4">
                        <span class="font-bold text-white text-outline-md-black">Status:</span> 
                        <span class="uppercase ${tournament.status === 'waiting' ? 'text-yellow-600' : tournament.status === 'active' ? 'text-green-600' : 'text-gray-600'}">
                            ${tournament.status}
                        </span>
                        ${appState.isSpectatorMode ? '<span class="ml-4 text-purple-600 font-bold text-outline-md-black">(SPECTATOR MODE)</span>' : ''}
                    </div>
                    <div class="text-center text-lg mb-4">
                        <span class="font-bold text-white text-outline-md-black">Players:</span> ${tournament.players.length}/4
                    </div>

                    <div class="actions text-center">
                        ${isHost && tournament.status === 'waiting' ? `
                            <div class="mb-4">
                                <button id="start-tournament-btn" 
                                        class="bg-green-500 text-white py-3 px-8 text-xl border-thick shadow-sharp hover-anarchy"
                                        ${tournament.players.length < 4 ? 'disabled' : ''}>
                                    START TOURNAMENT
                                </button>
                                <button id="delete-tournament-btn" 
                                        class="bg-red-500 text-white py-3 px-8 text-xl border-thick shadow-sharp hover-anarchy ml-4">
                                    DELETE TOURNAMENT
                                </button>
                            </div>
                        ` : ''}

                        ${appState.isSpectatorMode && tournament.status === 'waiting' ? `
                            <div class="text-center mb-4 bg-purple-100 p-4 border-thick">
                                <p class="text-lg font-teko uppercase">Tournament hasn't started yet</p>
                                <p class="text-sm text-gray-600">You can join as a participant if there's still space</p>
                                ${tournament.players.length < 4 ? `
                                    <button id="join-as-participant-btn" 
                                            class="bg-green-500 text-white py-2 px-6 text-lg border-thick shadow-sharp hover-anarchy mt-3">
                                        JOIN AS PARTICIPANT
                                    </button>
                                ` : ''}
                            </div>
                        ` : ''}
                    </div>

                    <div id="tournament-content">
                        ${tournament.status === 'waiting' ? renderPlayersView(tournament) : renderBracketView(tournament)}
                    </div>
                </div>
            </div>
        `;

        this.setupTournamentViewListeners(tournament);

        this.setupPlayerNameClickHandlers();

        // Always connect to WebSocket to receive real-time updates
        // (nickname changes, avatar updates, anonymization, deletion)
        connectWebSocketTournament(tournament.id, (data) => {
            this.handleWebSocketMessage(data)
        });
    }

    getCurrentTournamentId(): string | null {
        return this.currentTournament?.id || null;
    }

    private setupPlayerNameClickHandlers() {
        document.querySelectorAll('.player-name-link').forEach(element => {
            element.addEventListener('click', (e) => {
                e.preventDefault();
                e.stopPropagation();
                const nickname = (e.currentTarget as HTMLElement).dataset.playerNickname;
                if (nickname) {
                    showPublicProfileScreen(nickname);
                }
            });
        });

        document.querySelectorAll('.player-card[data-player-nickname]').forEach(element => {
            element.addEventListener('click', (e) => {
                const nickname = (e.currentTarget as HTMLElement).dataset.playerNickname;
                if (nickname) {
                    showPublicProfileScreen(nickname);
                }
            });
        });

        document.querySelectorAll('.winner-name-link').forEach(element => {
            element.addEventListener('click', (e) => {
                e.preventDefault();
                e.stopPropagation();
                const nickname = (e.currentTarget as HTMLElement).dataset.playerNickname;
                if (nickname) {
                    showPublicProfileScreen(nickname);
                }
            });
        });
    }

    private async loadActiveTournaments() {
        try {
            const response = await fetch('/api/tournament/active', {
                credentials: 'include'
            });

            if (!response.ok) {
                throw new Error('Failed to load tournaments');
            }

            const data = await response.json();
            renderActiveTournamentsList('tournaments-container', true);
        } catch (error) {
            console.error('Error loading tournaments for UI:', error);
            const container = document.getElementById('tournaments-container');
            if (container) {
                container.innerHTML = '<div class="text-center p-4 text-red-500">Failed to load tournaments</div>';
            }
        }
    }

    private setupTournamentViewListeners(tournament: Tournament) {
        document.getElementById('tournament-view-return-btn')?.addEventListener('click', () => {
            disconnectWebSocketTournament();
            appState.isSpectatorMode = false;
            this.showTournamentHome();
        });

        if (!appState.isSpectatorMode) {
            document.getElementById('start-tournament-btn')?.addEventListener('click', () => {
                startTournament(tournament.id);
            });

            document.getElementById('delete-tournament-btn')?.addEventListener('click', () => {
                deleteTournament(tournament.id);
            });

            document.querySelectorAll('.confirm-match-btn').forEach(btn => {
                const newBtn = btn.cloneNode(true) as HTMLElement;
                btn.parentNode?.replaceChild(newBtn, btn);
                
                newBtn.addEventListener('click', async (e) => {
                    e.preventDefault();
                    e.stopPropagation();
                    
                    const matchId = (e.target as HTMLElement).dataset.matchId;
                    if (matchId) {
                        console.log('Confirm button clicked for match:', matchId);
                        await confirmMatch(tournament.id, matchId);
                    } else {
                        console.error('No match ID found on confirm button');
                    }
                });
            });
        } else {
            document.getElementById('join-as-participant-btn')?.addEventListener('click', async () => {
                try {
                    await joinTournament(tournament.id);
                    appState.isSpectatorMode = false;
                    this.showTournamentView(tournament);
                } catch (error) {
                    showNotification('Failed to join tournament', 'error');
                }
            });
        }

        document.querySelectorAll('.join-game-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const gameId = (e.target as HTMLElement).dataset.gameId;
                if (gameId && this.currentTournament) {
                    startTournamentGame(gameId, this.currentUser?.id?.toString(), this.currentTournament.id);
                }
            });
        });

        document.querySelectorAll('.spectate-game-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const gameId = (e.target as HTMLElement).dataset.gameId;
                if (gameId) {
                    spectateGame(gameId);
                }
            });
        });
    }
    
    public handleWebSocketMessage(data: any) {
        console.log('Tournament WebSocket message received:', data);
        
        switch (data.type) {
            case 'tournament_updated':
                if (this.currentTournament && data.tournament.id === this.currentTournament.id) {
                    console.log('Updating tournament view with new data');
                    this.currentTournament = data.tournament;
                    
                    // Force a full re-render to show updated avatars/nicknames
                    const contentDiv = document.getElementById('tournament-content');
                    if (contentDiv && this.currentTournament) {
                        if (this.currentTournament.status === 'waiting') {
                            contentDiv.innerHTML = renderPlayersView(this.currentTournament);
                        } else {
                            contentDiv.innerHTML = renderBracketView(this.currentTournament);
                        }
                        
                        // Re-attach event listeners after re-render
                        this.setupPlayerNameClickHandlers();
                        if (this.currentTournament) {
                            this.setupTournamentViewListeners(this.currentTournament);
                        }
                    }
                }
                break;
                
            case 'tournament_deleted':
                if (this.currentTournament && data.tournamentId === this.currentTournament.id) {
                    console.log('Tournament was deleted, redirecting user');
                    this.handleTournamentDeleted(data.tournamentName || 'Unknown Tournament');
                }
                break;
                
            case 'player_joined':
                if (this.currentTournament) {
                    this.currentTournament.players = data.players;
                    this.refreshTournamentView();
                }
                break;
                
            case 'match_ready':
                showNotification('Your match is ready! Please confirm to start playing.', 'info');
                if (this.currentTournament) {
                    this.refreshTournamentView();
                }
                break;
                
            case 'match_confirmed':
                showNotification('Match confirmation received!', 'success');
                if (this.currentTournament) {
                    this.refreshTournamentView();
                }
                break;
                
            case 'game_ready':
                if (data.gameId && this.isPlayerInMatch(data.match)) {
                    showNotification('Game starting! Click "Join Game" to play.', 'success');
                    if (this.currentTournament) {
                        this.refreshTournamentView();
                    }
                }
                break;
                
            case 'game_finished':
                showNotification(`Match finished! Winner: ${data.winnerNickname}`, 'info');
                if (this.currentTournament) {
                    this.refreshTournamentView();
                }
                break;
        }
    }

    private handleTournamentDeleted(tournamentName: string) {
        // Show notification
        showNotification(`Tournament "${tournamentName}" has been deleted by the host`, 'error');
        
        // Disconnect from WebSocket
        disconnectWebSocketTournament();
        
        // Clear current tournament
        this.currentTournament = null;
        
        // Redirect to tournament home after a brief delay
        setTimeout(() => {
            this.showTournamentHome();
        }, 2000);
    }

    private isPlayerInMatch(match: any): boolean {
        if (!this.currentUser || !match) return false;
        return match.player1?.id === this.currentUser.id?.toString() || 
               match.player2?.id === this.currentUser.id?.toString();
    }

    private async refreshTournamentView() {
        if (!this.currentTournament) return;
        
        try {
            const response = await fetch(`/api/tournament/${this.currentTournament.id}`, {
                credentials: 'include'
            });
            
            if (response.ok) {
                const updatedTournament = await response.json();
                this.currentTournament = updatedTournament;
                this.showTournamentView(updatedTournament);
            }
        } catch (error) {
            console.error('Error refreshing tournament view:', error);
        }
    }

    public cleanup() {
        disconnectWebSocketTournament();
        appState.isSpectatorMode = false;
        this.currentTournament = null;
    }

    setCurrentUserId(userId: string) {
        this.currentUser = { id: userId };
    }

    showTournamentLobby() {
        this.showTournamentHome();
    }

    setTournamentId(tournamentId: string) {
        openTournament(tournamentId);
    }

    showBracket(tournament: Tournament) {
        this.showTournamentView(tournament);
    }
}