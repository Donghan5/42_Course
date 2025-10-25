import { appState } from '../state/state';
import { showSection, showNotification } from '../services/ui';
import { TournamentUI } from './tournament-ui';
import { returnToMainMenu } from '../utils/tools';
import { openTournamentFromHome, openTournament } from './tournament-services';
import { Tournament, BracketMatch, TournamentPlayer } from '../../../../packages/common-types/src/tournament';
import { navigateBack, pushToNavigationHistory } from '../utils/navigation';

export function showTournamentView(tournament: any) {
    showSection('tournament');
    
    if (!appState.tournamentUI) {
        appState.tournamentUI = new TournamentUI('tournamentSection');
    }

    if (appState.currentUser && appState.tournamentUI) {
        appState.tournamentUI.setCurrentUser(appState.currentUser);
    }

    appState.tournamentUI!.showTournamentView(tournament);
}

export function returnToTournamentLobby() {
    try {
        console.log('Returning to tournament lobby...');
        
        if (appState.tournamentUI && appState.currentTournament) {
            showSection('tournament');
            
            // Refresh the tournament view to get latest state
            openTournament(appState.currentTournament.id);
            showNotification('Returned to tournament lobby', 'info');
        } else {
            console.log('No tournament context available, returning to main menu');
            returnToMainMenu();
        }
    } catch (error) {
        console.error('Error returning to tournament lobby:', error);
        returnToMainMenu();
    }
}

export function showMyTournaments(tournaments: any[]) {
    if (!appState.tournamentUI) return;
    
    const container = document.getElementById('tournamentSection');
    if (!container) return;

    pushToNavigationHistory('tournament', undefined, 'my-tournaments');

    container.innerHTML = `
        <div class="tournament-view animate-pop">
            <button id="my-tournaments-return-btn" class="mb-4 bg-black text-white px-6 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy">&lt; Back</button>
            
            <div class="bg-yellow-300 border-thick shadow-sharp p-8">
                <h1 class="text-4xl text-white text-outline-lg-black mb-8">MY TOURNAMENTS</h1>
                
                <div class="space-y-4">
                    ${tournaments.length === 0 ?
                        `
                        <div class="text-center p-8 bg-gray-100 border-thick">
                            <p class="text-xl text-gray-600 mb-4">No tournaments created yet</p>
                            <button id="create-from-my-tournaments" class="bg-pink-500 text-white py-2 px-6 text-lg border-thick shadow-sharp hover-anarchy font-bold">
                                CREATE TOURNAMENT
                            </button>
                        </div>
                        ` : tournaments.map(t => `
                        <div class="bg-gray-50 border-thick p-6">
                            <div class="flex justify-between items-center">
                                <div class="flex-1">
                                    <h3 class="text-2xl font-bold mb-2">${t.name}</h3>
                                    <div class="grid grid-cols-2 gap-2 text-sm">
                                        <span class="font-bold">Status: <span class="uppercase ${
                                            t.status === 'waiting' ? 'text-yellow-600' :
                                            t.status === 'active' ? 'text-cyan-600' :
                                            'text-gray-600'
                                        }">${t.status}</span></span>
                                        <span class="font-bold">Players: ${t.players?.length || 0}/${t.maxPlayers}</span>
                                    </div>
                                </div>
                                <button data-tournament-id="${t.id}" class="open-my-tournament-btn bg-blue-500 text-white px-6 py-3 text-lg border-thick shadow-sharp hover-anarchy font-bold">
                                    OPEN
                                </button>
                            </div>
                        </div>
                    `).join('')}
                </div>
            </div>
        </div>
    `;

    document.getElementById('my-tournaments-return-btn')?.addEventListener('click', () => {
        console.log('My tournaments back button clicked');
        navigateBack();
    });

    document.getElementById('create-from-my-tournaments')?.addEventListener('click', () => {
        if (appState.tournamentUI) {
            appState.tournamentUI.showCreateTournament();
        }
    });
    
    document.querySelectorAll('.open-my-tournament-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const tournamentId = (e.currentTarget as HTMLElement).dataset.tournamentId;
            if (tournamentId) {
                openTournamentFromHome(tournamentId, 'participant');
            }
        });
    });
}

