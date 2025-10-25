import { FastifyInstance } from 'fastify';
import LocalAuthService from '../../../../auth/local.service';
import { AuthenticatedRequest, authMiddleware } from '../../../../middleware/auth.middleware';
import { PasswordChangeRequest } from '@trans/common-types';

export default async function (fastify: FastifyInstance) {
	// Route to handle local login by nickname (registering user)
	fastify.post('/register', async (request: any, reply: any) => {
		try {
			const { nickname, email, password, confirmPassword } = request.body;

			if (!nickname || !email || !password ) {
				return reply.status(400).send({ error: 'Nickname, email, and password are required' });
			}

			if (password !== confirmPassword) {
				return reply.status(400).send({ error: 'Passwords do not match' });
			}

			if (password.length < 8) {
				return reply.status(400).send({ error: 'Password must be at least 8 characters long' });
			}

			if (nickname.length < 3 || nickname.length > 20) {
				return reply.status(400).send({ error: 'Nickname must be between 3 and 20 characters long' });
			}


			// email regex validation
			const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
			if (!emailRegex.test(email)) {
				return reply.status(400).send({ error: 'Invalid email format' });
			}

			const token = await LocalAuthService.register(nickname, email, password);


			return reply.send({
				success: true,
				message: 'User registered successfully',
				requiresLogin: true
			});
		}
		catch (error) {
			console.error('Registration error:', error);
			const message = error instanceof Error ? error.message : 'Registration failed';
            return reply.status(400).send({ error: message });
		}
	});


	// login route for local authentication
	fastify.post('/login', async (request: any, reply: any) => {
		try {
			const { loginInput, password } = request.body;

			if (!loginInput || !password) {
				return reply.status(400).send({ error: 'Nickname and password are required' });
			}

			let token: string;

			const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
			if (emailRegex.test(loginInput)) {
				token = await LocalAuthService.loginByEmail(loginInput, password);
			} else {
				token = await LocalAuthService.login(loginInput, password);
			}

			reply.setCookie('auth_token', token, {
				httpOnly: true,
				secure: true,
				path: '/',
				sameSite: 'lax'
			});

			return reply.send({
				success: true,
				message: 'User logged in successfully',
			});
		}
		catch (error) {
			console.error('Login error:', error);
			const message = error instanceof Error ? error.message : 'Login failed';
			return reply.status(401).send({ error: message });
		}
	});

	fastify.post('/change-password', {
        preHandler: [authMiddleware]
    }, async (request: AuthenticatedRequest, reply: any) => {
        try {
            const { currentPassword, newPassword, confirmNewPassword } = request.body as PasswordChangeRequest;

            if (!currentPassword || !newPassword || !confirmNewPassword) {
                return reply.status(400).send({
                    error: 'Please provide all required fields: current password, new password, and confirm new password'
                });
            }

            if (newPassword !== confirmNewPassword) {
                return reply.status(400).send({ error: 'New passwords do not match' });
            }

            if (newPassword.length < 8) {
                return reply.status(400).send({ error: 'New password must be at least 8 characters long' });
            }

            if (currentPassword === newPassword) {
                return reply.status(400).send({ error: 'New password must be different from current password' });
            }

            const userId = request.user!.userId;

            await LocalAuthService.changePassword(userId, currentPassword, newPassword);

            return reply.send({
                success: true,
                message: 'Password changed successfully'
            });
			

        } catch (error) {
            console.error('Change password error:', error);
            const message = error instanceof Error ? error.message : 'Password change failed';

            const statusCode = message.includes('Current password is incorrect') ? 400 : 500;

            return reply.status(statusCode).send({ error: message });
        }
    });

}
