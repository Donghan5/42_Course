import { appState } from '../state/state';
import { renderActiveTournamentsList } from './tournament-render';

let isConnectingOrConnected = false;
let tournamentWs: WebSocket | null = null;

export function connectActiveTournamentsSocket() {
    if (isConnectingOrConnected) {
        console.log('✅ [Global] Active tournaments socket connection already in progress or established.');
        return;
    }

    if (appState.activeTournamentsWs) {
        appState.activeTournamentsWs.onclose = null; 
        appState.activeTournamentsWs.close();
    }

    isConnectingOrConnected = true;

    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.host}/api/tournament/ws/active`;

    appState.activeTournamentsWs = new WebSocket(wsUrl);

    appState.activeTournamentsWs.onopen = () => {
        console.log('✅ [Global] Connected to active tournaments feed.');
    };

    appState.activeTournamentsWs.onmessage = (event) => {
        try {
            const data = JSON.parse(event.data);
            if (data.type === 'activeTournaments') {
                console.log('[Global] Active tournaments list update received:', data.payload);

                appState.activeTournamentsData = data.payload || [];

                if (appState.currentSection === 'hero') {
                    const heroContainer = document.getElementById('homeTournamentsList');
                    if (heroContainer) {
                        renderActiveTournamentsList('homeTournamentsList', false);
                    }
                } else if (appState.currentSection === 'tournament') {
                    const tournamentHomeContainer = document.getElementById('tournaments-container');
                    if (tournamentHomeContainer) {
                        renderActiveTournamentsList('tournaments-container', true);
                    }
                }
            }
        } catch (error) {
            console.error('[Global] Active tournaments list update error:', error);
        }
    };

    appState.activeTournamentsWs.onclose = () => {
        console.log('[Global] Disconnected from active tournaments feed. Reconnecting in 5 seconds...');
        
        isConnectingOrConnected = false;
        setTimeout(() => {
            if (appState.currentUser) {
                connectActiveTournamentsSocket();
            }
        }, 5000);
    };

    appState.activeTournamentsWs.onerror = (error) => {
        console.error('[Global] Active tournaments WebSocket error:', error);
        isConnectingOrConnected = false;
    };
}

export function connectWebSocketTournament(tournamentId: string, onMessageCallback: (data: any) => void) {
    disconnectWebSocketTournament();
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    tournamentWs = new WebSocket(`${protocol}//${window.location.host}/api/tournament/ws/${tournamentId}`);

    tournamentWs.onmessage = (event) => {
        const data = JSON.parse(event.data);
        onMessageCallback(data);
    };

    tournamentWs.onerror = (error) => {
        console.error('Tournament WebSocket error:', error);
    };

    tournamentWs.onclose = () => {
        console.log('Tournament WebSocket closed');
    };
}

export function disconnectWebSocketTournament() {
    if (tournamentWs) {
        tournamentWs.close();
        tournamentWs = null;
    }
}


// private connectWebSocket(tournamentId: string) {
//     this.disconnectWebSocket();

//     const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
//     const wsUrl = `${protocol}//${window.location.host}/api/tournament/ws/${tournamentId}`;

//     this.ws = new WebSocket(wsUrl);

//     this.ws.onopen = () => {
//         console.log('Connected to tournament WebSocket');
//     };

//     this.ws.onmessage = (event) => {
//         try {
//             const data = JSON.parse(event.data);
//             this.handleWebSocketMessage(data);
//         } catch (error) {
//             console.error('Error parsing WebSocket message:', error);
//         }
//     };

//     this.ws.onerror = (error) => {
//         console.error('Tournament WebSocket error:', error);
//     };

//     this.ws.onclose = () => {
//         console.log('Tournament WebSocket closed');
//     };
// }