export function showTournamentHistory(history: any[]) {
    if (!appState.tournamentUI) return;
    
    const container = document.getElementById('tournamentSection');
    if (!container) return;

    pushToNavigationHistory('tournament', undefined, 'history');

    container.innerHTML = `
        <div class="tournament-view animate-pop">
            <button id="tournament-history-return-btn" class="mb-4 bg-black text-white px-6 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy">&lt; Back</button>
            
            <div class="bg-yellow-300 border-thick shadow-sharp p-8">
                <h1 class="text-4xl text-white text-outline-lg-black mb-8">TOURNAMENT HISTORY</h1>

                <div class="space-y-4">
                    ${history.length === 0 ? `
                        <div class="text-center p-8 bg-gray-100 border-thick">
                            <p class="text-xl text-gray-600 mb-4">You haven't participated in any tournaments yet</p>
                            <button id="join-tournament-from-empty" class="bg-pink-500 text-white py-2 px-6 text-lg border-thick shadow-sharp hover-anarchy font-bold">
                                JOIN A TOURNAMENT
                            </button>
                        </div>
                    ` : history.map(item => {
                        // The structure is { tournament, myMatches, finalBracket }
                        const tournament = item.tournament;
                        const myMatches = item.myMatches || [];
                        
                        const wonMatches = myMatches.filter((match: any) => 
                            match.winner && match.winner.id === appState.currentUser?.id?.toString()
                        ).length;
                        
                        const isWinner = tournament.winner?.id === appState.currentUser?.id?.toString();
                        
                        return `
                        <div class="tournament-history-item bg-gray-50 border-thick p-6">
                            <div class="flex justify-between items-start">
                                <div class="flex-1">
                                    <h3 class="text-2xl font-bold mb-2">${tournament.name}</h3>
                                    <div class="grid grid-cols-2 gap-2 text-sm mb-3">
                                        <span class="font-bold">Status: <span class="uppercase text-gray-600">${tournament.status || 'finished'}</span></span>
                                        <span class="font-bold">Players: ${tournament.players?.length || 0}/${tournament.maxPlayers || 'N/A'}</span>
                                        <span class="font-bold">Date: ${tournament.finishedAt ? new Date(tournament.finishedAt).toLocaleDateString() : new Date(tournament.createdAt).toLocaleDateString()}</span>
                                        <span class="font-bold">Your Record: <span class="text-blue-600">${wonMatches}/${myMatches.length} wins</span></span>
                                    </div>
                                    ${isWinner ? 
                                        '<p class="text-sm text-pink-600 font-bold">TOURNAMENT CHAMPION!</p>' : 
                                        tournament.winner ? 
                                        `<p class="text-sm text-gray-600">Winner: ${tournament.winner.nickname}</p>` :
                                        ''
                                    }
                                </div>
                                <button data-tournament-id="${tournament.id}" class="view-history-tournament-btn bg-purple-500 text-white px-6 py-3 text-lg border-thick shadow-sharp hover-anarchy font-bold">
                                    VIEW BRACKET
                                </button>
                            </div>
                        </div>
                    `;
                    }).join('')}
                </div>
            </div>
        </div>
    `;

    document.getElementById('tournament-history-return-btn')?.addEventListener('click', () => {
        console.log('Tournament history back button clicked');
        navigateBack();
    });

    if (history.length === 0) {
        document.getElementById('join-tournament-from-empty')?.addEventListener('click', () => {
            if (appState.tournamentUI) {
                appState.tournamentUI.showTournamentHome();
            }
        });
    }
    
    // Attach listeners to view buttons
    document.querySelectorAll('.view-history-tournament-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const tournamentId = (e.currentTarget as HTMLElement).dataset.tournamentId;
            if (tournamentId) {
                openTournamentFromHome(tournamentId, 'spectator');
            }
        });
    });
}

