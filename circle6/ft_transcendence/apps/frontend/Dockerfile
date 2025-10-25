FROM node:20-alpine AS build
WORKDIR /usr/src/app

COPY package*.json ./
COPY apps/frontend/package*.json ./apps/frontend/
COPY packages/common-types/package*.json ./packages/common-types/

RUN npm install

ARG VITE_API_URL
ENV VITE_API_URL=$VITE_API_URL

COPY apps/frontend/ ./apps/frontend/
COPY packages/common-types/src/ ./packages/common-types/src/
COPY tsconfig.json ./
COPY packages/common-types/tsconfig.json ./packages/common-types/tsconfig.json

RUN npm run build --workspace=ft-transcendence-frontend

FROM node:18-alpine
WORKDIR /app

COPY --from=build /usr/src/app/apps/frontend/dist ./dist
COPY --from=build /usr/src/app/apps/frontend/package.json ./package.json
COPY --from=build /usr/src/app/node_modules ./node_modules

EXPOSE 3001
CMD ["npm", "run", "serve", "--", "--host", "0.0.0.0", "--port", "3001"]
