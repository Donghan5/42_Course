CREATE TABLE IF NOT EXISTS users_friends (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id INTEGER NOT NULL,
  friend_id INTEGER NOT NULL,
  status TEXT CHECK(status IN ('accepted', 'blocked')) DEFAULT 'accepted',
  created_at TEXT DEFAULT (datetime('now', 'localtime')),
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
  FOREIGN KEY (friend_id) REFERENCES users(id) ON DELETE CASCADE,
  UNIQUE(user_id, friend_id)
);

CREATE INDEX idx_users_friends_user ON users_friends(user_id);
CREATE INDEX idx_users_friends_friend ON users_friends(friend_id);
