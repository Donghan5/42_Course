export function redirectGoogleLogin() {
	const backendApiUrl = import.meta.env.VITE_API_URL;

	if (!backendApiUrl) {
		console.error("VITE_API_URL is not defined! Check your .env and docker-compose.yaml files.");
		return;
	}

	const googleLoginUrl = `${backendApiUrl}/api/users/login/google`;
	window.location.href = googleLoginUrl;
}
