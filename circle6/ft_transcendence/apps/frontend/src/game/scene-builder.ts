// scene-builder.ts
import {
    Scene,
    Vector3,
    Color3,
    Color4,
    Mesh,
    MeshBuilder,
    StandardMaterial,
    ParticleSystem,
    Texture,
    ArcRotateCamera,
    HemisphericLight,
    DirectionalLight,
    TrailMesh,
    DynamicTexture
} from '@babylonjs/core';
import '@babylonjs/core/Rendering/outlineRenderer';
import {
    PADDLE_START_X,
    PADDLE_WIDTH,
    PADDLE_HEIGHT,
    PADDLE_DEPTH,
    BALL_DIAMETER,
    WALL_Z_POSITION,
    VISUAL_PLAYFIELD_WIDTH,
    VISUAL_PLAYFIELD_DEPTH,
    BORDER_HEIGHT,
    BORDER_THICKNESS,
    ARENA_WIDTH
} from "@trans/common-types";

export interface SceneObjects {
    player1Paddle: Mesh;
    player2Paddle: Mesh;
    ball: Mesh;
    particleSystem: ParticleSystem;
    spawnParticleSystem: ParticleSystem;
}

export function createSceneAndGameObjects(scene: Scene, canvas: HTMLCanvasElement): SceneObjects {
    const engine = scene.getEngine();
    engine.enableOfflineSupport = false;
    engine.setHardwareScalingLevel(1 / window.devicePixelRatio);
    engine.resize();
    
    const light1 = new HemisphericLight("light1", new Vector3(0, 1, 0), scene);
    light1.intensity = 0.8;
    light1.groundColor = new Color3(0.1, 0.1, 0.4);
    
    const light2 = new DirectionalLight("light2", new Vector3(0.5, -1, 0.5), scene);
    light2.intensity = 0.5;
    light2.diffuse = new Color3(1, 0.95, 0.85);
    
    createCuteEnvironment(scene);
    createVisualBorders(scene);
    const gameObjects = createMovableObjects(scene);
    setupCamera(scene, canvas);
    
    return gameObjects;
}

function createCuteEnvironment(scene: Scene): void {
    createSkybox(scene);
    
    createProfessionalBuildings(scene);
    createArchitecturalDetails(scene);
}

function createSkybox(scene: Scene): void {
    // Create skybox that follows camera
    const skybox = MeshBuilder.CreateSphere("skyBox", { diameter: 50 }, scene);
    
    const skyboxMaterial = new StandardMaterial("skyBox", scene);
    
    const texture = new Texture("/sky.png", scene, false, false, Texture.TRILINEAR_SAMPLINGMODE, () => {
        console.log("Skybox texture loaded successfully");
    }, (message?: string, exception?: any) => {
        console.error("Failed to load skybox texture:", message, exception);
        scene.clearColor = new Color4(0.7, 0.3, 0.5, 1.0);
        return ;
    });
    
    skyboxMaterial.diffuseTexture = texture;
    skyboxMaterial.disableLighting = true;
    skyboxMaterial.backFaceCulling = false;
    skyboxMaterial.emissiveTexture = texture;
    skyboxMaterial.emissiveColor = new Color3(0.8, 0.8, 0.8);
    
    // Disable depth writing so it doesn't interfere
    skyboxMaterial.disableDepthWrite = true;
    
    skybox.material = skyboxMaterial;
    skybox.renderingGroupId = 0; // Background
    
    // Make skybox follow camera position (but not rotation)
    scene.registerBeforeRender(() => {
        if (scene.activeCamera && !skybox.isDisposed()) {
            skybox.position.copyFrom(scene.activeCamera.position);
        }
    });
    
    scene.clearColor = new Color4(0.1, 0.1, 0.1, 1.0);
}

