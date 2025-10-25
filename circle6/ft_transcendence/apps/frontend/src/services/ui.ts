import { appState } from "../state/state";
import { TournamentUI } from '../tournament/tournament-ui';
import { showPublicProfileScreen } from '../services/user';
import { loadHomeTournaments } from '../tournament/tournament-services';
import { connectActiveTournamentsSocket } from '../tournament/tournament-ws';
import { pushToNavigationHistory } from '../utils/navigation';

let renderFriendLists: () => Promise<void> = async () => {};
let friendsRefreshInterval: ReturnType<typeof setInterval> | null = null;

export function handleProfileUpdate(payload: any) {
    console.log('Handling profile update:', payload);
    
    // Update current user if it's their own profile
    if (appState.currentUser && appState.currentUser.id === payload.userId) {
        if (payload.changeType === 'nickname' && payload.nickname) {
            appState.currentUser.nickname = payload.nickname;
            
            // Update nickname widget
            const widgetNickname = document.getElementById('widgetNickname');
            if (widgetNickname) widgetNickname.textContent = payload.nickname;
        }
        
        if (payload.changeType === 'avatar' && payload.avatarUrl) {
            const newAvatarUrl = payload.avatarUrl + '?t=' + new Date().getTime();
            appState.currentUser.avatarUrl = newAvatarUrl;
            appState.currentUser.avatar_url = newAvatarUrl;
            
            // Update all avatar images
            const widgetAvatar = document.getElementById('widgetAvatar') as HTMLImageElement;
            if (widgetAvatar) widgetAvatar.src = newAvatarUrl;
            
            const profileAvatar = document.getElementById('profileAvatar') as HTMLImageElement;
            if (profileAvatar) profileAvatar.src = newAvatarUrl;
        }
    }
    
    // If in friends section, refresh the friends list
    if (appState.currentSection === 'friends') {
        renderFriendLists();
    }
}

/**
 * @param sectionId - The ID of the section to show
 * @param pushToHistory - Whether to push this navigation to the browser history
 * @description Show a specific section by ID and hide others, with navigation history tracking
 */
