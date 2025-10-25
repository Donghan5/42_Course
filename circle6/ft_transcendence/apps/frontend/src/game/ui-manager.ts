import { GameState } from "@trans/common-types";

export class GameUIManager {
    private player1NameEl: HTMLElement | null;
    private player1ScoreEl: HTMLElement | null;
    private player2NameEl: HTMLElement | null;
    private player2ScoreEl: HTMLElement | null;
    private gameMode: string;
    public state: GameState | null = null;

    constructor(gameMode: string) {
        this.player1NameEl = document.getElementById('player1-name');
        this.player1ScoreEl = document.getElementById('player1-score');
        this.player2NameEl = document.getElementById('player2-name');
        this.player2ScoreEl = document.getElementById('player2-score');
        this.gameMode = gameMode;
    }

    public updateScoreDisplay(): void {
        if (!this.state) return;
        
        // Update names
        if (this.player1NameEl) {
            this.player1NameEl.textContent = this.state.player1.nickname || 'Player 1';
        }
        if (this.player2NameEl) {
            this.player2NameEl.textContent = this.state.player2.nickname || 'Player 2';
        }
        
        // Update scores
        if (this.player1ScoreEl) {
            this.player1ScoreEl.textContent = this.state.player1.score.toString();
        }
        if (this.player2ScoreEl) {
            this.player2ScoreEl.textContent = this.state.player2.score.toString();
        }
        
        // Update avatars if they're provided in the game state
        if (this.state.player1.avatarUrl) {
            const p1Avatar = document.getElementById('player1-avatar') as HTMLImageElement;
            if (p1Avatar && p1Avatar.src !== this.state.player1.avatarUrl) {
                p1Avatar.src = this.state.player1.avatarUrl + '?t=' + new Date().getTime();
            }
        }
        
        if (this.state.player2.avatarUrl) {
            const p2Avatar = document.getElementById('player2-avatar') as HTMLImageElement;
            if (p2Avatar && p2Avatar.src !== this.state.player2.avatarUrl) {
                p2Avatar.src = this.state.player2.avatarUrl + '?t=' + new Date().getTime();
            }
        }
    }

