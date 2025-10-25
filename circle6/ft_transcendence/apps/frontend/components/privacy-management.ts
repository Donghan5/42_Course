// apps/frontend/components/privacy-management.ts

import { showNotification } from '../src/services/ui';

export class PrivacyManagement {
    private container: HTMLElement;

    constructor(containerElement: HTMLElement) {
        this.container = containerElement;
        this.render();
        this.attachEventListeners();
    }

    private render(): void {
        this.container.innerHTML = `
            <div class="privacy-management bg-white border-thick shadow-sharp p-6 sm:p-8 mt-6 animate-pop">
                <h2 class="text-4xl uppercase mb-6 text-black text-center font-teko">Privacy & Data Management</h2>
                
                <!-- Data Info Section -->
                <div class="mb-8 bg-blue-50 border-thick p-4">
                    <h3 class="text-2xl uppercase mb-3 text-black font-teko">📊 Your Data Overview</h3>
                    <div id="dataInfoContent" class="text-sm text-gray-700">
                        <p class="mb-2">Loading your data information...</p>
                    </div>
                    <button id="loadDataInfoBtn" 
                            class="mt-3 bg-blue-500 text-white px-4 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy font-teko">
                        View Detailed Data Info
                    </button>
                </div>

                <!-- GDPR Actions -->
                <div class="grid grid-cols-1 md:grid-cols-3 gap-4 mb-6">
                    
                    <!-- Export Data -->
                    <div class="bg-green-50 border-thick p-4">
                        <h3 class="text-xl uppercase mb-2 text-black font-teko">📥 Export Data</h3>
                        <p class="text-sm mb-3 text-gray-700">Download all your data. Your account stays active.</p>
                        <button id="exportDataBtn" 
                                class="w-full bg-green-500 text-white px-4 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Download My Data
                        </button>
                    </div>

                    <!-- Anonymize Account -->
                    <div class="bg-yellow-50 border-thick p-4">
                        <h3 class="text-xl uppercase mb-2 text-black font-teko">👤 Anonymize</h3>
                        <p class="text-sm mb-3 text-gray-700">Remove personal info. Keep game history. <strong class="text-red-600">Cannot login again!</strong></p>
                        <button id="anonymizeAccountBtn" 
                                class="w-full bg-yellow-500 text-white px-4 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Anonymize Forever
                        </button>
                    </div>

                    <!-- Delete Account -->
                    <div class="bg-red-50 border-thick p-4">
                        <h3 class="text-xl uppercase mb-2 text-black font-teko">🗑️ Delete</h3>
                        <p class="text-sm mb-3 text-gray-700">Erase everything permanently. <strong class="text-red-600">All data gone!</strong></p>
                        <button id="deleteAccountBtn" 
                                class="w-full bg-red-600 text-white px-4 py-2 text-lg uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Delete Everything
                        </button>
                    </div>
                </div>

                <!-- Privacy Policy Link -->
                <div class="text-center text-sm text-gray-600 mt-4 border-t-2 border-black pt-4">
                    <p class="font-teko text-lg">
                        <span class="font-bold">🔒 Data Storage:</span> All data is stored locally on our servers.
                    </p>
                    <p class="font-teko text-lg mt-2">
                        <span class="font-bold">🛡️ Your Rights:</span> You have full control over your data under GDPR.
                    </p>
                </div>

                <!-- Status Messages -->
                <div id="privacyStatusMessage" class="mt-4 hidden"></div>
            </div>

            <!-- Modals -->
            <div id="anonymizeModal" class="hidden fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
                <div class="bg-white border-thick shadow-sharp p-8 max-w-lg mx-4 animate-pop">
                    <h3 class="text-3xl uppercase mb-4 text-yellow-600 font-teko">⚠️ Anonymize Account - WARNING!</h3>
                    
                    <div class="bg-red-50 border-thick border-red-500 p-4 mb-4">
                        <p class="text-red-700 font-bold text-lg mb-2">⚠️ YOU WILL BE LOGGED OUT FOREVER!</p>
                        <p class="text-red-600 font-bold">This account will NO LONGER BE USABLE for login.</p>
                    </div>

                    <p class="text-gray-700 mb-3 font-bold">This irreversible action will:</p>
                    <ul class="list-disc list-inside text-sm text-gray-700 mb-4 space-y-2 bg-gray-50 p-3 border-thick">
                        <li><strong>PERMANENTLY REMOVE</strong> your email, name, and personal info</li>
                        <li><strong>REPLACE</strong> your nickname with "Anonymous_XXXX"</li>
                        <li><strong>DELETE</strong> all friend connections</li>
                        <li><strong>DELETE</strong> tournaments you're hosting that aren't finished (all participants kicked)</li>
                        <li><strong>DELETE</strong> ACTIVE tournaments you're in (can't finish without you!)</li>
                        <li><strong>REMOVE YOU</strong> from waiting tournaments (not started yet)</li>
                        <li><strong>KEEP</strong> your finished tournament records (shown as Anonymous)</li>
                        <li><strong>KEEP</strong> your game history (shown as Anonymous)</li>
                        <li><strong>DISABLE</strong> this account forever - you cannot log back in</li>
                    </ul>

                    <div class="bg-yellow-50 border-thick border-yellow-500 p-4 mb-4">
                        <p class="text-yellow-800 font-bold">⚠️ AFTER CLICKING CONFIRM:</p>
                        <ul class="text-yellow-700 text-sm mt-2 space-y-1">
                            <li>• You will be immediately logged out</li>
                            <li>• You CANNOT log back in with this account</li>
                            <li>• This action CANNOT be undone</li>
                            <li>• You will need to create a NEW account to play again</li>
                        </ul>
                    </div>

                    <div class="mb-4">
                        <label class="block text-sm font-bold mb-2 text-red-600">Type "ANONYMIZE FOREVER" to confirm:</label>
                        <input type="text" 
                               id="anonymizeConfirmText" 
                               class="w-full p-3 border-thick text-center"
                               placeholder="Type: ANONYMIZE FOREVER">
                    </div>

                    <div class="flex gap-3">
                        <button id="confirmAnonymizeBtn" 
                                class="flex-1 bg-yellow-500 text-white px-6 py-3 text-xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Anonymize Forever
                        </button>
                        <button id="cancelAnonymizeBtn" 
                                class="flex-1 bg-gray-500 text-white px-6 py-3 text-xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Cancel
                        </button>
                    </div>
                </div>
            </div>

            <div id="deleteModal" class="hidden fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
                <div class="bg-white border-thick shadow-sharp p-8 max-w-lg mx-4 animate-pop">
                    <h3 class="text-3xl uppercase mb-4 text-red-600 font-teko">🗑️ Delete Account - PERMANENT!</h3>
                    
                    <div class="bg-red-100 border-thick border-red-600 p-4 mb-4">
                        <p class="text-red-800 font-bold text-xl mb-2">🗑️ COMPLETE DATA DESTRUCTION!</p>
                        <p class="text-red-700 font-bold">Everything will be PERMANENTLY DELETED.</p>
                        <p class="text-red-700 font-bold">You CANNOT log back in.</p>
                    </div>

                    <p class="text-gray-700 mb-3 font-bold">This IRREVERSIBLE action will PERMANENTLY:</p>
                    <ul class="list-disc list-inside text-sm text-gray-700 mb-4 space-y-2 bg-gray-50 p-3 border-thick">
                        <li><strong>DELETE</strong> all your personal data</li>
                        <li><strong>ERASE</strong> ALL your game history</li>
                        <li><strong>REMOVE</strong> all friend connections</li>
                        <li><strong>DELETE</strong> tournaments you're hosting that aren't finished (all participants kicked)</li>
                        <li><strong>DELETE</strong> ACTIVE tournaments you're in (can't finish without you!)</li>
                        <li><strong>REMOVE YOU</strong> from waiting tournaments (not started yet)</li>
                        <li><strong>REMOVE YOU</strong> from finished tournaments (history cleaned)</li>
                        <li><strong>DESTROY</strong> all statistics and achievements</li>
                        <li><strong>ELIMINATE</strong> your account completely</li>
                    </ul>

                    <div class="bg-red-50 border-thick border-red-500 p-4 mb-4">
                        <p class="text-red-800 font-bold">⚠️ AFTER DELETION:</p>
                        <ul class="text-red-700 text-sm mt-2 space-y-1">
                            <li>• Everything is GONE FOREVER</li>
                            <li>• You will be immediately logged out</li>
                            <li>• You CANNOT recover this account</li>
                            <li>• You CANNOT undo this action</li>
                            <li>• All traces removed from the system</li>
                        </ul>
                    </div>

                    <div class="mb-4">
                        <label class="block text-sm font-bold mb-2 text-red-600">Type "DELETE MY ACCOUNT" to confirm permanent deletion:</label>
                        <input type="text" 
                               id="deleteConfirmText" 
                               class="w-full p-3 border-thick text-center"
                               placeholder="Type: DELETE MY ACCOUNT">
                    </div>

                    <div class="flex gap-3">
                        <button id="confirmDeleteBtn" 
                                class="flex-1 bg-red-600 text-white px-6 py-3 text-xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Delete Forever
                        </button>
                        <button id="cancelDeleteBtn" 
                                class="flex-1 bg-gray-500 text-white px-6 py-3 text-xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
                            Cancel
                        </button>
                    </div>
                </div>
            </div>

            <div id="dataInfoModal" class="hidden fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50 overflow-y-auto">
                <div class="bg-white border-thick shadow-sharp p-8 max-w-2xl mx-4 my-8 animate-pop max-h-[90vh] overflow-y-auto">
                    <h3 class="text-3xl uppercase mb-4 text-black font-teko">📊 Your Data Information</h3>
                    <div id="dataInfoModalContent" class="text-sm text-gray-700">
                        <!-- Will be populated dynamically -->
                    </div>
                    <button id="closeDataInfoBtn" 
                            class="mt-4 bg-black text-white px-6 py-3 text-xl uppercase border-thick shadow-sharp hover-anarchy font-teko w-full">
                        Close
                    </button>
                </div>
            </div>
        `;
    }