export function showSection(
    sectionId: 'hero' | 'game' | 'profile' | 'login' | 'nicknameSetup' | 'friends' | 'publicProfile' | 'waiting' | 'tournament', 
    pushToHistory: boolean = true
) {
    const sections = ['heroSection', 'gameSection', 'profileSection', 'loginSection', 'appSection', 'nicknameSetupSection', 'friendsSection', 'publicProfileSection', 'waitingSection', 'tournamentSection'];
    
    console.log(`SHOW SECTION with ${sectionId}, pushToHistory=${pushToHistory}`);

    // IMPORTANT: Track navigation before changing section
    // But only if we're actually changing sections and pushing to history
    if (pushToHistory && appState.currentSection && appState.currentSection !== sectionId) {
        // Push the CURRENT section (where we're coming FROM) to history
        const currentTournamentId = appState.currentTournament?.id;
        // const currentTournamentView = appState.tournamentUI?.getCurrentView?.();
        
        pushToNavigationHistory(
            appState.currentSection,
            currentTournamentId,
            undefined
        );
    }

    sections.forEach(id => {
        const el = document.getElementById(id);
        if (el) {
            if ('loginSection' === id || 'nicknameSetupSection' === id) {
                el.style.display = 'none';
            } else {
                el.classList.add('hidden');
            }
        }
    });

    const targetSection = document.getElementById(`${sectionId}Section`);
    if (targetSection) {
        if (sectionId === 'login' || sectionId === 'nicknameSetup') {
            targetSection.style.display = 'flex';
        } else {
            targetSection.classList.remove('hidden');
        }
    }

    const appContainer = document.getElementById('appSection');
    if (appContainer) {
        if (sectionId === 'login') {
            appContainer.classList.add('hidden');
        } else {
            appContainer.classList.remove('hidden');
        }
    }

    // Initialize tournament UI when showing tournament section
    if (sectionId === 'tournament') {
        if (appState.tournamentUI) {
            appState.tournamentUI.cleanup();
        }
        appState.tournamentUI = new TournamentUI('tournamentSection');

        if (appState.currentUser) {
            appState.tournamentUI.setCurrentUser(appState.currentUser);
        }
        
        // Show the main tournament home screen
        appState.tournamentUI.showTournamentHome();
    }

    // Start/stop tournament polling based on section
    if (sectionId === 'hero') {
        // Load tournaments immediately when showing hero
        if (appState.currentUser) {
            loadHomeTournaments();
        }
    }

    // Rest of the showSection logic...
    if (appState.currentSection === sectionId && pushToHistory) {
        console.log(`Already in section ${sectionId}, skipping history push`);
        return;
    }

    if (pushToHistory && appState.currentSection !== sectionId) {
        const url = getUrlForSection(sectionId);
        const title = getTitleForSection(sectionId);
        const cleanUrl = url.replace(/#.*$/, '');
        const state = { sectionId };
        history.pushState(state, title, cleanUrl);
        document.title = title;
    }

    if (appState.currentSection === 'friends' && sectionId !== 'friends' && appState.statusManager) {
        appState.statusManager.offFriendUpdate(renderFriendLists);
    }

    appState.currentSection = sectionId;
}

/**
 * @description Show friends screen with real-time updates
 */
export async function showFriendsScreen() {
    showSection('friends', true);

    const friendsListEl = document.getElementById('friendsList');
    const receivedRequestsListEl = document.getElementById('receivedRequestsList');
    const sentRequestsListEl = document.getElementById('sentRequestsList');
    const addFriendForm = document.getElementById('addFriendForm');
    const friendNicknameInput = document.getElementById('friendNicknameInput') as HTMLInputElement;
    const addFriendStatus = document.getElementById('addFriendStatus');

    if (!friendsListEl || !receivedRequestsListEl || !sentRequestsListEl || !addFriendForm || !addFriendStatus) {
        console.error('Required DOM elements not found');
        return;
    }

    // Function to render all lists
    renderFriendLists = async () => {
        try {
            const response = await fetch('/api/user/friends/all', { credentials: 'include' });
            if (!response.ok) throw new Error('Failed to fetch friends data');

            const data = await response.json();

            // Get friend statuses if StatusManager is available
            const friendStatuses = appState.statusManager?.getFriends() || [];

            // Render Friends List
            friendsListEl.innerHTML = data.friends.map((friend: any) => {
                const statusInfo = friendStatuses.find(f => f.userId === friend.id);
                const status = statusInfo ? statusInfo.status : 'offline';
                const statusColor = getStatusColor(status);
                const statusText = status.charAt(0).toUpperCase() + status.slice(1);
                
                return `
                    <li class="bg-white p-3 border-thick flex justify-between items-center text-black">
                        <div class="flex items-center gap-x-3">
                            <div class="relative">
                                <img src="${friend.avatar_url || '/default-avatar.png'}" 
                                     alt="${friend.nickname}" 
                                     class="w-12 h-12 rounded-full border-2 border-black">
                                <div class="absolute bottom-0 right-0 w-4 h-4 rounded-full ${statusColor} border-2 border-white" 
                                     title="${statusText}"></div>
                            </div>
                            <div>
                                <span class="text-black font-bold block">${friend.nickname}</span>
                                <span class="text-sm text-gray-600">${statusText}</span>
                            </div>
                        </div>
                        <div class="flex items-center gap-x-2">
                            <button data-friend-id="${friend.id}" 
                                    class="friend-item bg-pink-500 text-white px-3 py-1 text-lg border-thick hover-anarchy" 
                                    data-nickname="${friend.nickname}">
                                VIEW PROFILE & STATS
                            </button>
                            <button data-friend-id="${friend.id}" 
                                    class="remove-friend-btn bg-red-600 text-white px-3 py-1 text-lg border-thick hover-anarchy">
                                REMOVE
                            </button>
                        </div>
                    </li>
                `;
            }).join('') || '<li class="bg-white p-3 border-thick text-black">No friends yet.</li>';

            // Render Received Requests (FIXED: was data.received, now data.receivedRequests)
            receivedRequestsListEl.innerHTML = data.receivedRequests.map((req: any) => `
                <li class="bg-white p-3 border-thick flex justify-between items-center text-black">
                    <div class="flex items-center gap-3">
                        <img src="${req.avatar_url || '/default-avatar.png'}" 
                             alt="${req.nickname}" 
                             class="w-10 h-10 rounded-full border-2 border-black">
                        <span class="font-bold">${req.nickname}</span>
                    </div>
                    <div>
                        <button data-request-id="${req.id}" 
                                class="accept-friend-btn bg-green-500 text-white px-3 py-1 text-lg border-thick hover-anarchy mr-2">
                            ACCEPT
                        </button>
                        <button data-request-id="${req.id}" 
                                class="reject-friend-btn bg-gray-500 text-white px-3 py-1 text-lg border-thick hover-anarchy">
                            REJECT
                        </button>
                    </div>
                </li>
            `).join('') || '<li class="bg-white p-3 border-thick text-black">No new friend requests.</li>';

            // Render Sent Requests (FIXED: was data.sent, now data.sentRequests)
            sentRequestsListEl.innerHTML = data.sentRequests.map((req: any) => `
                <li class="bg-white p-3 border-thick flex justify-between items-center text-black">
                    <div class="flex items-center gap-3">
                        <img src="${req.avatar_url || '/default-avatar.png'}" 
                             alt="${req.nickname}" 
                             class="w-10 h-10 rounded-full border-2 border-black">
                        <span class="font-bold">${req.nickname}</span>
                    </div>
                    <span class="text-gray-500">Pending</span>
                </li>
            `).join('') || '<li class="bg-white p-3 border-thick text-black">No sent requests.</li>';

            attachFriendActionListeners();

        } catch (error) {
            console.error('Error loading friends screen:', error);
            friendsListEl.innerHTML = '<li class="text-red-400">Failed to load data.</li>';
        }
    };

    // Function to attach event listeners to buttons
    const attachFriendActionListeners = () => {
        document.querySelectorAll('.accept-friend-btn').forEach(button => {
            button.addEventListener('click', async (e) => {
                const requestId = (e.target as HTMLElement).dataset.requestId;
                await handleFriendAction('/api/user/friends/accept', 'PUT', { requestId: Number(requestId) });
            });
        });
        
        document.querySelectorAll('.reject-friend-btn').forEach(button => {
            button.addEventListener('click', async (e) => {
                const requestId = (e.target as HTMLElement).dataset.requestId;
                await handleFriendAction('/api/user/friends/reject', 'PUT', { requestId: Number(requestId) });
            });
        });
        
        document.querySelectorAll('.remove-friend-btn').forEach(button => {
            button.addEventListener('click', async (e) => {
                const friendId = (e.target as HTMLElement).dataset.friendId;
                await handleFriendAction(`/api/user/friends/${friendId}`, 'DELETE');
            });
        });

        document.querySelectorAll('.friend-item').forEach(item => {
            item.addEventListener('click', async (e) => {
                const nickname = (e.currentTarget as HTMLElement).dataset.nickname;
                if (nickname) {
                    await showPublicProfileScreen(nickname);
                }
            });
        });
    };

    // Generic handler for friend actions
    const handleFriendAction = async (url: string, method: string, body?: object) => {
        try {
            const headers: HeadersInit = {};
            if (body) {
                headers['Content-Type'] = 'application/json';
            }
            const response = await fetch(url, {
                method,
                headers,
                body: body ? JSON.stringify(body) : undefined,
                credentials: 'include'
            });
            const result = await response.json();
            if (!response.ok) throw new Error(result.error || 'Action failed');

            await renderFriendLists();

        } catch (error) {
            console.error(`Failed to ${method} ${url}:`, error);
            alert(`Error: ${error instanceof Error ? error.message : 'An unknown error occurred'}`);
        }
    };

    // Handler for submitting the "add friend" form
    addFriendForm.onsubmit = async (e) => {
        e.preventDefault();
        const friendNickname = friendNicknameInput.value.trim();
        if (!friendNickname) return;

        addFriendStatus.textContent = 'Sending...';
        try {
            const response = await fetch('/api/user/friends/request', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ friendNickname }),
                credentials: 'include'
            });
            const result = await response.json();
            if (!response.ok) {
                throw new Error(result.error || 'Failed to send request');
            }
            addFriendStatus.textContent = 'Friend request sent!';
            friendNicknameInput.value = '';
            await renderFriendLists();
        } catch (error) {
            addFriendStatus.textContent = `Error: ${error instanceof Error ? error.message : 'Unknown error'}`;
        }
    };

    if (appState.statusManager) {
        appState.statusManager.onFriendUpdate(renderFriendLists);
    }

    if (friendsRefreshInterval) {
        clearInterval(friendsRefreshInterval);
        friendsRefreshInterval = null;
    }

    if (appState.statusManager && typeof appState.statusManager.onStatusUpdate === 'function' && typeof appState.statusManager.onFriendUpdate === 'function') {
        appState.statusManager.onStatusUpdate(updateFriendsDisplay);

        appState.statusManager.onFriendUpdate(renderFriendLists);
    }

    
    // Initial render
    await renderFriendLists();
}

