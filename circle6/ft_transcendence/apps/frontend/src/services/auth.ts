import { appState } from "../state/state";
import { redirectGoogleLogin } from '../../auth/login/google';
import { AuthService } from '../../auth/services/auth-service';
import { StatusManager } from "../status/status-manager";
import { showAppScreen, showSection } from "../services/ui";
import { showNicknameSetupScreen, showFriendsScreen } from "./ui";
import { showProfileScreen } from "./user";

/**
 * @description Sets up all UI and logic for the local authentication section (tabs and forms).
 */
export function setupLocalAuthHandlers(): void {
    setupAuthTabs();
    showAuthTab('login'); // Initialize with the login tab active
    setupFormSubmissionHandlers();
}

/**
 * @description Manages switching between Login and Register forms using Tailwind classes.
 * @param tabName The name of the tab to display: 'login' or 'register'.
 */
function showAuthTab(tabName: 'login' | 'register'): void {
    const loginForm = document.getElementById('login-form');
    const registerForm = document.getElementById('register-form');
    const loginTab = document.querySelector('[data-tab="login"]');
    const registerTab = document.querySelector('[data-tab="register"]');

    if (!loginForm || !registerForm || !loginTab || !registerTab) return;

    // Hide both forms
    loginForm.classList.add('hidden');
    registerForm.classList.add('hidden');

    // Reset both tabs to inactive state
    loginTab.classList.remove('bg-yellow-300', 'text-black');
    loginTab.classList.add('bg-gray-200', 'text-gray-600');
    registerTab.classList.remove('bg-yellow-300', 'text-black');
    registerTab.classList.add('bg-gray-200', 'text-gray-600');

    // Show selected form and activate corresponding tab
    if (tabName === 'login') {
        loginForm.classList.remove('hidden');
        loginTab.classList.remove('bg-gray-200', 'text-gray-600');
        loginTab.classList.add('bg-yellow-300', 'text-black');
    } else if (tabName === 'register') {
        registerForm.classList.remove('hidden');
        registerTab.classList.remove('bg-gray-200', 'text-gray-600');
        registerTab.classList.add('bg-yellow-300', 'text-black');
    }

    clearAuthMessages();
}

/**
 * @description Attaches click event listeners to the auth tabs.
 */
function setupAuthTabs(): void {
    const tabs = document.querySelectorAll('.auth-tab');
    tabs.forEach(tab => {
        tab.addEventListener('click', (e) => {
            const tabName = (e.currentTarget as HTMLElement).dataset.tab as 'login' | 'register';
            if (tabName) {
                showAuthTab(tabName);
            }
        });
    });
}

/**
 * @description Clear all authentication error and success messages
 */
export function clearAuthMessages(): void {
    const messageElements = document.querySelectorAll('#loginError, #registerError, #loginSuccess, #registerSuccess');
    messageElements.forEach(el => {
        el.classList.add('hidden');
        el.textContent = '';
    });
}

/**
 * @description Setup authentication form submission event listeners
 */
function setupFormSubmissionHandlers(): void {
    const loginForm = document.getElementById('loginForm') as HTMLFormElement;
    const registerForm = document.getElementById('registerForm') as HTMLFormElement;

    if (loginForm) {
        loginForm.addEventListener('submit', handleLocalLogin);
    }

    if (registerForm) {
        registerForm.addEventListener('submit', handleLocalRegister);
    }

    console.log('Auth form event listeners setup complete');
}

/**
 * @description Shows an error message
 * @param elementId
 * @param message
 */
function showAuthError(elementId: string, message: string) {
    const errorEl = document.getElementById(elementId);
    if (errorEl) {
        errorEl.textContent = message;
        errorEl.classList.remove('hidden');
    }
}

/**
 * @description Shows a success message
 * @param elementId
 * @param message
 */
function showAuthSuccess(elementId: string, message: string) {
    const successEl = document.getElementById(elementId);
    if (successEl) {
        successEl.textContent = message;
        successEl.classList.remove('hidden');
    }
}

/**
 * @description Handles user registration
 * @param e - The submit event
 */