export function renderActiveTournamentsList(containerId: string, showCreateButton: boolean = false): void {
    const container = document.getElementById(containerId);
    if (!container) {
        console.warn(`Tournament container ${containerId} not found`);
        return;
    }

    if (appState.activeTournamentsData.length === 0) {
        container.innerHTML = `
            <div class="text-center py-8">
                <p class="text-gray-600 mb-4 text-lg">No active tournaments at the moment</p>
                ${showCreateButton ? `
                    <button id="create-first-tournament-btn" 
                            class="bg-pink-500 text-white py-2 px-6 text-lg border-thick shadow-sharp hover-anarchy font-bold">
                        CREATE THE FIRST TOURNAMENT
                    </button>
                ` : ''}
            </div>
        `;
        
        if (showCreateButton) {
            document.getElementById('create-first-tournament-btn')?.addEventListener('click', () => {
                if (appState.tournamentUI) {
                    appState.tournamentUI.showCreateTournament();
                }
            });
        }
        return;
    }

    container.innerHTML = `
        <div class="tournaments-grid grid gap-6 md:grid-cols-2 lg:grid-cols-3">
            ${appState.activeTournamentsData.map(tournament => {
                const isUserParticipant = appState.currentUser && 
                                        tournament.players.some((p: any) => p.id === appState.currentUser.id?.toString());
                const isFull = tournament.players.length >= tournament.maxPlayers;
                const isWaiting = tournament.status === 'waiting';
                const isActive = tournament.status === 'active';
                
                // Determine which button to show
                let buttonHtml = '';
                if (isUserParticipant) {
                    // User is a participant - show OPEN button
                    buttonHtml = `
                        <button data-tournament-id="${tournament.id}" data-mode="participant"
                                class="tournament-action-btn flex-1 bg-blue-500 text-white py-3 px-4 font-black uppercase border-thick shadow-sharp hover-anarchy text-sm">
                            OPEN
                        </button>
                    `;
                } else if (isWaiting && !isFull) {
                    // Can join - show JOIN button
                    buttonHtml = `
                        <button data-tournament-id="${tournament.id}" data-mode="join"
                                class="tournament-action-btn flex-1 bg-cyan-500 text-white py-3 px-4 font-black uppercase border-thick shadow-sharp hover-anarchy text-sm">
                            JOIN
                        </button>
                    `;
                }
                
                // Show VIEW button only when tournament is active or finished (not during waiting)
                if (isActive && !isUserParticipant) {
                    buttonHtml += `
                        <button data-tournament-id="${tournament.id}" data-mode="spectator"
                                class="tournament-action-btn flex-1 bg-purple-500 text-white py-3 px-4 font-black uppercase border-thick shadow-sharp hover-anarchy text-sm">
                            VIEW
                        </button>
                    `;
                }

                // Show FULL indicator when waiting and full (no action available)
                if (isWaiting && isFull && !isUserParticipant) {
                    buttonHtml += `
                        <button disabled
                                class="tournament-action-btn flex-1 bg-gray-400 text-white py-3 px-4 font-black uppercase border-thick opacity-60 cursor-not-allowed text-sm">
                            FULL
                        </button>
                    `;
                }

                return `
                    <div class="tournament-card bg-white border-thick shadow-sharp p-6">
                        <!-- Tournament Title -->
                        <div class="mb-4 pb-4 border-b-4 border-black">
                            <h3 class="text-2xl font-black mb-2 text-black truncate" title="${tournament.name}">
                                ${tournament.name}
                            </h3>
                        </div>
                        
                        <!-- Tournament Info Grid -->
                        <div class="space-y-3 mb-4">
                            <!-- Status Badge -->
                            <div class="flex items-center gap-2">
                                <span class="font-black text-sm text-gray-700 min-w-[60px]">STATUS:</span>
                                <span class="px-3 py-1.5 text-xs font-black uppercase border-2 border-black shadow-[3px_3px_0_0_rgba(0,0,0,1)] ${
                                    isWaiting ? 'bg-yellow-300 text-yellow-900' :
                                    isActive ? 'bg-cyan-300 text-cyan-900' :
                                    'bg-gray-300 text-gray-900'
                                }">
                                    ${isWaiting ? 'WAITING' : isActive ? 'IN PROGRESS' : tournament.status.toUpperCase()}
                                </span>
                            </div>
                            
                            <!-- Players Count -->
                            <div class="flex items-center gap-2">
                                <span class="font-black text-sm text-gray-700 min-w-[60px]">PLAYERS:</span>
                                <div class="flex items-center gap-2">
                                    <span class="text-xl font-black ${isFull ? 'text-red-600' : 'text-blue-600'}">
                                        ${tournament.players.length}/${tournament.maxPlayers}
                                    </span>
                                    ${isFull ? '<span class="text-xs font-bold text-red-600 uppercase">FULL</span>' : ''}
                                </div>
                            </div>
                            
                            <!-- Host Info -->
                            <div class="flex items-center gap-2">
                                <span class="font-black text-sm text-gray-700 min-w-[60px]">HOST:</span>
                                <span class="font-bold text-purple-700 truncate">${tournament.hostNickname || 'Unknown'}</span>
                            </div>
                        </div>

                        <!-- Action Buttons -->
                        <div class="flex gap-2 mt-6">
                            ${buttonHtml}
                        </div>
                    </div>
                `;
            }).join('')}
        </div>
    `;

    // Attach event listeners after rendering the HTML
    container.querySelectorAll('.tournament-action-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const button = e.currentTarget as HTMLButtonElement;
            const tournamentId = button.dataset.tournamentId;
            const mode = button.dataset.mode as 'join' | 'participant' | 'spectator';
            
            if (tournamentId && mode) {
                openTournamentFromHome(tournamentId, mode);
            }
        });
    });
}

