import { PongGame3D } from '../game/render';
import { TournamentUI } from '../tournament/tournament-ui';
import { StatusManager } from '../status/status-manager';
import { StatsManager } from '../stats/stats-manager';

type UserResolver = (user: any) => void;

export interface NavigationHistoryEntry {
  sectionId: string;
  tournamentId?: string;
  tournamentView?: string;
  timestamp: number;
}

export interface AppState {
  currentUser: any | null;
  currentGame: PongGame3D | null;
  currentGameId: string | null;
  currentTournament: any | null;
  currentTournamentId: string | null;
  tournamentUI: TournamentUI | null;
  isSpectatorMode: boolean;
  matchmakingWs: WebSocket | null;
  activeTournamentsWs: WebSocket | null;
  statusManager: StatusManager | null;
  statsManager: StatsManager | null;
  currentSection: string | null;
  currentGameMode: string | null;
  activeTournamentsData: any[];
  userReady: Promise<any>;
  _resolveUserReady: UserResolver;
  navigationHistory: NavigationHistoryEntry[];
}

export const appState: AppState = (() => {
  let resolveUserFn: UserResolver = () => {};
  const userReadyPromise = new Promise<any>(resolve => {
    resolveUserFn = resolve;
  });

  return {
    currentUser: null,
    currentGame: null,
    currentGameId: null,
    currentTournament: null,
    currentTournamentId: null,
    tournamentUI: null,
    isSpectatorMode: false,
    matchmakingWs: null,
    activeTournamentsWs: null,
    statusManager: null,
    statsManager: null,
    currentSection: 'hero',
    currentGameMode: null,
    activeTournamentsData: [],
    userReady: userReadyPromise,
    _resolveUserReady: resolveUserFn,
    navigationHistory: [],
  };
})();