    private attachEventListeners(): void {
        // Export Data
        document.getElementById('exportDataBtn')?.addEventListener('click', () => this.exportData());

        // Anonymize Account
        document.getElementById('anonymizeAccountBtn')?.addEventListener('click', () => this.showAnonymizeModal());
        document.getElementById('confirmAnonymizeBtn')?.addEventListener('click', () => this.anonymizeAccount());
        document.getElementById('cancelAnonymizeBtn')?.addEventListener('click', () => this.hideAnonymizeModal());

        // Delete Account
        document.getElementById('deleteAccountBtn')?.addEventListener('click', () => this.showDeleteModal());
        document.getElementById('confirmDeleteBtn')?.addEventListener('click', () => this.deleteAccount());
        document.getElementById('cancelDeleteBtn')?.addEventListener('click', () => this.hideDeleteModal());

        // Data Info
        document.getElementById('loadDataInfoBtn')?.addEventListener('click', () => this.showDataInfo());
        document.getElementById('closeDataInfoBtn')?.addEventListener('click', () => this.hideDataInfoModal());

        // Load basic data info on init
        this.loadBasicDataInfo();
    }

    private async loadBasicDataInfo(): Promise<void> {
        const content = document.getElementById('dataInfoContent');
        if (!content) return;

        try {
            const response = await fetch('/api/user/privacy/info', {
                credentials: 'include'
            });

            if (!response.ok) throw new Error('Failed to load data info');

            const data = await response.json();
            
            content.innerHTML = `
                <div class="space-y-2 font-teko text-base">
                    <p><strong>🎮 Games Played:</strong> ${data.gameData.count}</p>
                    <p><strong>👥 Friends:</strong> ${data.socialData.friendsCount}</p>
                    <p><strong>🏆 Tournaments:</strong> ${data.tournamentData.count}</p>
                    <p class="text-xs text-gray-500 mt-3">Click "View Detailed Data Info" for complete information</p>
                </div>
            `;
        } catch (error) {
            console.error('Error loading basic data info:', error);
            content.innerHTML = '<p class="text-red-600">Failed to load data overview</p>';
        }
    }

