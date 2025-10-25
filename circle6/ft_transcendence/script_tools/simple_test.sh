#!/bin/bash
echo "🔧 Building TypeScript files..."

echo "Compile directly..."

npx tsc main.ts frontend/src/client/PongGame3D.ts --target ES2020 --module ES2020 --moduleResolution node

python3 -m http.server 8003
