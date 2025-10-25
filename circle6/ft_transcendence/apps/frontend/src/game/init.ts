import { PongGame3D } from './render'
import { Vector3 } from '@babylonjs/core'
import { initialBallVelocity } from '@trans/common-types'

export function initializeGame(
    containerId: string,
    gameId: string,
    playerId: string = 'player1',
    gameMode: string,
    nickname: string = 'Player',
    player1Avatar?: string,
    player2Avatar?: string,
    player1Nickname?: string,
    player2Nickname?: string
): PongGame3D | null {
    const container = document.getElementById(containerId)
    if (!container) return null
    
    container.innerHTML = `
        <div class="fixed top-0 left-0 w-screen h-screen bg-gray-900 overflow-hidden">
            <canvas id="game-canvas" tabindex="-1" class="block w-full h-full absolute top-0 left-0 touch-action-none"></canvas>
            
            <!-- Countdown Display -->
            <div id="countdown-display" 
                 class="hidden absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 
                        font-anton text-[10rem] text-yellow-400 
                        drop-shadow-[0_0_20px_rgba(250,204,21,0.5)] 
                        z-[1001] pointer-events-none">
            </div>
            
            <!-- PLAYER 1 - Tag style badge -->
            <div class="absolute top-6 left-6 z-[1000]">
                <div class="relative">
                    <!-- Player tag -->
                    <div class="bg-pink-500 border-4 border-black pl-4 pr-6 py-1 shadow-[4px_4px_0_rgba(0,0,0,1)] transform -rotate-2">
                        <div id="player1-name" class="font-anton text-lg text-white uppercase tracking-widest">
                            ${player1Nickname || 'PLAYER 1'}
                        </div>
                    </div>
                    <!-- Triangular pointer - properly aligned -->
                    <div class="absolute -bottom-3 left-3 w-0 h-0 border-l-[10px] border-l-transparent border-r-[10px] border-r-transparent border-t-[12px] border-t-black"></div>
                    <div class="absolute -bottom-2 left-4 w-0 h-0 border-l-[8px] border-l-transparent border-r-[8px] border-r-transparent border-t-[8px] border-t-pink-500"></div>
                    
                    <!-- Avatar circle with score -->
                    <div class="absolute top-10 left-2 flex items-center gap-3">
                        <div class="relative">
                            <div class="w-12 h-12 md:w-16 md:h-16 rounded-full border-4 border-black bg-yellow-300 flex items-center justify-center shadow-[4px_4px_0_rgba(0,0,0,1)]">
                                <img id="player1-avatar" 
                                     src="${player1Avatar || '/default-avatar.png'}" 
                                     alt="${player1Nickname || 'Player 1'}"
                                     class="w-10 h-10 md:w-14 md:h-14 rounded-full object-cover">
                            </div>
                        </div>
                        <!-- Score bubble -->
                        <div class="relative">
                            <div class="bg-yellow-300 border-4 border-black px-3 py-1 md:px-4 md:py-2 shadow-[5px_5px_0_rgba(0,0,0,1)]">
                                <div id="player1-score" 
                                     class="font-anton text-4xl md:text-5xl text-black leading-none">
                                    0
                                </div>
                            </div>
                            <!-- Speech bubble pointer - properly aligned -->
                            <div class="absolute top-1/2 -left-3 -translate-y-1/2 w-0 h-0 border-t-[12px] border-t-transparent border-b-[12px] border-b-transparent border-r-[14px] border-r-black"></div>
                            <div class="absolute top-1/2 -left-2 -translate-y-1/2 w-0 h-0 border-t-[10px] border-t-transparent border-b-[10px] border-b-transparent border-r-[10px] border-r-yellow-300"></div>
                        </div>
                    </div>
                </div>
            </div>
            
            <!-- PLAYER 2 - Tag style badge (mirrored) -->
            <div class="absolute top-6 right-6 z-[1000]">
                <div class="relative">
                    <!-- Player tag -->
                    <div class="bg-cyan-400 border-4 border-black pl-6 pr-4 py-1 shadow-[4px_4px_0_rgba(0,0,0,1)] transform rotate-2">
                        <div id="player2-name" class="font-anton text-lg text-black uppercase tracking-widest">
                            ${player2Nickname || 'PLAYER 2'}
                        </div>
                    </div>
                    <!-- Triangular pointer - properly aligned -->
                    <div class="absolute -bottom-3 right-3 w-0 h-0 border-l-[10px] border-l-transparent border-r-[10px] border-r-transparent border-t-[12px] border-t-black"></div>
                    <div class="absolute -bottom-2 right-4 w-0 h-0 border-l-[8px] border-l-transparent border-r-[8px] border-r-transparent border-t-[8px] border-t-cyan-400"></div>
                    
                    <!-- Avatar circle with score -->
                    <div class="absolute top-10 right-2 flex items-center gap-3">
                        <!-- Score bubble -->
                        <div class="relative">
                            <div class="bg-yellow-300 border-4 border-black px-3 py-1 md:px-4 md:py-2 shadow-[5px_5px_0_rgba(0,0,0,1)]">
                                <div id="player2-score" 
                                     class="font-anton text-4xl md:text-5xl text-black leading-none">
                                    0
                                </div>
                            </div>
                            <!-- Speech bubble pointer - properly aligned -->
                            <div class="absolute top-1/2 -right-3 -translate-y-1/2 w-0 h-0 border-t-[12px] border-t-transparent border-b-[12px] border-b-transparent border-l-[14px] border-l-black"></div>
                            <div class="absolute top-1/2 -right-2 -translate-y-1/2 w-0 h-0 border-t-[10px] border-t-transparent border-b-[10px] border-b-transparent border-l-[10px] border-l-yellow-300"></div>
                        </div>
                        <div class="relative">
                            <div class="w-12 h-12 md:w-16 md:h-16 rounded-full border-4 border-black bg-yellow-300 flex items-center justify-center shadow-[4px_4px_0_rgba(0,0,0,1)]">
                                <img id="player2-avatar" 
                                     src="${player2Avatar || '/default-avatar.png'}" 
                                     alt="${player2Nickname || 'Player 2'}"
                                     class="w-10 h-10 md:w-14 md:h-14 rounded-full object-cover">
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <!-- ESC - Simple corner badge -->
            <div class="absolute bottom-6 right-6 z-[1000]">
                <div class="bg-red-600 border-3 border-black px-4 py-2 shadow-[3px_3px_0_rgba(0,0,0,1)] transform rotate-2">
                    <div class="font-anton text-sm text-white uppercase tracking-wide">
                        ESC
                    </div>
                </div>
            </div>
        </div>
    `;
    
    const canvasElement = document.getElementById('game-canvas');
    if (!(canvasElement instanceof HTMLCanvasElement)) {
        console.error('Canvas element not found');
        return null;
    }
    
    return new PongGame3D(canvasElement, gameId, playerId, gameMode, nickname);
}
