// ft_transcendence/apps/frontend/src/game/render.ts

import {
    Scene, Engine, PointerEventTypes, Mesh, ParticleSystem, KeyboardEventTypes,
    Animation, IAnimationKey, EasingFunction, ElasticEase, Vector3, ArcRotateCamera,
    StandardMaterial, Color3, MeshBuilder, DynamicTexture
} from '@babylonjs/core';
import { GameState } from '@trans/common-types';
import { Connection } from './connection';
import { SceneObjects, createSceneAndGameObjects } from './scene-builder';
import { InputManager } from './input-manager';
import { GameUIManager } from './ui-manager';
import {
    PADDLE_DEPTH, BALL_RADIUS, PADDLE_COLLISION_X_LEFT, PADDLE_COLLISION_X_RIGHT,
    WALL_COLLISION_Z_TOP, WALL_COLLISION_Z_BOTTOM, PADDLE_Y_POSITION, GOAL_LINE_LEFT, GOAL_LINE_RIGHT
} from "@trans/common-types";

export class PongGame3D {
    private engine: Engine;
    private scene: Scene;
    private connection: Connection;
    private inputManager: InputManager;
    private uiManager: GameUIManager;

    private player1Paddle: Mesh;
    private player2Paddle: Mesh;
    private ball: Mesh;
    private particleSystem: ParticleSystem;
    private spawnParticleSystem: ParticleSystem;

    public state: GameState | null = null;
    private previousState: GameState | null = null;
    private gameEndHandled: boolean = false;
    private disposed: boolean = false;
    private resizeHandler: () => void;
    private previousBallPosition: Vector3 | null = null;
    private pauseOverlay: HTMLElement | null = null;
    private pauseCountdownInterval: any = null;

    constructor(canvas: HTMLCanvasElement, gameId: string, playerId: string, gameMode: string, playerNickname: string) {
        this.engine = new Engine(canvas, true);
        this.scene = new Scene(this.engine);

        canvas.focus();

        const sceneObjects: SceneObjects = createSceneAndGameObjects(this.scene, canvas);
        this.player1Paddle = sceneObjects.player1Paddle;
        this.player2Paddle = sceneObjects.player2Paddle;
        this.ball = sceneObjects.ball;
        this.particleSystem = sceneObjects.particleSystem;
        this.spawnParticleSystem = sceneObjects.spawnParticleSystem;

        this.connection = new Connection(gameId, playerId);
        this.inputManager = new InputManager(this.scene, this.connection, playerId, gameMode);
        this.uiManager = new GameUIManager(gameMode);

        this.setupConnectionHandlers();
        this.connection.connect().catch(err => console.error('Connection failed:', err));

        setTimeout(() => {
            this.engine.resize();
        }, 50); // change made here from 100 to 50 (modification)

        this.engine.runRenderLoop(() => {
            if (this.disposed) return;
            this.inputManager.update();
            this.interpolatePositions();
            this.updateBallSpeedEffects();
            this.detectCollisions();
            this.previousBallPosition = this.ball.position.clone();
            this.scene.render();
        });

        this.resizeHandler = () => this.engine.resize();
        window.addEventListener('resize', this.resizeHandler);
    }