/**
 * Mapping URL and Section ID
 * @param sectionId 
 * @returns 
 */
function getUrlForSection(sectionId: string): string {
	switch (sectionId) {
		case 'hero': return '/';
		case 'game': return '/game';
		case 'profile': return '/profile';
		case 'login': return '/login';
		case 'nicknameSetup': return '/nickname-setup';
		case 'friends': return '/friends';
		case 'publicProfile': return '/public-profile';
		case 'waiting': return '/waiting';
		case 'tournament': return '/tournament';
		default: return '/';
	}
}

function getTitleForSection(sectionId: string): string {
	switch (sectionId) {
		case 'hero': return 'PONG - Main Menu';
		case 'game': return 'PONG - Game';
		case 'profile': return 'PONG - Profile';
		case 'login': return 'PONG - Login';
		case 'nicknameSetup': return 'PONG - Setup Nickname';
		case 'friends': return 'PONG - Friends';
		case 'publicProfile': return 'PONG - Public Profile';
		case 'waiting': return 'PONG - Waiting';
		case 'tournament': return 'PONG - Tournaments';
		default: return 'PONG';
	}
}

function getStatusColor(status: string): string {
    switch (status) {
        case 'online': return 'bg-green-500';
        case 'in_game': return 'bg-yellow-500';
        case 'away': return 'bg-orange-500';
        case 'offline': return 'bg-gray-500';
        default: return 'bg-gray-500';
    }
}