export function renderPlayersView(tournament: Tournament): string {
    return `
        <div class="players-view">
            <h2 class="text-3xl mb-4 text-white text-outline-md-black">PLAYERS</h2>
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
                ${tournament.players.map(player => `
                    <div class="player-card bg-white p-4 border-thick shadow-sharp hover:shadow-[6px_6px_0_0_rgba(0,0,0,1)] transition-all cursor-pointer" 
                        data-player-nickname="${player.nickname}">
                        <div class="flex items-center gap-3 mb-2">
                            <img src="${player.avatarUrl || '/default-avatar.png'}" 
                                alt="${player.nickname}" 
                                class="w-12 h-12 rounded-full border-2 border-black">
                            <h3 class="font-black text-2xl text-black player-name-link" 
                                data-player-nickname="${player.nickname}">
                                ${player.nickname}
                            </h3>
                        </div>
                        <p class="text-lg font-bold">Rating: ${player.rankPoints}</p>
                    </div>
                `).join('')}
                ${Array.from({length: 4 - tournament.players.length}, (_, i) => `
                    <div class="player-card bg-gray-300 p-4 border-thick border-dashed">
                        <h3 class="text-xl text-gray-500">Waiting for player...</h3>
                    </div>
                `).join('')}
            </div>
        </div>
    `;
}

export function renderBracketView(tournament: Tournament): string {
    if (!tournament.bracket.length) {
        return `
            <div class="text-center p-8">
                <h3 class="text-2xl font-bold mb-2 text-white text-outline-md-black">Tournament Bracket Coming Soon</h3>
                <p class="text-gray-600">The bracket will be generated once enough players join</p>
            </div>
        `;
    }

    const rounds: { [round: number]: BracketMatch[] } = {};
    tournament.bracket.forEach(match => {
        if (!rounds[match.round]) rounds[match.round] = [];
        rounds[match.round].push(match);
    });

    const roundNumbers = Object.keys(rounds).map(r => parseInt(r)).sort((a, b) => a - b);
    const finalMatch = rounds[roundNumbers[roundNumbers.length - 1]][0];
    const tournamentWinner = tournament.status === 'finished' ? finalMatch?.winner : null;

    return `
        <div class="bracket-view relative">
            <h2 class="text-4xl font-black text-center mb-8 text-white text-outline-lg-black">TOURNAMENT BRACKET</h2>
            
            ${tournamentWinner ? renderWinnerDisplay(tournamentWinner) : ''}
            
            <div class="flex flex-col md:flex-row justify-center items-stretch gap-8 md:gap-4 lg:gap-8 w-full min-h-[450px]">
                
                <!-- Semi-finals Column -->
                <div class="flex-1 max-w-sm w-full flex flex-col justify-around">
                    <h3 class="text-2xl font-black text-center mb-6 bg-black text-yellow-300 p-3 border-thick shadow-sharp">SEMI-FINALS</h3>
                    ${rounds[1]?.map(match => renderMatch(match)).join('<div class="h-8"></div>')}
                </div>
                
                <!-- Connectors -->
                <div class="relative w-20 flex-shrink-0 h-auto hidden md:block">
                    <svg class="absolute inset-0 w-full h-full" preserveAspectRatio="none" viewBox="0 0 100 100">
                        <defs>
                            <marker id="arrowhead" markerWidth="8" markerHeight="6" 
                                    refX="7" refY="3" orient="auto"
                                    markerUnits="strokeWidth">
                                <polygon points="0 0, 8 3, 0 6" fill="black" />
                            </marker>
                        </defs>
                        <path d="M0 25 H 50 V 40 H 93" stroke="black" stroke-width="5" fill="none" vector-effect="non-scaling-stroke" marker-end="url(#arrowhead)"/>
                        <path d="M0 75 H 50 V 60 H 93" stroke="black" stroke-width="5" fill="none" vector-effect="non-scaling-stroke" marker-end="url(#arrowhead)"/>
                    </svg>
                </div>
                
                <!-- Final Column -->
                <div class="flex-1 max-w-sm w-full flex flex-col justify-center">
                    <h3 class="text-2xl font-black text-center mb-6 bg-black text-yellow-300 p-3 border-thick shadow-sharp">FINAL</h3>
                    ${rounds[2]?.map(match => renderMatch(match)).join('')}
                </div>

            </div>
        </div>
    `;
}

