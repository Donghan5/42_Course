-- This migration adds a permanent, non-login-able user record
-- to act as a placeholder for users who have deleted their accounts.
-- Using a negative ID (-99) is a safe way to ensure it never conflicts
-- with the auto-incrementing positive IDs of real users.

INSERT INTO users (id, nickname, email, name, auth_provider, profile_setup_complete, avatar_url)
VALUES (-99, '[Deleted User]', 'deleted-user@-99.local', 'Deleted User', 'system_deleted', 1, '/default-avatar.png')
ON CONFLICT(id) DO NOTHING;