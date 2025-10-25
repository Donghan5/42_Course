import { appState } from '../src/state/state';
import { showNotification } from '../src/services/ui';

interface User {
  id: number;
  nickname: string;
  avatar_url?: string;
  isOnline?: boolean;
  rating?: number;
}

interface Message {
  id: number;
  sender_id: number;
  receiver_id: number;
  sender_nickname: string;
  message: string;
  created_at: string;
}

interface Conversation {
  user_id: number;
  nickname: string;
  avatar_url?: string;
  last_message: string | null;
  last_msg_time: string | null;
  unread_count: number;
  isOnline?: boolean;
}

export class ChatWidget {
  private container: HTMLElement | null = null;
  private isOpen: boolean = false;
  private ws: WebSocket | null = null;
  private currentChatUserId: number | null = null;
  private currentChatUserNickname: string = '';
  private currentView: 'conversations' | 'chat' | 'blocked' = 'conversations';
  private typingTimeout: number | null = null;
  private isCurrentUserBlocked: boolean = false;
  private onlineUserIds: Set<number> = new Set();
  private pendingTournamentNotifications: Array<{ 
    id: string,
    message: string, 
    timestamp: string 
  }> = [];

  //Stocker l'ID de l'utilisateur actuel
  private currentUserId: number = 0;
  
  //Empêcher les invitations multiples
  private pendingInvites = new Set<number>();
  private inviteCooldowns = new Map<number, number>();

  constructor() {
    //Récupérer l'ID utilisateur au démarrage
    this.currentUserId = this.getCurrentUserId();
    console.log('💬 Chat initialized for user ID:', this.currentUserId);
    this.init();
  }

  private init(): void {
    this.createWidget();
    this.attachEventListeners();
    this.connectWebSocket();
    this.setupClickAwayListener();
    this.watchGameSection();

    setTimeout(() => {
      this.loadConversations();
    }, 500);
  }

  // Hide chat button when in game
  private watchGameSection(): void {
    setInterval(() => {
      const btn = document.getElementById('chat-toggle-btn');
      if (btn) {
        btn.style.display = appState.currentSection === 'game' ? 'none' : 'flex';
      }
    }, 100);

    document.addEventListener('gameEnded', () => {
      const btn = document.getElementById('chat-toggle-btn');
      if (btn) btn.style.display = 'flex';
    });
  }

  // Click away to close
  private setupClickAwayListener(): void {
    document.addEventListener('click', (e) => {
      if (!this.isOpen) return;
      const panel = document.getElementById('chat-panel');
      const btn = document.getElementById('chat-toggle-btn');
      const target = e.target as HTMLElement;
      if (panel && btn && !panel.contains(target) && !btn.contains(target)) {
        this.close();
      }
    });
  }

  private async connectWebSocket(): Promise<void> {
    try {
      const response = await fetch('/api/chat/token', { credentials: 'include' });
      if (!response.ok) {
        setTimeout(() => this.connectWebSocket(), 3000);
        return;
      }
      const data = await response.json();
      const token = data.token;
      if (!token) {
        setTimeout(() => this.connectWebSocket(), 3000);
        return;
      }

      const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
      const wsUrl = `${protocol}//${window.location.host}/api/chat/ws?token=${token}`;
      
      this.ws = new WebSocket(wsUrl);

      this.ws.onopen = () => console.log('Chat WebSocket connected');
      this.ws.onmessage = (event) => {
        try {
          this.handleWebSocketMessage(JSON.parse(event.data));
        } catch (error) {
          console.error('Error parsing message:', error);
        }
      };
      this.ws.onerror = (error) => console.error('WebSocket error:', error);
      this.ws.onclose = () => setTimeout(() => this.connectWebSocket(), 3000);
    } catch (error) {
      console.error('Exception in connectWebSocket:', error);
      setTimeout(() => this.connectWebSocket(), 3000);
    }
  }

  /**
   * Handles real-time profile updates (anonymization, deletion, nickname changes)
   */
  private handleProfileUpdate(payload: any): void {
    const { userId, changeType, nickname, anonymousNickname } = payload;
    
    console.log('💬 Chat: Profile update received', { userId, changeType });
    
    // Update nickname in conversations list
    const conversationEl = document.querySelector(`[data-user-id="${userId}"]`);
    if (conversationEl) {
      const nicknameSpan = conversationEl.querySelector('.font-black');
      if (nicknameSpan) {
        if (changeType === 'anonymize' && anonymousNickname) {
          nicknameSpan.textContent = anonymousNickname;
        } else if (changeType === 'nickname' && nickname) {
          nicknameSpan.textContent = nickname;
        }
      }
    }
    
    // If this is the currently open chat, handle it specially
    if (this.currentChatUserId === userId) {
      if (changeType === 'anonymize') {
        this.handleUserAnonymized(userId, anonymousNickname);
      } else if (changeType === 'delete') {
        this.handleUserDeleted(userId);
      } else if (changeType === 'nickname' && nickname) {
        // Update nickname in chat header
        this.currentChatUserNickname = nickname;
        const chatTitle = document.getElementById('chat-title');
        if (chatTitle) {
          chatTitle.textContent = nickname;
        }
      }
    }
    
    // Refresh conversation list to show updated info
    if (this.currentView === 'conversations') {
      this.loadConversations();
    }
  }

  private resetChatInputState(): void {
    const chatInput = document.getElementById('chat-input') as HTMLInputElement;
    const sendBtn = document.getElementById('chat-send-btn');
    const gameBtn = document.getElementById('game-btn');
    const blockBtn = document.getElementById('block-btn');
    
    if (chatInput) {
      chatInput.disabled = false;
      chatInput.placeholder = 'Type message...';
      chatInput.classList.remove('bg-gray-100', 'cursor-not-allowed');
    }
    
    if (sendBtn) {
      (sendBtn as HTMLButtonElement).disabled = false;
      sendBtn.classList.remove('opacity-50', 'cursor-not-allowed');
    }
    
    if (gameBtn) {
      (gameBtn as HTMLButtonElement).disabled = false;
      gameBtn.classList.remove('opacity-50', 'cursor-not-allowed');
    }
    
    if (blockBtn) {
      (blockBtn as HTMLButtonElement).disabled = false;
      blockBtn.classList.remove('opacity-50', 'cursor-not-allowed');
    }
  }