export function renderWinnerDisplay(winner: TournamentPlayer): string {
    return `
        <div class="winner-display mb-8 border-8 border-black shadow-[12px_12px_0_0_rgba(0,0,0,1)] relative overflow-hidden bg-champion-gradient">
            
            <div class="absolute inset-0 opacity-15 bg-polka-dots"></div>
            
            <div class="relative z-10 p-8">
                <div class="text-center mb-6">
                    <div class="inline-block bg-black border-4 border-white shadow-[6px_6px_0_0_rgba(255,0,255,1)] px-8 py-2 transform -rotate-1">
                        <h2 class="text-4xl font-black uppercase text-champion-header">
                            CHAMPION
                        </h2>
                    </div>
                </div>

                <div class="flex items-center justify-center gap-6 mb-6">
                    <div class="relative">
                        <div class="absolute inset-0 bg-white rounded-full animate-ping opacity-30"></div>
                        <img src="${winner.avatarUrl || '/default-avatar.png'}" 
                            alt="${winner.nickname}"
                            class="relative w-24 h-24 rounded-full border-6 border-black shadow-[6px_6px_0_0_rgba(0,0,0,1)]">
                    </div>
                    <div>
                        <div class="winner-name-link cursor-pointer transform hover:scale-105 transition-transform" 
                            data-player-nickname="${winner.nickname}">
                            <span class="text-5xl font-black uppercase block leading-none text-champion-name">
                                ${winner.nickname}
                            </span>
                        </div>
                        <div class="mt-2 bg-black border-3 border-white inline-block px-4 py-1">
                            <p class="text-xl font-black text-yellow-300">RATING: ${winner.rankPoints}</p>
                        </div>
                    </div>
                </div>

                <div class="text-center">
                    <div class="inline-block bg-black text-yellow-300 px-8 py-2 border-3 border-white shadow-[4px_4px_0_0_rgba(255,255,255,1)]">
                        <span class="text-3xl font-black uppercase">VICTORY</span>
                    </div>
                </div>
            </div>
        </div>
    `;
}