function createProfessionalBuildings(scene: Scene) {
    const buildingCount = 10;
    
    for (let i = 0; i < buildingCount; i++) {
        const buildingType = Math.floor(Math.random() * 3);
        let building: Mesh;
        let height = 30 + Math.random() * 40;
        
        if (buildingType === 0) {
            // Modern tower with taper
            building = MeshBuilder.CreateCylinder(`tower_${i}`, { 
                height: height,
                diameterTop: 4 + Math.random() * 3,
                diameterBottom: 8 + Math.random() * 4,
                tessellation: 8
            }, scene);
        } else if (buildingType === 1) {
            // Art deco style
            building = MeshBuilder.CreateBox(`artdeco_${i}`, {
                width: 6 + Math.random() * 4,
                height: height,
                depth: 6 + Math.random() * 4
            }, scene);
        } else {
            // Modern glass tower
            building = MeshBuilder.CreateCylinder(`modern_${i}`, { 
                height: height,
                diameter: 5 + Math.random() * 3,
                tessellation: 6
            }, scene);
        }
        
        // Create sophisticated building texture
        const buildingTex = new DynamicTexture(`buildingTex_${i}`, { width: 256, height: 512 }, scene, false);
        const ctx = buildingTex.getContext() as any;
        
        // Professional color palette - rich but sophisticated
        const professionalColors = [
            { base: "#8B4B8C", accent: "#A865A8" },  // Deep magenta
            { base: "#7A4B8B", accent: "#9865A8" },  // Purple-pink
            { base: "#9B4B7A", accent: "#B86598" },  // Rose-magenta
            { base: "#6B4B9B", accent: "#8865B8" },  // Deep purple
            { base: "#8B4B6B", accent: "#A86588" },  // Mauve
        ];
        
        const colorScheme = professionalColors[Math.floor(Math.random() * professionalColors.length)];
        
        // Create gradient
        const gradient = ctx.createLinearGradient(0, 0, 0, 512);
        gradient.addColorStop(0, colorScheme.accent);
        gradient.addColorStop(0.7, colorScheme.base);
        gradient.addColorStop(1, "#2A1A2A");
        
        ctx.fillStyle = gradient;
        ctx.fillRect(0, 0, 256, 512);
        
        // Add professional window grid
        ctx.fillStyle = "#FFE4B5";
        ctx.strokeStyle = "#000000";
        ctx.lineWidth = 1;
        
        for (let y = 20; y < 500; y += 25) {
            for (let x = 20; x < 240; x += 30) {
                if (Math.random() > 0.3) {
                    // Window
                    ctx.fillRect(x, y, 16, 12);
                    ctx.strokeRect(x, y, 16, 12);
                    
                    // Window divider
                    ctx.strokeStyle = "#666666";
                    ctx.lineWidth = 0.5;
                    ctx.beginPath();
                    ctx.moveTo(x + 8, y);
                    ctx.lineTo(x + 8, y + 12);
                    ctx.moveTo(x, y + 6);
                    ctx.lineTo(x + 16, y + 6);
                    ctx.stroke();
                    ctx.strokeStyle = "#000000";
                    ctx.lineWidth = 1;
                }
            }
        }
        
        // Add architectural details
        ctx.strokeStyle = colorScheme.accent;
        ctx.lineWidth = 3;
        for (let y = 50; y < 500; y += 100) {
            ctx.strokeRect(5, y, 246, 8);
        }
        
        buildingTex.update();
        
        const buildingMat = new StandardMaterial(`buildingMat_${i}`, scene);
        buildingMat.diffuseTexture = buildingTex;
        buildingMat.specularColor = new Color3(0.8, 0.6, 0.7);
        buildingMat.specularPower = 128;
        buildingMat.emissiveColor = new Color3(0.1, 0.05, 0.08);
        
        building.material = buildingMat;

        // Position much further away from game field
        const angle = (i / buildingCount) * Math.PI * 1.8 + Math.PI * 0.1;
        const distance = 120 + (i % 3) * 30; // Increased from 70 + (i % 3) * 20
        building.position.x = Math.sin(angle) * distance;
        building.position.z = Math.cos(angle) * distance;
        building.position.y = height / 2 - 5;
        
        building.freezeWorldMatrix();
    }
}

