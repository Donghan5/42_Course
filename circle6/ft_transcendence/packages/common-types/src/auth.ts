// apps/frontend/src/types/auth.ts
import { User } from './user';

export interface LocalLoginRequest {
    loginInput: string; // nickname or email
    password: string;
}

export interface LocalRegisterRequest {
    nickname: string;
    email: string;
    password: string;
    confirmPassword: string;
}

export interface AuthResponse {
    success: boolean;
    message: string;
    error?: string;
}

export interface AuthStatus {
    isAuthenticated: boolean;
    user?: User;
    profileComplete?: boolean;
}

export type AuthProvider = 'google' | 'local';

export interface PasswordChangeRequest {
    currentPassword: string;
    newPassword: string;
    confirmNewPassword: string;
}
