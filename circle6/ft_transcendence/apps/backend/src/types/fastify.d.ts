// apps/backend/src/types/fastify.d.ts

import 'fastify';
import { SocketStream } from '@fastify/websocket';
import { FastifyRequest } from 'fastify';

declare module 'fastify' {
  type WebsocketHandler = (
    connection: SocketStream,
    req: FastifyRequest,
  ) => void;

  export interface FastifyInstance {
    get(
      path: string,
      options: RouteShorthandOptions & { websocket: true },
      handler: WebsocketHandler
    ): FastifyInstance;
  }

  export interface FastifyRequest {
    user?: { userId: number; [key: string]: any };
  }
}