function createArchitecturalDetails(scene: Scene) {
    // Professional lighting fixtures
    for (let i = 0; i < 6; i++) {
        const lightFixture = MeshBuilder.CreateCylinder(`light_${i}`, {
            height: 12,
            diameterTop: 1,
            diameterBottom: 2,
            tessellation: 8
        }, scene);
        
        const lightMat = new StandardMaterial(`lightMat_${i}`, scene);
        lightMat.diffuseColor = new Color3(0.2, 0.2, 0.3);
        lightMat.specularColor = new Color3(0.9, 0.9, 1.0);
        lightMat.specularPower = 512;
        
        lightFixture.material = lightMat;
        
        // Light glow at top
        const glow = MeshBuilder.CreateSphere(`glow_${i}`, { diameter: 1.5 }, scene);
        const glowMat = new StandardMaterial(`glowMat_${i}`, scene);
        glowMat.emissiveColor = new Color3(1.0, 0.8, 0.6);
        glowMat.alpha = 0.7;
        glow.material = glowMat;

        const side = i < 3 ? -1 : 1;
        const offset = (i % 3) * 35; // Increased from 25
        
        lightFixture.position.x = side * (70 + offset); // Increased from (40 + offset)
        lightFixture.position.z = (Math.random() - 0.5) * 60; // Increased from 40
        lightFixture.position.y = 6;
        
        glow.position.x = lightFixture.position.x;
        glow.position.z = lightFixture.position.z;
        glow.position.y = 11;
        
        lightFixture.freezeWorldMatrix();
        glow.freezeWorldMatrix();

    }
    
    // Sophisticated floating elements - positioned further from game field
    for (let i = 0; i < 8; i++) {
        const element = MeshBuilder.CreatePolyhedron(`polyhedron_${i}`, {
            type: Math.floor(Math.random() * 4),
            size: 1 + Math.random() * 2
        }, scene);
        
        const elementMat = new StandardMaterial(`elementMat_${i}`, scene);
        elementMat.diffuseColor = new Color3(0.8, 0.4, 0.6);
        elementMat.emissiveColor = new Color3(0.2, 0.1, 0.15);
        elementMat.specularColor = new Color3(1, 0.9, 0.9);
        elementMat.specularPower = 256;
        elementMat.alpha = 0.85;
        
        element.material = elementMat;
        
        // Position much further away from the game field
        const angle = (i / 8) * Math.PI * 2;
        const distance = 80 + Math.random() * 25; // Increased from 45 + Math.random() * 15
        element.position.x = Math.sin(angle) * distance;
        element.position.z = Math.cos(angle) * distance;
        element.position.y = 25 + Math.random() * 10;
                
        // Sophisticated rotation animation
        const rotSpeed = 0.002 + Math.random() * 0.003;
        scene.registerBeforeRender(() => {
            if (!element.isDisposed()) {
                element.rotation.x += rotSpeed;
                element.rotation.y += rotSpeed * 0.7;
                element.rotation.z += rotSpeed * 0.3;
            }
        });
        
        element.freezeWorldMatrix();
    }
}

function createVisualBorders(scene: Scene) {
    const borderMat = new StandardMaterial('borderMat', scene);
    borderMat.emissiveColor = new Color3(1, 0.9, 0);
    borderMat.disableLighting = true;

    // Top wall
    const topWall = MeshBuilder.CreateBox("topWall", {
        width: VISUAL_PLAYFIELD_WIDTH,
        height: BORDER_HEIGHT,
        depth: BORDER_THICKNESS
    }, scene);
    topWall.position = new Vector3(0, BORDER_HEIGHT / 2, WALL_Z_POSITION + BORDER_THICKNESS / 2);
    topWall.material = borderMat;

    // Bottom wall
    const bottomWall = topWall.clone("bottomWall");
    bottomWall.position.z = -(WALL_Z_POSITION + BORDER_THICKNESS / 2);

    // Left wall
    const leftWall = MeshBuilder.CreateBox("leftWall", {
        width: BORDER_THICKNESS,
        height: BORDER_HEIGHT,
        depth: VISUAL_PLAYFIELD_DEPTH + BORDER_THICKNESS
    }, scene);
    leftWall.position = new Vector3(-ARENA_WIDTH / 2, BORDER_HEIGHT / 2, 0);
    leftWall.material = borderMat;

    // Right wall
    const rightWall = leftWall.clone("rightWall");
    rightWall.position.x = ARENA_WIDTH / 2;

    createComicFrameElements(scene);
}

