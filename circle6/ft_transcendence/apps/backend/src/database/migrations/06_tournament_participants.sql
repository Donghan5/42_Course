CREATE TABLE IF NOT EXISTS tournament_participants (
    tournament_id TEXT,
    user_id INTEGER,
    joined_at TEXT DEFAULT (datetime('now')),
    PRIMARY KEY (tournament_id, user_id),
    FOREIGN KEY (tournament_id) REFERENCES tournaments(id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE INDEX idx_tournament_participants_user ON tournament_participants(user_id);