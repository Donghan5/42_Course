// packages/common-types/src/tournament.ts

export interface TournamentPlayer {
    id: string;
    nickname: string;
    rankPoints: number;
    avatarUrl?: string;
}

export interface BracketMatch {
    id: string;
    player1: TournamentPlayer | null;
    player2: TournamentPlayer | null;
    winner: TournamentPlayer | null;
    round: number;
    position: number;
    status: 'waiting' | 'confirming' | 'playing' | 'finished';
    confirmations: string[]; // player IDs who confirmed
    gameId?: string;
}

export interface Tournament {
    id: string;
    name: string;
    hostId: string;
    players: TournamentPlayer[];
    bracket: BracketMatch[];
    status: 'waiting' | 'active' | 'finished';
    currentRound: number;
    maxPlayers: number;
    createdAt: string;
    finishedAt?: string;
    winner?: TournamentPlayer;
}

export interface TournamentHistory {
    tournament: Tournament;
    myMatches: BracketMatch[];
    finalBracket: BracketMatch[];
}