FROM node:20-slim
WORKDIR /usr/src/app

RUN apt-get update && apt-get install -y sqlite3

COPY package*.json ./
COPY apps/backend/package*.json ./apps/backend/
COPY packages/common-types/package*.json ./packages/common-types/

RUN npm install

COPY apps/backend/.mocharc.json ./apps/backend/
COPY apps/backend/src/ ./apps/backend/src/
COPY packages/common-types/src/ ./packages/common-types/src/
COPY tsconfig.json ./
COPY apps/backend/tsconfig.json ./apps/backend/tsconfig.json
COPY packages/common-types/tsconfig.json ./packages/common-types/tsconfig.json

RUN npm run build --workspace=ft-transcendence-backend

COPY apps/backend/src/database/migrations /usr/src/app/apps/backend/migrations/

# Create log directory
RUN mkdir -p /var/log/pong && chmod 755 /var/log/pong 

CMD ["node", "apps/backend/dist/src/main.js"]

