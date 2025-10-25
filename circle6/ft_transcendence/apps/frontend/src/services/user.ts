import { PasswordChangeForm } from '../../components/password-change-form';
import { AuthService } from '../../auth/services/auth-service';
import { appState } from '../state/state';
import { showSection, showNotification } from './ui';
import { StatsManager } from '../stats/stats-manager';
import { UserStats } from '@/status/status-manager';
import { PrivacyManagement } from '../../components/privacy-management';
import { ProfileEditForm } from '../../components/profile-edit-form';

export async function showPublicProfileScreen(nickname: string) {
    showSection('publicProfile');
    const publicProfileContent = document.getElementById('publicProfileContent');
    if (!publicProfileContent) return;

    publicProfileContent.innerHTML = '<p>Loading profile...</p>';

    try {
        const response = await fetch(`/api/user/profile/${nickname}`);

        if (!response.ok) throw new Error('Failed to fetch profile');

        const data = await response.json();

        publicProfileContent.innerHTML = `
            <div class="flex items-center gap-x-4 mb-6">
                <img src="${data.user.avatar_url || '/default-avatar.png'}" class="w-16 h-16 rounded-full border-thick">
                <h2 class="text-4xl uppercase">${data.user.nickname}</h2>
            </div>
            
            ${data.stats ? `
                <div class="bg-white p-6 border-thick shadow-sharp mb-6">
                    <h3 class="text-2xl font-bold mb-4">Player Stats</h3>
                    <div class="grid grid-cols-2 gap-4">
                        <div class="space-y-2">
                            <div class="flex justify-between">
                                <span>Rank:</span>
                                <span class="font-bold">${data.stats.rank} (${data.stats.rankPoints} RP)</span>
                            </div>
                            <div class="flex justify-between">
                                <span>Total Games:</span>
                                <span>${data.stats.totalGames}</span>
                            </div>
                            <div class="flex justify-between">
                                <span>Win Rate:</span>
                                <span>${data.stats.winRate}%</span>
                            </div>
                        </div>
                        <div class="space-y-2">
                            <div class="flex justify-between">
                                <span>Wins/Losses:</span>
                                <span>${data.stats.wins}/${data.stats.losses}</span>
                            </div>
                            <div class="flex justify-between">
                                <span>Current Streak:</span>
                                <span>${data.stats.currentStreak}</span>
                            </div>
                            <div class="flex justify-between">
                                <span>Max Streak:</span>
                                <span>${data.stats.maxStreak}</span>
                            </div>
                        </div>
                    </div>
                </div>
            ` : '<div class="bg-white p-4 border-thick mb-6"><p>No stats available</p></div>'}
            
            <h3 class="text-3xl uppercase mt-6 mb-3 border-t-4 border-black pt-4">Game History</h3>
            <ul class="space-y-2 font-teko text-2xl">
                ${data.gameHistory.map((game: any) => `
                    <li class="bg-white p-3 border-thick flex justify-between items-center text-black">
                        <span>vs ${game.opponent_nickname} (${game.game_type})</span>
                        <span class="font-bold ${game.result === 'Win' ? 'text-green-600' : 'text-red-600'}">${game.result}</span>
                    </li>
                `).join('')}
            </ul>
        `;
    } catch (error) {
        publicProfileContent.innerHTML = `
            <p class="text-red-500">Failed to load profile. Please try again later.</p>`;
        console.error('Error loading public profile:', error);
    }
}

/**
 * @description Show the profile screen
 */