  private async checkIfUserAnonymized(userId: number): Promise<void> {
    try {
      const response = await fetch(`/api/user/profile-by-id/${userId}`, { 
        credentials: 'include' 
      });
      
      if (!response.ok) {
        console.warn('Could not fetch user profile');
        return;
      }
      
      const data = await response.json();
      
      if (data.user && data.user.auth_provider === 'anonymized') {
        console.log('User is anonymized, disabling chat');
        this.disableChatForAnonymizedUser(data.user.nickname);
      }
    } catch (error) {
      console.error('Error checking if user is anonymized:', error);
    }
  }

  private disableChatForAnonymizedUser(anonymousNickname: string): void {
    const chatInput = document.getElementById('chat-input') as HTMLInputElement;
    const sendBtn = document.getElementById('chat-send-btn');
    const gameBtn = document.getElementById('game-btn');
    const blockBtn = document.getElementById('block-btn');
    const chatTitle = document.getElementById('chat-title');
    
    if (chatTitle) {
      chatTitle.textContent = anonymousNickname;
    }
    
    if (chatInput) {
      chatInput.disabled = true;
      chatInput.placeholder = 'User is anonymized - chat is read-only';
      chatInput.classList.add('bg-gray-100', 'cursor-not-allowed');
    }
    
    if (sendBtn) {
      (sendBtn as HTMLButtonElement).disabled = true;
      sendBtn.classList.add('opacity-50', 'cursor-not-allowed');
    }
    
    if (gameBtn) {
      (gameBtn as HTMLButtonElement).disabled = true;
      gameBtn.classList.add('opacity-50', 'cursor-not-allowed');
    }
    
    if (blockBtn) {
      (blockBtn as HTMLButtonElement).disabled = true;
      blockBtn.classList.add('opacity-50', 'cursor-not-allowed');
    }
    
    const container = document.getElementById('chat-messages');
    if (container && container.children.length > 0) {
      const lastMessage = container.lastElementChild;
      const isSystemMsg = lastMessage?.classList.contains('text-center');
      
      if (!isSystemMsg || !lastMessage?.textContent?.includes('anonymized')) {
        this.addSystemMsg('⚠️ This user has anonymized their account. Chat is read-only.');
      }
    }
  }

  /**
   * Handles when a user gets anonymized while chat is open
   */
  private handleUserAnonymized(userId: number, anonymousNickname: string): void {
    console.log('💬 User anonymized in real-time:', userId, anonymousNickname);
    
    // Only disable if this is the CURRENTLY OPEN chat
    if (this.currentChatUserId !== userId) {
      console.log('Anonymized user is not the current chat, ignoring');
      return;
    }
    
    this.currentChatUserNickname = anonymousNickname;
    this.disableChatForAnonymizedUser(anonymousNickname);
    
    showNotification('User has been anonymized - chat is read-only', 'info');
  }

  /**
   * Handles when a user gets deleted while chat is open
   */
  private handleUserDeleted(userId: number): void {
    console.log('💬 User deleted in real-time:', userId);
    
    // If we're viewing this user's chat, close it
    if (this.currentChatUserId === userId) {
      // Add system message
      this.addSystemMsg('⚠️ This user has deleted their account. Chat is no longer available.');
      
      // Disable all interactions
      const chatInput = document.getElementById('chat-input') as HTMLInputElement;
      const sendBtn = document.getElementById('chat-send-btn');
      const gameBtn = document.getElementById('game-btn');
      const blockBtn = document.getElementById('block-btn');
      
      if (chatInput) {
        chatInput.disabled = true;
        chatInput.value = '';
        chatInput.placeholder = 'User deleted - chat unavailable';
        chatInput.classList.add('bg-gray-100', 'cursor-not-allowed');
      }
      
      if (sendBtn) {
        (sendBtn as HTMLButtonElement).disabled = true;
        sendBtn.classList.add('opacity-50', 'cursor-not-allowed');
      }
      
      if (gameBtn) {
        (gameBtn as HTMLButtonElement).disabled = true;
        gameBtn.classList.add('opacity-50', 'cursor-not-allowed');
      }
      
      if (blockBtn) {
        (blockBtn as HTMLButtonElement).disabled = true;
        blockBtn.classList.add('opacity-50', 'cursor-not-allowed');
      }
      
      // Update chat title
      const chatTitle = document.getElementById('chat-title');
      if (chatTitle) {
        chatTitle.textContent = '[Deleted User]';
      }
      
      // Show notification
      showNotification('User has deleted their account', 'info');
      
      // Auto-close after a delay to show the message
      setTimeout(() => {
        this.goBack();
      }, 3000);
    }
    
    // Remove from conversations list
    const conversationEl = document.querySelector(`[data-user-id="${userId}"]`);
    if (conversationEl) {
      conversationEl.remove();
    }
    
    // Refresh conversation list
    if (this.currentView === 'conversations') {
      this.loadConversations();
    }
  }

  private handleWebSocketMessage(data: any): void {
    switch (data.type) {
      case 'friend_profile_updated':
      case 'profile_updated':
        this.handleProfileUpdate(data.payload);
        break;
      case 'online_users':
        this.updateOnlineUsers(data.users);
        break;
      case 'user_online':
        this.markUserOnline(data.userId);
        break;
      case 'user_offline':
        this.markUserOffline(data.userId);
        break;
      case 'new_message':
        this.handleNewMessage(data.message);
        break;
      case 'message_sent':
        this.handleMessageSent(data.message);
        break;
      case 'user_typing':
        this.showTypingIndicator(data.userId);
        break;
      case 'user_stop_typing':
        this.hideTypingIndicator(data.userId);
        break;
      case 'game_invitation':
        this.handleGameInvitation(data);
        break;
      case 'game_invite_sent':
        this.addSystemMsg(`Game invitation sent to ${data.receiverNickname}`);
        break;
      case 'game_invite_accepted_notification':
        this.handleGameInviteAcceptedNotification(data);
        break;
      case 'game_invite_declined_notification':
        // Retirer de la liste des invitations en attente
        if (this.currentChatUserId) {
          this.pendingInvites.delete(this.currentChatUserId);
        }
        // Réactiver le bouton
        const gameBtn = document.getElementById('game-btn');
        if (gameBtn) {
          gameBtn.classList.remove('opacity-50', 'cursor-not-allowed');
          (gameBtn as HTMLButtonElement).disabled = false;
        }
        showNotification(data.message, 'error');
        break;
      case 'tournament_notification':
        this.handleTournamentNotification(data);
        break;
      case 'error':
        //En cas d'erreur, retirer de pending aussi
        if (this.currentChatUserId) {
          this.pendingInvites.delete(this.currentChatUserId);
        }
        const gameBtnError = document.getElementById('game-btn');
        if (gameBtnError) {
          gameBtnError.classList.remove('opacity-50', 'cursor-not-allowed');
          (gameBtnError as HTMLButtonElement).disabled = false;
        }
        showNotification(data.message, 'error');
        break;
    }
  }