    private async showDataInfo(): Promise<void> {
        const modal = document.getElementById('dataInfoModal');
        const modalContent = document.getElementById('dataInfoModalContent');
        if (!modal || !modalContent) return;

        modal.classList.remove('hidden');
        modalContent.innerHTML = '<p>Loading detailed information...</p>';

        try {
            const response = await fetch('/api/user/privacy/info', {
                credentials: 'include'
            });

            if (!response.ok) throw new Error('Failed to load data info');

            const data = await response.json();
            
            modalContent.innerHTML = `
                <div class="space-y-4">
                    <div class="border-thick p-4 bg-blue-50">
                        <h4 class="font-bold text-lg mb-2">Personal Data Stored</h4>
                        <ul class="list-disc list-inside space-y-1">
                            ${data.personalData.stored.map((item: string) => `<li>${item}</li>`).join('')}
                        </ul>
                        <p class="mt-2 text-xs text-gray-600"><strong>Usage:</strong> ${data.personalData.usage}</p>
                    </div>

                    <div class="border-thick p-4 bg-green-50">
                        <h4 class="font-bold text-lg mb-2">Game Data (${data.gameData.count} games)</h4>
                        <ul class="list-disc list-inside space-y-1">
                            ${data.gameData.stored.map((item: string) => `<li>${item}</li>`).join('')}
                        </ul>
                        <p class="mt-2 text-xs text-gray-600"><strong>Usage:</strong> ${data.gameData.usage}</p>
                    </div>

                    <div class="border-thick p-4 bg-yellow-50">
                        <h4 class="font-bold text-lg mb-2">Social Data (${data.socialData.friendsCount} friends)</h4>
                        <ul class="list-disc list-inside space-y-1">
                            ${data.socialData.stored.map((item: string) => `<li>${item}</li>`).join('')}
                        </ul>
                        <p class="mt-2 text-xs text-gray-600"><strong>Usage:</strong> ${data.socialData.usage}</p>
                    </div>

                    <div class="border-thick p-4 bg-purple-50">
                        <h4 class="font-bold text-lg mb-2">Tournament Data (${data.tournamentData.count} tournaments)</h4>
                        <ul class="list-disc list-inside space-y-1">
                            ${data.tournamentData.stored.map((item: string) => `<li>${item}</li>`).join('')}
                        </ul>
                    </div>

                    <div class="border-thick p-4 bg-gray-50">
                        <h4 class="font-bold text-lg mb-2">Your GDPR Rights</h4>
                        <ul class="list-disc list-inside space-y-1">
                            ${data.yourRights.map((right: string) => `<li>${right}</li>`).join('')}
                        </ul>
                    </div>

                    <div class="border-thick p-4 bg-orange-50">
                        <h4 class="font-bold text-lg mb-2">Data Retention & Storage</h4>
                        <p class="mb-2">${data.dataRetention.policy}</p>
                        <p class="text-xs text-gray-600"><strong>Location:</strong> ${data.dataRetention.location}</p>
                    </div>
                </div>
            `;
        } catch (error) {
            console.error('Error loading data info:', error);
            modalContent.innerHTML = '<p class="text-red-600">Failed to load detailed data information</p>';
        }
    }