    public showCountdownSpotlight(playerSide: 'player1' | 'player2' | 'both' = 'both', count: number) {
        const isLocalPlay = this.gameMode === 'LOCAL_PVP' || this.gameMode === 'quick';
        const isSpectator = this.gameMode === 'spectator';
        
        // console.log(`[COUNTDOWN DEBUG] gameMode="${this.gameMode}", isLocalPlay=${isLocalPlay}, isSpectator=${isSpectator}, playerSide="${playerSide}"`);
        
        if (isLocalPlay && playerSide !== 'both') {
            // console.log('[COUNTDOWN] Forcing playerSide to "both" for local play');
            playerSide = 'both';
        }
        
        if (count === 0) {
            // console.log('[COUNTDOWN] Count is 0, hiding countdown (no GO!)');
            this.hideCountdownSpotlight();
            return;
        }
        
        const isFirstCall = count === 3 || !document.getElementById('countdown-spotlight-overlay');
        
        if (isFirstCall) {
            this.hideCountdownSpotlight();
            
            const overlay = document.createElement('div');
            overlay.id = 'countdown-spotlight-overlay';
            overlay.className = `fixed top-0 left-0 w-full h-full bg-black/75 pointer-events-none z-[10500] flex items-center ${isSpectator ? 'justify-center' : 'justify-between'} font-['Anton',sans-serif]`;
            
            if (isSpectator) {
                // SPECTATOR MODE - message at BOTTOM (SMALLER SIZE)
                const spectatorMessageBottom = document.createElement('div');
                spectatorMessageBottom.className = 'spectator-countdown-message-bottom fixed bottom-[5%] left-1/2 -translate-x-1/2 z-[10501] text-center opacity-0';
                
                spectatorMessageBottom.innerHTML = `
                    <div class="bg-white border-[6px] border-black p-4 px-8 shadow-[8px_8px_0_rgba(0,0,0,0.8)] -rotate-[2deg] mb-3">
                        <h1 class="text-[1.8rem] text-black uppercase tracking-[3px] m-0 [text-shadow:3px_3px_0_#FCD34D]">
                            SPECTATOR MODE
                        </h1>
                    </div>
                    
                    <div class="bg-purple-700 border-4 border-black py-3 px-6 shadow-[6px_6px_0_rgba(0,0,0,0.8)] rotate-1 text-base font-bold text-white uppercase [text-shadow:2px_2px_0_black] mb-2">
                        MATCH STARTING SOON
                    </div>
                    
                    <p class="text-[0.85rem] text-white [text-shadow:2px_2px_4px_rgba(0,0,0,0.5)] m-0">
                        Press ESC to return to tournament
                    </p>
                `;
                
                overlay.appendChild(spectatorMessageBottom);
                document.body.appendChild(overlay);
                
                // Animate in from bottom
                spectatorMessageBottom.animate([
                    { opacity: '0', transform: 'translateX(-50%) translateY(50px)' },
                    { opacity: '1', transform: 'translateX(-50%) translateY(0)' }
                ], {
                    duration: 500,
                    easing: 'cubic-bezier(0.34, 1.56, 0.64, 1)',
                    fill: 'forwards'
                });
            } else {
                // PLAYER MODE - Function to create a side panel
                const createSidePanel = (side: 'left' | 'right', playerNum: 1 | 2, playerName: string) => {
                    const panel = document.createElement('div');
                    
                    const isLeft = side === 'left';
                    const rotation = isLeft ? '-2deg' : '2deg';
                    const positionClass = isLeft ? 'left-[5%]' : 'right-[5%]';
                    const bgColorClass = playerNum === 1 ? 'bg-player1-pink' : 'bg-player2-blue';
                    const arrowColorClass = playerNum === 1 ? 'border-l-player1-pink' : 'border-r-player2-blue';

                    panel.className = `countdown-panel absolute ${positionClass} top-1/2 -translate-y-1/2 rotate-[${rotation}] w-[280px] z-[10501] opacity-0`;
                    
                    const arrowPositionClass = isLeft ? 'right-[-30px]' : 'left-[-30px]';
                    const arrowBorderClass = isLeft ? 'border-l-[30px]' : 'border-r-[30px]';
                    
                    panel.innerHTML = `
                        <div class="absolute ${arrowPositionClass} top-1/2 -translate-y-1/2 w-0 h-0 border-t-[20px] border-t-transparent border-b-[20px] border-b-transparent ${arrowBorderClass} ${arrowColorClass}"></div>
                        <div class="border-[6px] border-black p-8 px-6 shadow-[8px_8px_0_rgba(0,0,0,0.8)] relative ${bgColorClass}">
                            <div class="text-[2.5rem] text-white font-bold text-center [text-shadow:4px_4px_0_black] mb-4 uppercase">${playerName}</div>
                        </div>
                    `;
                    
                    panel.animate([
                        { transform: `translateY(-50%) translateY(-100px) rotate(${rotation})`, opacity: '0' },
                        { transform: `translateY(-50%) translateY(20px) rotate(${rotation})`, opacity: '1', offset: 0.6 },
                        { transform: `translateY(-50%) translateY(0) rotate(${rotation})`, opacity: '1' }
                    ], {
                        duration: 600,
                        easing: 'ease-out',
                        fill: 'forwards'
                    });
                    
                    return panel;
                };
                
                // Create panels for players
                if (playerSide === 'both' || playerSide === 'player1') {
                    const player1Label = 'YOU';
                    const leftPanel = createSidePanel('left', 1, player1Label);
                    overlay.appendChild(leftPanel);
                }
                
                if (playerSide === 'both' || playerSide === 'player2') {
                    const player2Label = isLocalPlay ? 'PLAYER 2' : 'YOU';
                    const rightPanel = createSidePanel('right', 2, player2Label);
                    overlay.appendChild(rightPanel);
                }
                
                // Add CONTROLS AT BOTTOM
                const controlsBottom = document.createElement('div');
                controlsBottom.className = 'countdown-controls-bottom fixed bottom-[5%] left-1/2 -translate-x-1/2 z-[10501] opacity-100';
                
                if (isLocalPlay) {
                    // LOCAL MODE - Show both players' controls
                    controlsBottom.innerHTML = `
                        <div class="bg-white border-[6px] border-black py-5 px-10 shadow-[8px_8px_0_rgba(0,0,0,0.8)] flex gap-16 items-center">
                            <div class="flex flex-col items-center gap-2.5">
                                <div class="text-xl font-bold text-player1-pink [text-shadow:2px_2px_0_black]">YOU</div>
                                <div class="flex gap-2.5">
                                    <div class="bg-player1-pink border-[3px] border-black py-2 px-4 text-2xl font-bold text-white [text-shadow:2px_2px_0_black]">W</div>
                                    <div class="bg-player1-pink border-[3px] border-black py-2 px-4 text-2xl font-bold text-white [text-shadow:2px_2px_0_black]">S</div>
                                </div>
                            </div>
                            
                            <div class="text-3xl font-bold text-black">VS</div>
                            
                            <div class="flex flex-col items-center gap-2.5">
                                <div class="text-xl font-bold text-player2-blue [text-shadow:2px_2px_0_black]">PLAYER 2</div>
                                <div class="flex gap-2.5">
                                    <div class="bg-player2-blue border-[3px] border-black py-2 px-4 text-2xl font-bold text-white [text-shadow:2px_2px_0_black]">↑</div>
                                    <div class="bg-player2-blue border-[3px] border-black py-2 px-4 text-2xl font-bold text-white [text-shadow:2px_2px_0_black]">↓</div>
                                </div>
                            </div>
                        </div>
                    `;
                } else {
                    // ONLINE MODE - Always show W/S (never arrows)
                    controlsBottom.innerHTML = `
                        <div class="bg-white border-[6px] border-black py-5 px-10 shadow-[8px_8px_0_rgba(0,0,0,0.8)]">
                            <div class="text-3xl text-black font-bold text-center">CONTROLS: <span class="text-player2-blue [text-shadow:2px_2px_0_white]">W / S</span></div>
                        </div>
                    `;
                }
                
                overlay.appendChild(controlsBottom);
                document.body.appendChild(overlay);
                
                if (isLocalPlay) {
                    const bannerDiv = document.createElement('div');
                    bannerDiv.className = 'countdown-banner fixed top-[8%] left-1/2 -translate-x-1/2 -rotate-[2deg] z-[10502] pointer-events-none';
                    
                    bannerDiv.innerHTML = `
                        <div class="bg-comic-yellow border-[6px] border-black p-4 px-12 shadow-[8px_8px_0_rgba(0,0,0,0.8)] relative">
                            <div class="text-[2.5rem] text-black font-bold uppercase tracking-[3px] [text-shadow:3px_3px_0_white]">LOCAL BATTLE!</div>
                            
                            <div class="absolute -top-1.5 -right-1.5 w-[30px] h-[30px] bg-gradient-to-br from-black from-50% to-comic-yellow to-50% border-l-[6px] border-b-[6px] border-black"></div>
                        </div>
                    `;
                    
                    document.body.appendChild(bannerDiv);
                    
                    bannerDiv.animate([
                        { opacity: '0', transform: 'translateX(-50%) translateY(-50px) rotate(-2deg)' },
                        { opacity: '1', transform: 'translateX(-50%) translateY(0) rotate(-2deg)' }
                    ], {
                        duration: 500,
                        easing: 'cubic-bezier(0.34, 1.56, 0.64, 1)',
                        fill: 'forwards'
                    });
                }
                
                setTimeout(() => {
                    const panels = document.querySelectorAll('.countdown-panel');
                    panels.forEach((panel, index) => {
                        setTimeout(() => {
                            (panel as HTMLElement).animate([
                                { opacity: '0', transform: 'translateY(-50%) scale(0.8)' },
                                { opacity: '1', transform: 'translateY(-50%) scale(1)' }
                            ], {
                                duration: 500,
                                easing: 'cubic-bezier(0.34, 1.56, 0.64, 1)',
                                fill: 'forwards'
                            });
                        }, index * 150);
                    });
                }, 100);
            }
        }
        
        // Update countdown number
        const oldCountdown = document.getElementById('countdown-number');
        if (oldCountdown) {
            oldCountdown.remove();
        }
        
        const countdownDiv = document.createElement('div');
        countdownDiv.id = 'countdown-number';
        countdownDiv.className = 'fixed top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 z-[10502]';
        
        const displayNumber = count.toString();
        const finalRotation = '-2deg';
        
        countdownDiv.innerHTML = `
            <div class="bg-white border-[8px] border-black p-12 px-20 shadow-[12px_12px_0_rgba(0,0,0,0.8)] -rotate-[2deg] relative">
                <div class="text-[10rem] text-black font-bold [text-shadow:6px_6px_0_#FCD34D] leading-none">${displayNumber}</div>
            </div>
        `;
        
        document.body.appendChild(countdownDiv);
        
        countdownDiv.animate([
            { transform: `translate(-50%, -50%) rotate(-7deg)`, opacity: '1' },
            { transform: `translate(-50%, -50%) rotate(-4deg)`, opacity: '1', offset: 0.5 },
            { transform: `translate(-50%, -50%) rotate(${finalRotation})`, opacity: '1' }
        ], {
            duration: 300,
            easing: 'ease-out',
            fill: 'forwards'
        });
    }

