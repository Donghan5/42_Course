import { FastifyInstance, FastifyRequest, FastifyReply } from 'fastify';
import jwt from 'jsonwebtoken';
import { dbGet } from '../../../database/helpers';
import { User } from '@trans/common-types';

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

export default async function authStatusRoute(fastify: FastifyInstance) {
    fastify.get('/me', { preHandler: [verifyJwt] }, async (request: any, reply: any) => {
        const userId = request.user.userId;

        try {
            // First, let's check what columns actually exist
            const user = await dbGet('SELECT * FROM users WHERE id = ?', [userId]);

            if (!user) {
                return reply.code(404).send({ error: 'User not found' });
            }

            // Build response based on what columns actually exist
            const userProfile: any = {
                id: user.id,
                email: user.email,
                name: user.name,
            };

            // Only add these fields if they exist
            if ('nickname' in user) {
                userProfile.nickname = user.nickname;
            }
            if ('avatar_url' in user) {
                userProfile.avatarUrl = user.avatar_url;
            }
            if ('profile_setup_complete' in user) {
                userProfile.profileComplete = user.profile_setup_complete || false;
            } else {
                // If column doesn't exist, assume profile is not complete
                userProfile.profileComplete = false;
            }

            return reply.send(userProfile);
        } catch (error: any) {
            fastify.log.error(error);
            const statusCode = error.statusCode || 500;
            return reply.code(statusCode).send({ error: error.message });
        }
    });

    fastify.get('/logout', async (request, reply) => {
        reply.clearCookie('auth_token', {
            path: '/',
            httpOnly: true,
            secure: true,
            sameSite: 'lax'
        });

		// Read the base URL from the environment variables
		const frontendUrl = process.env.FRONTEND_URL;

		// Construct the full redirect URL dynamically
		return reply.redirect(`${frontendUrl}/login.html`);
    });
}