/**
 * showing app screen
 * @param user - User object containing user information
 */
export async function showAppScreen(user: any) {
	appState.currentUser = user;

	showSection('hero');
	
	const widgetAvatar = document.getElementById('widgetAvatar') as HTMLImageElement;
	const widgetNickname = document.getElementById('widgetNickname') as HTMLSpanElement;
	if (widgetAvatar) widgetAvatar.src = user.avatarUrl || '/default-avatar.png';
	if (widgetNickname) widgetNickname.textContent = user.nickname || user.name;

	// Load tournaments initially
	await loadHomeTournaments();

    connectActiveTournamentsSocket();
}

export function setupPongLogoRedirect(): void {
	const pongLogo = document.getElementById('pongLogo');
	if (pongLogo) {
		pongLogo.addEventListener('click', () => {
			console.log('Pong logo clicked, returning to main menu');
			showSection('hero');
		});
	}
}

export function showNotification(message: string, type: 'success' | 'error' | 'info' = 'info') {
	let container = document.getElementById('notification-container');
	if (!container) {
		container = document.createElement('div');
		container.id = 'notification-container';
		container.className = 'fixed top-4 left-4 z-[9999] flex flex-col gap-3 pointer-events-none';
		container.style.maxWidth = '320px';
		document.body.appendChild(container);
	}

	const notification = document.createElement('div');
	notification.className = `p-4 border-thick shadow-sharp animate-pop pointer-events-auto ${
		type === 'success' ? 'bg-green-500 text-white' :
		type === 'error' ? 'bg-red-500 text-white' :
		'bg-blue-500 text-white'
	}`;
	notification.innerHTML = `
		<div class="flex justify-between items-center gap-2">
			<span class="font-teko text-lg uppercase font-black">${message}</span>
			<button class="ml-2 text-white hover:text-yellow-300 text-xl font-black leading-none">&times;</button>
		</div>
	`;
	
	// Append to container (stacks vertically)
	container.appendChild(notification);
	
	// Auto remove after 4 seconds
	const timeout = setTimeout(() => {
		if (notification.parentNode) {
			notification.remove();
			// Remove container if empty
			if (container && container.children.length === 0) {
				container.remove();
			}
		}
	}, 4000);
	
	// Manual close
	notification.querySelector('button')?.addEventListener('click', () => {
		clearTimeout(timeout);
		notification.remove();
		if (container && container.children.length === 0) {
			container.remove();
		}
	});
}

/**
 * @description Show nickname setup screen if user profile is not complete
 */
