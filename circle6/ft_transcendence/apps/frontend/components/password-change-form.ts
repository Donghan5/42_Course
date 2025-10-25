// apps/frontend/components/password-change-form.ts

import { AuthService } from '../auth/services/auth-service';
import { PasswordChangeRequest } from '@trans/common-types';

export class PasswordChangeForm {
    private form: HTMLElement;

    constructor(container: HTMLElement) {
        this.form = this.createForm();
        container.appendChild(this.form);
        this.attachEventListeners();
    }

    private createForm(): HTMLElement {
        const formContainer = document.createElement('div');
        formContainer.className = 'password-change-form';

        formContainer.innerHTML = `
			<div class="bg-white border-thick shadow-sharp p-6 sm:p-8 animate-pop">
				<h2 class="text-4xl uppercase mb-6 text-black text-center">Change Password</h2>

				<form id="changePasswordForm" class="space-y-5">
					<div class="form-row">
						<label for="currentPassword" class="form-label">Current Password</label>
						<input type="password"
							class="form-input"
							id="currentPassword"
							name="currentPassword"
							required
							placeholder="••••••••">
					</div>

					<div class="form-row">
						<label for="newPassword" class="form-label">New Password</label>
						<input type="password"
							class="form-input"
							id="newPassword"
							name="newPassword"
							required
							placeholder="MIN. 8 CHARACTERS">
					</div>

					<div class="form-row">
						<label for="confirmNewPassword" class="form-label">Confirm Password</label>
						<input type="password"
							class="form-input"
							id="confirmNewPassword"
							name="confirmNewPassword"
							required
							placeholder="TYPE IT AGAIN!">
					</div>

					<div class="flex gap-4 mt-8">
						<button type="button"
								id="changePasswordBtn"
								class="flex-1 bg-pink-500 text-white px-6 py-3 text-2xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
							Change Password
						</button>
						<button type="button"
								id="cancelBtn"
								class="flex-1 bg-black text-white px-6 py-3 text-2xl uppercase border-thick shadow-sharp hover-anarchy font-teko">
							Cancel
						</button>
					</div>

					<div id="errorMessage" class="error-message hidden"></div>
					<div id="successMessage" class="success-message hidden"></div>
				</form>
			</div>
		`;

        return formContainer;
    }

    private attachEventListeners(): void {
        const changeBtn = this.form.querySelector('#changePasswordBtn') as HTMLButtonElement;
        const cancelBtn = this.form.querySelector('#cancelBtn') as HTMLButtonElement;

        changeBtn.addEventListener('click', () => this.handlePasswordChange());
        cancelBtn.addEventListener('click', () => this.clearForm());

        this.form.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.handlePasswordChange();
            }
        });
    }

    private async handlePasswordChange(): Promise<void> {
        const currentPassword = (this.form.querySelector('#currentPassword') as HTMLInputElement).value;
        const newPassword = (this.form.querySelector('#newPassword') as HTMLInputElement).value;
        const confirmNewPassword = (this.form.querySelector('#confirmNewPassword') as HTMLInputElement).value;

        this.hideMessages();

        const passwordData: PasswordChangeRequest = {
            currentPassword,
            newPassword,
            confirmNewPassword
        };

        const validationError = AuthService.validatePasswordChange(passwordData);
        if (validationError) {
            this.showError(validationError);
            return;
        }

        const changeBtn = this.form.querySelector('#changePasswordBtn') as HTMLButtonElement;
        const originalText = changeBtn.textContent;
        changeBtn.textContent = 'Changing...';
        changeBtn.disabled = true;

        try {
            const result = await AuthService.changePassword(passwordData);

            if (result.success) {
                this.showSuccess(result.message || 'Password changed successfully.');
				setTimeout(() => {
					this.clearFormInputsOnly();
				}, 2000);
			} else {
                this.showError(result.error || 'Failed to change password.');
            }
        } catch (error) {
            console.error('Password change error:', error);
            this.showError('An error occurred while changing password.');
        } finally {
            changeBtn.textContent = originalText;
            changeBtn.disabled = false;
        }
    }

    private showError(message: string): void {
        const errorDiv = this.form.querySelector('#errorMessage') as HTMLElement;
        errorDiv.textContent = message;
        errorDiv.style.display = 'block';
    }

    private showSuccess(message: string): void {
        const successDiv = this.form.querySelector('#successMessage') as HTMLElement;
        successDiv.textContent = message;
        successDiv.style.display = 'block';
    }

    private hideMessages(): void {
        const errorDiv = this.form.querySelector('#errorMessage') as HTMLElement;
        const successDiv = this.form.querySelector('#successMessage') as HTMLElement;
        errorDiv.style.display = 'none';
        successDiv.style.display = 'none';
    }

    private clearForm(): void {
        const inputs = this.form.querySelectorAll('input');
        inputs.forEach(input => input.value = '');
        this.hideMessages();
    }

	private clearFormInputsOnly(): void {
        const inputs = this.form.querySelectorAll('input');
        inputs.forEach(input => input.value = '');
    }
}
