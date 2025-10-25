import bcrypt from 'bcrypt';
import jwt from 'jsonwebtoken';
import { dbGet, dbRun } from '../database/helpers';

class LocalAuthService {
	public static async register(nickname: string, email: string, password: string): Promise<void> {
		const existingUser = await dbGet(
			`SELECT * FROM users WHERE email = ? OR nickname = ?`,
			[email, nickname]
		);

		if (existingUser) {
			if (existingUser.email === email) {
				throw new Error('Email already in use');
			}
			if (existingUser.nickname === nickname) {
				throw new Error('Nickname already in use');
			}
		}

		// Password hashing
		const saltRounds = 12;
		const passwordHash = await bcrypt.hash(password, saltRounds);

		const result = await dbRun(
			`INSERT INTO users (nickname, email, password_hash, auth_provider, name, profile_setup_complete) VALUES (?, ?, ?, ?, ?, ?)`,
			[nickname, email, passwordHash, 'local', nickname, true]
		);
	}

	public static async login(nickname: string, password: string): Promise<string> {
		// Find the user by nickname (Just for local auth)
		const user = await dbGet(`SELECT * FROM users WHERE nickname = ? AND auth_provider = ?`, [nickname, 'local']);

		if (!user) {
			throw new Error('User not found');
		}

		if (!user.password_hash) {
			throw new Error('Not a local user');
		}

		const isPasswordValid = await bcrypt.compare(password, user.password_hash);
		if (!isPasswordValid) {
			throw new Error('Invalid password');
		}

		const jwtPayload = {
			userId: user.id,
			profileComplete: user.profile_setup_complete || false,
		};

		const secret = process.env.JWT_SECRET;
		if (!secret) {
			throw new Error('JWT secret is not defined');
		}

		return jwt.sign(jwtPayload, secret, { expiresIn: '24h' });
	}

	public static async loginByEmail(email: string, password: string): Promise<string> {
		const user = await dbGet(`SELECT * FROM users WHERE email = ? AND auth_provider = ?`, [email, 'local']);

		if (!user) {
			throw new Error('User not found');
		}

		if (!user.password_hash) {
			throw new Error('Not a local user');
		}

		const isPasswordValid = await bcrypt.compare(password, user.password_hash);
		if (!isPasswordValid) {
			throw new Error('Invalid password');
		}

		const jwtPayload = {
			userId: user.id,
			profileComplete: user.profile_setup_complete || false,
		};

		const secret = process.env.JWT_SECRET;
		if (!secret) {
			throw new Error('JWT secret is not defined');
		}

		return jwt.sign(jwtPayload, secret, { expiresIn: '24h' });
	}

	public static async changePassword(userId: number, oldPassword: string, newPassword: string): Promise<void> {
		const user = await dbGet(`SELECT * FROM users WHERE id = ? AND auth_provider = ?`, [userId, 'local']);

		if (!user) {
			throw new Error('User not found');
		}

		if (!user.password_hash) {
			throw new Error('Not a local user');
		}

		const isOldPasswordValid = await bcrypt.compare(oldPassword, user.password_hash);
		if (!isOldPasswordValid) {
			throw new Error('Invalid old password');
		}

		const saltRounds = 12;
		const newPasswordHash = await bcrypt.hash(newPassword, saltRounds);

		const result = await dbRun(
			`UPDATE users SET password_hash = ? WHERE id = ? AND auth_provider = ?`,
			[newPasswordHash, userId, 'local']
		);

		if (result.changes === 0) {
			throw new Error('Failed to change password');
		}
	}
}

export default LocalAuthService;