    private hideDataInfoModal(): void {
        document.getElementById('dataInfoModal')?.classList.add('hidden');
    }

    private async exportData(): Promise<void> {
        const btn = document.getElementById('exportDataBtn') as HTMLButtonElement;
        const originalText = btn.textContent;
        btn.textContent = 'Exporting...';
        btn.disabled = true;

        try {
            const response = await fetch('/api/user/privacy/export', {
                credentials: 'include'
            });

            if (!response.ok) throw new Error('Failed to export data');

            const blob = await response.blob();
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = `pong-data-export-${Date.now()}.json`;
            document.body.appendChild(a);
            a.click();
            window.URL.revokeObjectURL(url);
            document.body.removeChild(a);

            showNotification('Data exported successfully! Check your downloads.', 'success');
        } catch (error) {
            console.error('Export error:', error);
            showNotification('Failed to export data. Please try again.', 'error');
        } finally {
            btn.textContent = originalText;
            btn.disabled = false;
        }
    }

    private async showAnonymizeModal(): Promise<void> {
        document.getElementById('anonymizeModal')?.classList.remove('hidden');
    }

    private hideAnonymizeModal(): void {
        document.getElementById('anonymizeModal')?.classList.add('hidden');
        (document.getElementById('anonymizeConfirmText') as HTMLInputElement).value = '';
    }

