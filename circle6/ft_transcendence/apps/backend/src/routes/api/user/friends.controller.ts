import { FastifyInstance, FastifyRequest, FastifyReply, RouteShorthandOptions } from 'fastify';
import jwt from 'jsonwebtoken';
import { dbAll, dbGet, dbRun } from '../../../database/helpers';
import { OnlineStatusManager } from '../../../core/status/online-status-manager';

async function verifyJwt(request: FastifyRequest, reply: FastifyReply) {
    try {
        const token = (request.cookies as any).auth_token;
        if (!token) {
            return reply.code(401).send({ error: 'Unauthorized' });
        }
        const decoded = jwt.verify(token, process.env.JWT_SECRET!) as { userId: number };
        request.user = decoded;
    } catch (err) {
        return reply.code(401).send({ error: 'Unauthorized' });
    }
}

interface FriendRequest { Body: { friendNickname: string } }
interface FriendResponseRequest { Body: { requestId: number } }
interface FriendIdParam { Params: { friendId: number } }

export default async function friendsRoute(fastify: FastifyInstance) {
    const opts: RouteShorthandOptions = {
        preHandler: [verifyJwt]
    };
    const statusManager = OnlineStatusManager.getInstance();

    fastify.get('/friends/all', opts, async (request: FastifyRequest, reply: FastifyReply) => {
        const userId = request.user?.userId;
        if (!userId) return reply.code(401).send({ error: 'Unauthorized' });

        try {
            const friends = await dbAll(`
                SELECT u.id, u.nickname, u.avatar_url FROM users_friends uf
                JOIN users u ON u.id = uf.friend_id WHERE uf.user_id = ?
            `, [userId]);
            const receivedRequests = await dbAll(`
                SELECT fr.id, u.id as user_id, u.nickname, u.avatar_url FROM friend_requests fr
                JOIN users u ON u.id = fr.requester_id WHERE fr.receiver_id = ? AND fr.status = 'pending'
            `, [userId]);
            const sentRequests = await dbAll(`
                SELECT fr.id, u.id as user_id, u.nickname, u.avatar_url FROM friend_requests fr
                JOIN users u ON u.id = fr.receiver_id WHERE fr.requester_id = ? AND fr.status = 'pending'
            `, [userId]);

            return reply.send({ friends, receivedRequests, sentRequests });
        } catch (error) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Internal Server Error' });
        }
    });

    fastify.post<FriendRequest>('/friends/request', opts, async (request, reply) => {
        const userId = request.user?.userId;
        const { friendNickname } = request.body;
        if (!userId) return reply.code(401).send({ error: 'Unauthorized' });

        try {
            const friend = await dbGet('SELECT id FROM users WHERE nickname = ? AND id != ?', [friendNickname, userId]);
            if (!friend) return reply.code(404).send({ error: 'User not found or you tried to add yourself.' });

            const existingFriend = await dbGet(
                'SELECT * FROM users_friends WHERE (user_id = ? AND friend_id = ?) OR (user_id = ? AND friend_id = ?)',
                [userId, friend.id, friend.id, userId]
            );
            if (existingFriend) return reply.code(409).send({ error: 'You are already friends.' });

            const existingRequest = await dbGet(
                "SELECT * FROM friend_requests WHERE ((requester_id = ? AND receiver_id = ?) OR (requester_id = ? AND receiver_id = ?)) AND status = 'pending'",
                [userId, friend.id, friend.id, userId]
            );
            if (existingRequest) return reply.code(409).send({ error: 'Friend request already pending.' });

            await dbRun(
                "DELETE FROM friend_requests WHERE ((requester_id = ? AND receiver_id = ?) OR (requester_id = ? AND receiver_id = ?)) AND status = 'rejected'",
                [userId, friend.id, friend.id, userId]
            );

            const result = await dbRun("INSERT INTO friend_requests (requester_id, receiver_id) VALUES (?, ?)", [userId, friend.id]);
            const requester = await dbGet('SELECT nickname, avatar_url FROM users WHERE id = ?', [userId]);
            statusManager.sendFriendUpdate(friend.id, 'friend_request_received', {
                id: result.lastID,
                user_id: userId,
                nickname: requester.nickname,
                avatar_url: requester.avatar_url
            });
            
            return reply.code(201).send({ message: 'Friend request sent' });
        } catch (error) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Internal Server Error' });
        }
    });

    fastify.put<FriendResponseRequest>('/friends/accept', opts, async (request, reply) => {
        const userId = request.user?.userId;
        const { requestId } = request.body;
        if (!userId) return reply.code(401).send({ error: 'Unauthorized' });

        try {
            const req = await dbGet("SELECT * FROM friend_requests WHERE id = ? AND receiver_id = ? AND status = 'pending'", [requestId, userId]);
            if (!req) return reply.code(404).send({ error: 'Request not found.' });

            await dbRun('BEGIN TRANSACTION');
            await dbRun("UPDATE friend_requests SET status = 'accepted' WHERE id = ?", [requestId]);
            await dbRun("INSERT INTO users_friends (user_id, friend_id) VALUES (?, ?)", [req.receiver_id, req.requester_id]);
            await dbRun("INSERT INTO users_friends (user_id, friend_id) VALUES (?, ?)", [req.requester_id, req.receiver_id]);
            await dbRun('COMMIT');
            statusManager.sendFriendUpdate(req.requester_id, 'friend_request_accepted', {
                friendId: userId,
            });

            statusManager.sendFriendUpdate(userId, 'friends_list_updated', {});

            return reply.send({ message: 'Friend request accepted' });
        } catch (error) {
            await dbRun('ROLLBACK');
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Internal Server Error' });
        }
    });

    fastify.put<FriendResponseRequest>('/friends/reject', opts, async (request, reply) => {
        const userId = request.user?.userId;
        const { requestId } = request.body;
        if (!userId) return reply.code(401).send({ error: 'Unauthorized' });

        try {
            await dbRun("UPDATE friend_requests SET status = 'rejected' WHERE id = ? AND receiver_id = ?", [requestId, userId]);
            return reply.send({ message: 'Friend request rejected' });
        } catch (error) {
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Internal Server Error' });
        }
    });

	fastify.delete<FriendIdParam>('/friends/:friendId', opts, async (request, reply) => {
        const userId = request.user?.userId;
        const { friendId } = request.params;
        if (!userId) return reply.code(401).send({ error: 'Unauthorized' });

        // Parse friendId to number (URL params are always strings!)
        const friendIdNum = parseInt(friendId as any, 10);
        
        if (isNaN(friendIdNum)) {
            return reply.code(400).send({ error: 'Invalid friend ID' });
        }

        try {
            await dbRun('BEGIN TRANSACTION');
            await dbRun('DELETE FROM users_friends WHERE (user_id = ? AND friend_id = ?) OR (user_id = ? AND friend_id = ?)', [userId, friendIdNum, friendIdNum, userId]);
            await dbRun('DELETE FROM friend_requests WHERE (requester_id = ? AND receiver_id = ?) OR (requester_id = ? AND receiver_id = ?)', [userId, friendIdNum, friendIdNum, userId]);
            await dbRun('COMMIT');

            statusManager.sendFriendUpdate(friendIdNum, 'friend_removed', { friendId: userId });
            statusManager.sendFriendUpdate(userId, 'friend_removed', { friendId: friendIdNum });

            return reply.send({ message: 'Friend removed' });
        } catch (error) {
            await dbRun('ROLLBACK');
            fastify.log.error(error);
            return reply.code(500).send({ error: 'Internal Server Error' });
        }
    });
}