export function renderMatch(match: BracketMatch): string {
    const isCurrentUserMatch = !appState.isSpectatorMode && 
                                (match.player1?.id === appState.currentUser?.id?.toString() || 
                                match.player2?.id === appState.currentUser?.id?.toString());
    const needsConfirmation = match.status === 'confirming' && isCurrentUserMatch && 
                            !match.confirmations.includes(appState.currentUser?.id?.toString());

    // BUG FIX: Only determine a winner if the match status is 'finished'
    const isMatchFinished = match.status === 'finished';
    const isPlayer1Winner = isMatchFinished && match.winner?.id === match.player1?.id;
    const isPlayer2Winner = isMatchFinished && match.winner?.id === match.player2?.id;

    const player1SlotClass = isPlayer1Winner
        ? 'bg-yellow-300'
        : 'bg-gray-100';

    const player2SlotClass = isPlayer2Winner
        ? 'bg-yellow-300'
        : 'bg-gray-100';

    const winnerBadge = `
        <div class="absolute top-1/2 -right-4 transform -translate-y-1/2 rotate-12 z-10">
            <div class="bg-pink-500 text-white font-black uppercase text-center px-4 py-1 shadow-sharp border-2 border-black text-sm">
                WIN
            </div>
        </div>
    `;

    return `
        <div class="bg-white border-thick shadow-sharp p-4 ${match.status === 'playing' ? 'bg-cyan-50' : ''}">
            <div class="match-players space-y-2">
                <div class="player-slot relative flex items-center gap-4 p-3 ${player1SlotClass} border-thick">
                    ${isPlayer1Winner ? winnerBadge : ''}
                    ${match.player1 ? `
                        <img src="${match.player1.avatarUrl || '/default-avatar.png'}" 
                                alt="${match.player1.nickname}" 
                                class="w-12 h-12 rounded-full border-3 border-black shadow-sharp flex-shrink-0">
                        <span class="font-black text-2xl text-black flex-1 player-name-link cursor-pointer transition-transform duration-200 hover:scale-110 origin-left" data-player-nickname="${match.player1.nickname}">${match.player1.nickname}</span>
                    ` : `
                        <div class="w-12 h-12 rounded-full bg-gray-300 border-3 border-black flex-shrink-0"></div>
                        <span class="text-gray-500 flex-1 font-bold text-xl">TBD</span>
                    `}
                </div>
                
                <div class="text-center text-xl font-black text-white text-outline-md-black">VS</div>
                
                <div class="player-slot relative flex items-center gap-4 p-3 ${player2SlotClass} border-thick">
                    ${isPlayer2Winner ? winnerBadge : ''}
                    ${match.player2 ? `
                        <img src="${match.player2.avatarUrl || '/default-avatar.png'}" 
                                alt="${match.player2.nickname}" 
                                class="w-12 h-12 rounded-full border-3 border-black shadow-sharp flex-shrink-0">
                        <span class="font-black text-2xl text-black flex-1 player-name-link cursor-pointer transition-transform duration-200 hover:scale-110 origin-left" data-player-nickname="${match.player2.nickname}">${match.player2.nickname}</span>
                    ` : `
                        <div class="w-12 h-12 rounded-full bg-gray-300 border-3 border-black flex-shrink-0"></div>
                        <span class="text-gray-500 flex-1 font-bold text-xl">TBD</span>
                    `}
                </div>
            </div>
            
            <div class="match-status mt-4 text-center">
                <span class="text-md font-black uppercase px-4 py-2 border-thick ${getStatusColor(match.status)}">${getStatusText(match.status)}</span>
            </div>

            ${needsConfirmation ? `
                <div class="text-center mt-4">
                    <button class="confirm-match-btn bg-green-500 text-white py-3 px-8 text-lg font-bold border-thick shadow-sharp hover-anarchy uppercase" 
                            data-match-id="${match.id}">
                        Confirm Ready
                    </button>
                </div>
            ` : ''}

            ${match.status === 'playing' && match.gameId ? `
                <div class="text-center mt-4">
                    ${isCurrentUserMatch ? `
                        <button class="join-game-btn bg-blue-500 text-white py-3 px-8 text-lg font-bold border-thick shadow-sharp hover-anarchy uppercase w-full" 
                                data-game-id="${match.gameId}">
                            Join Game
                        </button>
                    ` : `
                        <button class="spectate-game-btn bg-purple-500 text-white py-3 px-8 text-lg font-bold border-thick shadow-sharp hover-anarchy uppercase w-full" data-game-id="${match.gameId}">
                            View Game
                        </button>
                    `}
                </div>
            ` : ''}
        </div>
    `;
}

function getRoundName(roundNum: number, totalRounds: number): string {
    const roundsFromEnd = totalRounds - roundNum + 1;
    switch (roundsFromEnd) {
        case 1: return 'FINAL';
        case 2: return 'SEMI-FINALS';
        default: return `ROUND ${roundNum}`;
    }
}

function getStatusColor(status: string): string {
    switch (status) {
        case 'waiting': return 'bg-gray-300 text-black';
        case 'confirming': return 'bg-yellow-400 text-black';
        case 'playing': return 'bg-cyan-400 text-black';
        case 'finished': return 'bg-purple-500 text-white';
        default: return 'bg-gray-300 text-black';
    }
}

function getStatusText(status: string): string {
    switch (status) {
        case 'waiting': return 'WAITING';
        case 'confirming': return 'CONFIRMING';
        case 'playing': return 'PLAYING';
        case 'finished': return 'FINISHED';
        default: return status.toUpperCase();
    }
}