    private async anonymizeAccount(): Promise<void> {
        const confirmTextInput = document.getElementById('anonymizeConfirmText') as HTMLInputElement;
        const confirmBtn = document.getElementById('confirmAnonymizeBtn') as HTMLButtonElement;
        const originalText = confirmBtn.textContent;
        
        // Check confirmation text
        if (confirmTextInput.value !== 'ANONYMIZE FOREVER') {
            showNotification('Please type "ANONYMIZE FOREVER" to confirm', 'error');
            return;
        }

        confirmBtn.textContent = 'Processing...';
        confirmBtn.disabled = true;

        try {
            const response = await fetch('/api/user/privacy/anonymize', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                credentials: 'include',
                body: JSON.stringify({}) // No password needed - JWT is sufficient
            });

            const result = await response.json();

            if (response.ok) {
                // Show a clear message
                showNotification('⚠️ Account anonymized. This account cannot be used again.', 'success');
                setTimeout(() => {
                    window.location.href = '/login.html';
                }, 3000);
            } else {
                showNotification(result.error || 'Failed to anonymize account', 'error');
                confirmBtn.textContent = originalText;
                confirmBtn.disabled = false;
            }
        } catch (error) {
            console.error('Anonymization error:', error);
            showNotification('An error occurred. Please try again.', 'error');
            confirmBtn.textContent = originalText;
            confirmBtn.disabled = false;
        }
    }

    private async showDeleteModal(): Promise<void> {
        document.getElementById('deleteModal')?.classList.remove('hidden');
    }

    private hideDeleteModal(): void {
        document.getElementById('deleteModal')?.classList.add('hidden');
        (document.getElementById('deleteConfirmText') as HTMLInputElement).value = '';
    }

    private async deleteAccount(): Promise<void> {
        const confirmTextInput = document.getElementById('deleteConfirmText') as HTMLInputElement;
        const confirmBtn = document.getElementById('confirmDeleteBtn') as HTMLButtonElement;
        const originalText = confirmBtn.textContent;

        if (confirmTextInput.value !== 'DELETE MY ACCOUNT') {
            showNotification('Please type "DELETE MY ACCOUNT" to confirm', 'error');
            return;
        }

        confirmBtn.textContent = 'Deleting...';
        confirmBtn.disabled = true;

        try {
            const response = await fetch('/api/user/privacy/delete', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                credentials: 'include',
                body: JSON.stringify({
                    confirmText: confirmTextInput.value
                    // No password needed - JWT + text confirmation is sufficient
                })
            });

            const result = await response.json();

            if (response.ok) {
                showNotification('🗑️ Account permanently deleted. All data destroyed.', 'success');
                setTimeout(() => {
                    window.location.href = '/login.html';
                }, 3000);
            } else {
                showNotification(result.error || 'Failed to delete account', 'error');
                confirmBtn.textContent = originalText;
                confirmBtn.disabled = false;
            }
        } catch (error) {
            console.error('Deletion error:', error);
            showNotification('An error occurred. Please try again.', 'error');
            confirmBtn.textContent = originalText;
            confirmBtn.disabled = false;
        }
    }
}