function createComicFrameElements(scene: Scene): void {
    const speedLineMat = new StandardMaterial('speedLineMat', scene);
    speedLineMat.emissiveColor = new Color3(1, 0.9, 0);
    speedLineMat.disableLighting = true;
    
    for (let i = 0; i < 5; i++) {
        const speedLine = MeshBuilder.CreateBox(`speedLine${i}`, {
            width: 0.15 * (1 - i * 0.1),
            height: 0.15 * (1 - i * 0.1),
            depth: VISUAL_PLAYFIELD_DEPTH
        }, scene);
        speedLine.position.x = - (ARENA_WIDTH / 2 + i * 0.3);
        speedLine.position.y = 0.5 - i * 0.1;
        speedLine.material = speedLineMat;
        
        const speedLine2 = speedLine.clone(`speedLine2_${i}`);
        speedLine2.position.x = ARENA_WIDTH / 2 + i * 0.3;
    }
}

function createBallTrail(scene: Scene, ball: Mesh): void {
    const trail = new TrailMesh('ballTrail', ball, scene, 0.3, 40, true);
    const trailMaterial = new StandardMaterial('trailMat', scene);
    trailMaterial.emissiveColor = new Color3(1, 0.3, 0.6);
    trailMaterial.disableLighting = true;
    trailMaterial.alpha = 0.7;
    trail.material = trailMaterial;
}

function createSpawnParticleEffect(scene: Scene, emitter: Mesh): ParticleSystem {
    const particleSystem = new ParticleSystem('spawnParticles', 800, scene);
    particleSystem.particleTexture = new Texture("https://playground.babylonjs.com/textures/flare.png", scene);
    particleSystem.emitter = emitter;
    particleSystem.minLifeTime = 0.3;
    particleSystem.maxLifeTime = 0.8;
    particleSystem.color1 = new Color4(1, 0, 0.5, 1.0);
    particleSystem.color2 = new Color4(1, 1, 0, 1.0);
    particleSystem.colorDead = new Color4(0, 1, 1, 0.0);
    particleSystem.minSize = 2.0;
    particleSystem.maxSize = 4.5;
    particleSystem.minScaleX = 0.8;
    particleSystem.minScaleY = 2.0;
    particleSystem.minAngularSpeed = -Math.PI * 6;
    particleSystem.maxAngularSpeed = Math.PI * 6;
    particleSystem.manualEmitCount = 300;
    particleSystem.maxEmitPower = 20;
    particleSystem.minEmitPower = 12;
    particleSystem.blendMode = ParticleSystem.BLENDMODE_ADD;
    particleSystem.gravity = new Vector3(0, -2, 0);
    particleSystem.createSphereEmitter(1.5);
    particleSystem.stop();
    return particleSystem;
}

