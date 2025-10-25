CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    google_id TEXT UNIQUE,
    email TEXT NOT NULL,
    name TEXT,
    token TEXT,
    nickname TEXT UNIQUE,
    avatar_url TEXT,
	password_hash VARCHAR(255),
	auth_provider TEXT DEFAULT 'google',
    profile_setup_complete BOOLEAN DEFAULT FALSE,
    last_seen TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    rating INTEGER DEFAULT 1000,
    wins INTEGER DEFAULT 0,
    losses INTEGER DEFAULT 0,
    tournament_points INTEGER DEFAULT 0,
    tournament_wins INTEGER DEFAULT 0
);