export async function handleLocalLogin(e: Event) {
    e.preventDefault();
    clearAuthMessages();

    const formData = new FormData(e.target as HTMLFormElement);
    const loginData = {
        loginInput: formData.get('loginInput') as string,
        password: formData.get('password') as string
    };

    const validationError = AuthService.validateLoginInput(loginData);
    if (validationError) {
        showAuthError('loginError', validationError);
        return;
    }

    const result = await AuthService.localLogin(loginData);

    if (result.success) {
        showAuthSuccess('loginSuccess', result.message);

        try {
            const userResponse = await fetch('/api/auth/me', { credentials: 'include' });
            if (userResponse.ok) {
                const userData = await userResponse.json();
                console.log('User data after login:', userData);
                appState.currentUser = userData;

                if (appState._resolveUserReady) {
                    appState._resolveUserReady(userData);
                }

                appState.statusManager = StatusManager.getInstance();
                await appState.statusManager!.initializeStatusConnection('', userData);
                console.log('StatusManager successfully initialized for new session.');
                
                if (!userData.profileComplete) {
                    showNicknameSetupScreen();
                } else {
                    showAppScreen(userData);
                }

                //CHAT - Déclencher l'événement de connexion
                document.dispatchEvent(new Event('userLoggedIn'));
            } else {
                throw new Error('Failed to fetch user data');
            }
        } catch (error) {
            console.error('Error fetching user data:', error);
            showAuthError('loginError', 'Failed to load user profile');
        }
    } else {
        showAuthError('loginError', result.error || 'Login failed.');
    }
}

/**
 * @description Handles local user registration
 * @param e - The submit event
 */
export async function handleLocalRegister(e: Event) {
    e.preventDefault();
    clearAuthMessages();

    const formData = new FormData(e.target as HTMLFormElement);
    const registerData = {
        nickname: formData.get('nickname') as string,
        email: formData.get('email') as string,
        password: formData.get('password') as string,
        confirmPassword: formData.get('confirmPassword') as string
    };

    const validationError = AuthService.validateRegisterInput(registerData);
    if (validationError) {
        showAuthError('registerError', validationError);
        return;
    }

    const result = await AuthService.localRegister(registerData);

    if (result.success) {
        showAuthSuccess('registerSuccess', result.message);

        (e.target as HTMLFormElement).reset(); // Reset the form

        setTimeout(() => {
            showAuthTab('login'); // Switch to login tab after successful registration
            showAuthSuccess('registerSuccess', 'Please login to continue.');
        }, 1000);
    } else {
        showAuthError('registerError', result.error || 'Registration failed.');
    }
}

/**
 * showing login screen
 */
export function showLoginScreen(){
    console.log('Showing login screen');

    if (appState.statusManager) {
        appState.statusManager.disconnect();
        appState.statusManager = null; 
        console.log('Disconnected existing StatusManager for logout.');
    }

    if (window.location.pathname !== '/login') {
        history.replaceState({ sectionId: 'login' }, 'PONG - Login', '/login');
    }

    showSection('login');
    
    document.getElementById('loginSection')?.classList.remove('hidden');
    document.getElementById('appSection')?.classList.add('hidden');

    const loginButton = document.getElementById('loginBtn');
    if (loginButton) {
        loginButton.addEventListener('click', () => {
            console.log('Login button clicked');
            redirectGoogleLogin();
        });
    }
}


/**
 * @description Updated updateLoginStatus function
 */
export async function updateLoginStatus() {
    try {
        const response = await fetch('/api/auth/me', { credentials: 'include' });
        
        if (!response.ok) {
            throw new Error('Not logged in');
        }
        
        const user = await response.json();
        appState.currentUser = user;
        
        if (appState._resolveUserReady) {
            appState._resolveUserReady(user);
        }

        // Initialize StatusManager once (non-blocking, fire-and-forget)
        appState.statusManager = StatusManager.getInstance();
        await appState.statusManager.initializeStatusConnection('', user);
        console.log('StatusManager successfully re-initialized for new session.');
        
        const currentPath = window.location.pathname;
        
        if (!user.profileComplete) {
            showNicknameSetupScreen();
            return;
        }
        
        // Handle normal navigation based on URL path
        if (currentPath === '/profile') {
            showAppScreen(user);
            showProfileScreen();
        } else if (currentPath === '/friends') {
            showAppScreen(user);
            await showFriendsScreen();
        } else if (currentPath === '/tournament') {
            showAppScreen(user);
            showSection('tournament');
        } else {
            showAppScreen(user);
        }

        //CHAT - Déclencher l'événement de connexion
        document.dispatchEvent(new Event('userLoggedIn'));
        
    } catch (error) {
        console.error('Not logged in or session expired');
        showLoginScreen();
    }
}