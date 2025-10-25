// apps/frontend/src/services/auth-service.ts

import { LocalLoginRequest, LocalRegisterRequest, AuthResponse, AuthStatus, PasswordChangeRequest } from '@trans/common-types';

export class AuthService {
    private static readonly BASE_URL = '/api/users/login/local';
    private static readonly AUTH_STATUS_URL = '/api/auth/me';

    /**
     * local user login
     */
    static async localLogin(loginData: LocalLoginRequest): Promise<AuthResponse> {
        try {
            const response = await fetch(`${this.BASE_URL}/login`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                credentials: 'include',
                body: JSON.stringify(loginData)
            });

            const result = await response.json();

            if (response.ok) {
                return { success: true, message: result.message };
            } else {
                return { success: false, message: '', error: result.error };
            }
        } catch (error) {
            console.error('Local login error:', error);
            return {
                success: false,
                message: '',
                error: 'Error occurred while logging in'
            };
        }
    }

	/**
	 * @description local user registration
	 * @param registerData
	 * @returns
	 */
    static async localRegister(registerData: LocalRegisterRequest): Promise<AuthResponse> {
        try {
            const response = await fetch(`${this.BASE_URL}/register`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                credentials: 'include',
                body: JSON.stringify(registerData)
            });

            const result = await response.json();

            if (response.ok) {
                return { success: true, message: result.message };
            } else {
                return { success: false, message: '', error: result.error };
            }
        } catch (error) {
            console.error('Local register error:', error);
            return {
                success: false,
                message: '',
                error: 'Error occurred while registering'
            };
        }
    }

    /**
	 * @description Checks the authentication status of the user
	 * @returns AuthStatus
	 */
    static async checkAuthStatus(): Promise<AuthStatus> {
        try {
            const response = await fetch(this.AUTH_STATUS_URL, {
                credentials: 'include'
            });

            if (response.ok) {
                const userData = await response.json();
                return {
                    isAuthenticated: true,
                    user: userData,
                    profileComplete: userData.profileSetupComplete
                };
            } else {
                return { isAuthenticated: false };
            }
        } catch (error) {
            console.error('Auth status check error:', error);
            return { isAuthenticated: false };
        }
    }

    /**
     * Change password (local account only)
     */
    static async changePassword(passwordData: PasswordChangeRequest): Promise<AuthResponse> {
        try {
            const response = await fetch(`${this.BASE_URL}/change-password`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                credentials: 'include',
                body: JSON.stringify(passwordData)
            });

            const result = await response.json();

            if (response.ok) {
                return { success: true, message: result.message };
            } else {
                return { success: false, message: '', error: result.error };
            }
        } catch (error) {
            console.error('Password change error:', error);
            return {
                success: false,
                message: '',
                error: 'Error occurred while changing password'
            };
        }
    }

    /**
     * Input validation utility
     */
    static validateLoginInput(loginData: LocalLoginRequest): string | null {
        if (!loginData.loginInput || !loginData.password) {
            return 'Input all fields';
        }

        if (loginData.loginInput.trim().length === 0) {
            return 'Please enter a nickname or email';
        }

        if (loginData.password.length < 1) {
            return 'Please enter a password';
        }

        return null;
    }

    static validateRegisterInput(registerData: LocalRegisterRequest): string | null {
        if (!registerData.nickname || !registerData.email || !registerData.password || !registerData.confirmPassword) {
            return 'Input all fields';
        }

        if (registerData.nickname.length < 2 || registerData.nickname.length > 20) {
            return 'Nickname must be between 2 and 20 characters';
        }

        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        if (!emailRegex.test(registerData.email)) {
            return 'Invalid email format';
        }

        if (registerData.password.length < 8) {
            return 'Password must be at least 8 characters long';
        }

        if (registerData.password !== registerData.confirmPassword) {
            return 'Password confirmation does not match';
        }

        return null;
    }

    static validatePasswordChange(passwordData: PasswordChangeRequest): string | null {
        if (!passwordData.currentPassword || !passwordData.newPassword || !passwordData.confirmNewPassword) {
            return 'Input all fields';
        }

        if (passwordData.newPassword.length < 8) {
            return 'New password must be at least 8 characters long';
        }

        if (passwordData.newPassword !== passwordData.confirmNewPassword) {
            return 'Password confirmation does not match';
        }

        if (passwordData.currentPassword === passwordData.newPassword) {
            return 'New password must be different from the current password';
        }

        return null;
    }
}
