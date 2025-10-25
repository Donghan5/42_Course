interface TournamentState {
    tournamentId: string;
    isCreator: boolean;
    tournamentName: string;
    lastActivity: number;
}

interface AppState {
    currentSection: string;
    tournament?: TournamentState;
    lastUser?: any;
}

class StateManager {
    private static readonly STORAGE_KEY = 'appState';
    private static readonly TOURNAMENT_KEY = 'tournamentState';
    private static readonly EXPIRY_TIME = 30 * 60 * 1000; // 30 minutes

    static saveTournamentState(tournamentId: string, isCreator: boolean, tournamentName: string): void {
        const state: TournamentState = {
            tournamentId,
            isCreator,
            tournamentName,
            lastActivity: Date.now()
        };

        localStorage.setItem(this.TOURNAMENT_KEY, JSON.stringify(state));
        console.log('Tournament state saved:', state);
    }

    static getTournamentState(): TournamentState | null {
        try {
            const saved = localStorage.getItem(this.TOURNAMENT_KEY);
            if (!saved) return null;

            const state: TournamentState = JSON.parse(saved);

            if (Date.now() - state.lastActivity > this.EXPIRY_TIME) {
                this.clearTournamentState();
                return null;
            }

            return state;
        } catch (error) {
            console.error('Error retrieving tournament state:', error);
            this.clearTournamentState();
            return null;
        }
    }

    static clearTournamentState(): void {
        localStorage.removeItem(this.TOURNAMENT_KEY);
        console.log('Tournament state cleared');
    }

    static saveCurrentSection(section: string): void {
        const state: AppState = {
            currentSection: section,
            tournament: this.getTournamentState() || undefined
        };

        localStorage.setItem(this.STORAGE_KEY, JSON.stringify(state));
    }

    static getCurrentSection(): string | null {
        try {
            const saved = localStorage.getItem(this.STORAGE_KEY);
            if (!saved) return null;

            const state: AppState = JSON.parse(saved);
            return state.currentSection;
        } catch (error) {
            console.error('Error retrieving current section:', error);
            return null;
        }
    }

    static updateTournamentActivity(): void {
        const state = this.getTournamentState();
        if (state) {
            state.lastActivity = Date.now();
            localStorage.setItem(this.TOURNAMENT_KEY, JSON.stringify(state));
        }
    }

    static isInTournament(): boolean {
        return this.getTournamentState() !== null;
    }

    static getTournamentId(): string | null {
        const state = this.getTournamentState();
        return state ? state.tournamentId : null;
    }
}

export default StateManager;