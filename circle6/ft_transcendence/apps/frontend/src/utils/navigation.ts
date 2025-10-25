import { appState, NavigationHistoryEntry } from '../state/state';
import { showSection } from '../services/ui';
import { TournamentUI } from '../tournament/tournament-ui';
import { openTournament } from '../tournament/tournament-services';

// Maximum navigation history size to prevent memory issues
const MAX_HISTORY_SIZE = 50;

/**
 * Push current location to navigation history before navigating
 */
export function pushToNavigationHistory(sectionId: string, tournamentId?: string, tournamentView?: string) {
    const entry: NavigationHistoryEntry = {
        sectionId,
        tournamentId,
        tournamentView,
        timestamp: Date.now()
    };
    
    // Don't add duplicate consecutive entries
    const lastEntry = appState.navigationHistory[appState.navigationHistory.length - 1];
    if (lastEntry && 
        lastEntry.sectionId === sectionId && 
        lastEntry.tournamentId === tournamentId &&
        lastEntry.tournamentView === tournamentView) {
        console.log('Skipping duplicate navigation history entry');
        return;
    }
    
    appState.navigationHistory.push(entry);
    
    // Limit history size
    if (appState.navigationHistory.length > MAX_HISTORY_SIZE) {
        appState.navigationHistory.shift();
    }
    
    console.log('Navigation history pushed:', entry);
    console.log('History stack size:', appState.navigationHistory.length);
}

/**
 * Navigate back using the navigation history stack
 */
export function navigateBack() {
    console.log('navigateBack called, history size:', appState.navigationHistory.length);
    
    // Pop and GET where we should go (the last entry is where we came from)
    const previousEntry = appState.navigationHistory.pop();
    
    if (!previousEntry) {
        console.log('No previous entry, going to hero');
        showSection('hero', true);
        return;
    }
    
    console.log('Navigating back to:', previousEntry);
    
    // Handle tournament navigation specially
    if (previousEntry.sectionId === 'tournament') {
        showSection('tournament', false); // Don't push to history again
        
        if (!appState.tournamentUI) {
            appState.tournamentUI = new TournamentUI('tournamentSection');
            if (appState.currentUser) {
                appState.tournamentUI.setCurrentUser(appState.currentUser);
            }
        }
        
        // Restore the specific tournament view
        if (previousEntry.tournamentId) {
            // If we were viewing a specific tournament, restore it
            openTournament(previousEntry.tournamentId).catch((error) => {
                console.error('Failed to restore tournament view:', error);
                // Fallback to tournament home if we can't restore the specific tournament
                appState.tournamentUI!.showTournamentHome();
            });
        } else {
            // Just show the tournament home/list
            appState.tournamentUI.showTournamentHome();
        }
    } else {
        // For non-tournament sections, just show the section
        showSection(previousEntry.sectionId as any, false); // Don't push to history again
    }
}

/**
 * Clear navigation history (useful when logging out or resetting)
 */
export function clearNavigationHistory() {
    appState.navigationHistory = [];
    console.log('Navigation history cleared');
}

/**
 * Get a summary of the navigation history for debugging
 */
export function getNavigationHistorySummary(): string {
    return appState.navigationHistory
        .map(entry => `${entry.sectionId}${entry.tournamentId ? `(${entry.tournamentId})` : ''}${entry.tournamentView ? `[${entry.tournamentView}]` : ''}`)
        .join(' → ');
}