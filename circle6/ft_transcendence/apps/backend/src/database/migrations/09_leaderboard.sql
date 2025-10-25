CREATE VIEW IF NOT EXISTS leaderboard AS
SELECT 
    u.id,
    u.nickname,
    u.rating,
    u.wins,
    u.losses,
    u.tournament_points,
    u.tournament_wins,
    CASE 
        WHEN (u.wins + u.losses) > 0 
        THEN ROUND(CAST(u.wins AS FLOAT) / (u.wins + u.losses) * 100, 2)
        ELSE 0 
    END as win_rate
FROM users u
WHERE u.profile_setup_complete = 1
ORDER BY u.rating DESC, u.tournament_points DESC;