    /**
     * Show pause overlay when opponent disconnects
     */
    private showPauseOverlay(message: string, remainingTime: number): void {
        this.removePauseOverlay();

        const overlay = document.createElement('div');
        overlay.id = 'pauseOverlay';
        overlay.className = 'fixed inset-0 bg-black/80 flex flex-col items-center justify-center z-[11000] font-anton';
        overlay.style.backgroundImage = 'repeating-linear-gradient(45deg, transparent, transparent 10px, rgba(255,255,255,.05) 10px, rgba(255,255,255,.05) 20px)';

        overlay.innerHTML = `
            <div class="text-center">
                <div class="bg-white border-[8px] border-black p-12 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] -rotate-2 mb-12">
                    <h1 class="text-5xl text-black uppercase tracking-wider m-0 [text-shadow:4px_4px_0_#ff4444]">
                        PLAYER DISCONNECTED
                    </h1>
                </div>
                
                <div class="bg-red-500 border-[8px] border-black p-8 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] rotate-1">
                    <div id="pauseCountdown" class="text-[10rem] text-white font-bold [text-shadow:6px_6px_0_black] leading-none">
                        ${remainingTime}
                    </div>
                </div>
            </div>
        `;

        document.body.appendChild(overlay);
        this.pauseOverlay = overlay;

        let currentTime = remainingTime;
        this.pauseCountdownInterval = setInterval(() => {
            currentTime--;
            const countdownEl = document.getElementById('pauseCountdown');
            if (countdownEl) {
                countdownEl.textContent = String(currentTime);
                countdownEl.style.transform = 'scale(1.1)';
                setTimeout(() => countdownEl.style.transform = 'scale(1)', 150);
            }
            if (currentTime <= 0) this.removePauseOverlay();
        }, 1000);
    }

    /**
     * Remove pause overlay
     */
    private removePauseOverlay(): void {
        if (this.pauseCountdownInterval) {
            clearInterval(this.pauseCountdownInterval);
            this.pauseCountdownInterval = null;
        }
        
        if (this.pauseOverlay) {
            this.pauseOverlay.remove();
            this.pauseOverlay = null;
        }
    }

    /**
     * Show resume countdown
     */
    private showResumeCountdown(countdown: number): void {
        this.removePauseOverlay();

        const overlay = document.createElement('div');
        overlay.id = 'resumeOverlay';
        overlay.className = 'fixed inset-0 bg-black/75 flex flex-col items-center justify-center z-[11000] font-anton';
        overlay.style.backgroundImage = 'repeating-linear-gradient(45deg, transparent, transparent 10px, rgba(255,255,255,.05) 10px, rgba(255,255,255,.05) 20px)';

        overlay.innerHTML = `
            <div class="text-center">
                <div class="bg-white border-[8px] border-black p-12 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] -rotate-2 mb-12">
                    <h1 class="text-5xl text-black uppercase tracking-wider m-0 [text-shadow:4px_4px_0_#4CAF50]">
                        RESUMING
                    </h1>
                </div>
                
                <div class="bg-green-500 border-[8px] border-black p-8 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] rotate-1">
                    <div id="resumeCountdown" class="text-[10rem] text-white font-bold [text-shadow:6px_6px_0_black] leading-none">
                        ${countdown}
                    </div>
                </div>
            </div>
        `;

        document.body.appendChild(overlay);

        let current = countdown;
        const countdownInterval = setInterval(() => {
            current--;
            const countdownEl = document.getElementById('resumeCountdown');
            if (countdownEl && current > 0) {
                countdownEl.textContent = String(current);
                countdownEl.style.transform = 'scale(1.1)';
                setTimeout(() => countdownEl.style.transform = 'scale(1)', 150);
            } else {
                clearInterval(countdownInterval);
                document.getElementById('resumeOverlay')?.remove();
            }
        }, 1000);
    }

