import { showNotification } from '../src/services/ui';

export class ProfileEditForm {
    private container: HTMLElement;
    private currentUser: any;
    private onUpdate: () => void;

    constructor(containerElement: HTMLElement, currentUser: any, onUpdate: () => void) {
        this.container = containerElement;
        this.currentUser = currentUser;
        this.onUpdate = onUpdate;
        this.render();
        this.attachEventListeners();
    }

    private render(): void {
        const isLocalAuth = this.currentUser.auth_provider === 'local';
        
        this.container.innerHTML = `
            <div class="profile-edit-form bg-white border-thick shadow-sharp p-6 mt-6 animate-pop">
                <h2 class="text-3xl uppercase mb-6 text-black text-center font-teko">Edit Profile</h2>
                
                <form id="profileEditForm" class="space-y-4">
                    <div>
                        <label class="block text-lg uppercase mb-2 font-teko" for="editNickname">
                            Nickname
                        </label>
                        <input 
                            type="text" 
                            id="editNickname" 
                            name="nickname"
                            value="${this.currentUser.nickname || ''}"
                            class="w-full p-3 text-xl border-thick"
                            placeholder="Your nickname"
                            minlength="3"
                            required
                        >
                        <p class="text-sm text-gray-600 mt-1">Minimum 3 characters. Must be unique.</p>
                    </div>

                    <div>
                        <label class="block text-lg uppercase mb-2 font-teko" for="editName">
                            Display Name
                        </label>
                        <input 
                            type="text" 
                            id="editName" 
                            name="name"
                            value="${this.currentUser.name || ''}"
                            class="w-full p-3 text-xl border-thick"
                            placeholder="Your display name"
                            required
                        >
                    </div>

                    <div>
                        <label class="block text-lg uppercase mb-2 font-teko ${isLocalAuth ? '' : 'text-gray-500'}">
                            Email ${isLocalAuth ? '' : '(Cannot be changed)'}
                        </label>
                        <input 
                            type="email" 
                            id="editEmail"
                            name="email"
                            value="${this.currentUser.email || ''}"
                            class="w-full p-3 text-xl border-thick ${isLocalAuth ? '' : 'bg-gray-100 text-gray-500'}"
                            placeholder="Your email"
                            ${isLocalAuth ? '' : 'disabled readonly'}
                        >
                        <p class="text-sm ${isLocalAuth ? 'text-gray-600' : 'text-gray-500'} mt-1">
                            ${isLocalAuth 
                                ? 'Changing email requires password confirmation below' 
                                : 'Email is linked to your Google account and cannot be changed'}
                        </p>
                    </div>

                    ${isLocalAuth ? `
                        <div id="passwordConfirmSection" class="hidden bg-yellow-50 border-thick border-yellow-500 p-4">
                            <label class="block text-lg uppercase mb-2 font-teko text-yellow-700" for="confirmPassword">
                                ⚠️ Confirm Password (Required for Email Change)
                            </label>
                            <input 
                                type="password" 
                                id="confirmPassword" 
                                name="password"
                                class="w-full p-3 text-xl border-thick"
                                placeholder="Enter your password"
                            >
                            <p class="text-sm text-yellow-700 mt-1">Enter your current password to confirm email change.</p>
                        </div>
                    ` : ''}

                    <button 
                        type="submit" 
                        class="w-full bg-green-500 text-white px-6 py-3 text-xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
                        Save Changes
                    </button>
                </form>

                <div class="mt-4 text-sm text-gray-600 text-center border-t-2 border-black pt-4">
                    <p class="font-teko">
                        🛡️ <strong>Your GDPR Rights:</strong> You have the right to update your personal information at any time.
                    </p>
                </div>
            </div>
        `;
    }

    private attachEventListeners(): void {
        const form = document.getElementById('profileEditForm') as HTMLFormElement;
        const emailInput = document.getElementById('editEmail') as HTMLInputElement;
        const passwordSection = document.getElementById('passwordConfirmSection');
        const passwordInput = document.getElementById('confirmPassword') as HTMLInputElement;

        // Show password confirmation when email is changed (for local users only)
        if (emailInput && passwordSection && this.currentUser.auth_provider === 'local') {
            const originalEmail = this.currentUser.email;
            
            emailInput.addEventListener('input', () => {
                if (emailInput.value !== originalEmail) {
                    passwordSection.classList.remove('hidden');
                    if (passwordInput) {
                        passwordInput.required = true;
                    }
                } else {
                    passwordSection.classList.add('hidden');
                    if (passwordInput) {
                        passwordInput.required = false;
                        passwordInput.value = '';
                    }
                }
            });
        }

        form?.addEventListener('submit', async (e) => {
            e.preventDefault();
            await this.handleSubmit(e);
        });
    }

    private async handleSubmit(event: Event): Promise<void> {
        event.preventDefault();
        
        const form = event.target as HTMLFormElement;
        const formData = new FormData(form);
        const nickname = formData.get('nickname') as string;
        const name = formData.get('name') as string;
        const email = formData.get('email') as string;
        const password = formData.get('password') as string;

        const submitBtn = form.querySelector('button[type="submit"]') as HTMLButtonElement;
        const originalText = submitBtn.textContent;
        submitBtn.textContent = 'Saving...';
        submitBtn.disabled = true;

        try {
            const updateData: any = { nickname, name };
            
            // Only include email if it's changed (for local users)
            if (this.currentUser.auth_provider === 'local' && email !== this.currentUser.email) {
                updateData.email = email;
                updateData.password = password;
            }

            const response = await fetch('/api/user/update', {
                method: 'PATCH',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(updateData),
                credentials: 'include'
            });

            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.error || 'Failed to update profile');
            }

            const data = await response.json();
            showNotification('Profile updated successfully!', 'success');
            
            // Clear form and trigger update
            this.container.innerHTML = '';
            this.onUpdate();
            
        } catch (error) {
            console.error('Error updating profile:', error);
            showNotification(
                `Failed to update profile: ${error instanceof Error ? error.message : 'Unknown error'}`,
                'error'
            );
            submitBtn.textContent = originalText;
            submitBtn.disabled = false;
        }
    }
}