  private handleGameInviteAcceptedNotification(data: any): void {
    showNotification('Invitation Accepted! Starting matchmaking...', 'success');

    //Retirer de la liste des invitations en attente
    if (this.currentChatUserId) {
      this.pendingInvites.delete(this.currentChatUserId);
    }
    
    //Réactiver le bouton
    const gameBtn = document.getElementById('game-btn');
    if (gameBtn) {
      gameBtn.classList.remove('opacity-50', 'cursor-not-allowed');
      (gameBtn as HTMLButtonElement).disabled = false;
    }

    this.close();
    setTimeout(() => {
      if (typeof (window as any).handleGameStart === 'function') {
        (window as any).handleGameStart('pvp');
      }
    }, 1500);
  }

  private createWidget(): void {
    const widget = document.createElement('div');
    widget.id = 'chat-widget';
    widget.className = 'fixed bottom-0 right-0 z-[1000]';

    // Generated a longer and wider zig-zag path to ensure it covers all screen heights and is more pronounced.
    const zigZagClipPath = "polygon(0 0, 24px 16px, 0px 32px, 24px 48px, 0px 64px, 24px 80px, 0px 96px, 24px 112px, 0px 128px, 24px 144px, 0px 160px, 24px 176px, 0px 192px, 24px 208px, 0px 224px, 24px 240px, 0px 256px, 24px 272px, 0px 288px, 24px 304px, 0px 320px, 24px 336px, 0px 352px, 24px 368px, 0px 384px, 24px 400px, 0px 416px, 24px 432px, 0px 448px, 24px 464px, 0px 480px, 24px 496px, 0px 512px, 24px 528px, 0px 544px, 24px 560px, 0px 576px, 24px 592px, 0px 608px, 24px 624px, 0px 640px, 24px 656px, 0px 672px, 24px 688px, 0px 704px, 24px 720px, 0px 736px, 24px 752px, 0px 768px, 24px 784px, 0px 800px, 24px 816px, 0px 832px, 24px 848px, 0px 864px, 24px 880px, 0px 896px, 24px 912px, 0px 928px, 24px 944px, 0px 960px, 24px 976px, 0px 992px, 24px 1008px, 0px 1024px, 24px 1040px, 0px 1056px, 24px 1072px, 0px 1088px, 24px 1104px, 0px 1120px, 24px 1136px, 0px 1152px, 24px 1168px, 0px 1184px, 24px 1200px, 0px 1216px, 24px 1232px, 0px 1248px, 24px 1264px, 0px 1280px, 24px 1296px, 0px 1312px, 24px 1328px, 0px 1344px, 24px 1360px, 0px 1376px, 24px 1392px, 0px 1408px, 24px 1424px, 0px 1440px, 24px 1456px, 0px 1472px, 24px 1488px, 0px 1504px, 24px 1520px, 0px 1536px, 24px 1552px, 0px 1568px, 24px 1584px, 0px 1600px, 24px 1616px, 0px 1632px, 24px 1648px, 0px 1664px, 24px 1680px, 0px 1696px, 24px 1712px, 0px 1728px, 24px 1744px, 0px 1760px, 24px 1776px, 0px 1792px, 24px 1808px, 0px 1824px, 24px 1840px, 0px 1856px, 24px 1872px, 0px 1888px, 24px 1904px, 0px 1920px, 24px 1936px, 0px 1952px, 24px 1968px, 0px 1984px, 24px 2000px, 0px 2016px, 24px 2032px, 0px 2048px, 24px 2064px, 0px 2080px, 24px 2096px, 0px 2112px, 24px 2128px, 0px 2144px, 24px 2160px, 0px 2176px, 24px 2192px, 0px 2208px, 24px 2224px, 0px 2240px, 24px 2256px, 0px 2272px, 24px 2288px, 0px 2304px, 24px 2320px, 0px 2336px, 24px 2352px, 0px 2368px, 24px 2384px, 0px 2400px, 0 100%, 100% 100%, 100% 0)";
    const zigZagPolyline = "0,0 24,16 0,32 24,48 0,64 24,80 0,96 24,112 0,128 24,144 0,160 24,176 0,192 24,208 0,224 24,240 0,256 24,272 0,288 24,304 0,320 24,336 0,352 24,368 0,384 24,400 0,416 24,432 0,448 24,464 0,480 24,496 0,512 24,528 0,544 24,560 0,576 24,592 0,608 24,624 0,640 24,656 0,672 24,688 0,704 24,720 0,736 24,752 0,768 24,784 0,800 24,816 0,832 24,848 0,864 24,880 0,896 24,912 0,928 24,944 0,960 24,976 0,992 24,1008 0,1024 24,1040 0,1056 24,1072 0,1088 24,1104 0,1120 24,1136 0,1152 24,1168 0,1184 24,1200 0,1216 24,1232 0,1248 24,1264 0,1280 24,1296 0,1312 24,1328 0,1344 24,1360 0,1376 24,1392 0,1408 24,1424 0,1440 24,1456 0,1472 24,1488 0,1504 24,1520 0,1536 24,1552 0,1568 24,1584 0,1600 24,1616 0,1632 24,1648 0,1664 24,1680 0,1696 24,1712 0,1728 24,1744 0,1760 24,1776 0,1792 24,1808 0,1824 24,1840 0,1856 24,1872 0,1888 24,1904 0,1920 24,1936 0,1952 24,1968 0,1984 24,2000 0,2016 24,2032 0,2048 24,2064 0,2080 24,2096 0,2112 24,2128 0,2144 24,2160 0,2176 24,2192 0,2208 24,2224 0,2240 24,2256 0,2272 24,2288 0,2304 24,2320 0,2336 24,2352 0,2368 24,2384 0,2400";
    
    widget.innerHTML = `
      <button id="chat-toggle-btn" class="fixed bottom-6 right-6 w-16 h-16 bg-gradient-to-br from-pink-500 to-pink-600 border-thick shadow-sharp text-white text-2xl transition-all duration-300 hover:scale-110 z-[1001] flex items-center justify-center">
        <svg class="w-6 h-6" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"></path>
        </svg>
        <span id="unread-badge" class="hidden absolute -top-1 -right-1 bg-yellow-400 text-black text-xs font-black px-2 py-1 border-2 border-black min-w-[20px]">0</span>
      </button>

      <div id="chat-panel" class="fixed top-0 right-0 w-[440px] h-screen bg-gradient-to-b from-yellow-50 to-yellow-100 shadow-2xl transform translate-x-full transition-transform duration-300 flex flex-col z-[1002]" style="clip-path: ${zigZagClipPath};">
        
        <!-- Black zig-zag outline following the cut edge -->
        <svg class="absolute left-0 top-0 h-full pointer-events-none z-10" style="width: 30px;">
          <polyline points="${zigZagPolyline}" 
                    stroke="black" stroke-width="4" fill="none" stroke-linejoin="miter"/>
        </svg>
        
        <div class="bg-gradient-to-r from-pink-500 to-pink-600 p-4 pl-[40px] flex items-center gap-3 text-white flex-shrink-0 border-b-thick">
          <button id="chat-back-btn" class="hidden px-4 py-2 bg-yellow-300 text-black border-thick shadow-sharp hover:scale-110 hover:bg-yellow-400 transition font-black uppercase text-sm">← Back</button>
          <h3 id="chat-title" class="flex-1 text-2xl font-black uppercase text-outline-sm-black">Messages</h3>
          <div class="flex gap-2">
            <button id="blocked-btn" class="px-4 py-2 bg-white text-black border-thick shadow-sharp hover:scale-110 hover:bg-gray-200 transition font-black uppercase text-sm" title="Blocked users">Block</button>
            <button id="chat-close-btn" class="px-4 py-2 bg-red-500 text-white border-thick shadow-sharp hover:scale-110 hover:bg-red-600 transition text-xl font-black leading-none">×</button>
          </div>
        </div>

        <div id="conversations-view" class="flex-1 flex flex-col overflow-hidden pl-[40px]">
          <div id="conversations-list" class="flex-1 overflow-y-auto p-2"></div>
        </div>

        <div id="blocked-view" class="hidden flex-1 flex flex-col overflow-hidden pl-[40px]">
          <div id="blocked-list" class="flex-1 overflow-y-auto p-2"></div>
        </div>

        <div id="chat-view" class="hidden flex-1 flex flex-col overflow-hidden pl-[40px]">
          <div class="flex gap-2 p-3 bg-white border-b-thick flex-shrink-0">
            <button id="view-profile-btn" class="flex-1 px-3 py-2 border-thick text-sm font-black uppercase text-yellow-600 bg-yellow-50 hover:bg-yellow-100 transition shadow-sharp">👤 PROFILE</button>
            <button id="block-btn" class="flex-1 px-3 py-2 border-thick text-sm font-black uppercase text-red-600 bg-red-50 hover:bg-red-100 transition shadow-sharp">🚫 BLOCK</button>
          </div>
          <div id="chat-messages" class="flex-1 overflow-y-auto p-4 flex flex-col gap-3 bg-yellow-50"></div>
          <div id="typing-indicator" class="hidden flex items-center gap-1 px-4 py-2">
            <span class="w-2 h-2 bg-pink-500 animate-bounce"></span>
            <span class="w-2 h-2 bg-pink-500 animate-bounce [animation-delay:0.2s]"></span>
            <span class="w-2 h-2 bg-pink-500 animate-bounce [animation-delay:0.4s]"></span>
          </div>
          <div class="p-4 bg-white border-t-thick flex gap-2 items-center flex-shrink-0">
            <button id="game-btn" class="p-3 text-lg bg-yellow-300 border-thick hover:scale-110 transition shadow-sharp" title="Invite to play">🎮</button>
              <input type="text" id="chat-input" placeholder="Type message..." 
                class="flex-1 px-4 py-3 border-thick outline-none bg-yellow-50 focus:bg-white transition font-black" />
              <button id="chat-send-btn" class="w-12 h-12 bg-pink-500 border-thick text-white text-xl hover:scale-105 transition flex-shrink-0 shadow-sharp font-black">→</button>
          </div>
        </div>

      </div>
    `;

    document.body.appendChild(widget);
    this.container = widget;
  }