    private setupConnectionHandlers(): void {
        this.connection.on('gameState', (newState: GameState) => {
            if (this.gameEndHandled) return;

            this.previousState = this.state ? { ...this.state } : newState;
            this.state = newState;
            this.uiManager.state = newState;
            this.inputManager.state = newState;  // added to update input state

            if (this.state.status === 'countdown' && this.state.countdownValue !== undefined) {
                 const playerSide = this.state.player1Id === this.connection.playerId ? 'player1' : 'player2';
                 this.uiManager.showCountdownSpotlight(playerSide, this.state.countdownValue);
            } else {
                 this.uiManager.hideCountdownSpotlight();
            }
            
            if (this.state.status !== 'playing') {
                 this.uiManager.hideWaitingForOpponent();
            }

            this.uiManager.updateScoreDisplay();

            if (this.previousState && (this.previousState.player1.score !== newState.player1.score || this.previousState.player2.score !== newState.player2.score)) {
                this.onScoreUpdate();
            }
        });

        this.connection.on('waitingForOpponent', (playerSide) => {
            this.uiManager.showWaitingForOpponent(playerSide);
        });
        
        this.connection.on('playerJoined', () => {
             this.uiManager.hideWaitingForOpponent();
        });

        this.connection.on('gameEnd', (data) => {
            this.onGameEnd(data);
        });

        this.connection.on('connectionLost', () => {
            this.uiManager.hideWaitingForOpponent();
        });

        this.connection.on('gamePaused', (data) => {
            console.log('🛑 Game paused:', data);
            this.showPauseOverlay(data.message, data.remainingTime);
        });

        this.connection.on('gameResumed', (data) => {
            console.log('▶️ Game resuming:', data);
            this.showResumeCountdown(data.countdown);
        });

        this.connection.on('reconnected', (data) => {
            console.log('🔄 Reconnected to game:', data);
        });

        this.connection.on('session_replaced', (data) => {
            console.log('⚠️ Session replaced:', data.message);
            alert(data.message);
            // Clean up and redirect
            this.dispose();
            window.location.href = '/login';
        });
    }

    private onGameEnd(data: { winnerNickname: string, winnerId: string, finalScore: any }): void {
        if (this.gameEndHandled) return;
        this.gameEndHandled = true;

        this.removePauseOverlay();
        this.connection.disconnect();
        this.uiManager.dispose();

        const modal = document.getElementById('gameOverModal');
        const modalContent = document.getElementById('gameOverModalContent');
        const winnerMessage = document.getElementById('winnerMessage');
        const gameOverTitle = document.getElementById('gameOverTitle');

        if (modal && modalContent && winnerMessage && gameOverTitle) {
            const isWinner = data.winnerId?.toString() === this.connection.playerId?.toString();
            
            modalContent.classList.remove('bg-yellow-300', 'bg-red-600');
            
            if (isWinner) {
                gameOverTitle.textContent = 'VICTORY!';
                modalContent.classList.add('bg-yellow-300');
                gameOverTitle.className = 'text-6xl text-red-600 text-outline-lg-black font-black mb-4';
            } else {
                gameOverTitle.textContent = 'DEFEAT!';
                modalContent.classList.add('bg-red-600');
                gameOverTitle.className = 'text-6xl text-yellow-300 text-outline-lg-black font-black mb-4';
            }
            
            winnerMessage.textContent = `${data.winnerNickname} WINS!`;
            modal.classList.remove('hidden');
        }
    }

    private interpolatePositions(): void {
        if (this.disposed || !this.state || !this.previousState) return;
        const now = Date.now();
        const timeSinceUpdate = now - this.state.lastUpdate;
        const serverUpdateInterval = 1000 / 60;
        let interpolationFactor = timeSinceUpdate / (serverUpdateInterval * 1.5);
        if (timeSinceUpdate > serverUpdateInterval * 3) {
            interpolationFactor = 1;
        }
        interpolationFactor = Math.min(interpolationFactor, 1);
        this.ball.position.x = lerp(this.previousState.ball.position.x, this.state.ball.position.x, interpolationFactor);
        this.ball.position.y = lerp(this.previousState.ball.position.y, this.state.ball.position.y, interpolationFactor);
        this.ball.position.z = lerp(this.previousState.ball.position.z, this.state.ball.position.z, interpolationFactor);
        this.player1Paddle.position.z = lerp(
            this.previousState.player1.paddleZ,
            this.state.player1.paddleZ,
            interpolationFactor
        );
        this.player2Paddle.position.z = lerp(
            this.previousState.player2.paddleZ,
            this.state.player2.paddleZ,
            interpolationFactor
        );
    }

