// apps/backend/src/routes/api/users/login/google.controller.ts
import { FastifyInstance } from 'fastify';
import GoogleService from '../../../../auth/google.service';

export default async function (fastify: FastifyInstance) {
    fastify.get('/callback', async (request: any, reply: any) => {
        try {
            const { token } = await (fastify as any).googleOAuth2.getAccessTokenFromAuthorizationCodeFlow(request);

            const ourJwtToken = await GoogleService.handleGoogleLogin(token);

            reply.setCookie('auth_token', ourJwtToken, {
                httpOnly: true,
                secure: true,
                path: '/',
                sameSite: 'lax'
            });

            const frontendUrl = process.env.FRONTEND_URL;
            if (!frontendUrl) {
                throw new Error("FRONTEND_URL environment variable is not set!");
            }

            const redirectUrl = `${frontendUrl}?auth=success`;
            return reply.redirect(redirectUrl);
            // return reply.redirect(frontendUrl); // Redirect to the dashboard or any other page
        } catch (error) {
            console.error('======================================================');
            console.error('CRITICAL ERROR in /login/google/callback:');
            console.error('Full Error Object:', error); // 에러 객체 전체를 출력
            console.error('======================================================');

            const message = error instanceof Error ? error.message : 'An unknown error occurred';
            reply.status(500).send({
                message: 'Internal Server Error (google.controller.ts)',
                errorDetails: message
            });
        }
    });
}