  private attachEventListeners(): void {
    document.getElementById('chat-toggle-btn')?.addEventListener('click', () => this.toggle());
    document.getElementById('chat-close-btn')?.addEventListener('click', () => this.close());
    document.getElementById('chat-back-btn')?.addEventListener('click', () => this.goBack());
    document.getElementById('blocked-btn')?.addEventListener('click', () => this.toggleBlockedView());
    document.getElementById('chat-send-btn')?.addEventListener('click', () => this.sendMessage());
    document.getElementById('game-btn')?.addEventListener('click', () => this.sendGameInvite());
    document.getElementById('block-btn')?.addEventListener('click', () => this.blockUser());
    document.getElementById('view-profile-btn')?.addEventListener('click', () => this.viewProfile());
    
    const input = document.getElementById('chat-input') as HTMLInputElement;
    input?.addEventListener('keypress', (e) => {
      if (e.key === 'Enter') this.sendMessage();
    });
    input?.addEventListener('input', () => this.handleTyping());

    document.getElementById('conv-search')?.addEventListener('input', (e) => 
      this.filterConversations((e.target as HTMLInputElement).value)
    );
  }

  private getCurrentUserId(): number {
    if (appState.currentUser && appState.currentUser.id) {
      return appState.currentUser.id;
    }
    const userData = localStorage.getItem('userData');
    if (userData) {
      try {
        const parsed = JSON.parse(userData);
        return parsed.id || 0;
      } catch (e) {}
    }
    return 0;
  }

  private toggle(): void {
    this.isOpen ? this.close() : this.open();
  }

  private open(): void {
    if (!this.container) return;
    const panel = document.getElementById('chat-panel')!;
    panel.classList.remove('translate-x-full');
    panel.classList.add('translate-x-0');
    this.isOpen = true;
    this.showConversationsView();
    
    setTimeout(() => {
      this.updateBadge();
    }, 200);
    
    if (this.currentView === 'conversations') {
      setTimeout(() => {
        this.displayPendingNotifications();
      }, 100);
    }
  }

  private close(): void {
    if (!this.container) return;
    const panel = document.getElementById('chat-panel')!;
    panel.classList.remove('translate-x-0');
    panel.classList.add('translate-x-full');
    this.isOpen = false;
    
    this.updateBadge();
  }

