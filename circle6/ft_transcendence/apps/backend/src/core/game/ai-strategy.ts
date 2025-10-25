import { AI_STRATEGY, selectAiStrategy, predictBallPosition } from "./ai-logic";
import { GameState, PADDLE_DEPTH } from "@trans/common-types";
import { initState } from "./logic";

/**
 * @description Enum representing different AI strategies.
 */
enum AIState {
	DEFENSIVE,
	AGGRESSIVE,
	CENTERING,
	TRICK_SHOT
}

class AiStrategy {
	private currentState: AIState = AIState.CENTERING;
	private lastStateChange: number = 0;
	private reactionThresholdX: number = 0;
	private hasTarget: boolean = false;
	private targetZ: number = 0;

	/**
	 * @description Make the decision of next AI state based on the current game state
	 * @param game: current game state
	 * @returns: void
	 */
	private decideNextState(game: GameState): void {
		const ball = game.ball;
		const aiPlayer = game.player2;
		const now = Date.now();

		if (ball.velocity.x < 0) {
			this.currentState = AIState.CENTERING;
			this.hasTarget = false;
			return;
		}

		if (ball.velocity.x > 0 && !this.hasTarget) {
			this.lastStateChange = now;
			const levelKey = (game.aiStrategy || 'Defensive') as keyof typeof AI_STRATEGY;
			const level = AI_STRATEGY[levelKey];

			const perfectTargetZ = predictBallPosition(ball, game.player2.position.x, level);
			let finalTargetZ = perfectTargetZ;

			if (Math.random() < level.mistakeChance) {
				console.log("AI decided to make a mistake!");
				const mistakeDirection = perfectTargetZ > 0 ? -1 : 1;
				finalTargetZ = perfectTargetZ + (mistakeDirection * PADDLE_DEPTH * 0.9);
			} else {
				if (Math.random() < level.mistakeChance) {
					const maxError = (1 - level.accuracy) * 15;
					const error = (Math.random() - 0.5) * maxError;
					finalTargetZ += error;
				}
			}


			this.targetZ = finalTargetZ;
			this.hasTarget = true;
			
			const speed = Math.sqrt(ball.velocity.x ** 2 + ball.velocity.z ** 2);
			const scoreDiff = aiPlayer.score - game.player1.score;

			if (scoreDiff > 2 && Math.random() < 0.15) {
				this.currentState = AIState.TRICK_SHOT;
			} else if (speed < 20 || (scoreDiff < 0 && Math.random() < 0.3)) {
				this.currentState = AIState.AGGRESSIVE;
			} else {
				this.currentState = AIState.DEFENSIVE;
			}

		}
	}

	/**
	 * @description Execute the current AI defensive action
	 */
	private executeDefensive(game: GameState, level: any): void {
		const targetZ = predictBallPosition(game.ball, game.player2.position.x, level);
		const diff = targetZ - game.player2.paddleZ;
		game.player2.paddleZ += diff * level.speed;
	}

	/**
	 * @description Execute the current AI aggressive action
	 */
	private executeAggressive(game: GameState, level: any): void {
		let targetZ = predictBallPosition(game.ball, game.player2.position.x, level);

		const paddleEdgeOffset = (PADDLE_DEPTH / 2) * 0.7;
		if (game.ball.velocity.z > 0) {
			targetZ -= paddleEdgeOffset;
		} else {
			targetZ += paddleEdgeOffset;
		}

		const diff = targetZ - game.player2.paddleZ;
		game.player2.paddleZ += diff * level.speed * 1.2;
	}

	/**
	 * @description Execute the current AI centering action
	 */
	private executeCentering(game: GameState, level: any): void {
		const diff = 0 - game.player2.paddleZ;
		game.player2.paddleZ += diff * 0.08;
	}

	/**
	 * @description Execute a trick shot to confuse the opponent
	 */
	private executeTrickShot(game: GameState, level: any): void {
		const timeToPaddle = Math.abs((game.player2.position.x - game.ball.position.x) / game.ball.velocity.x);

		if (timeToPaddle > 0.15) {
			const fakeTargetZ = game.player2.paddleZ + (Math.random() > 0.5 ? 2 : -2);
			const diff = fakeTargetZ - game.player2.paddleZ;
			game.player2.paddleZ += diff * level.speed * 0.5;
		} else {
			const targetZ = predictBallPosition(game.ball, game.player2.position.x, level);
			const diff = targetZ - game.player2.paddleZ;
			game.player2.paddleZ += diff * level.speed * 2.5; 
		}
	}

	/**
	 * @description to update the game state with AI decisions
	 * @param game: current game state
	 */
	public updateAIPaddle(game: GameState): void {
		this.decideNextState(game);

		const levelKey = (game.aiStrategy || 'Defensive') as keyof typeof AI_STRATEGY;
		const level = AI_STRATEGY[levelKey];

		this.reactionThresholdX = 4;

		if (game.ball.velocity.x > 0 && game.ball.position.x < this.reactionThresholdX) {
			this.executeCentering(game, level);
		} else {
			switch (this.currentState) {
				case AIState.AGGRESSIVE:
					this.executeAggressive(game, level);
					break;
				case AIState.CENTERING:
					this.executeCentering(game, level);
					break;
				case AIState.TRICK_SHOT:
					this.executeTrickShot(game, level);
					break;
				case AIState.DEFENSIVE:
				default:
					this.executeDefensive(game, level);
					break;
			}
		}

		const paddleLimit = 16;
		if (game.player2.paddleZ > paddleLimit) game.player2.paddleZ = paddleLimit;
		if (game.player2.paddleZ < -paddleLimit) game.player2.paddleZ = -paddleLimit;
	}
}

export const strategy = new AiStrategy();