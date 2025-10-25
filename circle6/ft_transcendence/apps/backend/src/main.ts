import fastify, { FastifyInstance } from 'fastify';
import cors from '@fastify/cors';
import websocketPlugin from '@fastify/websocket';
import sensible from '@fastify/sensible';
import path from 'path';
import fastifyOAuth2 from '@fastify/oauth2';
import cookie from '@fastify/cookie';
import * as dotenv from 'dotenv';
import { initializeDatabase } from './database/db';
import multipart from '@fastify/multipart';
import privacyRoute from './routes/api/user/privacy.controller';
import profileRoute from './routes/api/user/profile.controller';

dotenv.config({ path: path.resolve(__dirname, '../../../../.env')});

import gameRoute from './routes/api/games';
import googleLoginRoute from './routes/api/user/login/google.controller';
import authStatusRoute from './routes/api/auth/status.controller';
import friendsRoute from './routes/api/user/friends.controller';
import localLoginRoute from './routes/api/user/login/local.controller';
import statusRoute from './routes/api/user/status.controller';
import statsRoute from './routes/api/user/stats.controller';
import tournamentRoutes from './routes/api/tournament';
//CHAT
import chatRoutes from './routes/api/chat';

export async function buildServer(): Promise<FastifyInstance> {
    const server = fastify({ logger: true, trustProxy: true });
	
	// get URL from env
	server.register(cors, {
        origin: process.env.FRONTEND_URL,
        credentials: true
    });

    server.register(websocketPlugin);
    server.register(sensible);
	server.register(multipart);
    await server.register(cookie);


	const backendUrl = process.env.BACKEND_URL;
    if (!backendUrl) {
        throw new Error("BACKEND_URL is not set");
    }
    
    const url = new URL(backendUrl);
    url.protocol = 'https';
    const callbackUri = `${url.toString()}api/users/login/google/callback`;

    console.log('Forced HTTPS Google OAuth2 callback URI:', callbackUri);

    // Google OAuth2
    server.register<any>(fastifyOAuth2, {
        name: 'googleOAuth2',
        scope: ['profile', 'email'],
        credentials: {
            client: {
                id: process.env.GOOGLE_CLIENT_ID,
                secret: process.env.GOOGLE_CLIENT_SECRET
            },
            auth: fastifyOAuth2.GOOGLE_CONFIGURATION
        },
        startRedirectPath: '/api/users/login/google',
        callbackUri: callbackUri
    });

    // register Routes API
    server.register(gameRoute, { prefix: '/api/games' });
    server.register(googleLoginRoute, { prefix: '/api/users/login/google' });
    server.register(authStatusRoute, { prefix: '/api/auth' });
	server.register(profileRoute, { prefix: '/api/user' });
	server.register(friendsRoute, { prefix: '/api/user' });
    server.register(statusRoute, { prefix: '/api/user' });
    server.register(statsRoute, { prefix: '/api/user/stats' });
	server.register(localLoginRoute, { prefix: '/api/users/login/local' });
    server.register(privacyRoute, { prefix: '/api/user' });
    await server.register(tournamentRoutes, { prefix: '/api/tournament' });
    //CHAT
    await server.register(chatRoutes, { prefix: '/api/chat' });

    server.setErrorHandler((error, request, reply) => {
        request.log.error(error);
        reply.status(500).send({
            error: 'Internal Server Error',
            message: 'Something went wrong on the server.',
        });
    });

    server.setNotFoundHandler((request, reply) => {
        reply.status(404).send({
            error: 'Not Found',
            message: 'The requested resource could not be found.',
        });
    });
    return server;
}

async function start() {
	//Fail-Fast
	if (!process.env.FRONTEND_URL || !process.env.BACKEND_URL) {
        console.error("FATAL: FRONTEND_URL and BACKEND_URL must be set in the environment.");
        process.exit(1);
    }

    try {
        console.log('Initializing database...');
        const db = await initializeDatabase();

        console.log('Initializing tournament manager...');
        const { tournamentManager } = await import('./core/tournament/tournament-manager');
        // await tournamentManager.initializeTournaments();
        console.log('Tournament manager initialized successfully');

        // Temporary: Force check if users table exists
        try {
            const result = await new Promise((resolve, reject) => {
                db.get("SELECT name FROM sqlite_master WHERE type='table' AND name='users'", (err, row) => {
                    if (err) reject(err);
                    else resolve(row);
                });
            });
            console.log('Users table check result:', result);
        } catch (error) {
            console.error('Error checking users table:', error);
        }

        const server = await buildServer();
        const port = Number(process.env.PORT) || 3000;
        const host = process.env.HOST || '0.0.0.0';

        await server.listen({ port, host });
        server.log.info(`Server is running at http://${host}:${port}`);

    } catch (error) {
        console.error('Error starting server:', error);
        process.exit(1);
    }
}

// Graceful shutdown
const signals: NodeJS.Signals[] = ['SIGINT', 'SIGTERM'];
signals.forEach((signal) => {
    process.on(signal, async () => {
        console.log(`Received ${signal}, shutting down gracefully...`);
        console.log('Server shutting down...');
        process.exit(0);
    });
});

if (require.main === module) {
    start();
}