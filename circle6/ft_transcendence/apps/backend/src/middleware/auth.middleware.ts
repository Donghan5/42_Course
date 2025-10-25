// apps/backend/src/middleware/auth.middleware.ts

import jwt from 'jsonwebtoken';
import { FastifyRequest, FastifyReply } from 'fastify';

export interface AuthenticatedRequest extends FastifyRequest {
    user?: {
        userId: number;
        profileComplete?: boolean;
		[key: string]: any; // Additional properties can be added as needed
    };
}

export const authMiddleware = async (request: AuthenticatedRequest, reply: FastifyReply) => {
    try {
        const token = request.cookies.auth_token;

        if (!token) {
            return reply.status(401).send({ error: 'Authentication required' });
        }

        const secret = process.env.JWT_SECRET;
        if (!secret) {
            throw new Error('JWT secret is not defined');
        }

        const decoded = jwt.verify(token, secret) as { userId: number; profileComplete: boolean };

        // commented out before
        request.user = {
            userId: decoded.userId,
            profileComplete: decoded.profileComplete
        };

        request.user = decoded;

        console.log('Auth middleware - decoded token:', decoded);
        console.log('Auth middleware - request.user:', request.user);

    } catch (error) {
        console.error('JWT verification error:', error);
        return reply.status(401).send({ error: 'Invalid authentication token' });
    }
};