export async function showProfileScreen() {
    showSection('profile', true);

    const profileContent = document.getElementById('profileContent');
    if (!profileContent) return;

    profileContent.innerHTML = '<p>Loading profile...</p>';

    try {
        const response = await fetch('/api/user/profile', {
            credentials: 'include'
        });
        if (!response.ok) {
            throw new Error('Failed to fetch profile');
        }

        const data = await response.json();

         profileContent.innerHTML = `
            <div class="flex flex-col md:flex-row items-center justify-between gap-4 mb-6 pb-4 border-b-4 border-black">
                <div class="flex flex-col md:flex-row items-center gap-x-6">
                    <img id="profileAvatar" src="${data.user.avatar_url || '/default-avatar.png'}" alt="User Avatar" class="w-24 h-24 rounded-full border-thick shadow-sharp mb-4 md:mb-0">
                    <div class="text-center md:text-left">
                        <h2 class="text-5xl uppercase">${data.user.nickname}</h2>
                        <p class="font-teko text-2xl text-black/80">${data.user.name} (${data.user.email})</p>
                        ${data.user.auth_provider ? `<p class="text-sm text-gray-700 uppercase mt-1">Auth: ${data.user.auth_provider}</p>` : ''}
                    </div>
                </div>
                
                <button id="editProfileBtn" 
                        class="bg-blue-500 text-white px-6 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy font-teko">
                    Edit Profile
                </button>
            </div>

            <!-- Edit Form Container -->
            <div id="profileEditContainer"></div>

            <div class="bg-white p-4 border-thick shadow-sharp mb-6">
                 <h3 class="text-2xl uppercase mb-2">Upload Avatar</h3>
                 <form id="avatarForm" class="flex items-center gap-2">
                    <input type="file" id="avatarUpload" accept="image/*" class="w-full p-2 text-lg border-thick bg-white">
                    <button type="submit" class="bg-pink-500 text-white px-6 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy">
                        Save
                    </button>
                </form>
            </div>
            
            ${data.stats ? `
                <div class="bg-white p-6 border-thick shadow-sharp mb-6">
                    <h3 class="text-3xl uppercase mb-4 text-center">YOUR STATS</h3>
                    <div class="grid grid-cols-2 md:grid-cols-4 gap-4 text-center">
                        <div class="bg-pink-500 text-white p-4 border-thick">
                            <div class="text-2xl font-bold">${data.stats.rank}</div>
                            <div class="text-sm uppercase">Rank</div>
                            <div class="text-xs">${data.stats.rankPoints} RP</div>
                        </div>
                        <div class="bg-yellow-400 text-black p-4 border-thick">
                            <div class="text-2xl font-bold">${data.stats.totalGames}</div>
                            <div class="text-sm uppercase">Total Games</div>
                        </div>
                        <div class="bg-green-500 text-white p-4 border-thick">
                            <div class="text-2xl font-bold">${data.stats.winRate}%</div>
                            <div class="text-sm uppercase">Win Rate</div>
                            <div class="text-xs">${data.stats.wins}W / ${data.stats.losses}L</div>
                        </div>
                        <div class="bg-red-500 text-white p-4 border-thick">
                            <div class="text-2xl font-bold">${data.stats.currentStreak}</div>
                            <div class="text-sm uppercase">Current Streak</div>
                            <div class="text-xs">Max: ${data.stats.maxStreak}</div>
                        </div>
                    </div>
                    
                    <div class="mt-4 grid grid-cols-2 gap-4 text-center">
                        <div class="bg-blue-500 text-white p-3 border-thick">
                            <div class="text-xl font-bold">${data.stats.averageScore.toFixed(1)}</div>
                            <div class="text-xs uppercase">Average Score</div>
                        </div>
                        <div class="bg-purple-500 text-white p-3 border-thick">
                            <div class="text-xl font-bold">${data.stats.maxScore}</div>
                            <div class="text-xs uppercase">Max Score</div>
                        </div>
                    </div>
                </div>
            ` : `
                <div class="bg-white p-6 border-thick shadow-sharp mb-6">
                    <h3 class="text-3xl uppercase text-center">YOUR STATS</h3>
                    <p class="text-center text-gray-500 mt-4">No games played yet. Start playing to see your stats!</p>
                </div>
            `}
            
            <div>
                <h3 class="text-4xl text-outline-white text-center mb-4">Game History</h3>
                <ul class="space-y-2 font-teko text-2xl uppercase">
                    ${data.gameHistory.map((game: any) => `
                        <li class="bg-white p-3 border-thick shadow-sharp flex justify-between items-center text-black">
                            <div>
                                vs ${game.opponent_nickname}
                                <span class="text-lg text-black/70">(${game.game_type})</span>
                            </div>
                            <div class="text-right ${game.result === 'Win' ? 'text-green-600' : 'text-red-600'}">
                                <span class="text-3xl font-bold">${game.result}</span>
                                <div class="text-sm">${game.player1_score} - ${game.player2_score}</div>
                            </div>
                        </li>
                    `).join('') || '<li class="bg-white p-3 border-thick text-center text-black">No games played yet.</li>'}
                </ul>
            </div>
        `;

        const editBtn = document.getElementById('editProfileBtn');
        editBtn?.addEventListener('click', () => {
            const editContainer = document.getElementById('profileEditContainer');
            if (editContainer) {
                if (editContainer.innerHTML.trim() !== '') {
                    // Hide the form
                    editContainer.innerHTML = '';
                    editBtn.textContent = 'Edit Profile';
                } else {
                    // Show the form
                    editBtn.textContent = 'Cancel Edit';
                    new ProfileEditForm(editContainer, data.user, async () => {
                        await showProfileScreen();
                    });
                }
            }
        });

        if (data.user.auth_provider === 'local') {
            const passwordChangeContainer = document.getElementById('password-change-container');
            if (passwordChangeContainer) {
                passwordChangeContainer.innerHTML = '';
                new PasswordChangeForm(passwordChangeContainer);
            }
        }

        // Privacy & Data Management section
        const privacyContainer = document.createElement('div');
        privacyContainer.id = 'privacyManagementContainer';
        profileContent.appendChild(privacyContainer);
        new PrivacyManagement(privacyContainer);

        attachAvatarFormListener();

    } catch (error) {
        profileContent.innerHTML = `
            <p class="text-red-500">Failed to load profile. Please try it later</p>`;
        console.error('Error loading profile:', error);
    }
}

/**
 * @description Attach Avatar images
 */
