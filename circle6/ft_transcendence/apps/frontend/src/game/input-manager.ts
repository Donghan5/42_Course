import { KeyboardEventTypes } from '@babylonjs/core/Events/keyboardEvents';
import { GameState } from "@trans/common-types";
import { Scene, Engine } from '@babylonjs/core';
import { Connection } from './connection';


export class InputManager {
    public state: GameState | null = null;
    private scene: Scene;
    private localPlayerId: string;
    private connection: Connection;
    private gameMode: string;
    
    private lastSentInputStateP1: 'up' | 'down' | 'stop' = 'stop';
    private lastSentInputStateP2: 'up' | 'down' | 'stop' = 'stop';
    private inputStatePlayer1 = { up: false, down: false };
    private inputStatePlayer2 = { up: false, down: false };

    constructor(scene: Scene, connection: Connection, localPlayerId: string, gameMode: string) {
        this.scene = scene;
        this.connection = connection;
        this.localPlayerId = localPlayerId;
        this.gameMode = gameMode;

        this.setupControls();
    }

    public setupControls(): void {
        this.scene.onKeyboardObservable.add((kbInfo) => {
            switch (kbInfo.type) {
                case KeyboardEventTypes.KEYDOWN:
                    if (kbInfo.event.key === 'w' || kbInfo.event.key === 'W') {
                        this.inputStatePlayer1.up = true;
                    } else if (kbInfo.event.key === 's' || kbInfo.event.key === 'S') {
                        this.inputStatePlayer1.down = true;
                    }
                    else if (kbInfo.event.key === 'ArrowUp') {
                        this.inputStatePlayer2.up = true;
                    } else if (kbInfo.event.key === 'ArrowDown') {
                        this.inputStatePlayer2.down = true;
                    }
                    break;
                case KeyboardEventTypes.KEYUP:
                    if (kbInfo.event.key === 'w' || kbInfo.event.key === 'W') {
                        this.inputStatePlayer1.up = false;
                    } else if (kbInfo.event.key === 's' || kbInfo.event.key === 'S') {
                        this.inputStatePlayer1.down = false;
                    }
                    else if (kbInfo.event.key === 'ArrowUp') {
                        this.inputStatePlayer2.up = false;
                    } else if (kbInfo.event.key === 'ArrowDown') {
                        this.inputStatePlayer2.down = false;
                    }
                    break;
            }
        });
    }

    public update(): void {
        if (this.gameMode === 'quick' || this.gameMode === 'LOCAL_PVP') {
            this.sendPlayer1InputState();
            this.sendPlayer2InputState();
        } else {
            this.sendPlayer1InputState();
        }
    }

    public sendPlayer1InputState(): void {
        let inputState: 'up' | 'down' | 'stop' = 'stop';
        if (this.inputStatePlayer1.up) {
            inputState = 'up';
        }
        else if (this.inputStatePlayer1.down) {
            inputState = 'down';
        }
        if (this.lastSentInputStateP1 !== inputState) {
            this.connection.sendPlayerInput(inputState, this.localPlayerId);
            this.lastSentInputStateP1 = inputState;
        }
    }

    private sendPlayer2InputState(): void {
        if (!this.state?.player2Id || this.state.player2Id === 'AI') return;
        let inputState: 'up' | 'down' | 'stop' = 'stop';
        if (this.inputStatePlayer2.up) {
            inputState = 'up';
        } else if (this.inputStatePlayer2.down) {
            inputState = 'down';
        }
        if (this.lastSentInputStateP2 !== inputState) {
            this.connection.sendPlayerInput(inputState, this.state.player2Id);
            this.lastSentInputStateP2 = inputState;
        }
    }

    public dispose(): void {
        // dispose the input state
        this.inputStatePlayer1 = { up: false, down: false };
        this.inputStatePlayer2 = { up: false, down: false };
    }
}