    private onScoreUpdate(): void {
        if (!this.state || !this.previousState) return;

        let goalPosition: Vector3;
        const ballZ = this.previousBallPosition ? this.previousBallPosition.z : 0;
        
        if (this.state.player1.score > this.previousState.player1.score) {
            goalPosition = new Vector3(GOAL_LINE_RIGHT, PADDLE_Y_POSITION, ballZ);
        } else {
            goalPosition = new Vector3(GOAL_LINE_LEFT, PADDLE_Y_POSITION, ballZ);
        }

        const dummyEmitter = MeshBuilder.CreateBox("tempEmitter", { size: 0.1 }, this.scene);
        dummyEmitter.position = goalPosition.clone();
        dummyEmitter.isVisible = false;

        const originalEmitter = this.spawnParticleSystem.emitter;
        this.spawnParticleSystem.emitter = dummyEmitter;
        
        this.spawnParticleSystem.manualEmitCount = 300;
        this.spawnParticleSystem.start();
        
        setTimeout(() => {
            this.spawnParticleSystem?.stop();
            this.spawnParticleSystem.emitter = originalEmitter;
            dummyEmitter.dispose();
        }, 100);
        
        this.createScreenShake(1.5);
        
        const scoringPaddle = this.state.player1.score > this.previousState.player1.score
            ? this.player1Paddle
            : this.player2Paddle;
            
        if (scoringPaddle) {
            this.createHitAnimation(scoringPaddle);
            const originalMat = scoringPaddle.material as StandardMaterial;
            const flashMat = originalMat.clone(originalMat.name + "_flash");
            flashMat.emissiveColor = new Color3(1, 1, 1);
            scoringPaddle.material = flashMat;
            setTimeout(() => {
                scoringPaddle.material = originalMat;
                flashMat.dispose();
            }, 200);
        }
        
        this.createComicTextEffect(goalPosition);
    }

