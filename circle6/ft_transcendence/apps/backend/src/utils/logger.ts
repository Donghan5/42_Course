import winston from 'winston';

// Create the main logger
const logger = winston.createLogger({
  level: process.env.LOG_LEVEL || 'info',
  format: winston.format.combine(
    winston.format.timestamp(),
    winston.format.errors({ stack: true }),
    winston.format.json()
  ),
  defaultMeta: { service: 'pong-backend' },
  transports: [
    // Console logging
    new winston.transports.Console({
      format: winston.format.combine(
        winston.format.colorize(),
        winston.format.simple()
      )
    }),
    
    // File logging (picked up by Logstash)
    new winston.transports.File({
      filename: '/var/log/pong/app.log',
      level: 'info'
    }),
    
    // Error file
    new winston.transports.File({
      filename: '/var/log/pong/error.log',
      level: 'error'
    }),
  ]
});

// Specific loggers for different components
export const gameLogger = logger.child({ component: 'game' });
export const apiLogger = logger.child({ component: 'api' });
export const userLogger = logger.child({ component: 'user' });

// Dedicated loggers for game events and user activity (created once for efficiency)
const gameEventLogger = winston.createLogger({
  format: winston.format.json(),
  transports: [
    new winston.transports.File({
      filename: '/var/log/pong/game-events.log'
    })
  ]
});

const userActivityLogger = winston.createLogger({
  format: winston.format.json(),
  transports: [
    new winston.transports.File({
      filename: '/var/log/pong/user-activity.log'
    })
  ]
});

// Game-specific logging functions
export const logGameEvent = (gameId: string, eventType: string, data: any) => {
  const logEntry = {
    game_id: gameId,
    event_type: eventType,
    timestamp: new Date().toISOString(),
    ...data
  };

  // Log to dedicated game events file
  gameEventLogger.info(logEntry);

  // Also log to main logger
  gameLogger.info(`Game event: ${eventType}`, logEntry);
};

// User activity logging
export const logUserActivity = (userId: string, activity: string, metadata: any) => {
  const logEntry = {
    user_id: userId,
    activity,
    timestamp: new Date().toISOString(),
    ...metadata
  };

  // Log to dedicated user activity file
  userActivityLogger.info(logEntry);

  // Also log to main logger
  userLogger.info(`User activity: ${activity}`, logEntry);
};

export default logger;