  private handleGameInvitation(data: any): void {
    //Vérifier si une modale existe déjà
    const existingModal = document.getElementById('game-invite-modal');
    if (existingModal) {
      console.log('Invitation modal already open, ignoring duplicate');
      return;
    }
  
    // Create dark overlay
    const overlay = document.createElement('div');
    overlay.id = 'game-invite-overlay';
    overlay.className = 'fixed inset-0 bg-black/80 flex items-center justify-center z-[10500] animate-fade-in';
    
    // Create centered modal
    const modal = document.createElement('div');
    modal.id = 'game-invite-modal'; //Ajouter un ID
    modal.className = 'bg-white border-thick shadow-sharp p-8 max-w-md mx-4 animate-pop';
    modal.innerHTML = `
      <div class="text-center">
        <div class="text-6xl mb-4">🎮</div>
        <h3 class="text-3xl font-black text-black mb-2 uppercase">Game Invitation!</h3>
        <p class="text-gray-700 mb-6 font-bold">
          <span class="font-black text-pink-500">${this.esc(data.senderNickname)}</span> 
          invites you to play Pong!
        </p>
        <div class="flex gap-3">
          <button id="decline-invite" class="flex-1 px-6 py-3 bg-gray-200 text-gray-700 border-thick font-black hover:scale-105 transition shadow-sharp uppercase">
            Decline
          </button>
          <button id="accept-invite" class="flex-1 px-6 py-3 bg-gradient-to-r from-pink-500 to-pink-600 text-white border-thick font-black hover:scale-105 transition shadow-sharp uppercase">
            Accept!
          </button>
        </div>
      </div>
    `;
    
    overlay.appendChild(modal);
    document.body.appendChild(overlay);
    
    overlay.querySelector('#decline-invite')?.addEventListener('click', () => {
      overlay.remove();
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        this.ws.send(JSON.stringify({
          type: 'game_invite_declined',
          senderId: data.senderId
        }));
      }
    });
    