    private createComicTextEffect(effectPosition: Vector3): void {
        // console.log('[DEBUG] createComicTextEffect called');
        const texts = ["POW!", "BAM!", "BOOM!", "WHAM!", "ZAP!", "SMASH!"];
        const randomText = texts[Math.floor(Math.random() * texts.length)];
        
        const textPlane = MeshBuilder.CreatePlane("comicText", { size: 9 }, this.scene);
        textPlane.position = effectPosition.clone();
        textPlane.position.y += 1.5;
        textPlane.billboardMode = Mesh.BILLBOARDMODE_ALL;
        
        // console.log('[DEBUG] Text plane created at position:', textPlane.position);
        const dynamicTexture = new DynamicTexture("comicTexture", 256, this.scene, true);
        dynamicTexture.hasAlpha = true;
        const ctx = dynamicTexture.getContext() as CanvasRenderingContext2D;
        ctx.fillStyle = "rgba(0, 0, 0, 0)";
        ctx.clearRect(0, 0, 256, 256);
        ctx.font = "bold 80px Anton";
        ctx.fillStyle = "#FFD700";
        ctx.strokeStyle = "black";
        ctx.lineWidth = 19;
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.strokeText(randomText, 128, 128);
        ctx.fillText(randomText, 128, 128);
        dynamicTexture.update();
        
        // console.log('[DEBUG] Dynamic texture updated with text:', randomText);
        const textMat = new StandardMaterial("comicTextMat", this.scene);
        textMat.diffuseTexture = dynamicTexture;
        textMat.opacityTexture = dynamicTexture;
        textMat.emissiveColor = new Color3(1, 1, 1);
        textMat.disableLighting = true;
        textMat.backFaceCulling = false;
        textMat.useAlphaFromDiffuseTexture = true;
        textPlane.material = textMat;
        
        // console.log('[DEBUG] Material applied, initial alpha:', textMat.alpha);
        const easingFunc = new ElasticEase(2, 3);
        easingFunc.setEasingMode(EasingFunction.EASINGMODE_EASEOUT);
        
        const scaleAnim = new Animation(
            "comicScale",
            "scaling",
            60,
            Animation.ANIMATIONTYPE_VECTOR3,
            Animation.ANIMATIONLOOPMODE_CONSTANT
        );
        const scaleKeys: IAnimationKey[] = [
            { frame: 0, value: new Vector3(0.5, 0.5, 0.5) },
            { frame: 10, value: new Vector3(1.5, 1.5, 1.5) },
            { frame: 20, value: new Vector3(1.2, 1.2, 1.2) },
            { frame: 30, value: new Vector3(0.1, 0.1, 0.1) }
        ];
        scaleAnim.setKeys(scaleKeys);
        scaleAnim.setEasingFunction(easingFunc);
        
        const alphaAnim = new Animation(
            "comicAlpha",
            "material.alpha",
            60,
            Animation.ANIMATIONTYPE_FLOAT,
            Animation.ANIMATIONLOOPMODE_CONSTANT
        );
        const alphaKeys: IAnimationKey[] = [
            { frame: 0, value: 0 },
            { frame: 5, value: 1 },
            { frame: 20, value: 1 },
            { frame: 30, value: 0 }
        ];
        alphaAnim.setKeys(alphaKeys);
        alphaAnim.setEasingFunction(easingFunc);
        
        const rotAnim = new Animation(
            "comicRot",
            "rotation.z",
            60,
            Animation.ANIMATIONTYPE_FLOAT,
            Animation.ANIMATIONLOOPMODE_CONSTANT
        );
        const startRot = (Math.random() - 0.5) * 1.0;
        const rotKeys: IAnimationKey[] = [
            { frame: 0, value: startRot },
            { frame: 30, value: -startRot * 0.5 }
        ];
        rotAnim.setKeys(rotKeys);
        rotAnim.setEasingFunction(easingFunc);
        
        textPlane.animations = [scaleAnim, alphaAnim, rotAnim];
        // console.log('[DEBUG] Animations assigned to textPlane:', textPlane.animations.length);
        
        const animation = this.scene.beginAnimation(textPlane, 0, 30, false, 1.0, () => {
            // console.log('[DEBUG] Comic text animation completed, disposing resources');
            textPlane.dispose();
            dynamicTexture.dispose();
            textMat.dispose();
        });
        
        // let debugFrameCount = 0;
        // const debugObserver = this.scene.onBeforeRenderObservable.add(() => {
        //     if (textPlane.isDisposed() || debugFrameCount >= 30) {
        //         this.scene.onBeforeRenderObservable.remove(debugObserver);
        //         return;
        //     }
        //     console.log(`[DEBUG] Animation progress: scale=${textPlane.scaling.x.toFixed(2)}, alpha=${textMat.alpha.toFixed(2)}, rotation.z=${textPlane.rotation.z.toFixed(2)}`);
        //     debugFrameCount++;
        // });
        
        setTimeout(() => {
            if (!textPlane.isDisposed()) {
                // console.warn('[DEBUG] Fallback cleanup triggered for comic text');
                textPlane.dispose();
                dynamicTexture.dispose();
                textMat.dispose();
                animation.stop();
            }
        }, 600);
    }

