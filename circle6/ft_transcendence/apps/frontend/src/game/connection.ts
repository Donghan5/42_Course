export interface ConnectionEvenets {
	'gameState': (gameState: any) => void;
	'playerJoined': (playerId: string) => void;
	'waitingForOpponent': (playerSide: 'player1' | 'player2') => void;
	'playerLeft': (playerId: string) => void;
	'updateScore': (scores: { player1: number, player2: number }) => void;
	'gameEnd': (data: { winnerId: string, winnerNickname: string, finalScore: { player1: number, player2: number }}) => void;
	'connectionLost': () => void;
	'connectionRestored': () => void;
	'error': (error: string) => void;
	'gamePaused': (data: { message: string, disconnectedPlayerId: string, remainingTime: number }) => void;
	'gameResumed': (data: { message: string, countdown: number }) => void;
	'gameFullyResumed': (data: {}) => void;
	'reconnected': (data: { message: string, gameState: any }) => void;
	'session_replaced': (data: { message: string }) => void;
}

export class Connection {
	private ws: WebSocket | null = null
	public gameId: string
	public playerId: string
	private reconnectAttempts: number = 0
	private maxReconnectAttempts: number = 5
	private reconnectDelay: number = 1000
	private isConnected = false
	private heartbeatInterval: ReturnType<typeof setTimeout> | null = null

	private eventListeners: Map<keyof ConnectionEvenets, Function[]> = new Map()

	constructor(gameId: string, playerId: string) {
		this.gameId = gameId
		this.playerId = playerId

		console.log(`🔌 Player ${this.playerId} connected to game ${this.gameId}`)
	}

	on<K extends keyof ConnectionEvenets>(event: K, callback: ConnectionEvenets[K]): void {
		if (!this.eventListeners.has(event)) {
			this.eventListeners.set(event, [])
		}
		this.eventListeners.get(event)?.push(callback)
	}

	private emit<K extends keyof ConnectionEvenets>(
		event: K,
		...args: Parameters<ConnectionEvenets[K]>
	): void {
		const listeners = this.eventListeners.get(event) || []
		listeners.forEach(listener => listener(...args))
	}

	async connect(): Promise<void> {
		try {
			const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
			const wsUrl = `${protocol}//${window.location.host}/api/games/ws/${this.gameId}?playerId=${this.playerId}`;

			console.log(`🔌 Connecting to WebSocket server at ${wsUrl}`);

			this.ws = new WebSocket(wsUrl);

			return new Promise<void>((resolve, reject) => {
				if (!this.ws) {
					reject(new Error('WebSocket connection failed'));
					return;
				}

				this.ws.onopen = () => {
					console.log(`🔌 WebSocket connection opened for player ${this.playerId}`);
					this.isConnected = true;
					this.reconnectAttempts = 0;
					this.startHeartbeat();
					this.emit('connectionRestored');
					resolve();
				}

				this.ws.onmessage = (event) => {
					try {
						const data = JSON.parse(event.data.toString());
						this.handleMessage(data);
					} catch (error) {
						console.error('❌ Message parsing error:', error);
					}
				}

				this.ws.onclose = (event) => {
					console.log(`🔌 WebSocket connection closed for player ${this.playerId}`);
					this.isConnected = false;
					this.stopHeartbeat();

					if (!event.wasClean) {
						this.emit('connectionLost');
						this.attemptReconnect();
					}
				}

				this.ws.onerror = (error) => {
					console.error(`❌ WebSocket error for player ${this.playerId}:`, error);
					this.emit('error', 'WebSocket connection error');
					reject(new Error('WebSocket connection failed'));
				}
			});
		} catch (error) {
			console.error('❌ WebSocket connection failed:', error);
			throw error;
		}
	}

	private handleMessage(data: any): void {
		switch (data.type) {
			case 'gameState':
				this.emit('gameState', data.payload);
				break;
			case 'playerJoined':
				this.emit('playerJoined', data.playerId);
				break;
			case 'waitingForOpponent':
				this.emit('waitingForOpponent', data.playerSide);
				break;
			case 'playerLeft':
				this.emit('playerLeft', data.playerId);
				break;
			case 'updateScore':
				this.emit('updateScore', data.scores);
				break;
			case 'gameEnd':
				this.emit('gameEnd', data.payload);
				break;
			case 'pong':
				console.log('Heartbeat response received');
				break;
			case 'gamePaused':
				this.emit('gamePaused', data);
				break;
			case 'gameResumed':
				this.emit('gameResumed', data);
				break;
			case 'gameFullyResumed':
				this.emit('gameFullyResumed', data);
				break;
			case 'reconnected':
				this.emit('reconnected', data);
				console.log('✅ Reconnected successfully:', data.message);
				break;
			case 'session_replaced':
				this.emit('session_replaced', data);
				alert(data.message);
				window.location.href = '/login';
				break;
			default:
				console.log('Unknown message type:', data.type);
		}
	}

	private send(message: string): void {
		if (this.ws && this.ws.readyState === WebSocket.OPEN) {
			this.ws.send(message);
		}
	}

	private attemptReconnect(): void {
		if (this.reconnectAttempts >= this.maxReconnectAttempts) {
			console.error('❌ Max reconnection attempts reached');
			this.emit('error', 'Failed to reconnect after maximum attempts');
			return;
		}

		this.reconnectAttempts++;
		console.log(`🔄 Attempting to reconnect... (${this.reconnectAttempts}/${this.maxReconnectAttempts})`);

		setTimeout(() => {
			this.connect().catch(error => {
				console.error('❌ Reconnection failed:', error);
				this.attemptReconnect();
			});
		}, this.reconnectDelay * this.reconnectAttempts);
	}

	private startHeartbeat(): void {
		this.heartbeatInterval = setInterval(() => {
			if (this.isConnected && this.ws && this.ws.readyState === WebSocket.OPEN) {
				this.send(JSON.stringify({ type: 'ping', timestamp: Date.now() }));
			}
		}, 30000);
	}

	private stopHeartbeat(): void {
		if (this.heartbeatInterval) {
			clearInterval(this.heartbeatInterval);
			this.heartbeatInterval = null;
		}
	}

	disconnect(): void {
		this.isConnected = false;
		this.stopHeartbeat();

		if (this.ws) {
			this.ws.close();
			this.ws = null;
		}
	}

	sendPlayerInput(inputState: 'up' | 'down' | 'stop', playerId?: string): void {
        this.send(JSON.stringify({
            type: 'playerInput',
            inputState: inputState,
            playerId: playerId || this.playerId,
            timestamp: Date.now()
        }));
    }

	sendGameAction(action: string, data?: any): void {
		this.send(JSON.stringify({
			type: 'gameAction',
			action,
			data,
			playerId: this.playerId
		}));
	}
}

export function updateConnectionStatus(status: 'connecting' | 'connected' | 'disconnected') {
	const indicator = document.getElementById('statusIndicator')
	const statusText = document.getElementById('statusText')

	if (indicator && statusText) {
		indicator.className = 'w-3 h-3 rounded-full animate-pulse'

		switch(status) {
			case 'connecting':
				indicator.classList.add('bg-yellow-500')
				break
			case 'connected':
				indicator.classList.add('bg-green-500')
				break
			case 'disconnected':
				indicator.classList.add('bg-red-500')
				break
		}

		statusText.textContent = status
	}
}