function attachAvatarFormListener() {
	const avatarForm = document.getElementById('avatarForm');
	if (avatarForm) {
		avatarForm.addEventListener('submit', async (event) => {
			event.preventDefault();
			const avatarInput = document.getElementById('avatarUpload') as HTMLInputElement;
			const avatarFiles = avatarInput.files;
			
			if (!avatarFiles || avatarFiles.length === 0) {
				console.error('No avatar file selected. Showing default avatar.');
				return;
			}

			const formData = new FormData();
			formData.append('avatar', avatarFiles[0]);

			try {
				const uploadResponse = await fetch('/api/user/avatar', {
					method: 'POST',
					body: formData,
					credentials: 'include'
				});
				const result = await uploadResponse.json();

				if (uploadResponse.ok) {
					console.log('Avatar uploaded successfully:', result);
					
					const newAvatarUrl = result.avatarUrl + '?t=' + new Date().getTime();
					
					// Update widget avatar (navigation bar)
					const widgetAvatar = document.getElementById('widgetAvatar') as HTMLImageElement;
					if (widgetAvatar) {
						widgetAvatar.src = newAvatarUrl;
					}
					
					// Update profile page avatar
					const profileAvatar = document.getElementById('profileAvatar') as HTMLImageElement;
					if (profileAvatar) {
						profileAvatar.src = newAvatarUrl;
					}
					
					// Update currentUser object so tournament uses new avatar
					if (appState.currentUser) {
						appState.currentUser.avatarUrl = result.avatarUrl;
						appState.currentUser.avatar_url = result.avatarUrl;
					}
					
					// Update tournament UI if it exists and is active
					if (appState.tournamentUI && appState.currentUser) {
						appState.tournamentUI.setCurrentUser(appState.currentUser);
					}
					
					// Show success notification
					if (typeof showNotification === 'function') {
						showNotification('Avatar updated successfully!', 'success');
					}
				} else {
					throw new Error(result.error || 'Failed to upload avatar');
				}
			} catch (error) {
				console.error('Error uploading avatar:', error);
				if (typeof showNotification === 'function') {
					showNotification('Failed to upload avatar', 'error');
				}
			}
		});
	}
}

export async function viewProfile(userId: number): Promise<void> {
    try {
        if (!appState.statsManager) return;

        const stats = await appState.statsManager.getPublicStats(userId);
        if (stats) {
            showUserStatsModal(stats);
        }
    } catch (error) {
        console.error('Error viewing profile:', error);
    }
}

function showUserStatsModal(stats: UserStats): void {
    const modal = document.createElement('div');
    modal.className = 'fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50';
    modal.innerHTML = `
        <div class="bg-white p-6 border-thick shadow-sharp max-w-md w-full mx-4">
            <h2 class="text-2xl font-bold mb-4">${stats.nickname}'s Stats</h2>
            <div class="space-y-2">
                <div class="flex justify-between">
                    <span>Rank:</span>
                    <span class="font-bold">${stats.rank} (${stats.rankPoints} RP)</span>
                </div>
                <div class="flex justify-between">
                    <span>Total Games:</span>
                    <span>${stats.totalGames}</span>
                </div>
                <div class="flex justify-between">
                    <span>Wins/Losses:</span>
                    <span>${stats.wins}/${stats.losses}</span>
                </div>
                <div class="flex justify-between">
                    <span>Win Rate:</span>
                    <span>${stats.winRate}%</span>
                </div>
                <div class="flex justify-between">
                    <span>Current Streak:</span>
                    <span>${stats.currentStreak}</span>
                </div>
                <div class="flex justify-between">
                    <span>Max Streak:</span>
                    <span>${stats.maxStreak}</span>
                </div>
            </div>
            
            <h3 class="text-lg font-bold mt-4 mb-2">Recent Games</h3>
            <div class="space-y-1 max-h-32 overflow-y-auto">
                ${stats.recentGames.map(game => `
                    <div class="text-sm flex justify-between ${game.result === 'win' ? 'text-green-600' : 'text-red-600'}">
                        <span>vs ${game.opponentNickname}</span>
                        <span>${game.result.toUpperCase()}</span>
                    </div>
                `).join('')}
            </div>
            
            <button onclick="this.parentElement.parentElement.remove()" 
                    class="w-full mt-4 bg-gray-500 text-white py-2 border-thick hover-anarchy">
                Close
            </button>
        </div>
    `;
    
    document.body.appendChild(modal);

	modal.querySelector('.close-modal-btn')?.addEventListener('click', () => {
        modal.remove();
    });
    
    modal.addEventListener('click', (e) => {
        if (e.target === modal) {
            modal.remove();
        }
    });
}

// CHAT - Écouter l'événement de visualisation de profil depuis le chat
window.addEventListener('viewUserProfile', async (event: Event) => {
  const customEvent = event as CustomEvent<{ userId: number }>;
  const { userId } = customEvent.detail;
  console.log('Opening profile for user ID:', userId);
  
  try {
    // Récupérer le nickname depuis l'ID
    const response = await fetch(`/api/user/profile-by-id/${userId}`, { 
      credentials: 'include' 
    });
    
    if (!response.ok) throw new Error('Failed to fetch user');
    
    const data = await response.json();
    
    // Utiliser ta fonction existante showPublicProfileScreen
    await showPublicProfileScreen(data.user.nickname);
    
  } catch (error) {
    console.error('Error opening profile:', error);
    alert('Failed to load user profile');
  }
});