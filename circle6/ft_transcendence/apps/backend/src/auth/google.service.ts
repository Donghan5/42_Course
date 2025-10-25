// apps/backend/src/auth/google.service.ts
import { OAuth2Token } from '@fastify/oauth2';
import { User } from '@trans/common-types'
import { FastifyInstance } from 'fastify';
import jwt from 'jsonwebtoken';
import { dbGet, dbRun } from '../database/helpers';

class GoogleService {
    public static async handleGoogleLogin(token: any): Promise<string> {
        const response = await fetch('https://www.googleapis.com/oauth2/v2/userinfo', {
            headers: {
                Authorization: `Bearer ${token.access_token}`,
            },
        });

        if (!response.ok) {
            const errorBody = await response.text();
            console.error(`Google API error: ${response.statusText}`, errorBody);
            throw new Error(`Google API error: ${response.statusText}`);
        }

        const googleUser = await response.json();
        const userName = googleUser.name || googleUser.email.split('@')[0];

        let user: User | undefined = await dbGet('SELECT * FROM "users" WHERE email = ?', [googleUser.email]);

		if (!user) {     // If user does not exist, create a new user
			console.log('Creating new user in the database:', googleUser.email);
			const result = await dbRun(
				'INSERT INTO "users" (google_id, email, name, token) VALUES (?, ?, ?, ?)',
				[googleUser.id, googleUser.email, googleUser.name, token.access_token]
			);
			user = await dbGet('SELECT * FROM "users" WHERE id = ?', [result.lastID]);
		} else {
			await dbRun('UPDATE "users" SET token = ?, google_id = ? WHERE id = ?', [token.access_token, googleUser.id, user.id]);
			user = await dbGet('SELECT * FROM "users" WHERE id = ?', [user.id]);
		}

        if (!user) {
            throw new Error('User not found after creation/update');
        }

        const secret = process.env.JWT_SECRET;
        if (!secret) {
            throw new Error('JWT secret is not defined');
        }

        const jwtPayload = {
            userId: user.id,
            profileComplete: user.profile_setup_complete || false,
        };

        const ourJwtToken = jwt.sign(jwtPayload, secret, { expiresIn: '24h' });
        return ourJwtToken;
    }
}

export default GoogleService;