export function showNicknameSetupScreen() {
	console.log('Showing nickname setup screen');

	if (window.location.pathname !== '/nickname-setup') {
        history.replaceState({ sectionId: 'nicknameSetup' }, 'PONG - Set Up Nickname', '/nickname-setup');
    }
	
	showSection('nicknameSetup');
	const nicknameForm = document.getElementById('nicknameForm');
	if (!nicknameForm) {
		console.error('Nickname form not found');
		return;
	}
	
	const newForm = nicknameForm.cloneNode(true) as HTMLFormElement;
	nicknameForm.parentNode?.replaceChild(newForm, nicknameForm);
	
	newForm.addEventListener('submit', async (event) => {
		event.preventDefault();
		const nicknameInput = document.getElementById('nicknameInput') as HTMLInputElement;
		const nickname = nicknameInput.value.trim();

		if (!nickname) {
			alert('Nickname cannot be empty');
			return;
		}

		try {
			const response = await fetch('/api/user/setup', {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json',
				},
				body: JSON.stringify({ nickname }),
				credentials: 'include'
			});

			console.log(`response: ${response.status} ${response.statusText}`);

			if (!response.ok) {
				const errorData = await response.json();
				throw new Error(errorData.error || 'Failed to set nickname');
			}

			const data = await response.json();
			console.log('Nickname set successfully:', data);

			const userResponse = await fetch('/api/auth/me', { credentials: 'include' });
			if (userResponse.ok) {
				const userData = await userResponse.json();
				console.log('Updated user data after nickname setup:', userData);
				console.log('Profile complete status:', userData.profileComplete);
				
				appState.currentUser = userData;
				
				if (userData.profileComplete) {
					showAppScreen(userData);

                    //CHAT - Déclencher l'événement de connexion
                    document.dispatchEvent(new Event('userLoggedIn'));
				} else {
					alert('Profile setup failed. Please try again.');
				}
			} else {
				throw new Error('Failed to fetch updated user data');
			}
			
		} catch (error) {
			console.error('Error setting nickname:', error);
			alert(`Failed to set nickname: ${error instanceof Error ? error.message : 'Unknown error'}`);
		}
	});
}

function updateFriendsDisplay(friends: any[]): void {
    console.log('Updating friends display with data:', friends); 
    const friendsList = document.getElementById('friendsList');
    if (!friendsList) return;

    // Fetch the current friends list from the API to get avatar URLs
    fetch('/api/user/friends/all', { credentials: 'include' })
        .then(response => response.json())
        .then(data => {
            friendsList.innerHTML = data.friends.map((friend: any) => {
                const statusInfo = friends.find(f => f.userId === friend.id);
                const status = statusInfo ? statusInfo.status : 'offline';
                const statusColor = getStatusColor(status);

                return `
                    <li class="bg-white p-3 border-thick flex justify-between items-center text-black">
                        <div class="flex items-center gap-x-3">
                            <div class="w-3 h-3 rounded-full ${statusColor}" title="${status.charAt(0).toUpperCase() + status.slice(1)}"></div>
                            <img src="${friend.avatar_url || '/default-avatar.png'}" alt="${friend.nickname}" class="w-10 h-10 rounded-full">
                            <span class="text-black font-bold">${friend.nickname}</span>
                        </div>
                        <div class="flex items-center gap-x-2">
                            <button data-friend-id="${friend.id}" class="friend-item bg-pink-500 text-white px-3 py-1 text-lg border-thick hover-anarchy" data-nickname="${friend.nickname}">VIEW PROFILE & STATS</button>
                            <button data-friend-id="${friend.id}" class="remove-friend-btn bg-red-600 text-white px-3 py-1 text-lg border-thick hover-anarchy">REMOVE</button>
                        </div>
                    </li>
                `;
            }).join('') || '<li class="bg-white p-3 border-thick text-black">No friends yet.</li>';
            
            // Re-attach event listeners after updating HTML
            document.querySelectorAll('.friend-item').forEach(item => {
                item.addEventListener('click', async (e) => {
                    const nickname = (e.currentTarget as HTMLElement).dataset.nickname;
                    if (nickname) {
                        await showPublicProfileScreen(nickname);
                    }
                });
            });
        })
        .catch(error => console.error('Error updating friends display:', error));
}

/**
 * @description popstate event listener for browser navigation
 */
window.addEventListener('popstate', (event) => {
    console.log('Browser navigation detected:', {
        state: event.state,
        url: window.location.href,
        hash: window.location.hash
    });

    if (event.state && event.state.sectionId) {
        console.log('event.state.sectionId:', event.state.sectionId);
        showSection(event.state.sectionId, false);
    } else {
        const path = window.location.pathname;
        let sectionId: any = 'hero';
        
        if (path === '/game') sectionId = 'game';
        else if (path === '/profile') sectionId = 'profile';
        else if (path === '/login') sectionId = 'login';
        else if (path === '/nickname-setup') sectionId = 'nicknameSetup';
        else if (path === '/friends') sectionId = 'friends';
        else if (path === '/public-profile') sectionId = 'publicProfile';
        else if (path === '/waiting') sectionId = 'waiting';
        else if (path === '/tournament') sectionId = 'tournament';
        
        console.log('sectionId:', sectionId);
        showSection(sectionId, false);
    }
});