    public hideCountdownSpotlight() {
        const overlay = document.getElementById('countdown-spotlight-overlay');
        if (overlay) overlay.remove();
        
        const countdown = document.getElementById('countdown-number');
        if (countdown) countdown.remove();
        
        // Remove all countdown-related elements
        document.querySelectorAll('.countdown-panel, .countdown-banner, .countdown-controls-bottom').forEach(el => el.remove());
    }


    public showWaitingForOpponent(playerSide: 'player1' | 'player2') {
        this.hideWaitingForOpponent();
        
        const player1UI = document.querySelector('.absolute.top-6.left-6') as HTMLElement;
        const player2UI = document.querySelector('.absolute.top-6.right-6') as HTMLElement;
        if (player1UI) player1UI.style.display = 'none';
        if (player2UI) player2UI.style.display = 'none';
        
        const isSpectator = this.gameMode === 'spectator';
        const isLocalMode = this.gameMode === 'LOCAL_PVP';
        
        const overlay = document.createElement('div');
        overlay.id = 'waiting-opponent-overlay';
        overlay.className = 'fixed inset-0 bg-black/80 flex flex-col items-center justify-center z-[11000] font-anton';
        overlay.style.backgroundImage = 'repeating-linear-gradient(45deg, transparent, transparent 10px, rgba(255,255,255,.05) 10px, rgba(255,255,255,.05) 20px)';

        if (isLocalMode) {
            overlay.innerHTML = `
                <div class="text-center">
                    <div class="bg-white border-[8px] border-black p-12 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] -rotate-2 mb-12">
                        <h1 class="text-6xl text-black uppercase tracking-wider m-0 [text-shadow:4px_4px_0_#FCD34D]">
                            QUICK GAME!
                        </h1>
                    </div>
                    
                    <div class="flex gap-16 justify-center items-center">
                        <div class="text-center">
                            <div class="bg-player1-pink border-[6px] border-black p-8 shadow-[8px_8px_0_rgba(0,0,0,0.5)] mb-4">
                                <div class="text-5xl text-white font-bold [text-shadow:3px_3px_0_black]">PINK</div>
                            </div>
                            <div class="bg-white border-4 border-black p-4 text-2xl font-bold">
                                W / S
                            </div>
                        </div>
                        
                        <div class="text-center">
                            <div class="bg-player2-blue border-[6px] border-black p-8 shadow-[8px_8px_0_rgba(0,0,0,0.5)] mb-4">
                                <div class="text-5xl text-white font-bold [text-shadow:3px_3px_0_black]">BLUE</div>
                            </div>
                            <div class="bg-white border-4 border-black p-4 text-2xl font-bold">
                                ↑ / ↓
                            </div>
                        </div>
                    </div>
                </div>
            `;
        } else if (isSpectator) {
            overlay.innerHTML = `
                <div class="text-center">
                    <div class="bg-white border-[8px] border-black py-12 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] -rotate-2 mb-8">
                        <h1 class="text-6xl text-black uppercase tracking-wider m-0 [text-shadow:4px_4px_0_#FCD34D]">
                            WAITING...
                        </h1>
                    </div>
                    
                    <div class="bg-comic-yellow border-[6px] border-black py-6 px-8 shadow-[8px_8px_0_rgba(0,0,0,0.5)] rotate-1 text-2xl font-bold uppercase">
                        FOR PLAYERS TO CONNECT
                    </div>
                    
                    <p class="text-base text-white mt-8 [text-shadow:2px_2px_4px_rgba(0,0,0,0.5)]">
                        Press ESC to return to tournament
                    </p>
                </div>
            `;
        } else {
            const sideColorClass = playerSide === 'player1' ? 'bg-player1-pink' : 'bg-player2-blue';
            const sideName = playerSide === 'player1' ? 'PINK' : 'BLUE';
            
            overlay.innerHTML = `
                <div class="text-center">
                    <div class="bg-white border-[8px] border-black py-12 px-16 shadow-[12px_12px_0_rgba(0,0,0,0.5)] -rotate-2 mb-8">
                        <h1 class="text-6xl text-black uppercase tracking-wider m-0 [text-shadow:4px_4px_0_#FCD34D]">
                            WAITING...
                        </h1>
                    </div>
                    
                    <div class="${sideColorClass} border-[6px] border-black py-6 px-8 shadow-[8px_8px_0_rgba(0,0,0,0.5)] rotate-1 mb-8">
                        <div class="text-4xl text-white font-bold uppercase [text-shadow:3px_3px_0_black]">
                            YOU ARE ${sideName}
                        </div>
                    </div>
                    
                    <div class="bg-comic-yellow border-[6px] border-black py-6 px-8 shadow-[8px_8px_0_rgba(0,0,0,0.5)] -rotate-1 text-2xl font-bold uppercase">
                        WAITING FOR OPPONENT
                    </div>
                    
                    <p class="text-base text-white mt-8 [text-shadow:2px_2px_4px_rgba(0,0,0,0.5)]">
                        Press ESC to forfeit and return
                    </p>
                </div>
            `;
        }
        
        document.body.appendChild(overlay);
    }

    public hideWaitingForOpponent() {
        const overlay = document.getElementById('waiting-opponent-overlay');
        if (overlay) overlay.remove();
        
        const styles = document.getElementById('waiting-screen-styles');
        if (styles) styles.remove();
        
        const player1UI = document.querySelector('.absolute.top-6.left-6') as HTMLElement;
        const player2UI = document.querySelector('.absolute.top-6.right-6') as HTMLElement;
        if (player1UI) player1UI.style.display = 'block';
        if (player2UI) player2UI.style.display = 'block';
    }

    public dispose(): void {
        this.hideCountdownSpotlight();
        this.hideWaitingForOpponent();
    }
}