function createMovableObjects(scene: Scene): SceneObjects {
    const p1Mat = new StandardMaterial('p1Mat', scene);
    p1Mat.diffuseColor = new Color3(1, 0.2, 0.5);
    p1Mat.specularColor = new Color3(1, 1, 1);
    p1Mat.specularPower = 64;
    
    const player1Paddle = MeshBuilder.CreateBox('p1Paddle', {
        width: PADDLE_WIDTH,
        height: PADDLE_HEIGHT,
        depth: PADDLE_DEPTH
    }, scene);
    player1Paddle.position = new Vector3(-PADDLE_START_X, 0.7, 0);
    player1Paddle.material = p1Mat;
    player1Paddle.renderOutline = true;
    player1Paddle.outlineWidth = 0.15;
    player1Paddle.outlineColor = Color3.Black();
    
    const p2Mat = new StandardMaterial('p2Mat', scene);
    p2Mat.diffuseColor = new Color3(0, 0.7, 1);
    p2Mat.specularColor = new Color3(1, 1, 1);
    p2Mat.specularPower = 64;
    
    const player2Paddle = MeshBuilder.CreateBox('p2Paddle', {
        width: PADDLE_WIDTH,
        height: PADDLE_HEIGHT,
        depth: PADDLE_DEPTH
    }, scene);
    player2Paddle.position = new Vector3(PADDLE_START_X, 0.7, 0);
    player2Paddle.material = p2Mat;
    player2Paddle.renderOutline = true;
    player2Paddle.outlineWidth = 0.15;
    player2Paddle.outlineColor = Color3.Black();
    
    const ballMat = new StandardMaterial('ballMat', scene);
    ballMat.diffuseColor = new Color3(1, 1, 0);
    ballMat.specularColor = new Color3(1, 1, 1);
    ballMat.specularPower = 128;
    
    const ball = MeshBuilder.CreateSphere('ball', {
        diameter: BALL_DIAMETER,
        segments: 32
    }, scene);
    ball.position = new Vector3(0, 0.65, 0);
    ball.material = ballMat;
    ball.renderOutline = true;
    ball.outlineWidth = 0.12;
    ball.outlineColor = Color3.Black();
    
    createBallTrail(scene, ball);
    const particleSystem = createEnhancedParticleEffects(scene, ball);
    particleSystem.start();
    const spawnParticleSystem = createSpawnParticleEffect(scene, ball);
    
    return { player1Paddle, player2Paddle, ball, particleSystem, spawnParticleSystem };
}

function createEnhancedParticleEffects(scene: Scene, emitter: Mesh): ParticleSystem {
    const particleSystem = new ParticleSystem('particles', 2000, scene);
    particleSystem.particleTexture = new Texture("https://playground.babylonjs.com/textures/flare.png", scene);
    particleSystem.emitter = emitter;
    particleSystem.color1 = new Color4(1, 1, 0, 1.0);
    particleSystem.color2 = new Color4(1, 0.5, 0, 1.0);
    particleSystem.colorDead = new Color4(1, 0, 0, 0.0);
    particleSystem.minSize = 0.2;
    particleSystem.maxSize = 0.8;
    particleSystem.minLifeTime = 0.3;
    particleSystem.maxLifeTime = 0.8;
    particleSystem.emitRate = 1500;
    particleSystem.createSphereEmitter(0.5);
    particleSystem.minEmitPower = 1;
    particleSystem.maxEmitPower = 2;
    particleSystem.updateSpeed = 0.008;
    particleSystem.blendMode = ParticleSystem.BLENDMODE_ADD;
    particleSystem.gravity = new Vector3(0, 0, 0);
    particleSystem.minAngularSpeed = -2;
    particleSystem.maxAngularSpeed = 2;
    return particleSystem;
}

function setupCamera(scene: Scene, canvas: HTMLCanvasElement): void {
    const camera = new ArcRotateCamera('camera',
        -Math.PI / 2,
        Math.PI / 4,
        120,
        new Vector3(0, 0, 0),
        scene
    );
    camera.attachControl(canvas, true);
    camera.inputs.remove(camera.inputs.attached.keyboard);
    // camera.lowerRadiusLimit = 70;
    // camera.upperRadiusLimit = 160;
    // camera.upperBetaLimit = Math.PI / 3;
    // camera.inertia = 0.9;
    // camera.speed = 2;

    camera.lowerRadiusLimit = 10;     // Much closer zoom
    camera.upperRadiusLimit = 500;    // Much further zoom
    camera.lowerBetaLimit = -Math.PI; // Can go below ground level
    camera.upperBetaLimit = Math.PI;  // Can go above and flip over
    
    // Enhanced camera controls
    camera.inertia = 0.9;
    camera.speed = 2;
    camera.wheelDeltaPercentage = 0.01; // Smooth mouse wheel zooming
    camera.angularSensibilityX = 1000;  // Mouse sensitivity horizontal
    camera.angularSensibilityY = 1000;  // Mouse sensitivity vertical
}