    overlay.querySelector('#accept-invite')?.addEventListener('click', () => {
      overlay.remove();
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        this.ws.send(JSON.stringify({
          type: 'game_invite_accepted',
          senderId: data.senderId
        }));
      }
      this.close();
      if (typeof (window as any).handleGameStart === 'function') {
        (window as any).handleGameStart('pvp');
      }
    });
  }

  private sendGameInvite(): void {
    if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
      showNotification('Connection lost!', 'error');
      return;
    }
  
    if (!this.currentChatUserId) return;

    //Vérifier si une invitation est déjà en attente
    if (this.pendingInvites.has(this.currentChatUserId)) {
      showNotification('An invitation is already pending!', 'error');
      return;
    }
    
    //Vérifier le cooldown (10 secondes entre chaque invitation)
    const now = Date.now();
    const lastInviteTime = this.inviteCooldowns.get(this.currentChatUserId) || 0;
    const cooldownTime = 10000; // 10 secondes
    
    if (now - lastInviteTime < cooldownTime) {
      const remainingSeconds = Math.ceil((cooldownTime - (now - lastInviteTime)) / 1000);
      // alert(`Please wait ${remainingSeconds} seconds before sending another invitation to this player.`);
      showNotification(`Please wait ${remainingSeconds} seconds!`, 'error');
      return;
    }

    //Ajouter à la liste des invitations en attente
    this.pendingInvites.add(this.currentChatUserId);
    this.inviteCooldowns.set(this.currentChatUserId, now);
    
    //Désactiver le bouton temporairement
    const gameBtn = document.getElementById('game-btn');
    if (gameBtn) {
      gameBtn.classList.add('opacity-50', 'cursor-not-allowed');
      (gameBtn as HTMLButtonElement).disabled = true;
    }

    this.ws.send(JSON.stringify({
      type: 'game_invite',
      receiverId: this.currentChatUserId
    }));
    
    //Timeout de sécurité : retirer de pending après 30 secondes
    setTimeout(() => {
      this.pendingInvites.delete(this.currentChatUserId!);
      if (gameBtn) {
        gameBtn.classList.remove('opacity-50', 'cursor-not-allowed');
        (gameBtn as HTMLButtonElement).disabled = false;
      }
    }, 30000);
  }

  private sendMessage(): void {
    const input = document.getElementById('chat-input') as HTMLInputElement;
    const text = input.value.trim();

    if (!text || !this.currentChatUserId) return;
    if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
      showNotification('Connection lost, reconnecting...', 'error'); // CHANGED
      this.connectWebSocket();
      return;
    }

    this.ws.send(JSON.stringify({
      type: 'send_message',
      receiverId: this.currentChatUserId,
      text
    }));

    input.value = '';
  }

  private handleTyping(): void {
    if (!this.ws || this.ws.readyState !== WebSocket.OPEN) return;
    if (!this.currentChatUserId) return;
    
    this.ws.send(JSON.stringify({
      type: 'typing',
      receiverId: this.currentChatUserId
    }));

    if (this.typingTimeout) clearTimeout(this.typingTimeout);
    this.typingTimeout = window.setTimeout(() => {
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        this.ws.send(JSON.stringify({
          type: 'stop_typing',
          receiverId: this.currentChatUserId
        }));
      }
    }, 1000);
  }

  private async loadConversations(): Promise<void> {
    if (this.currentView !== 'conversations') return;
    await this.loadFriendsForChat();
  }

  private async loadFriendsForChat(): Promise<void> {
    try {
      const res = await fetch('/api/user/friends/all', { credentials: 'include' });
      if (!res.ok) throw new Error('Failed to load friends');
      const data = await res.json();
      const friends = data.friends || [];
      
      const convRes = await fetch('/api/chat/conversations', { credentials: 'include' });
      const convData = await convRes.json();
      const conversations = convData.conversations || [];
      
      const existingConvIds = new Set(conversations.map((c: any) => c.user_id));
      const allChats = [
        ...conversations,
        ...friends
          .filter((f: any) => !existingConvIds.has(f.id))
          .map((f: any) => ({
            user_id: f.id,
            nickname: f.nickname,
            avatar_url: f.avatar_url,
            last_message: null,
            unread_count: 0,
            last_msg_time: null,
            isOnline: false
          }))
      ];
      
      allChats.sort((a, b) => {
        if (!a.last_msg_time) return 1;
        if (!b.last_msg_time) return -1;
        return new Date(b.last_msg_time).getTime() - new Date(a.last_msg_time).getTime();
      });
      
      this.renderConversations(allChats);
    } catch (err) {
      console.error('Error loading friends for chat:', err);
    }
  }

  private renderConversations(convs: Conversation[]): void {
    const list = document.getElementById('conversations-list')!;
    
    if (convs.length === 0) {
      list.innerHTML = '<div class="text-center py-16 px-5 text-gray-500 font-bold uppercase">No friends yet<br><span class="text-sm">Add friends to start chatting!</span></div>';
      this.updateBadge();
      return;
    }

    list.innerHTML = convs.map(c => {
      const isOnline = this.onlineUserIds.has(c.user_id);
      
      return `
        <div class="flex items-center gap-3 p-3 bg-white border-thick mb-2 cursor-pointer hover:bg-yellow-100 hover:shadow-sharp transition" 
            data-user-id="${c.user_id}"
            onclick="chatWidget.openChat(${c.user_id}, '${this.esc(c.nickname)}')">
          <div class="relative flex-shrink-0">
            ${this.getAvatar(c)}
            <span class="online-indicator absolute bottom-0 right-0 w-3 h-3 ${isOnline ? 'bg-green-500' : 'bg-gray-400'} border-2 border-white"></span>
          </div>
          <div class="flex-1 min-w-0">
            <div class="font-black text-black truncate">${this.esc(c.nickname)}</div>
            <div class="text-sm text-gray-600 truncate font-bold">${c.last_message ? this.esc(c.last_message) : 'Start conversation'}</div>
          </div>
          ${c.unread_count > 0 ? `<span class="flex-shrink-0 bg-pink-500 text-white text-xs font-black px-2 py-1 border-2 border-black min-w-[20px] text-center">${c.unread_count}</span>` : ''}
        </div>
      `;
    }).join('');
    
    this.updateBadge();
  }

  private addMessageToUI(sender: string, text: string, isOwn: boolean, timestamp?: string): void {
    const container = document.getElementById('chat-messages')!;
    const noMsg = container.querySelector('.text-center');
    if (noMsg) noMsg.remove();

    const time = timestamp ? new Date(timestamp).toLocaleTimeString('en-US', {hour:'2-digit',minute:'2-digit'}) : '';
    const div = document.createElement('div');
    div.className = `flex flex-col gap-1 transition-all duration-300 ${isOwn ? 'items-end' : ''}`;
    div.innerHTML = `
      <div class="max-w-[80%] px-4 py-3 border-thick shadow-sharp ${
        isOwn 
          ? 'bg-gradient-to-r from-pink-500 to-pink-400 text-white' 
          : 'bg-white text-gray-800'
      }">
        <div class="text-xs font-black opacity-80 mb-1">${this.esc(sender)}</div>
        <div class="text-sm leading-relaxed break-words font-bold">${this.esc(text)}</div>
      </div>
      <div class="text-xs text-gray-500 px-2 font-bold">${time}</div>
    `;
    
    container.appendChild(div);
    container.scrollTop = container.scrollHeight;
  }

  private addSystemMsg(text: string): void {
    const container = document.getElementById('chat-messages');
    if (!container) return;
    
    const msgDiv = document.createElement('div');
    msgDiv.className = 'flex justify-center my-2';
    msgDiv.innerHTML = `
      <div class="bg-gray-100 border-thick px-4 py-2 text-sm text-gray-600 font-bold uppercase text-center max-w-xs">
        ${this.esc(text)}
      </div>
    `;
    container.appendChild(msgDiv);
    container.scrollTop = container.scrollHeight;
  }

  private esc(str: string): string {
    const div = document.createElement('div');
    div.textContent = str;
    return div.innerHTML;
  }

  private showConfirmModal(title: string, message: string, onConfirm: () => void): void {
    // Create dark overlay
    const overlay = document.createElement('div');
    overlay.id = 'confirm-modal-overlay';
    overlay.className = 'fixed inset-0 bg-black/80 flex items-center justify-center z-[10500] animate-fade-in';
    
    const modal = document.createElement('div');
    modal.className = 'bg-white border-thick shadow-sharp p-8 max-w-md mx-4 animate-pop';
    modal.innerHTML = `
      <div class="text-center">
        <h3 class="text-3xl font-black text-black mb-3 uppercase">${this.esc(title)}</h3>
        <p class="text-gray-700 mb-6 font-bold">${this.esc(message)}</p>
        <div class="flex gap-3">
          <button id="confirm-cancel" class="flex-1 px-6 py-3 bg-gray-200 text-gray-700 border-thick font-black hover:scale-105 transition shadow-sharp uppercase">
            Cancel
          </button>
          <button id="confirm-ok" class="flex-1 px-6 py-3 bg-gradient-to-r from-pink-500 to-pink-600 text-white border-thick font-black hover:scale-105 transition shadow-sharp uppercase">
            Confirm
          </button>
        </div>
      </div>
    `;
    
    overlay.appendChild(modal);
    document.body.appendChild(overlay);
    
    overlay.querySelector('#confirm-cancel')?.addEventListener('click', () => {
      overlay.remove();
    });
    
    overlay.querySelector('#confirm-ok')?.addEventListener('click', () => {
      overlay.remove();
      onConfirm();
    });
    
    // Click outside to cancel
    overlay.addEventListener('click', (e) => {
      if (e.target === overlay) {
        overlay.remove();
      }
    });
  }

  private getAvatar(user: User | Conversation): string {
    const avatarUrl = user.avatar_url;
    if (!avatarUrl) {
      return `<div class="w-12 h-12 bg-gradient-to-br from-pink-400 to-pink-600 flex items-center justify-center text-white font-black text-xl border-thick">${user.nickname.charAt(0).toUpperCase()}</div>`;
    }
    return `<img src="${avatarUrl}" class="w-12 h-12 object-cover border-thick" />`;
  }

  private async showConversationsView(): Promise<void> {
    this.currentView = 'conversations';
    this.hideAllViews();
    const view = document.getElementById('conversations-view')!;
    view.classList.remove('hidden');
    view.classList.add('flex');
    
    const chatTitle = document.getElementById('chat-title');
    if (chatTitle) chatTitle.textContent = 'Messages';
    
    const backBtn = document.getElementById('chat-back-btn');
    if (backBtn) backBtn.classList.add('hidden');
    
    const blockedBtn = document.getElementById('blocked-btn');
    if (blockedBtn) {
      blockedBtn.classList.remove('hidden');
      blockedBtn.innerHTML = 'Block';
      blockedBtn.title = 'Blocked users';
    }
    
    await this.loadConversations();
    
    setTimeout(() => {
      this.displayPendingNotifications();
    }, 100);
  }

  private showBlockedView(): void {
      this.currentView = 'blocked';
      this.hideAllViews();
      document.getElementById('blocked-view')!.classList.remove('hidden');
      document.getElementById('blocked-view')!.classList.add('flex');
      
      document.getElementById('chat-back-btn')!.classList.remove('hidden');
      
      const blockedBtn = document.getElementById('blocked-btn');
      if (blockedBtn) {
        blockedBtn.classList.add('hidden');
      }
      
      document.getElementById('chat-title')!.textContent = 'Blocked Users';
      this.loadBlockedUsers();
  }
  
  private toggleBlockedView(): void {
    if (this.currentView === 'blocked') {
      this.showConversationsView();
    } else {
      this.showBlockedView();
    }
  }

  private async showChatView(userId: number, nickname: string): Promise<void> {
      this.currentView = 'chat';
      this.currentChatUserId = userId;
      this.currentChatUserNickname = nickname;
      await this.checkIfBlocked(userId);
      
      this.hideAllViews();
      document.getElementById('chat-view')!.classList.remove('hidden');
      document.getElementById('chat-view')!.classList.add('flex');
      document.getElementById('chat-back-btn')!.classList.remove('hidden');
      document.getElementById('chat-title')!.textContent = nickname;
      
      const blockedBtn = document.getElementById('blocked-btn');
      if (blockedBtn) blockedBtn.classList.add('hidden');
      
      this.resetChatInputState();
      
      this.updateBlockButton();
      await this.loadMessageHistory(userId);
      
      await this.checkIfUserAnonymized(userId);
      
      setTimeout(() => {
        this.updateBadge();
      }, 200);
  }
  
  private async checkIfBlocked(userId: number): Promise<void> {
    try {
      const response = await fetch('/api/chat/blocked', { credentials: 'include' });
      const data = await response.json();
      const blockedUsers = data.blockedUsers || [];
      this.isCurrentUserBlocked = blockedUsers.some((u: User) => u.id === userId);
    } catch (error) {
      this.isCurrentUserBlocked = false;
    }
  }
  
  private updateBlockButton(): void {
    const blockBtn = document.getElementById('block-btn')!;
    if (this.isCurrentUserBlocked) {
      blockBtn.textContent = '✅ UNBLOCK';
      blockBtn.className = 'flex-1 px-3 py-2 border-thick text-sm font-black uppercase text-green-600 bg-green-50 hover:bg-green-100 transition shadow-sharp';
    } else {
      blockBtn.textContent = '🚫 BLOCK';
      blockBtn.className = 'flex-1 px-3 py-2 border-thick text-sm font-black uppercase text-red-600 bg-red-50 hover:bg-red-100 transition shadow-sharp';
    }
  }
  
  private hideAllViews(): void {
    document.querySelectorAll('#conversations-view, #blocked-view, #chat-view').forEach(v => {
      v.classList.add('hidden');
      v.classList.remove('flex');
    });
  }
  
  private goBack(): void {
    this.showConversationsView();
  }
  
  private updateOnlineUsers(users: number[]): void {
    users.forEach(id => {
      this.onlineUserIds.add(id);
      this.markUserOnline(id);
    });
  }

  private markUserOnline(userId: number): void {
    this.onlineUserIds.add(userId);
    const userElements = document.querySelectorAll(`[data-user-id="${userId}"] .online-indicator`);
    userElements.forEach(el => {
      el.classList.remove('bg-gray-400');
      el.classList.add('bg-green-500');
    });
  }

  private markUserOffline(userId: number): void {
    this.onlineUserIds.delete(userId);
    const userElements = document.querySelectorAll(`[data-user-id="${userId}"] .online-indicator`);
    userElements.forEach(el => {
      el.classList.remove('bg-green-500');
      el.classList.add('bg-gray-400');
    });
  }

  private markMessagesAsRead(senderId: number): void {
    if (!this.ws || this.ws.readyState !== WebSocket.OPEN) return;
    
    this.ws.send(JSON.stringify({
      type: 'mark_read',
      senderId: senderId
    }));
  }

  private handleNewMessage(msg: Message): void {
    if (msg.sender_id === this.currentChatUserId || msg.receiver_id === this.currentChatUserId) {
      const isSentByMe = msg.sender_id === this.currentUserId;
      const displayName = isSentByMe ? 'You' : msg.sender_nickname;
      this.addMessageToUI(displayName, msg.message, isSentByMe, msg.created_at);
      
      if (!isSentByMe && this.currentView === 'chat') {
        this.markMessagesAsRead(msg.sender_id);
      }
    }
    
    this.loadFriendsForChat().then(() => {
      this.updateBadge();
    });
  }
  
  private handleMessageSent(msg: Message): void {
    this.addMessageToUI('You', msg.message, true, msg.created_at);
  }
  
  private showTypingIndicator(userId: number): void {
    if (userId === this.currentChatUserId) {
      document.getElementById('typing-indicator')!.classList.remove('hidden');
      document.getElementById('typing-indicator')!.classList.add('flex');
    }
  }
  
  private hideTypingIndicator(userId: number): void {
    if (userId === this.currentChatUserId) {
      document.getElementById('typing-indicator')!.classList.add('hidden');
      document.getElementById('typing-indicator')!.classList.remove('flex');
    }
  }

  private handleTournamentNotification(data: any): void {
    console.log('🎮 Tournament notification received:', data);
    
    try {
      const notifId = `notif-${Date.now()}-${Math.random()}`;
      
      this.pendingTournamentNotifications.push({
        id: notifId,
        message: data.message,
        timestamp: data.timestamp || new Date().toISOString()
      });
      
      showNotification(data.message, 'info');
      
      if (this.isOpen && this.currentView === 'conversations') {
        this.displayPendingNotifications();
      }
      
      this.updateBadge();
      
      setTimeout(() => {
        const index = this.pendingTournamentNotifications.findIndex(n => n.id === notifId);
        if (index > -1) {
          this.pendingTournamentNotifications.splice(index, 1);
          
          this.updateBadge();
          
          const element = document.getElementById(notifId);
          if (element) {
            element.remove();
          }
        }
      }, 60000);
      
    } catch (error) {
      console.error('Error handling tournament notification:', error);
    }
  }

  private updateBadge(): void {
    const badge = document.getElementById('unread-badge');
    if (!badge) return;
    
    // Count total unread messages from conversations
    const totalUnreadMessages = this.getTotalUnreadCount();
    
    // Count tournament notifications
    const tournamentNotifCount = this.pendingTournamentNotifications.length;
    
    // Total = unread messages + tournament notifications
    const totalCount = totalUnreadMessages + tournamentNotifCount;
    
    if (totalCount > 0) {
      badge.classList.remove('hidden');
      badge.textContent = totalCount.toString();
    } else {
      badge.classList.add('hidden');
    }
  }

  private getTotalUnreadCount(): number {
    // Get all conversation elements and sum their unread counts
    const convElements = document.querySelectorAll('#conversations-list [data-user-id]');
    let total = 0;
    
    convElements.forEach(el => {
      const unreadBadge = el.querySelector('.bg-pink-500');
      if (unreadBadge) {
        const count = parseInt(unreadBadge.textContent || '0');
        if (!isNaN(count)) {
          total += count;
        }
      }
    });
    
    return total;
  }

  private displayPendingNotifications(): void {
    try {
      const list = document.getElementById('conversations-list');
      if (!list || this.pendingTournamentNotifications.length === 0) {
        return;
      }
      
      console.log(`Rendering ${this.pendingTournamentNotifications.length} notifications`);
      
      this.pendingTournamentNotifications.forEach(notif => {
        if (document.getElementById(notif.id)) {
          return;
        }
        
        const notifEl = document.createElement('div');
        notifEl.id = notif.id;
        notifEl.className = 'p-3 mb-2 bg-purple-100 border-l-4 border-purple-500 text-sm tournament-notification';
        notifEl.innerHTML = `
          <div class="font-bold text-purple-900">Tournament Update</div>
          <div class="text-purple-700">${this.esc(notif.message)}</div>
          <div class="text-xs text-purple-500 mt-1">${new Date(notif.timestamp).toLocaleTimeString()}</div>
        `;
        list.prepend(notifEl);
      });
            
    } catch (error) {
      console.error('Error displaying pending notifications:', error);
    }
  }

  private filterConversations(query: string): void {
    const items = document.querySelectorAll('#conversations-list > div');
    items.forEach(item => {
      const name = item.querySelector('.font-black')?.textContent?.toLowerCase() || '';
      (item as HTMLElement).style.display = name.includes(query.toLowerCase()) ? '' : 'none';
    });
  }
  
  private viewProfile(): void {
    if (!this.currentChatUserId) return;
    this.close();
    window.dispatchEvent(new CustomEvent('viewUserProfile', {
      detail: { userId: this.currentChatUserId }
    }));
  }
  
  public async blockUser(): Promise<void> {
    if (!this.currentChatUserId) return;
    
    if (this.isCurrentUserBlocked) {
      // Show unblock confirmation
      this.showConfirmModal(
        `Unblock ${this.currentChatUserNickname}?`,
        'This user will be able to message you again.',
        async () => {
          try {
            const res = await fetch(`/api/chat/block/${this.currentChatUserId}`, {
              method: 'DELETE',
              credentials: 'include'
            });
            if (!res.ok) throw new Error('Failed to unblock user');
            showNotification(`${this.currentChatUserNickname} unblocked`, 'success');
            this.isCurrentUserBlocked = false;
            this.updateBlockButton();
          } catch (err) {
            showNotification('Failed to unblock user', 'error');
          }
        }
      );
    } else {
      // Show block confirmation
      this.showConfirmModal(
        `Block ${this.currentChatUserNickname}?`,
        'You will no longer receive messages from this user.',
        async () => {
          try {
            const res = await fetch(`/api/chat/block/${this.currentChatUserId}`, {
              method: 'POST',
              credentials: 'include'
            });
            if (!res.ok) throw new Error('Failed to block user');
            showNotification(`${this.currentChatUserNickname} blocked`, 'success');
            this.showConversationsView();
          } catch (err) {
            showNotification('Failed to block user', 'error');
          }
        }
      );
    }
  }
  
  public async unblock(userId: number, nickname: string): Promise<void> {
    this.showConfirmModal(
      `Unblock ${nickname}?`,
      'This user will be able to message you again.',
      async () => {
        try {
          const res = await fetch(`/api/chat/block/${userId}`, {
            method: 'DELETE',
            credentials: 'include'
          });
          if (!res.ok) throw new Error('Failed to unblock user');
          showNotification(`${nickname} unblocked`, 'success');
          this.loadBlockedUsers();
        } catch (err) {
          showNotification('Failed to unblock user', 'error');
        }
      }
    );
  }
  
  private async loadBlockedUsers(): Promise<void> {
    try {
      const response = await fetch('/api/chat/blocked', { credentials: 'include' });
      const data = await response.json();
      this.renderBlockedUsers(data.blockedUsers);
    } catch (error) {}
  }
  
  private renderBlockedUsers(users: User[]): void {
    const list = document.getElementById('blocked-list')!;
    if (users.length === 0) {
      list.innerHTML = '<div class="text-center py-16 text-gray-500 font-bold uppercase">No blocked users</div>';
      return;
    }
    list.innerHTML = users.map(u => `
      <div class="flex items-center gap-3 p-3 bg-white border-thick mb-1">
        <div class="flex-shrink-0">${this.getAvatar(u)}</div>
        <div class="flex-1 min-w-0">
          <div class="font-black text-gray-800 truncate uppercase">${this.esc(u.nickname)}</div>
          <div class="text-sm text-gray-500 font-bold">🚫 Blocked</div>
        </div>
        <button onclick="chatWidget.unblock(${u.id}, '${this.esc(u.nickname)}')" 
                class="flex-shrink-0 px-4 py-2 bg-green-500 text-white text-sm font-black border-thick hover:scale-105 transition shadow-sharp uppercase">
          UNBLOCK
        </button>
      </div>
    `).join('');
  }
  
  private async loadMessageHistory(userId: number): Promise<void> {
    try {
      const response = await fetch(`/api/chat/messages/${userId}`, { credentials: 'include' });
      const data = await response.json();
      const container = document.getElementById('chat-messages')!;
      container.innerHTML = '';
      if (data.messages.length === 0) {
        container.innerHTML = '<div class="text-center py-16 text-gray-500 font-bold uppercase">No messages yet. Say hi! 👋</div>';
        return;
      }
      data.messages.forEach((msg: Message) => {
        const isSentByMe = msg.sender_id === this.currentUserId;
        const displayName = isSentByMe ? 'You' : msg.sender_nickname;
        this.addMessageToUI(displayName, msg.message, isSentByMe, msg.created_at);
      });
    } catch (error) {}
  }
  
  public openChat(userId: number, nickname: string): void {
    this.showChatView(userId, nickname);
  }

  public destroy(): void {
    if (this.ws) {
      this.ws.close();
      this.ws = null;
    }
    if (this.container) {
      this.container.remove();
      this.container = null;
    }
    if (this.typingTimeout) {
      clearTimeout(this.typingTimeout);
      this.typingTimeout = null;
    }
  }
}

declare global {
  interface Window {
    chatWidget: ChatWidget;
  }
}