    private detectCollisions(): void {
        if (!this.previousBallPosition) return;
        const ballPos = this.ball.position;
        const prevBallPos = this.previousBallPosition;
        if (prevBallPos.x > PADDLE_COLLISION_X_LEFT && ballPos.x <= PADDLE_COLLISION_X_LEFT) {
            const paddleTopBallCenterLimit = this.player1Paddle.position.z + (PADDLE_DEPTH / 2) + BALL_RADIUS;
            const paddleBottomBallCenterLimit = this.player1Paddle.position.z - (PADDLE_DEPTH / 2) - BALL_RADIUS;
            if (ballPos.z >= paddleBottomBallCenterLimit && ballPos.z <= paddleTopBallCenterLimit) {
                this.createHitAnimation(this.player1Paddle);
                this.createScreenShake(0.3);
            }
        }
        if (prevBallPos.x < PADDLE_COLLISION_X_RIGHT && ballPos.x >= PADDLE_COLLISION_X_RIGHT) {
            const paddleTopBallCenterLimit2 = this.player2Paddle.position.z + (PADDLE_DEPTH / 2) + BALL_RADIUS;
            const paddleBottomBallCenterLimit2 = this.player2Paddle.position.z - (PADDLE_DEPTH / 2) - BALL_RADIUS;
            if (ballPos.z >= paddleBottomBallCenterLimit2 && ballPos.z <= paddleTopBallCenterLimit2) {
                this.createHitAnimation(this.player2Paddle);
                this.createScreenShake(0.3);
            }
        }
        if (prevBallPos.z < WALL_COLLISION_Z_TOP && ballPos.z >= WALL_COLLISION_Z_TOP) {
            this.createScreenShake(0.15);
        }
        if (prevBallPos.z > WALL_COLLISION_Z_BOTTOM && ballPos.z <= WALL_COLLISION_Z_BOTTOM) {
            this.createScreenShake(0.15);
        }
    }
        private createHitAnimation(paddle: Mesh): void {
        const scalingAnimation = new Animation(
            "paddleHit",
            "scaling",
            60,
            Animation.ANIMATIONTYPE_VECTOR3,
            Animation.ANIMATIONLOOPMODE_CONSTANT
        );
        const keys: IAnimationKey[] = [];
        keys.push({ frame: 0, value: new Vector3(1, 1, 1) });
        keys.push({ frame: 5, value: new Vector3(1.3, 1.3, 1.3) });
        keys.push({ frame: 10, value: new Vector3(0.9, 0.9, 0.9) });
        keys.push({ frame: 15, value: new Vector3(1, 1, 1) });
        scalingAnimation.setKeys(keys);
        const easingFunction = new ElasticEase(1.5, 4);
        easingFunction.setEasingMode(EasingFunction.EASINGMODE_EASEINOUT);
        scalingAnimation.setEasingFunction(easingFunction);
        paddle.animations.push(scalingAnimation);
        this.scene.beginAnimation(paddle, 0, 15, false);
    }

    private createScreenShake(intensity: number = 0.5): void {
        const camera = this.scene.activeCamera as ArcRotateCamera;
        if (!camera) return;
        const originalAlpha = camera.alpha;
        const originalBeta = camera.beta;
        const originalRadius = camera.radius;
        let shakeTime = 0;
        const shakeObserver = this.scene.onBeforeRenderObservable.add(() => {
            shakeTime += 0.1;
            if (shakeTime > 1) {
                camera.alpha = originalAlpha;
                camera.beta = originalBeta;
                camera.radius = originalRadius;
                this.scene.onBeforeRenderObservable.remove(shakeObserver);
                return;
            }
            camera.alpha = originalAlpha + Math.sin(shakeTime * 50) * intensity * 0.01;
            camera.beta = originalBeta + Math.cos(shakeTime * 50) * intensity * 0.01;
            camera.radius = originalRadius + Math.sin(shakeTime * 30) * intensity;
        });
    }

    private updateBallSpeedEffects(): void {
        if (!this.state || !this.ball) return;
        const velocity = this.state.ball.velocity;
        const speed = Math.sqrt(velocity.x ** 2 + velocity.z ** 2);
        const normalizedSpeed = Math.min(speed / 20, 1);
        const ballMat = this.ball.material as StandardMaterial;
        if (ballMat) {
            ballMat.emissiveColor = new Color3(
                0.8 + normalizedSpeed * 0.2,
                0.7 - normalizedSpeed * 0.3,
                normalizedSpeed * 0.5
            );
            const scale = 1 + normalizedSpeed * 0.2;
            this.ball.scaling = new Vector3(scale, scale, scale);
        }
        if (this.particleSystem) {
            this.particleSystem.emitRate = 1000 + normalizedSpeed * 1500;
            this.particleSystem.minEmitPower = 0.5 + normalizedSpeed * 1.5;
            this.particleSystem.maxEmitPower = 1.5 + normalizedSpeed * 2;
        }
    }


    public dispose(): void {
        if (this.disposed) return;
        this.disposed = true;

        this.engine.stopRenderLoop();
        this.connection.disconnect();
        this.inputManager.dispose();
        this.uiManager.dispose();
        window.removeEventListener('resize', this.resizeHandler);

        this.scene.dispose();
        this.engine.dispose();
        this.removePauseOverlay();
    }
}

function lerp(start: number, end: number, factor: number): number {
    return start + (end - start) * factor;
}